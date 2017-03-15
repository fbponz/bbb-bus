//Borja Ponz - fbponz@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <modbus.h>
#define LED_PLACA_XBEE 60
#define PULSADOR_PLACA_XBEE 50
#define SYSGPIO_DIR "/sys/class/gpio"
#define USER_LED_DIR "/sys/class/leds/beaglebone:green:usr"
#define USER_LED_END "/brightness"

int set_led(unsigned int led,unsigned int value){
    int fd, len;
    char buf[64];
    len = snprintf(buf, sizeof(buf),"/sys/class/leds/beaglebone:green:usr%d/brightness", led);
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        return fd;
    }
    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);
    close(fd);
    return 0;

}
int gpio_export(unsigned int gpio){
    int fd, len;
    char buf[64];

    fd=open(SYSGPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

        return 0;
}
int gpio_set_dir(unsigned int gpio, unsigned int out_flag){
    /*
    1->Out.
    0->In.
    */
    int fd, len;
    char buf[64];
    len = snprintf(buf, sizeof(buf), SYSGPIO_DIR  "/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    if (out_flag)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);

    close(fd);
    return 0;
}
int gpio_set_value(unsigned int gpio, unsigned int value){
    int fd, len;
    char buf[64];

    len = snprintf(buf, sizeof(buf), SYSGPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}
int gpio_get_value(unsigned int gpio, unsigned int *value){
    int fd, len;
    char buf[64];

    len = snprintf(buf, sizeof(buf), SYSGPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        return fd;
    }

    gpio_get_value_fd(fd, value);

    close(fd);
    return 0;
}
int gpio_get_value_fd(int fd, unsigned int *value){
    char ch;

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    return 0;
}
void gpio_fd_open(unsigned int gpio){
    int fd, len;
    char buf[64];
    len=snprintf(buf,sizeof(buf),SYSGPIO_DIR "/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY | O_NONBLOCK );
}
int gpio_unexport(unsigned int gpio){
    int fd, len;
    char buf[64];
    fd = open(SYSGPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        return -1;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}
void actualizar_gpio(modbus_mapping_t *mb_mapping){

    int estado_pulsador=0;
    uint8_t valor=8;

    gpio_get_value(PULSADOR_PLACA_XBEE,&estado_pulsador);
    mb_mapping->tab_input_bits[0]=estado_pulsador;
    gpio_set_value(LED_PLACA_XBEE, mb_mapping->tab_bits[0]);
    valor=mb_mapping->tab_registers[0];
    actualizar_led(valor);

}
void actualizar_led(uint8_t valor){
    FILE *Fichero_led = NULL;
    char buffer[12];

    snprintf(buffer, 12,"%d",(valor & ( 1 << 0 )) >> 0);
    if((Fichero_led = fopen(USER_LED_DIR "0" USER_LED_END, "r+")) != NULL){
        fwrite(buffer, sizeof(char), 1, Fichero_led);
        fclose(Fichero_led);
    }
    snprintf(buffer, 12,"%d",(valor & ( 1 << 1 )) >> 1);
    if((Fichero_led = fopen(USER_LED_DIR "1" USER_LED_END, "r+")) != NULL){
        fwrite(buffer, sizeof(char), 1, Fichero_led);
        fclose(Fichero_led);
    }
     snprintf(buffer, 12,"%d",(valor & ( 1 << 2 )) >> 2);
    if((Fichero_led = fopen(USER_LED_DIR "2" USER_LED_END, "r+")) != NULL){
        fwrite(buffer, sizeof(char), 1, Fichero_led);
        fclose(Fichero_led);
    }
     snprintf(buffer, 12,"%d",(valor & ( 1 << 3 )) >> 3);
    if((Fichero_led = fopen(USER_LED_DIR "3" USER_LED_END, "r+")) != NULL){
        fwrite(buffer, sizeof(char), 1, Fichero_led);
        fclose(Fichero_led);
    }
}


void inicializar_gpio(){
    gpio_export(LED_PLACA_XBEE);
    gpio_set_dir(LED_PLACA_XBEE,1);
    gpio_fd_open(LED_PLACA_XBEE);

    gpio_export(PULSADOR_PLACA_XBEE);
    gpio_set_dir(PULSADOR_PLACA_XBEE, 0);
    gpio_fd_open(PULSADOR_PLACA_XBEE);
}


int main(int argc, char *argv[])
{
    int s=-1,rc,longitud_header;
    modbus_t *control_tx;
    modbus_mapping_t *mb_mapping;

    inicializar_gpio();

    control_tx= modbus_new_tcp("127.0.0.1",502);
    //longitud_header = modbus_get_header_length(control_tx);
    modbus_set_debug(control_tx,TRUE);
    mb_mapping = modbus_mapping_new(1,1,1,0);

    if(mb_mapping == NULL){
        printf("Fallo al crear el mapa");
        modbus_free(control_tx);
        return -1;
    }

    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    s = modbus_tcp_listen(control_tx, 1);


    for(;;){
        modbus_tcp_accept(control_tx, &s);
        rc=0;
        do{
            rc=modbus_receive(control_tx, query);
            if(rc>=0){
                modbus_reply(control_tx, query, rc, mb_mapping);
                actualizar_gpio(mb_mapping);
            }
            printf("%d/n",rc);
        }while(rc!=-1);



    }
    close(s);
    modbus_mapping_free(mb_mapping);
    free(query);
    modbus_close(control_tx);
    modbus_free(control_tx);
    return 0;
}

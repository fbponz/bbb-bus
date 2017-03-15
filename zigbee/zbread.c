//Borja Ponz - fbponz@gmail.com
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define tamanyo_buffer_tx 20
#define SYSGPIO_DIR "/sys/class/gpio"
#define USER_LED_DIR "/sys/class/leds/beaglebone:green:usr"
#define USER_LED_END "/brightness"
char buffer_tx[tamanyo_buffer_tx];
time_t t_ahora;
time_t t_ultcnx;
char buffer_sh[20];

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

void transmitir(int uart4_manejador, char *mensaje_a_enviar,bool modo_comando) {
    if(modo_comando==true){
        strcpy(buffer_tx, "+++");
        if(uart4_manejador !=-1){
           int longitud_buffer_tx=strlen(buffer_tx);
           int cuenta_escrito =write(uart4_manejador, buffer_tx,longitud_buffer_tx);
           if(longitud_buffer_tx!=cuenta_escrito) printf("UART Transciever error\r\n");
           usleep(1000000);
           unsigned char buffer_rx[256];
           memset(buffer_rx, 0, sizeof(buffer_rx));
           int cuenta_leido = read(uart4_manejador, (void*)buffer_rx, 255);
           if(cuenta_leido<0){
               //printf("Error\r\n");
           }else if(cuenta_leido==0){
              // printf("No tengo que leer\r\n");
           }else{
               buffer_rx[cuenta_leido] = '\0';
               printf(": %s\n", buffer_rx);
           }
        }
    }else if(modo_comando==false){
        strcpy(buffer_tx, mensaje_a_enviar);
        if(uart4_manejador !=-1){
            int longitud_buffer_tx=strlen(buffer_tx);
            int cuenta_escrito =write(uart4_manejador, buffer_tx,longitud_buffer_tx);
            if(longitud_buffer_tx!=cuenta_escrito) printf("UART  Transciever error\r\n");
            usleep(1000000);
            int cuenta_leido = 0;
            while(cuenta_leido ==0){
                unsigned char buffer_rx[256];
                memset(buffer_rx, 0, sizeof(buffer_rx));
                cuenta_leido=read(uart4_manejador,(void*)buffer_rx, 255);
                if(cuenta_leido<0){
                   //printf("Error\r\n");
                }else if(cuenta_leido==0){
                   // printf("No tengo que leer\r\n");
                }else{
                    buffer_rx[cuenta_leido] = '\0';
                    printf(" : %s \n", buffer_rx);
                }
            }
        }

    }
}
void conectarse(int uart4_manejador){
    printf("+++:");
    transmitir(uart4_manejador,"+++\r\n",true);
    printf(", at:");
    transmitir(uart4_manejador,"at\r\n",false);
    printf(", atni:");
    transmitir(uart4_manejador,"atni PUESTO03\r\n",false);
    printf(", atsm:");
    transmitir(uart4_manejador,"atsm 4\r\n",false);
    printf(", atso:");
    transmitir(uart4_manejador,"atso 2\r\n",false);
    printf(", atct:");
    transmitir(uart4_manejador,"atct 258\r\n",false);
    printf(", atac:");
    transmitir(uart4_manejador,"atac\r\n",false);
    printf(", atwr:");
    transmitir(uart4_manejador,"atwr\r\n",false);
    printf("\n");

}

void obtenerparametrosred(int uart4_manejador){
    printf("AI:");
    transmitir(uart4_manejador,"atai\r\n",false);
    printf(", SH:");
    transmitir(uart4_manejador,"atsh\r\n",false);
    printf(", SL:");
    transmitir(uart4_manejador,"atsl\r\n",false);
    printf(", CH:");
    transmitir(uart4_manejador,"atch\r\n",false);
    printf(", DB:");
    transmitir(uart4_manejador,"atdb\r\n",false);
    printf(", OP:");
    transmitir(uart4_manejador,"atop\r\n",false);
    printf(", OI:");
    transmitir(uart4_manejador,"atoi\r\n",false);
    printf(", NI:");
    transmitir(uart4_manejador,"atni\r\n",false);
    printf(", MY:");
    transmitir(uart4_manejador,"atmy\r\n",false);
    printf(", Cerrando: ");
    transmitir(uart4_manejador,"atcn\r\n",false);
    printf("\n");
}

bool ai(int uart4_manejador){
    strcpy(buffer_tx, "atai\r\n");
    if(uart4_manejador !=-1){
        int longitud_buffer_tx=strlen(buffer_tx);
        int cuenta_escrito =write(uart4_manejador, buffer_tx,longitud_buffer_tx);
        if(longitud_buffer_tx!=cuenta_escrito) printf("UART  Transciever error\r\n");
        usleep(100000);
        int cuenta_leido = 0;
        while(cuenta_leido ==0){
            unsigned char buffer_rx[256];
            memset(buffer_rx, 0, sizeof(buffer_rx));
            cuenta_leido=read(uart4_manejador,(void*)buffer_rx, 255);
            if(cuenta_leido<0){
               // printf("Error\r\n");
            }else if(cuenta_leido==0){
               // printf("No tengo que leer.\r\n");
            }else{
                buffer_rx[cuenta_leido] = '\0';

            }
            if(strtol((char *)buffer_rx,NULL,16)== 0x00)return true;
            return false;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    int uart4_manejador = -1;
    struct termios configuracion;
    unsigned char buffer_rx[10];
    uart4_manejador = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart4_manejador == -1){
        printf("Error- no podemos abrir la UART.\r\n");
        return -1;
    }

    tcgetattr(uart4_manejador, &configuracion);
    configuracion.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    configuracion.c_iflag = IGNPAR;
    configuracion.c_oflag = 0;
    configuracion.c_lflag = 0;

    tcflush(uart4_manejador, TCIFLUSH);
    tcsetattr(uart4_manejador, TCSANOW, &configuracion);


    conectarse(uart4_manejador);
    obtenerparametrosred(uart4_manejador);
    time(&t_ultcnx);
    while(1){
        time(&t_ahora);
        memset(buffer_rx, 0, sizeof(buffer_rx));
        int caracteres_leidos=read(uart4_manejador,(void*)buffer_rx,10);

        if(caracteres_leidos>0){
            buffer_tx[caracteres_leidos]= '\0';
            actualizar_led(buffer_rx[0]-48);
            sleep(1);
            time(&t_ultcnx);
        }
        if(difftime(t_ahora,t_ultcnx)>6){
           printf("Reconectamos");
           sleep(6);
           conectarse(uart4_manejador);
           if(ai(uart4_manejador)) obtenerparametrosred(uart4_manejador);
           time(&t_ultcnx);
        }
    }
    close(uart4_manejador);
    return 0;
}

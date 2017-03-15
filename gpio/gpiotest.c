//Borja Ponz - fbponz@gmail.com
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>

#define SYSGPIO_DIR "/sys/class/gpio"
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
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
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
int gpio_set_value(unsigned int gpio, unsigned int value)
{
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
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
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
int gpio_get_value_fd(int fd, unsigned int *value)
{
    char ch;

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    return 0;
}
int gpio_unexport(unsigned int gpio)
{
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

int main(void)
{
    unsigned int *valor_pulsador=0;
    uint8_t entrada=8;
    int pausa=0,sentido=0;


    while(1){
        if(!gpio_export(50))
            if(!gpio_set_dir(50,0)){
                       gpio_get_value(50,&valor_pulsador);

                }


        if(!gpio_export(60))
            if(!gpio_set_dir(60,1)){
                gpio_set_value(60,valor_pulsador);
            }

        if(entrada==8){
            sentido=1;
        }else if(entrada==1){
            sentido=0;
        }
        if(sentido==1){
            entrada = entrada >> 1;
        }else{
            entrada = entrada<<1;
        }
        set_led(0,0);
        set_led(1,0);
        set_led(2,0);
        set_led(3,0);
        switch(entrada){
        case 1:
                set_led(0,1);
                break;
        case 2:
                set_led(1,1);
                break;
        case 4:
                set_led(2,1);
                break;
        case 8:
                set_led(3,1);
                break;
        }

        while(pausa<90000000) pausa++;
        pausa=0;


    }
    gpio_unexport(50);
    gpio_unexport(60);
    return 0;
}

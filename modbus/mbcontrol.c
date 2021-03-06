//Borja Ponz - fbponz@gmail.com
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

modbus_t *mb;


int main(int argc, char *argv[])
{
    uint8_t tab_rp_bits[64];
    uint16_t tab_rp_registers[64];
    int rc;
    int direcciones;
    int salida,i,pausa,sentido;
    uint8_t entrada=8;

    mb = modbus_new_tcp("192.168.7.2",502);
    if(modbus_connect(mb) == -1){
        printf("Fallo al conectar. \n");
        return -1;
    }else{
        printf("Conectado \n");
    }
    while(1){

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


        rc = modbus_write_register(mb, 0, entrada);

        while(pausa<90000000) pausa++;
        pausa=0;

    }

}

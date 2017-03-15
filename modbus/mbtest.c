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

    mb = modbus_new_tcp("192.168.7.2",502);
    if(modbus_connect(mb) == -1){
        printf("Fallo al conectar. \n");
        return -1;
    }else{
        printf("Conectado \n");
    }
    printf("Read coils\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_read_bits(mb, direcciones, 1, tab_rp_bits);

            if (rc != 1) {
                printf("Fallado \n");
            }else{
               printf("Aceptado \n");
            }
    }
    printf("Read entradas discretas\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_read_input_bits(mb, direcciones, 1, tab_rp_bits);

            if (rc != 1) {
                printf("Fallado\n");
            }else{
               printf("Aceptado \n");
            }
    }
    printf("Read Holding Register\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_read_registers(mb, direcciones, 1, tab_rp_registers);
            if (rc != 1) {
                printf("Fallado \n");
            }else{
               printf("Aceptado \n");
            }
    }
    printf("Read input register\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_read_input_registers(mb, direcciones, 1, tab_rp_registers);
            if (rc != 1) {
                printf("Fallado \n");
            }else{
               printf("Aceptado \n");
            }
    }
    printf("write bit\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_write_bit(mb, direcciones, 1);
            if (rc != 1) {
                printf("Fallado \n");
            }else{
               printf("Aceptado \n");
            }
    }
    printf("write register\n");
    for(direcciones=0; direcciones<6; direcciones++)
    {

            rc = modbus_write_register(mb, direcciones, 1);
            if (rc != 1) {
                printf("Fallado \n");
            }else{
               printf("Aceptado \n");
            }
    }

    modbus_close(mb);
    modbus_free(mb);

    return 0;
}

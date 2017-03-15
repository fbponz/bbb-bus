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

    while(1){
        rc = modbus_read_input_bits(mb, 0, 1, tab_rp_bits);
        if (tab_rp_bits[0]==1) {
            rc = modbus_write_bit(mb, 0, 1);
        }else{
           rc = modbus_write_bit(mb, 0, 0);
        }
    }

}

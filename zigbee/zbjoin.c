//Borja Ponz - fbponz@gmail.com
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>
#define tamanyo_buffer_tx 20
char buffer_tx[tamanyo_buffer_tx];

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
              // printf("no tengo que leer\r\n");
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
                   // printf("no tengo que leer\r\n");
                }else{
                    buffer_rx[cuenta_leido] = '\0';
                    printf(" : %s \n", buffer_rx);
                }
            }
        }

    }
}

int main(int argc, char *argv[])
{
    //Configurar UART
    int uart4_manejador = -1;
    struct termios configuracion;
    uart4_manejador = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);

    if(uart4_manejador == -1){
        printf("Error- no podemos abrir la UART.");
    }

    tcgetattr(uart4_manejador, &configuracion);
    configuracion.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    configuracion.c_iflag = IGNPAR;
    configuracion.c_oflag = 0;
    configuracion.c_lflag = 0;

    tcflush(uart4_manejador, TCIFLUSH);
    tcsetattr(uart4_manejador, TCSANOW, &configuracion);

   
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
    printf("\n AI:");
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

    close(uart4_manejador);
    return 0;
}

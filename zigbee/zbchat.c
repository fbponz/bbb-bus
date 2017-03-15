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
bool mantenedor_chat=true;

void transmitir(int uart4_manejador, char *mensaje_a_enviar,bool modo_comando, bool mostrar) {
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
               //if(mostrar==true)printf("Error\r\n");
           }else if(cuenta_leido==0){
              // if(mostrar==true)printf("no tengo que leer\r\n");
           }else{
               buffer_rx[cuenta_leido] = '\0';
               if(mostrar==true)printf(": %s\n", buffer_rx);
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
                   //if(mostrar==true)printf("Error\r\n");
                }else if(cuenta_leido==0){
                   //if(mostrar==true)printf("no tengo que leer\r\n");
                }else{
                    buffer_rx[cuenta_leido] = '\0';
                    if(mostrar==true)printf(" : %s \n", buffer_rx);
                }
            }
        }

    }
}

void conectarse(int uart4_manejador,bool mostrar){
    if(mostrar==true)printf(" +++:");
    transmitir(uart4_manejador,"+++\r\n",true,mostrar);
    if(mostrar==true)printf(", at:");
    transmitir(uart4_manejador,"at\r\n",false,mostrar);
    if(mostrar==true)printf(", atni:");
    transmitir(uart4_manejador,"atni PUESTO03\r\n",false,mostrar);
    if(mostrar==true)printf(", atsm:");
    transmitir(uart4_manejador,"atsm 4\r\n",false,mostrar);
    if(mostrar==true)printf(", atso:");
    transmitir(uart4_manejador,"atso 2\r\n",false,mostrar);
    if(mostrar==true)printf(", atct:");
    transmitir(uart4_manejador,"atct 258\r\n",false,mostrar);
    if(mostrar==true)printf(", atac:");
    transmitir(uart4_manejador,"atac\r\n",false,mostrar);
    if(mostrar==true)printf(", atwr:");
    transmitir(uart4_manejador,"atwr\r\n",false,mostrar);
    transmitir(uart4_manejador,"atdh 0013A200\r\n",false,mostrar);//0013A200
    transmitir(uart4_manejador,"atdl 40A8B9B9\r\n",false,mostrar);//40A8BA26 //40A8BA71
    if(mostrar==true)printf("\n");
    
}

void obtenerparametrosred(int uart4_manejador,bool mostrar){
    if(mostrar==true)printf(" AI:");
    transmitir(uart4_manejador,"atai\r\n",false,mostrar);
    if(mostrar==true)printf(", SH:");
    transmitir(uart4_manejador,"atsh\r\n",false,mostrar);
    if(mostrar==true)printf(", SL:");
    transmitir(uart4_manejador,"atsl\r\n",false,mostrar);
    if(mostrar==true)printf(", CH:");
    transmitir(uart4_manejador,"atch\r\n",false,mostrar);
    if(mostrar==true)printf(", DB:");
    transmitir(uart4_manejador,"atdb\r\n",false,mostrar);
    if(mostrar==true)printf(", OP:");
    transmitir(uart4_manejador,"atop\r\n",false,mostrar);
    if(mostrar==true)printf(", OI:");
    transmitir(uart4_manejador,"atoi\r\n",false,mostrar);
    if(mostrar==true)printf(", NI:");
    transmitir(uart4_manejador,"atni\r\n",false,mostrar);
    if(mostrar==true)printf(", MY:");
    transmitir(uart4_manejador,"atmy\r\n",false,mostrar);
    if(mostrar==true)printf(", Cerrando: ");
    transmitir(uart4_manejador,"atcn\r\n",false,mostrar);
    if(mostrar==true)printf("\n");
}

int main(int argc, char *argv[]) {
    //Configurar UART
    int uart4_manejador = -1;
    int manejador;
    struct termios configuracion;
    struct termios condiciones_iniciales;
    unsigned char buffer_rx[250];
    char buffer_chat_rx[250];
    char buffer_chat_tx[250];
    int _v_caracteres_leidos,_v_caracteres_escritos;

    uart4_manejador = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart4_manejador == -1){
        printf("Error- no podemos abrir la UART.\r\n");
        return -1;
    }

    tcgetattr(0,&condiciones_iniciales);
    condiciones_iniciales.c_lflag &=~ICANON;
    condiciones_iniciales.c_lflag &=~ECHO;
    condiciones_iniciales.c_lflag &=~ISIG;
    condiciones_iniciales.c_cc[VMIN]=0;
    condiciones_iniciales.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &condiciones_iniciales);

    tcgetattr(uart4_manejador, &configuracion);
    configuracion.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    configuracion.c_iflag = IGNPAR;
    configuracion.c_oflag = 0;
    configuracion.c_lflag = 0; 
    tcflush(uart4_manejador, TCIFLUSH);
    tcsetattr(uart4_manejador, TCSANOW, &configuracion);

    _v_caracteres_leidos=0;
    _v_caracteres_escritos=0;
    memset(buffer_chat_tx,0,sizeof(buffer_chat_tx));

    conectarse(uart4_manejador,true);
    obtenerparametrosred(uart4_manejador,true);
    printf("Escribe Ahora:");
    while(mantenedor_chat){

        int ch=getchar();

        if(ch==0x1B) mantenedor_chat=false;

        if(ch!=EOF && mantenedor_chat==true){
            printf("%c",ch);
            if(ch==0xA||ch==0xD){
                buffer_chat_tx[_v_caracteres_leidos]='\r';
                buffer_chat_tx[_v_caracteres_leidos+1]='\n';
                buffer_chat_tx[_v_caracteres_leidos+2]='\0';
                transmitir(uart4_manejador,buffer_chat_tx,false,false);
                buffer_chat_tx[_v_caracteres_leidos]='\0';
                printf("\nEnviando: [%s]\nEscribe Ahora:",buffer_chat_tx);
                _v_caracteres_leidos=0;
                memset(buffer_chat_tx,0,sizeof(buffer_chat_tx));
            }else{
                buffer_chat_tx[_v_caracteres_leidos]=ch;
                _v_caracteres_leidos++;
            }
         }

        int ch_leidos=read(uart4_manejador,(void*)buffer_rx,250);
        if(ch_leidos>0&&mantenedor_chat==true){
            for(manejador=0; manejador<ch_leidos; manejador++){
                if((buffer_rx[manejador]<0x30||buffer_rx[manejador]>0x39)&&buffer_rx[manejador]!=0x2B){
                        buffer_chat_rx[_v_caracteres_escritos]=buffer_rx[manejador];
                        _v_caracteres_escritos++;
                }
            }
            if(_v_caracteres_escritos>0){
                if(buffer_chat_rx[_v_caracteres_escritos-1]=='\n'||buffer_chat_rx[_v_caracteres_escritos-1]=='\r'){//
                        buffer_chat_rx[_v_caracteres_escritos-1]='\0';//Cerramos cadena.
                        printf("\nRecibido: [%s]\nEscribe Ahora:",buffer_chat_rx);
                        _v_caracteres_escritos=0;
                        memset(buffer_chat_rx,0,sizeof(buffer_chat_rx));
                 }
            }
        }

    }
    close(uart4_manejador);
    tcsetattr(0,TCSANOW,&condiciones_iniciales);
    return 0;
}

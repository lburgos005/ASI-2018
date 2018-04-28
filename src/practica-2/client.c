/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Laura
 *
 * Created on 23 de febrero de 2018, 12:51
 */




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <string.h>

/*
 * 
 */
void ejercicio1();
void ejercicio2();
void ejercicio3();
void ejercicio4();
void ejercicio5();
void ejercicio6();
void ejercicio7();
void lb();

#define FIFO1 "/tmp/fifo_monitor_1"
#define FIFO2 "/tmp/fifo_monitor_2"
#define LONGITUD1 16

key_t clave= 0x79124871L;
char secreto1[LONGITUD1];
char secreto2[LONGITUD1];
char secreto3[LONGITUD1];
char secreto4[LONGITUD1];

typedef struct Mensaje{
   long Id_mensaje;
   char cadena[LONGITUD1];
    
}Mensaje;

Mensaje m1, m2, m3, m4; 
pid_t pidMoni;
int idCola, idCola2;
int FileHndlr, File;


int main(int argc, char** argv) {
    char ejercicio;
    pidMoni= atoi(argv[1]);
    while(ejercicio!= '0'){
        printf("Elegir ejercicio 1-7\n");
        lb();
        scanf("%c", &ejercicio);
        
        switch(ejercicio){
            
            case '1':{
                ejercicio1();
            }break;
            
            case '2':{
                ejercicio2();
            }break;
            
            case '3':{
                ejercicio3();
            }break;
            
            case '4':{
                ejercicio4();
            }break;
            
            case '5':{
                ejercicio5();
            }break;
            
            case '6':{
                ejercicio6();
            }break;
            
            case '7':{
                ejercicio7();
            }break;
        }
    }
    
    
    

    return (EXIT_SUCCESS);
}

void lb()
{
    __fpurge(stdin);
    fflush(stdout);
}

void ejercicio1(){
   
   int numBytes;
   FileHndlr= open(FIFO1, O_RDWR);
   
   if(FileHndlr == -1){
       printf("Error fifo1\n");
   }
   numBytes= read(FileHndlr, secreto1, LONGITUD1);
   printf("El secreto 1 es %s", secreto1);
   

   
}

void ejercicio2(){
    int FifoError;
    int numBytes;
    int FileHndlr;
    //Crear FIFO2
    FifoError= mkfifo(FIFO2, S_IRWXU);
    if(FifoError == -1){
        printf("Error creando fifo2\n");
    }
    //Abrir FIFO 2
    File= open(FIFO2, O_RDWR);
    //Escribimos el secreto 1
    numBytes= write(File, secreto1, LONGITUD1);
    //Abrimos FIFO1
    FileHndlr= open(FIFO1, O_RDWR);
    if(FileHndlr == -1){
       printf("Error fifo1\n");
    }
    //Leemos secreto 2
    numBytes= read(FileHndlr, secreto2, LONGITUD1);
    
    //Escribimos secreto 2 en la fifo 2 
    numBytes= write(File, secreto2, LONGITUD1);

    
      
}

void ejercicio3(){
    
    
    idCola= msgget(clave, 0666);
    if(idCola){
        printf("Error abriendo la cola\n");   
    }
    msgrcv(idCola, &m1, sizeof(Mensaje), 0, 0);
    printf("El secreto 3 %ld y secreto 4 %s\n", m1.Id_mensaje, m1.cadena);
    
}

void ejercicio4(){
    
    idCola2= msgget(clave, 0666|IPC_CREAT);
    m2.Id_mensaje= m1.Id_mensaje;
	
    sprintf(m2.cadena, "<%ld>%s", m1.Id_mensaje, m1.cadena);
  

    msgsnd(idCola2, &m2, sizeof(Mensaje)- sizeof(long),0);
    
}

void ejercicio5(){
   
    idCola2= msgget(clave, 0666);
    if(idCola2 == -1){
    	printf("Error abriendo cola\n");
    }
    msgrcv(idCola2, &m3, sizeof(Mensaje), (long)getpid(), 0);
    printf("El tipo de mensaje es %ld, y el pid es %ld\n", m3.Id_mensaje, getpid());
    printf("El secreto 6 es %s\n", m3.cadena); 
	
}




void ejercicio6(){
    
    idCola2= msgget(clave, 0666);
    if(idCola2 == -1){
    	printf("Error abriendo cola\n");
    }
    m4.Id_mensaje= (long) pidMoni;
    sprintf(m4.cadena, "%s", m3.cadena);
    msgsnd(idCola2, &m4, sizeof(Mensaje), 0);

}


void ejercicio7(){

    msgsnd(idCola2, &m4, sizeof(Mensaje), 0);

    close(idCola);
    msgctl(idCola, IPC_RMID, (struct msqid_ds*)NULL);
    close(idCola2);
    msgctl(idCola2, IPC_RMID, (struct msqid_ds*)NULL);

    close(FileHndlr);
    unlink(FIFO1);
    close(File);
    unlink(FIFO2);
      
}





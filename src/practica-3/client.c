/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Laura
 *
 * Created on 1 de abril de 2018, 12:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define CLAVE 0x79124871L

void lb(void);
void ejercicio1(void);
void ejercicio2(void);
void ejercicio3(void);
void ejercicio4(void);
void ejercicio5(void);
void ejercicio6(void);
void ejercicio7(void);

int shmid1, secreto1, idsem, secreto8;
char secreto2[10];
char *shm;
key_t clave;
struct sembuf P={0, -1, 0}, V={0, 1, 0}, aP[2]= {{0, -1, 0}, {1, -1, 0}}, aV[2]= {{0, 1, 0}, {1, 1, 0}};
/*
 * 
 */
int main(int argc, char** argv) {
    
    char ejercicio = '8';
    
    while(ejercicio != '0')
    {
        printf("Elige ejercicio 1-7, 0 para salir\n");
        lb();
        scanf("%c", &ejercicio);
        switch(ejercicio)
        {
            case '1':
            {
                ejercicio1();
            }break;

	    case '2':
            {
                ejercicio2();
            }break;
            
            case '3':
            {
                ejercicio3();
            }
            
            case '4':
            {
                ejercicio4();
            }
            
            case '5':
            {
                ejercicio5();
            }
            
            case '6':
            {
                ejercicio6();
            }
            
            case '7':
            {
                ejercicio7();
            }
        }
        
    }
    

    return (EXIT_SUCCESS);
}

void lb(void)
{
    __fpurge(stdin);
    fflush(stdout);
    
}

void ejercicio1(void)
{
    int offset;
    clave = 0x79124871L;
    shmid1 = shmget(clave, SHM_SIZE, 0666);
    shm = shmat(shmid1, 0, 0);
    secreto1 = *((int*)shm);
    offset = *((int*)(shm + sizeof(int)));
    sprintf(secreto2, "%s", shm+offset);
    sleep(3);
    shmdt(shm);
}

void ejercicio2(void)
{
    int dsp;
    int clave2;
    clave = 0x79124871L;
    shmid1 = shmget(clave, SHM_SIZE, 0666|IPC_CREAT);
    shm = shmat(shmid1, 0, 0);
    memcpy(shm, secreto2, strlen(secreto2));
    sscanf(secreto2, "<%d>", &clave2);
    dsp= clave2 + 16;
    sprintf(shm+dsp, "<%d>", secreto1);
    sleep(2);
    shmdt(shm);
    
}

void ejercicio3(void)
{
    idsem= semget(CLAVE, 1, 0666|IPC_CREAT);
    semctl(idsem, 0, SETVAL, 871);
    
}

void ejercicio4(void)
{
    
    idsem= semget(CLAVE, 2, 0666|IPC_CREAT);
    semctl(idsem, 0, SETVAL, 1);  
    semctl(idsem, 1, SETVAL, 2); 
    
}

void ejercicio5(void)
{
    clave = 0x79124871L;
    //semaforo
    int nsems = 1;
    idsem= semget(clave, nsems, 0666);
    //shared memory
    shmid1 = shmget(clave, SHM_SIZE, 0666);
    shm = shmat(shmid1, 0, 0);
    if(shm < 0)
    {
      printf("Error al engancharse a la memoria\n");
    }
    //Intentar entrar, restamos uno al semáforo. Cuando esté en verde podremos leer
    semop(idsem, &P, 1);
    //Leer un int en la posición 0
    secreto8= *((int*)shm);
    printf("El secreto 8 es %d", secreto8);
    //Escribirlo con signo contrario
    *((int*)shm) = -secreto8;
    sleep(3);
    //poner en verde el semaforo
    semop(idsem, &V, 1); 
    shmdt(shm);
}

void ejercicio6(void)
{
    clave = 0x79124871L;
    //semaforo
    int nsems = 2;
    idsem= semget(clave, nsems, 0666);
    //shared memory
    shmid1 = shmget(clave, SHM_SIZE, 0666);
    shm = shmat(shmid1, 0, 0);
    //Intentar entrar, restamos uno a los semáforos. Cuando estén en verde podremos leer
    semop(idsem, aP, 2);
    //Leer un int en la posición 0
    secreto8= *((int*)shm);
    //Escribirlo con signo contrario
    *((int*)shm) = -secreto8;
    sleep(3);
    //poner en verde los semaforos
    semop(idsem, aV, 2); 
    
}

void ejercicio7(void)
{
   clave = 0x79124871L;
   int nsems = 1;
   idsem= semget(clave, nsems, 0666|IPC_CREAT);
   shmid1 = shmget(clave, SHM_SIZE, 0666);
   shm = shmat(shmid1, 0, 0);
   //Esperar a que este en verde para eliminar los recursos
   semop(idsem, &P, 1);
   //Eliminar recursos
   shmdt(shm);
   shmctl(shmid1,IPC_RMID,NULL); //eliminamos el segmento de memoria
   semctl(idsem,0,IPC_RMID,NULL);//eliminamos el semaforo
   
    
    
}

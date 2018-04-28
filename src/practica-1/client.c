/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Laura
 *
 * Created on 6 de febrero de 2018, 11:01
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
void handler();
void lb();
void ejercicio1();
void ejercicio2();
void ejercicio3();
void *actividad();
void crearHilo();
void cambiarFin();
void ejercicio4();
void startCount();
void contarSen();
void stopSet();
void ejercicio5();
void ejercicio6();

pid_t hijo1, hijo2;
sig_atomic_t fin = 0, stop = 0;
int pidMoni, numSen= 0;

void handler(){
    kill(pidMoni, SIGUSR2);
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    pidMoni= atoi(argv[1]);
    char ejercicio;
    while(1){
        
        printf("Pulsa el numero de ejercicio (1-6)\n");
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
           
            case '0':
            {
                //SALIR
                
            }break;

        }

    }

    
    

    return (EXIT_SUCCESS);
}

void lb(){
    fflush(stdout);
    __fpurge(stdin);
}

void ejercicio1(){
    printf("Ejercicio1...\n");
    kill(pidMoni, SIGUSR1);
    sleep(5);
}

void ejercicio2(){
    printf("Ejercicio2...\n");
    kill(pidMoni, SIGUSR1);
    signal(SIGUSR2, handler);
            
}

void ejercicio3(){
    int status1, status2;
    printf("Ejercicio3...\n");
    kill(pidMoni, SIGUSR1);
    sleep(1);
    
    if((hijo1= fork()) == 0){
        //este es el hijo1
    }
    else if((hijo2= fork()) == 0){
        //Este es el hijo2
    }
    //este es el padre
    kill(pidMoni , SIGUSR1);
    waitpid(hijo1, &status1, 0);
    waitpid(hijo2, &status2, 0);

                  
}

void *actividad(){
    int i;
    for(i= 0; i< 4; i++){
        printf("Soy el thread %d\n", (int) pthread_self());
        sleep(5);
    }
    pthread_exit(0);
}

void crearHilo(){
    int numHilos = 0; 
    pthread_t hilo;
    pthread_create(&hilo, NULL, actividad, (void*)NULL);
    numHilos++;
    printf("Hilo creado, este es el hijo %d\n", numHilos);
}

void cambiarFin(){
    fin = 1;
    
}

void ejercicio4(){
	printf("Ejercicio 4...");
    int status1, status2;
    if((hijo1= fork()) == 0){
        //este es el hijo1
        signal(SIGUSR2, crearHilo);
	
        signal(SIGALRM, cambiarFin);
        alarm(20);
        while(!fin);
    }
    else if((hijo2= fork()) == 0){
        //Este es el hijo2
    }
    //este es el padre
    waitpid(hijo1, &status1, 0);
    waitpid(hijo2, &status2, 0);

}

void startCount(){

	alarm(60);
	signal(SIGUSR1, stopSet);


}

void contarSen(){
	numSen++;
	kill(pidMoni, SIGUSR2);
	printf("Señal %d", numSen);
}

void stopSet(){

	stop= 1;
}

void ejercicio5(){
	printf("Ejercicio 5...");
    int status1, status2;
    kill(pidMoni, SIGUSR1);

    if((hijo1= fork()) == 0){
        //este es el hijo1
      
    }
    else if((hijo2= fork()) == 0){
        //Este es el hijo2
	signal(SIGUSR1, startCount);
	signal(SIGALRM, stopSet); 
	signal(SIGUSR2, contarSen);
	while(stop == 0);
    }
    //este es el padre
    waitpid(hijo1, &status1, 0);
    waitpid(hijo2, &status2, 0);

}

void ejercicio6(){
	
	printf("Ejercicio 6...");
 
    kill(pidMoni, SIGUSR1);
    sleep(1);
    kill(hijo1, SIGKILL);
    kill(hijo2, SIGKILL);



}




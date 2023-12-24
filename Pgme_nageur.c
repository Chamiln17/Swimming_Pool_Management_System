#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#define scab 0
#define mutex 1
#define span 2

#define np 5
#define nc 2

#define N 10

struct sembuf Pscab = {scab,-1,0};
struct sembuf Vscab = {scab,1,0};
struct sembuf Pmutex = {mutex,-1,0};
struct sembuf Vmutex = {mutex,1,0};
struct sembuf Pspan = {span,-1,0};
struct sembuf Vspan = {span,1,0};

void Demander_panier(int * Nump){
    key_t cles;
    int semaphores;
    int *Mdispo;
    

// Recuperation des semaphores 
    /* recuperation d'une clé  */
     cles = ftok("main.c" ,1);
     if(cles ==-1) {
	printf("\n erreur : la clé n’a pas été créée");
	exit(1);
	} 
    
    semaphores = semget(cles, 3, IPC_CREAT|0666);  
    if( semaphores==-1){
        printf("error de creation de semaphore \n");
        exit(2);
    }  

     //------------------------------------------------
    // Recuperation de la memoire partagee 
    /* Recuperer le segment de mémoire partagée. */
    key_t clem= ftok("main.c",2);

	int taille = 2 * sizeof(int);
       int adr = shmget (clem, taille, IPC_CREAT | 0666);
     if(adr==-1){
	 printf(" \n erreur  dans la creation du segment "); 
	exit(0);
	}   
	/* Attacher le segment de mémoire partagée.*/
	Mdispo = (int *) shmat (adr, 0, 0);
	if(Mdispo == NULL) 
	{ printf("\nErreur dans l'attachement du segment ");
 	exit(13);
 	} 
    /* Programme*/

    semop(semaphores,&Pmutex,1);  // P(mutex)
    Mdispo[0]++; //ndp++
    if (Mdispo[1]==np){
       semop(semaphores,&Vmutex,1);  // V(mutex)    
       semop(semaphores,&Pspan,1);  // P(span)
    }
    Mdispo[0]--;
    Mdispo[1]++;
    *Nump=Mdispo[1];
    semop(semaphores,&Vmutex,1);  // V(mutex)    
}



void Liberer_panier(int i,int *Nump){
     key_t cles;
    int semaphores;
    int *Mdispo;
    

// Recuperation des semaphores 
    /* recuperation d'une clé  */
     cles = ftok("main.c" ,1);
     if(cles ==-1) {
	printf("\n erreur : la clé n’a pas été créée");
	exit(1);
	} 
    
    semaphores = semget(cles, 3, IPC_CREAT|0666);  
    if( semaphores==-1){
        printf("error de creation de semaphore \n");
        exit(2);
    }  

     //------------------------------------------------
    // Recuperation de la memoire partagee 
    /* Recuperer le segment de mémoire partagée. */
    key_t clem= ftok("main.c",2);

	int taille = 2 * sizeof(int);
       int adr = shmget (clem, taille, IPC_CREAT | 0666);
     if(adr==-1){
	 printf(" \n erreur  dans la creation du segment "); 
	exit(0);
	}   
	/* Attacher le segment de mémoire partagée.*/
	Mdispo = (int *) shmat (adr, 0, 0);
	if(Mdispo == NULL) 
	{ printf("\nErreur dans l'attachement du segment ");
 	exit(13);
 	} 
    /* Programme*/
    semop(semaphores,&Pmutex,1);  // P(mutex)
    Mdispo[1]--;
    *Nump=Mdispo[1];
    if (Mdispo[0]>0){
        printf("Nageur %d : va liberer un panier et il ya : %d demandes en attente \n",i,Mdispo[0]);
        semop(semaphores,&Vspan,1);  // V(span)
    }else {
        semop(semaphores,&Vmutex,1);  // V(mutex)
    }

}

void Demander_cabine(){
    key_t cles;
    int semaphores;
    

// Recuperation des semaphores 
    /* recuperation d'une clé  */
     cles = ftok("main.c" ,1);
     if(cles ==-1) {
	printf("\n erreur : la clé n’a pas été créée");
	exit(1);
	} 
    
    semaphores = semget(cles, 3, IPC_CREAT|0666);  
    if( semaphores==-1){
        printf("error de creation de semaphore \n");
        exit(2);
    }  
    /*Programme principale*/
    semop(semaphores,&Pscab,1);  // P(scab)
}

void Liberer_cabine(){
    key_t cles;
    int semaphores;
    

// Recuperation des semaphores 
    /* recuperation d'une clé  */
     cles = ftok("main.c" ,1);
     if(cles ==-1) {
	printf("\n erreur : la clé n’a pas été créée");
	exit(1);
	} 
    
    semaphores = semget(cles, 3, IPC_CREAT|0666);  
    if( semaphores==-1){
        printf("error de creation de semaphore \n");
        exit(2);
    }  
    /*Programme principale*/
    semop(semaphores,&Vscab,1);  // V(scab)
}

int Nump;
int main(int argc, char const *argv[])
{
    if (argc<2){
        printf("error");
        exit(1);
    }
    int i = atoi(argv[1]);
    Demander_panier(&Nump);
    Demander_cabine();
    /*Se changer ..*/
    sleep(4);
    Liberer_cabine();
    sleep(7);
    printf("je suis le nageur numero = %d , j'occupe le pannier %d \n",i,Nump);
    Demander_cabine();
    sleep(4);
    Liberer_cabine();
    Liberer_panier(i,&Nump);
    printf("je suis le nageur %d , j'ai liberer un panier , il reste %d  paniers libres \n",i,Nump);
    return 0;
}

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

int semaphores;
int *Mdispo;
int main(int argc, char *argv[]){
    key_t cles;
    

// Creation des semaphores 
    /* Création d'une clé  */
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
    semctl(semaphores,scab,SETVAL,nc);//mutex
    semctl(semaphores,mutex,SETVAL,1);//nlibre
    semctl(semaphores,span,SETVAL,0);//nlibre

     //--------------------------------------------------
    // Creation de la memoire partagee 
    /* Créer un segment de mémoire partagée. */
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
	/* Utiliser la mémoire partagée : Initialiser  */
	// initialiser les variables ndp et npo à 0 
	for(int i=0; i<2; i++) {Mdispo[i]= 0; }  // mdispo[0]=ndp , mdispo[1]=npo
   /* Détacher le segment de mémoire partagée. */
    shmdt (Mdispo);


    for (int i=0;i<N;i++){
        int p=fork();
        if (p==0){
            char ch[10];
            sprintf(ch,"%d",i);
            execlp("./nageur","nageur",ch ,(char*) NULL);
        }
    }

    /* Attente de la fin des programmes */
    int status;
    while(wait(&status) != -1);
    /* Destruction */
    shmctl(adr,IPC_RMID,0);
    semctl(semaphores,3,IPC_RMID,0); 

    return 0;
    }
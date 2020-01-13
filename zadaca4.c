#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<stdlib.h>
#include<signal.h>

struct scommon
{
	char buffer[5];
	sem_t sem[4];//semafor
	int ulaz;
};

struct scommon *c;
int shmid;

void proizvodjac(int n)
{
	int i,semval[1];
	char str[200];
	sem_wait(&c->sem[1]);//cekamo na semafor
	printf("unesi znakove proizvodjaca %i:",n);
	scanf("%s",str);
	sem_post(&c->sem[1]);//otkljucava semafora na koji pokazuje c
	if(sem_getvalue(&c->sem[3],semval)!=-1 && *semval==1)
	{
		sem_wait(&c->sem[3]);
		sem_wait(&c->sem[3]);
	}
	else if(sem_getvalue(&c->sem[3],semval)!=-1 && *semval==0)
	{
		sem_post(&c->sem[3]);
	}
		for(i=0;i<=strlen(str);i++)
		{
			sem_wait(&c->sem[0]);
			sem_wait(&c->sem[1]);
			c->buffer[c->ulaz]=str[i];
			c->ulaz=(c->ulaz+1)%5;
			printf("proizvodjac %d -> %c\n",n,str[i]);
			sem_post(&c->sem[1]);
			sem_post(&c->sem[2]);
		}

}

void potrosac()
{
	int i,izlaz=0,kraj=0;
	char str[200];
	for(i=0;kraj<2;i++)
	{
		sem_wait(&c->sem[2]);
		str[i]=c->buffer[izlaz];
		if(str[i]=='\0' && (++kraj)<2)
		{
			--i;
		}
		printf("potrosac <- %c\n",c->buffer[izlaz]);
		sleep(1);
		izlaz=(izlaz+1)%5;
		sem_post(&c->sem[0]);
	}
	printf("primljeno je %s\n",str);
}

void brisi(int stat)
{
	shmdt(c);
	shmctl(shmid,IPC_RMID,NULL);
	exit(stat);
}

int main()
{
	shmid=shmget(IPC_PRIVATE,sizeof(struct scommon),SHM_R|SHM_W);
	sigset(SIGINT,brisi);
	c=shmat(shmid,NULL,0);
	c->ulaz=0;
	c->buffer[0]='\0';
	
	sem_init(&(c->sem[0]),1,1);
	sem_init(&(c->sem[1]),1,1);
	sem_init(&(c->sem[2]),1,0);
	sem_init(&(c->sem[3]),1,1);

	if(fork()==0)
	{
		proizvodjac(1);
	}
	else if(fork()==0)
	{
		proizvodjac(2);
	}
	else
	{
		potrosac();
	}
	wait(NULL);
	//}
	wait(NULL);
	

	brisi(0);
	return 0;
}

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
  
#define N 5
#define MISLI 0
#define GLADAN 1
#define JEDE 2
#define LIJEVO (fil_br+4)%N
#define DESNO (fil_br+1)%N
  
sem_t mutex;
sem_t S[N];


void *filozof(void *br);
void uzima_zlicu(int);
void stavlja_zlicu(int);
void test(int);
  
int stanje[N]; 
int fil_br[N]={0,1,2,3,4};

int main()
{
	int i;
	pthread_t dretva_id[N];
	sem_init(&mutex,0,1);
	for(i=0;i<N;i++)
	{
		pthread_create(&dretva_id[i],NULL,filozof,&fil_br[i]);
		printf("filozof %d misli\n",i+1);
	}
	for(i=0;i<N;i++)
	{
		pthread_join(dretva_id[i],NULL);
	}
}

void *filozof(void *br)
{
	while(1)
	{
		int *i=br;
		sleep(1);
		uzima_zlicu(*i);
		sleep(0);
		stavlja_zlicu(*i);
	}
}

void uzima_zlicu(int fil_br)
{
	sem_wait(&mutex);
	stanje[fil_br]=GLADAN;
	printf("filozof %d je gladan\n",fil_br+1);
	test(fil_br);
	sem_post(&mutex);
	sem_wait(&S[fil_br]);
	sleep(1);
}

void test(int fil_br)
{
	if(stanje[fil_br]==GLADAN && stanje[LIJEVO] != JEDE && stanje[DESNO] != JEDE)
	{
		stanje[fil_br]=JEDE;
		sleep(2);
		printf("filozof %d uzima vilicu %d i %d\n",fil_br+1,LIJEVO+1,fil_br+1);
		printf("filozof %d jede\n",fil_br+1);
		sem_post(&S[fil_br]);
	}
}

void stavlja_zlicu(int fil_br)
{
	sem_wait(&mutex);
	stanje[fil_br]=MISLI;
	printf("filozof %d stavlja vilicu %d i %d na stol\n",fil_br+1,LIJEVO+1,fil_br+1);
	printf("filozof %d misli\n",fil_br+1);
	test(LIJEVO);
	test(DESNO);
	sem_post(&mutex);
}

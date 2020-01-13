#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int *pravo,*zastavica,id_p,id_z;

void kriticni_odsjecak(int i,int j)
{
	zastavica[i]=1;//p1 zeli  ulaz
	while(zastavica[j]!=0)//sve dok p2 zeli ulaz
	{
		if(*pravo==j)//ako p2 ima prednost
		{
			zastavica[i]=0;//p1 odustaje
			while(*pravo==j);//p1 ceka dok p2 zavrsi
			zastavica[i]=1;//p1 javlja da zeli ulaz
		}
	}
}

void izlaz_iz_ko(int i,int j)
{
	*pravo=j;//prednost dajemo p2
	zastavica[i]=0;//p1 izlazi
}

void izvrsi(int i,int j)
{
	int k,m;
	for(k=1;k<=5;k++)//ulaz u kriticni odsjecak
	{
		kriticni_odsjecak(i,j);
		for(m=1;m<=5;m++)//ispis
		{
			printf("Proces %i,kriticni odsjecak br: %i(%i/5)\n",i+1,k,m);
			sleep(1);//usporavanje
		}
		izlaz_iz_ko(i,j);
	}
}

void brisi()
{
	shmdt(pravo);
	shmdt(zastavica);
	shmctl(id_p,IPC_RMID,NULL);//kontrola zajednicke memorije,oznava segment koji se unistava
	shmctl(id_z,IPC_RMID,NULL);
	exit(0);
}

int main(int argc,char *argv[])
{
	id_p = shmget(IPC_PRIVATE,sizeof(int),SHM_R|SHM_W);//zauzimanje zajednicke memorije
	id_z = shmget(IPC_PRIVATE,sizeof(int)*2,SHM_R|SHM_W);
	pravo = shmat(id_p,NULL,0);//povezivanje procesa sa segmentom memorije
	zastavica = shmat(id_z,NULL,0);
	if(fork()==0)
	{
		izvrsi(0,1);
	}else{
		sigset(SIGINT,brisi);//ako se prekine izvrsavanje pozove se brisi da se pocisti
		izvrsi(1,0);
		wait(NULL);//ceka izvrsavanje djeteta
		brisi();
	}
	return 0;
}

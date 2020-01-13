#include<stdio.h>
#include<signal.h>
#include<stdlib.h>


void pokreniProces()
{
	printf("This is my PID %d\n",getpid());
	sleep(3);
}

void ispisiProcese()
{
	execl("/bin/ps","ps","-la",NULL);
}

void zavrsiProces(int pid)
{
	printf("Zavrsavam proces sa PID:%d\n",pid);
	kill(pid,SIGKILL);
}

void menu()
{
        printf("Odaberi opciju: \n -Pokreni proces: unesite 1,\n -Ispisi procese unesite 2,\n -Zavrsi proces unesite 3\n");
        int i;
        scanf("%d",&i);
	int pid;
	if(i==3){
	printf("Unesite PID procesa koji zelite unistiti\n");
	scanf("%d",&pid);
	}	
	switch(i)
	{
		case 1: 
		while(1){
		pokreniProces();
		}
		break;
		case 2:
		ispisiProcese();
		break;
		case 3:
		zavrsiProces(pid);
		break;
	}

}

int main(void)
{
	menu();
	return 0;
}

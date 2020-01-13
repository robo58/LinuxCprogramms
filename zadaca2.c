#include <stdio.h>
#include <string.h>

void ispisPrograma(void)
{
	printf("\nProgrami koje mozete pokrenuti: \n");
	system("find ./ -type f -perm -og+rx");
	printf("\n");
}

void main()
{
	char command[50],naredba[20];
	system("clear");
	printf("unesite program.Za listu programa unesite \"ispis\".Za izlaz unesite \"izlaz\".\n");
	do{
		printf("naredba : " );
		fflush(stdin);
		scanf("%s",&naredba);
		if(strcmp(naredba,"ispis")==0){
			ispisPrograma();
		}else if(strcmp(naredba,"izlaz")){
			sprintf(command,"exec ./%s",naredba);
			system(command);
		}
	}while(strcmp(naredba,"izlaz"));
}

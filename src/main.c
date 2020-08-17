/* Raphael Carvalho - RA: 205125
	Laboratório 04 - contador multiprocessos
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESS 4

/* funcao simples que detecta se um número eh ou nao primo.
* retorna 0 se nao primo
* retorna 1 se primo
*/

int prime_number(int number)
{
	int aux; //variavel auxiliar para calculo de numeros primos 

	if(number == 0 || number == 1)
 		return 0; // caso o numero seja 0 ou 1, considera-se nao primo

	/* loop para testar se num eh divisel por outro num alem dele mesmo */
	for(aux = 2; aux < number; aux++)	
	{
		if(number % aux == 0)
			return 0;
	}

	return 1;	// numero eh primo	
}

int main() 
{
	pid_t pid; // PID de cada processo
	int *shared_memory;	// memoria compartilhada entre processos
	int input = 0; // entrada do programa
	int child = 0; // contador do numero de filhos
	int i = 0; // variavel auxiliar
	

	/*Definir flags de protecao e visibilidade de memoria*/
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	/*Criação da memória compartilhada para o resultado*/
	shared_memory = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	
	do
	{
		if(child < MAX_PROCESS) // garantir apenas 4 processos paralelos
		{
			scanf("%d", &input); // ler a entrada numero a numero
			
			pid = fork(); // criar processos
			
			if(pid == 0)
			{
				(*shared_memory) += prime_number(input);
				exit(0); // encerra processo
			}
			
			child++; // contador do numero de processos paralelos
			
			if(child == 4) // agaurda um dos processos acabar quando ha 4 filhos
			{
				while (waitpid(pid, NULL, 0) > 0)
				{
					child--; 
				}	
			}
		}
	}while((input = getchar()) != '\n');
	
	/*espera todos os filhos acabarem*/
	while(wait(NULL) > 0);

	printf("%d\n", *shared_memory);

 	return 0;
}











/*------------------------------------------
		Definicao de constantes
--------------------------------------------*/

/*Constantes que definem o tamanho de vetores*/
#define		MAX_DESCRICAO				64		/*Numero max de char que definicao de um evento pode ter*/
#define		MAX_RESPONSAVEL				64		/*Numero max de char que o nome do responsavel pode ter*/
#define     MAX_UM_PARTICIPANTE    		64   	/*Numero max de char que o nome de UM participante pode ter*/
#define		MAX_PARTICIPANTES			193  	/*Numero max de char que o nome do responsavel pode ter 
													(63x3 + 3(':') + 1('\0')) */
#define		MAX_EVENTOS_SALA			100	 	/*NAO MEXER So podem haver 100 eventos por sala*/
#define 	TOTAL_SALAS					10	
#define     MAX_CARACTERES_INPUT    	337		/*Numero max de char que umalinha de input, sem a instrucao,
													pode ter*/
#define		MAX_EVENTOS 				1000	/*No total, so pode haver um total de 1000 eventos*/
#define     MAX_PARTICIPANTES_EVENTO    4		/*Numero maximo de participantes num evento, contando com o responsavel*/
#define		MIN_PARTICIPANTES_EVENTO	2		/*Numero minimo de participantes num evento, contando com o responsavel*/
#define 	MAX_VETOR_INDICES 			2 		/*Tamanho maximo do vetor indices[]*/

/*Numero das Salas*/
	/*Como estas constantes representam os indices de cada sala na tabela
		"agenda", sao (nr sala -1)*/
#define 	SALA1 						0
#define 	SALA2 						1
#define 	SALA3 						2
#define 	SALA4 						3
#define 	SALA5 						4
#define 	SALA6 						5
#define 	SALA7 						6
#define 	SALA8 						7
#define 	SALA9 						8
#define 	SALA10 						9

/*Constantes Booleanas*/
#define		FALSE						0
#define		TRUE						1

#define 	SALA_OCUPADA				-1		/*"Flag" de que a sala esta ocupada*/
#define 	ADICIONA					0		/*"Flag" de que podemos adicionar o evento a agenda*/

/*Numero do indice, no array usado na funcao DataMaiorQ, que representa o ano de um evento*/
#define     ANO                 		0		
/*Numero do indice, no array usado na funcao DataMaiorQ, que representa o mes de um evento*/
#define     MES                 		1
/*Numero do indice, no array usado na funcao DataMaiorQ, que representa o dia de um evento*/
#define     DIA                 		2

/*Numero do indice, no array usado em diversas funcoes, que representa a sala de um evento*/
#define     SALA                		0		
/*Numero do indice, no array usado em diversas funcoes, que representa a posicao de um evento, no array da sua sala*/
#define     EVENTO                 		1

#define 	INICIO_VETOR 				0		/*Posicao onde comecamos os algoritmos de ordenacao*/
#define     END_STR             		NULL	/*Caracter que indica o fim de uma string*/

/*-------------------------------------
			Estruturas
---------------------------------------*/

/*Um evento e definido pela sua descricao, dia, hora de inicio, duracao do evento, sala, nome do responsavel
pelo evento, e pelo nome dos seus participantes*/
typedef struct 
{
	char 			descricao[MAX_DESCRICAO];			/*Descricao do evento*/
	int 			data;								/*Dia do evento*/
	int 			tempo_ini; 							/*Hora de Inicio*/
	int 			duracao;							/*Duracao do evento*/
	int 			sala;								/*Sala*/
	char 			responsavel[MAX_RESPONSAVEL];       /*Nome do responsavel pelo evento*/
	char 			participantes[MAX_PARTICIPANTES];   /*String com o nome dos participantes, excluindo o responsavel*/
}Evento;

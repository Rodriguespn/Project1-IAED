/*==============================================================================================================

									PROJETO 1 IAED 2018/19

										Pedro Rodrigues
											n90531

===============================================================================================================*/
#include <stdio.h>
#include <string.h>

#include "constantes.h"
#include "funcoes.h"

/*-------------------------------------------------------------------
	Variaveis estaticas com o numero de eventos da respetiva sala
---------------------------------------------------------------------*/
static int eventos_sala1;
static int eventos_sala2;
static int eventos_sala3;
static int eventos_sala4;
static int eventos_sala5;
static int eventos_sala6;
static int eventos_sala7;
static int eventos_sala8;
static int eventos_sala9;
static int eventos_sala10;

/*==================================================================================

										MAIN

====================================================================================
E nesta funcao que corre o programa completo.

Observacoes:
	- Cada "evento" e do tipo struct e tem o nome de Evento;
	- Cada Evento e caracterizado pelos componentes descritos no ficheiro "constantes.h";
	- A agenda e uma tabela 10x100 de Eventos, onde o primeiro indice da tabela determina o (numero da
sala dos eventos - 1), e o segundo indice indica a posicao do evento, nessa sala.
P.ex. agenda[0][10] contem o decimo-primeiro evento da sala 1;
	- A agenda nao se encontra ordenada. A ordenacao e feita quando as instrucoes 's' e 'l' sao chamadas;
*/

/*A tabela de 10x100 onde vamos colocar os eventos, organizados por salas*/
Evento agenda[TOTAL_SALAS][MAX_EVENTOS_SALA];

int main()
{
	extern Evento agenda[][MAX_EVENTOS_SALA];
	char c;  	/*Caracter que indica a instrucao dado no input*/

	while ((c = getchar()) != 'x')
	{
		switch(c)
		{
			/*Instrucao "adiciona eventos"*/
			case 'a':
				Instrucao_adiciona_eventos(agenda);
			break;

			/*Instrucao "lista eventos por sala"*/
			case 's':
				Instrucao_lista_salas(agenda);
			break;

			/*Instrucao "lista todos eventos"*/
			case 'l':
				Instrucao_lista_eventos(agenda);
			break;

			/*Instrucao "muda hora de inicio"*/
			case 'i':
				Instrucao_altera_hora(agenda);
			break;

			/*Instrucao "muda duracao"*/
			case 't':
				Instrucao_altera_duracao(agenda);
			break;

			/*Instrucao "muda sala"*/
			case 'm':
				Instrucao_muda_sala(agenda);
			break;

			/*Instrucao "remove evento"*/
			case 'r':
				Instrucao_apaga_evento(agenda);
			break;

			/*Instrucao "adiciona participante"*/
			case 'A':
				Instrucao_acrescenta_participante(agenda);
			break;

			/*Instrucao "remove participante"*/
			case 'R':
				Instrucao_remove_participante(agenda);
			break;
		}
	}
	return 0;
}

/*=====================================================================================

									ADICIONA EVENTO
								
		Aqui estao as funcoes que definem a instrucao "a" (ou Adiciona Evento)
									
=======================================================================================

Adiciona o evento a agenda, caso seja possivel*/
void Instrucao_adiciona_eventos(Evento agenda[][MAX_EVENTOS_SALA])
{
	/*resposta = numero devolvido pela funcao AdicionaQ; nr_p_ocupados = comprimento do vetor
	"p_ocupados"*/
	int resposta, i, nr_p_ocupados;
	char input[MAX_CARACTERES_INPUT];

	/*String onde guardamos o nome dos participantes sobrepostos*/
	char p_ocupados[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE];

	Evento new_ev; /*Criamos um novo evento*/

	leInput(input);

	new_ev = Cria_evento(input);

	/*Verificamos se podemos adicionar o evento a agenda ou nao*/
	resposta = AdicionaQ(new_ev, agenda, p_ocupados);

	switch(resposta)
	{
		case ADICIONA:
			agenda[new_ev.sala-1][EscolheSala(new_ev.sala-1)] = new_ev;
			AumentaCapacidadeSala(new_ev.sala-1);
		break;

		case SALA_OCUPADA:
			printf("Impossivel agendar evento %s. Sala%d ocupada.\n", new_ev.descricao, new_ev.sala);	
		break;

		default:
			/*Se resposta nao for 0 nem 1, encontramos participantes sobrepostos, e o numero de participantes
			ocupados esta em "resposta"*/
			nr_p_ocupados = resposta; 

			for (i = 0; i < nr_p_ocupados; i++)
			{
				printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", new_ev.descricao,
					p_ocupados[i]);
			}
		break;
	}
}

/*Retorna o numero de participantes de "novo_evento" que tem eventos sobrepostos, e coloca-os em 
p_ocupados*/
int ParticipanteOcupado(Evento novo_evento, Evento array[], int nr_ev_sobrepostos, char p_ocupados[][MAX_UM_PARTICIPANTE])
{
	/*Tabelas por onde vao ser colocados os nomes dos participantes do evento*/
	char participantes1[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE];
	char participantes2[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE];
	int i, p1, p2;

	/*nr_p1 = Numero de participantes em "novo_evento"; nr_p2 = Numero de participantes em cada evento sobreposto*/ 
	int nr_p1 = ContaParticipantes(novo_evento, participantes1), nr_p_agenda; 
	int ocupado = FALSE; /*Antes de verificarmos nao existem participantes ocupados*/

	for (i = 0; i < nr_ev_sobrepostos; i++)
	{
		/*Se o evento que estamos a analisar nao e o proprio evento*/
		if (strcmp(novo_evento.descricao, array[i].descricao))
		{
			nr_p_agenda = ContaParticipantes(array[i], participantes2);

			for (p1 = 0; p1 < nr_p1; p1++)
			{
				for (p2 = 0; p2 < nr_p_agenda; p2++)
				{
					/*Se o nome dos participantes nos dois evento for igual, adicionamos a tabela de partc.
					ocupados e incrementamos o numero de ocupados*/
					if (!strcmp(participantes1[p1], participantes2[p2]))
					{
						strcpy(p_ocupados[ocupado++], participantes1[p1]);
						break;
					}
				}
			}
		}
	}
	return ocupado;
}

/*Devolve o numero de participantes em "new_ev" e coloca-os em tab*/
int ContaParticipantes(Evento new_ev, char tab[][MAX_UM_PARTICIPANTE])
{
	int conta = FALSE; /*No inicio o nr de participantes no evento e 0*/
	char *token;

	/*Para nao "estragar" a string de participantes do evento com strtok, copiamo la para uma
	string diferente*/
	char participantes_temp[MAX_PARTICIPANTES]; 

	strcpy(tab[conta++], new_ev.responsavel);
	
	strcpy(participantes_temp, new_ev.participantes);
	token = strtok(participantes_temp, ":");
	
	while(token != NULL)
	{
		strcpy(tab[conta++], token);
		token = strtok(NULL, ":");
	}
	return conta;
}

/*=================================================================================

								LISTA EVENTOS POR SALA
								
		Aqui estao as funcoes que definem a instrucao "s" (ou lista eventos por sala)
									
===================================================================================

A instrucao "lista eventos por sala" mostra todos os eventos de uma sala, por ordem cronologica.
Como a agenda esta organizada por salas, basta organizar o array de eventos da sala que queremos.
Esta instrucao usa uma funcao com as bases do algoritmo de ordenacao "QuickSort"*/

void Instrucao_lista_salas(Evento agenda[][MAX_EVENTOS_SALA])
{
	int sala;
	char input[MAX_CARACTERES_INPUT];

	leInput(input);

	sala = TransformaEmNumero(input)-1;
	OrdenaPorSala(agenda[sala], INICIO_VETOR, EscolheSala(sala)-1);

	ImprimeEvento(agenda[sala], EscolheSala(sala));
}

/*Ordena um array de eventos cronologicamente, usando os principios do algoritmo
de ordenacao "QuickSort"*/
void OrdenaPorSala(Evento a[], int inicio , int len)
{
	int i;
	if (len <= inicio) return;

	i = PartitionSala(a,inicio,len);
	OrdenaPorSala(a,inicio,i-1);
	OrdenaPorSala(a,i+1,len);
}

/*Funcao auxiliar usada no OrdenaPorSalas*/
int PartitionSala(Evento a[], int inicio, int len)
{
	Evento pivot;
	int i ,j;

	pivot = a[len];
	i = inicio-1;
	j = len;

	while(i < j)
	{
		/*Se o dia do evento, a esquerda do pivot, for maior que o do pivot OU o dia for o mesmo
		mas a hora de inicio e maior que a do pivot OU a data e a hora de inicio sao iguais mas a sala
		do evento, a esquerda do pivot, e maior que a do pivot, encontramos um evento fora do sitio*/
		while(DataMaior(pivot.data, a[++i].data) || (a[i].data == pivot.data && a[i].tempo_ini < pivot.tempo_ini));

		/*Se o dia do pivot for maior que o do que um evento a sua direita OU o dia for o mesmo
		mas a hora de inicio do pivot e maior OU a data e a hora de inicio sao iguais mas a sala
		do pivot e maior que a sala dum evento a sua direita, encontramos um evento fora do sitio*/
		while(DataMaior(a[--j].data, pivot.data) || (a[j].data == pivot.data && pivot.tempo_ini < a[j].tempo_ini))
		{
			if (j == len) /*Se j == limite inferior, chegamos ao final do vetor e queremos sair do ciclo*/
				break;
		}
		if(i < j) /*Se o i e o j ainda nao se cruzaram, trocamos a posicao de a[i] com a de a[j]*/
			Exchange(a, i, j);
	}
	/*Chegando ao final do ciclo, trocamos o pivot com a[i]*/
	Exchange(a, i, len);

	return i;
}

/*Imprime as strings correspondentes aos nomes dos participantes do evento*/
void ImprimeParticipantes(char string[], Evento evento)
{
	char *token;

	strcpy(string, evento.participantes);
	token = strtok(string, ":");

	while (token != END_STR)
	{
		putchar(' ');
		printf("%s", token);
		token = strtok(END_STR, ":");
	}
	putchar('\n');
}

/*Troca a posicao do Evento a[i] com a posicao do Evento a[j]*/
void Exchange(Evento a[], int i, int j)
{
	Evento swap;

	swap = a[i];
	a[i] = a[j];
	a[j] = swap;
}

/*=================================================================================

									LISTA EVENTOS
								
		Aqui estao as funcoes que definem a instrucao "l" (ou lista eventos)
									
===================================================================================
	
	A instrucao "lista eventos" mostra todos os eventos por ordem cronologica. Caso
os eventos acontecam na mesma data, a mesma hora, mostra primeiro o evento que tiver a sala
com o numero menor.
Como a agenda apenas esta orgaizada por salas, e podem haver eventos em datas defierentes, em salas diferentes
e necessario verificar os eventos de todas as salas em simultaneo. Para isso, copio os eventos todos da agenda
para um array de eventos e, de seguida, ordeno-os. 
Esta instrucao usa uma funcao com as bases do algoritmo de ordenacao "QuickSort"*/

void Instrucao_lista_eventos(Evento agenda[][MAX_EVENTOS_SALA])
{
	int len_ordenada; /*Comprimento da lista ordenada*/
	Evento agenda_ordenada[MAX_EVENTOS];

	getchar(); /*Por causa do caracter '\n' a seguir a 'l'*/
	
	len_ordenada = TransfereAgenda(agenda_ordenada, agenda);

	/*Ordenamos o array de eventos*/
	OrdenaTodos(agenda_ordenada, INICIO_VETOR, len_ordenada);

	ImprimeEvento(agenda_ordenada, ++len_ordenada);
}

/*Transfere todos os eventos da agenda, que e uma tabela, para um array de eventos e devolve o indice do
ultimo elemento do array*/
int TransfereAgenda(Evento array[], Evento agenda[][MAX_EVENTOS_SALA])
{
	int sala, evento, len=-1;

	for (sala = 0; sala < TOTAL_SALAS; sala++)
	{
		for (evento = 0; evento < EscolheSala(sala); evento++)
			array[++len] = agenda[sala][evento];
	}
	return len;
}

/*Ordena um array de eventos cronologicamente, usando os principios do algoritmo
de ordenacao "QuickSort"*/
void OrdenaTodos(Evento a[], int inicio , int len)
{
	int i;
	if (len <= inicio) return;

	i = PartitionTodos(a,inicio,len);
	OrdenaTodos(a,inicio,i-1);
	OrdenaTodos(a,i+1,len);
}

/*Funcao auxiliar usada no OrdenaTodos*/
int PartitionTodos(Evento a[], int inicio, int len)
{
	Evento pivot;
	int i ,j;

	pivot = a[len];
	i = inicio-1;
	j = len;

	while(i < j)
	{
		/*Se o dia do evento, a esquerda do pivot, for maior que o do pivot OU o dia for o mesmo
		mas a hora de inicio e maior que a do pivot OU a data e a hora de inicio sao iguais mas a sala
		do evento, a esquerda do pivot, e maior que a do pivot, encontramos um evento fora do sitio*/
		while(DataMaior(pivot.data, a[++i].data) || (a[i].data == pivot.data && a[i].tempo_ini < pivot.tempo_ini)
			|| (a[i].data == pivot.data && a[i].tempo_ini == pivot.tempo_ini && a[i].sala < pivot.sala));

		/*Se o dia do pivot for maior que o do que um evento a sua direita OU o dia for o mesmo
		mas a hora de inicio do pivot e maior OU a data e a hora de inicio sao iguais mas a sala
		do pivot e maior que a sala dum evento a sua direita, encontramos um evento fora do sitio*/
		while(DataMaior(a[--j].data, pivot.data) || (a[j].data == pivot.data && pivot.tempo_ini < a[j].tempo_ini)
			|| (a[j].data == pivot.data && pivot.tempo_ini == a[j].tempo_ini && pivot.sala < a[j].sala))
		{
			if (j == len) /*Se j == limite inferior, chegamos ao final do vetor e queremos sair do ciclo*/
				break;
		}
		if(i < j) /*Se o i e o j ainda nao se cruzaram, trocamos a posicao de a[i] com a de a[j]*/
			Exchange(a, i, j);
	}
	/*Chegando ao final do ciclo, trocamos o pivot com a[i]*/
	Exchange(a, i, len);

	return i;
}

/*Imprime os dados dos eventos (descricao, data, hora de inicio, etc...) do array de eventos,
no formato explicito no enunciado*/
void ImprimeEvento(Evento array[], int len_ordenada)
{
	int i;

	char participantes_temp[MAX_PARTICIPANTES]; /*Esta variavel e necessaria para imprimir os 
	participantes, sem alterar a string "participantes" da estrutura Evento*/

	for (i = 0; i < len_ordenada; i++)
	{
		printf("%s %08d %04d %d Sala%d %s\n", array[i].descricao, array[i].data,
		array[i].tempo_ini, array[i].duracao, array[i].sala, array[i].responsavel);
		putchar('*');
		ImprimeParticipantes(participantes_temp, array[i]);
	}
}

/*========================================================================================

								ALTERA HORA DE INICIO
								
		Aqui estao as funcoes que definem a instrucao "i" (ou altera hora de inicio)
									
==========================================================================================

	A instrucao "Altera Hora", se o evento existir, altera a hora de inicio desse evento
se a sala nao estiver ocupada a essa hora, ou se nenhum dos participantes estiver ocupado
a essa hora.
	Esta instrucao le a descricao do evento e a nova duracao, a partir do input, cria uma copia
desse evento, altera a duracao dessa copia para a nova duracao. Depois, com recurso a funcao Adiciona,
verifica se a copia pode ser inserida na agenda, ou seja, se a sala, nesse periodo, nao esta ocupada 
e se os seus participantes nao tem ja um evento a essa hora.
	Como a copia do evento tem a mesma descricao do evento, que ja se encontra na agenda, para
evitar a sobreposicao da copia com o evento original, quando encontrarmos o evento com a mesma descricao
que a copia, passamos esse evento a frente.*/

void Instrucao_altera_hora(Evento agenda[][MAX_EVENTOS_SALA])
{
	int i, nova_hora, sala, evento, resposta, nr_p_ocupados;

	/*Vetor onde vao ser guardados as "coordenadas", do evento que procuramos, na agenda*/
	int indices[MAX_VETOR_INDICES];

	/*Mesma variavel que na funcao "AdicionaQ"*/
	char p_ocupados[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE], descricao[MAX_DESCRICAO];
	Evento new_ev; /*Novo Evento para onde vamos copiar o evento que queremos modificar*/

	char input[MAX_CARACTERES_INPUT];

	leInput(input);

	nova_hora = leDescricao(input, descricao);

	/*Procuramos o evento com a descricao do input*/
	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		/*Duplicamos o evento que corresponde a descricao para podermos mudar-lhe a hora de inicio*/
		sala = indices[SALA];
		evento = indices[EVENTO];
		new_ev = agenda[sala][evento];
		new_ev.tempo_ini = nova_hora;

		resposta = AdicionaQ(new_ev, agenda, p_ocupados); 

		switch(resposta)
		{
			case ADICIONA:
				agenda[sala][evento].tempo_ini = nova_hora;
			break;

			case SALA_OCUPADA:
				printf("Impossivel agendar evento %s. Sala%d ocupada.\n", new_ev.descricao, new_ev.sala);	
			break;

			default:
				nr_p_ocupados = resposta;
				for (i = 0; i < nr_p_ocupados; i++)
				{
					printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", new_ev.descricao,
						p_ocupados[i]);
				}
			break;
		}
	}
	else
		printf("Evento %s inexistente.\n", descricao);
}

/*=================================================================================

									ALTERA DURACAO
								
		Aqui estao as funcoes que definem a instrucao "t" (ou altera duracao)
									
===================================================================================

	A instrucao "altera duracao", se o evento existir, altera a duracao desse evento
se a sala nao estiver ocupada a essa hora, ou se nenhum dos participantes estiver ocupado
a essa hora.

	Funciona de maneira semelhante a "Altera Hora", mas com a duracao, em vez das horas de inicio*/

void Instrucao_altera_duracao(Evento agenda[][MAX_EVENTOS_SALA])
{
	int i;
	int nova_duracao, sala, evento, resposta, nr_p_ocupados;

	/*Vetor onde vao ser guardados as "coordenadas", do evento que procuramos, na agenda*/
	int indices[MAX_VETOR_INDICES];

	/*Mesma variavel que na funcao "AdicionaQ"*/
	char p_ocupados[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE], descricao[MAX_DESCRICAO];
	Evento new_ev;

	char input[MAX_CARACTERES_INPUT]; /*Novo Evento para onde vamos copiar o evento que queremos modificar*/

	leInput(input);

	nova_duracao = leDescricao(input, descricao);

	/*Procuramos o evento com a descricao do input*/
	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		/*Duplicamos o evento que corresponde a descricao para podermos mudar-lhe a duracao*/
		sala = indices[SALA];
		evento = indices[EVENTO];
		new_ev = agenda[sala][evento];
		new_ev.duracao = nova_duracao;

		resposta = AdicionaQ(new_ev, agenda, p_ocupados);

		switch(resposta)
		{
			case ADICIONA:
				agenda[sala][evento].duracao = nova_duracao;
			break;

			case SALA_OCUPADA:
				printf("Impossivel agendar evento %s. Sala%d ocupada.\n", new_ev.descricao, new_ev.sala);	
			break;

			default:
				nr_p_ocupados = resposta;
				for (i = 0; i < nr_p_ocupados; i++)
				{
					printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", new_ev.descricao,
						p_ocupados[i]);
				}
			break;
		}
	}
	else
		printf("Evento %s inexistente.\n", descricao);
}

/*Devolve 1 se o evento com a descricao num dos argumentos estiver na agenda, colocando as coordenadas no
vetor "indices". Caso nao esteja, devolve 0.*/
int ProcuraEventoQ(char descricao[], Evento agenda[][MAX_EVENTOS_SALA], int indices[MAX_VETOR_INDICES])
{
	int sala, evento;

	/*Percorremos as salas todas*/
	for (sala = 0; sala < TOTAL_SALAS; sala++)
	{	
		/*Dentro das salas, percorremos os eventos*/
		for (evento = 0; evento < EscolheSala(sala) ; evento++)
		{
			/*Se a descricao for igual, encontramos o evento*/
			if (!strcmp(descricao, agenda[sala][evento].descricao))
			{
				/*Guardamos a sala e a posicao no array da sala do evento e acabamos a funcao por aqui,
				nao precisando de continuar a procurar na agenda*/
				indices[SALA] = sala;
				indices[EVENTO] = evento;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*=================================================================================

									MUDA SALA
								
		Aqui estao as funcoes que definem a instrucao "m" (ou muda sala)
														
===================================================================================

	A instrucao "muda sala", se o evento existir, altera a sala desse evento
se a sala nao estiver ocupada a essa hora.

	Funciona de maneira semelhante a "Altera Hora", mas com a sala, em vez das horas de inicio*/

void Instrucao_muda_sala(Evento agenda[][MAX_EVENTOS_SALA])
{
	int nova_sala, sala, evento, resposta;

	/*Vetor onde vao ser guardados as "coordenadas", do evento que procuramos, na agenda*/
	int indices[MAX_VETOR_INDICES];

	/*Mesma variavel que na funcao "AdicionaQ"*/
	char p_ocupados[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE], descricao[MAX_DESCRICAO];
	Evento new_ev;

	char input[MAX_CARACTERES_INPUT];

	leInput(input);

	nova_sala = leDescricao(input, descricao);

	/*Procuramos o evento com a descricao do input*/
	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		/*Duplicamos o evento que corresponde a descricao para podermos mudar-lhe a sala*/
		sala = indices[SALA];
		evento = indices[EVENTO];
		new_ev = agenda[sala][evento];
		new_ev.sala = nova_sala;

		resposta = AdicionaQ(new_ev, agenda, p_ocupados);

		switch(resposta)
		{
			case ADICIONA:
				nova_sala--; /*O indice correspondente a sala e (nr da sala - 1)*/
				agenda[nova_sala][EscolheSala(nova_sala)] = new_ev;
				AumentaCapacidadeSala(nova_sala);
				ApagaEventoDaAgenda(sala, evento, agenda);
			break;

			case SALA_OCUPADA:
				printf("Impossivel agendar evento %s. Sala%d ocupada.\n", new_ev.descricao, new_ev.sala);	
			break;
		}
	}
	else
		printf("Evento %s inexistente.\n", descricao);
}
/*Recebe um numero que corresponde ao numero (sala do evento-1) e decrementa o valor
da variavel estatica que contem o numero total de eventos nessa sala*/
void Diminui_capacidade_sala(int sala)
{
	switch (sala)
		{
			case SALA1:
				eventos_sala1--;
			break;

			case SALA2:
				eventos_sala2--;
			break;

			case SALA3:
				eventos_sala3--;
			break;

			case SALA4:
				eventos_sala4--;
			break;

			case SALA5:
				eventos_sala5--;
			break;

			case SALA6:
				eventos_sala6--;
			break;

			case SALA7:
				eventos_sala7--;
			break;

			case SALA8:
				eventos_sala8--;
			break;

			case SALA9:
				eventos_sala9--;
			break;

			case SALA10:
				eventos_sala10--;
			break;
		}
}

/*=================================================================================

									APAGA EVENTO
								
		Aqui estao as funcoes que definem a instrucao "r" (ou apaga eventos)

===================================================================================

	A instrucao "Apaga Evento" apaga o evento, com a mesma descricao que recebemos no input,
da nossa agenda. Caso o evento nao exista na agenda, imprime a mensagem 'Evento
<descricao> inexistente.'
	A maneria como esta instrucao apaga o evento da agenda e simples: Procuramos o evento, com
a mesma descricao do input, na nossa agenda e, se existir, guardamos as suas coordenadas da agenda.
De seguida, arrastamos todos os eventos dessa sala uma posicao para a esquerda, na agenda, comecando
na posicao do evento que queremos apagar, acabando por escrever "em cima" dele na agenda, 
apagando-o da mesma.*/


/*Recebe a agenda de eventos, le o input do teclado ou ficheiro, e remove o evento com a descricao do input
da agenda. Caso este evento nao exista, imprime a mensagem 'Evento <descricao> inexistente.'*/
void Instrucao_apaga_evento(Evento agenda[][MAX_EVENTOS_SALA])
{
	int sala, evento, indices[MAX_VETOR_INDICES];
	char descricao[MAX_DESCRICAO], input[MAX_CARACTERES_INPUT];

	leInput(input);
	leApenasDescricao(input, descricao); 

	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		/*Guardei as posicoes do evento em duas variaveis para tornar o codigo mais legivel*/
		sala = indices[SALA]; 		/*O numero da sala-1 do evento que procuramos fica guardado em sala*/
		evento = indices[EVENTO];	/*E a posicao, no array dessa sala, do evento fica guardado em evento*/

		ApagaEventoDaAgenda(sala, evento, agenda);
	}
	/*Se o evento nao existir*/
	else
		printf("Evento %s inexistente.\n", descricao);
}

/*Apaga o evento que se situa na posicao agenda[sala][nr_evento]*/
void ApagaEventoDaAgenda(int sala, int nr_evento, Evento agenda[][MAX_EVENTOS_SALA])
{
	int i;

	/*Sabendo onde o evento que queremos apagar esta, comecamos o ciclo "for" nessa posicao
	e movemos todos os eventos a sua direita, uma posicao para a esquerda, escrevendo em cima 
	do evento que queremos apagar, apagando o da agenda.*/
	for (i = nr_evento; i < EscolheSala(sala); i++)
		agenda[sala][i] = agenda[sala][i+1];

	/*De seguida, diminuimos o numero de eventos da sala onde acabamos de apagar o evento*/
	Diminui_capacidade_sala(sala);
}

/*Recebe uma string com a informacao do input e guarda a descricao do evento*/
void leApenasDescricao(char input[], char descricao[])
{
	char *token;

	token = strtok(input, ":"); 

	/*Guardamos a descricao*/
	strcpy(descricao, token);
}

/*=====================================================================================

								ACRESCENTA PARTICIPANTE
								
		Aqui estao as funcoes que definem a instrucao "A" (ou Acrescenta participante)
									
=======================================================================================

	A instrucao "Acrescenta Participante" procura o evento com a descricao igual a do input e, se
nao houver nenhum participante sobreposto, acrescentamos o participante

	Verificamos se o evento que procuramos esta na agenda. Se estiver, fazemos uma copia desse evento
e verificamos se esse evento ja tem 3 participantes. Se nao tiver, verificamos se o participante que
queremos adicionar ja esta no evento. Se nao estiver, adicionamo-lo a copia e percorremos a agenda
a procura de participantes em eventos sobrepostos a nossa copia. Analisamos, um a um, os participantes
sobrepostos e comparamo-los com o participante que queremos adicinar. Se algum dos nomes for igual
o participante esta ocupado nessa data e nao pode ser adicionado ao evento. Se nao o econtrarmos
nos participantes ocupados, adicionamo-lo ao evento.*/

void Instrucao_acrescenta_participante(Evento agenda[][MAX_EVENTOS_SALA])
{
	/*nr_part = Nr de participantes */
	int i, sala, evento, resposta, nr_p_ocupados, nr_part;

	/*Variavel que nos indica se encontramos o participante no evento ou nao*/
	int found = FALSE;
	int indices[MAX_VETOR_INDICES];

	/*novo_participante = string onde guardamos o nome do participante que queremos adicionar*/
	char novo_participante[MAX_UM_PARTICIPANTE], p_ocupados[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE], descricao[MAX_DESCRICAO];

	/*Tabela onde vao ficar o nome dos participantes do evento*/
	char tab[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE];
	Evento new_ev;

	char input[MAX_CARACTERES_INPUT];

	leInput(input);
	leDescEParticipante(input, descricao, novo_participante);

	/*Procuramos o evento com a descricao do input*/
	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		/*Duplicamos o evento que corresponde a descricao para podermos acrescentar-lhe o participante*/
		sala = indices[SALA];
		evento = indices[EVENTO];
		new_ev = agenda[sala][evento];

		/*Se o numero de participantes do evento nao for maior que o maximo*/
		if ((nr_part = ContaParticipantes(new_ev, tab)) < MAX_PARTICIPANTES_EVENTO)
		{
			/*Se o novo participante ainda nao estiver no evento*/
			if (!ParticipanteNoEvento(tab, novo_participante, nr_part))
			{
				AcrescentaParticipante(new_ev.participantes, novo_participante);
				resposta = AdicionaQ(new_ev, agenda, p_ocupados);
				
				switch(resposta)
				{
					case ADICIONA:
						AcrescentaParticipante(agenda[sala][evento].participantes, novo_participante);
					break;

					default:
						nr_p_ocupados = resposta;
						for (i = 0; i < nr_p_ocupados; i++)
						{
							/*Se strcmp == 0, encontramos o novo_participante nos participantes ocupados*/
							if (!strcmp(p_ocupados[i], novo_participante))
							{
								found++;
								printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", 
									novo_participante);
								break;
							}
						}
						/*Se nao encontramos o participante na tabela de participantes ocupados*/
						if (!found)
							AcrescentaParticipante(agenda[sala][evento].participantes, novo_participante);
					break;
				}
			}
		}
		else
			printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", descricao);
	}
	else
		printf("Evento %s inexistente.\n", descricao);
}

/*Junta a string "participantes_atuais" a string ':"participante"'*/
void AcrescentaParticipante(char participantes_atuais[], char participante[])
{
	strcat(participantes_atuais, ":");
	strcat(participantes_atuais, participante);
}

/*Coloca a descricao do evento em "descricao" e o nome do participante em "particiipante"*/
void leDescEParticipante(char input[], char descricao[], char participante[])
{
	char *token;

	token = strtok(input, ":"); 

	/*Guardamos a descricao*/
	strcpy(descricao, token);

	token = strtok(NULL, ":");
	/*Guardamos o participante*/
	strcpy(participante, token);
}

/*A partir dos dados do input, cria um evento novo*/
Evento Cria_evento(char input[])
{
	char *token;
	Evento novo_evento;

	/*Lemos a descricao*/
	token = strtok(input, ":");
	strcpy(novo_evento.descricao, token);

	/*Lemos a data*/
	token = strtok(NULL, ":");
	novo_evento.data = TransformaEmNumero(token);  

	/*Lemos o tempo de inicio*/
	token = strtok(NULL, ":");
	novo_evento.tempo_ini = TransformaEmNumero(token);

	/*Lemos a duracao do evento*/
	token = strtok(NULL, ":");
	novo_evento.duracao = TransformaEmNumero(token);

	/*Lemos a sala*/
	token = strtok(NULL, ":");
	novo_evento.sala = TransformaEmNumero(token);

	/*Lemos o responsavel*/
	token = strtok(NULL, ":");
	strcpy(novo_evento.responsavel, token);

	/*Lemos os participantes*/
	token = strtok(NULL, "\0");
	strcpy(novo_evento.participantes, token);

	return novo_evento;
}

/*Trnsforma uma string de inteiros num numero inteiro*/
int TransformaEmNumero(char linha[])
{
	int i, num = 0;
	for (i = 0; linha[i] != '\0' ; i++)
		num = num * 10 + (linha[i] - '0');
	return num;
}

/*Adiciona a um array de Eventos os eventos na agenda que acontecem no mesmo dia, a mesma 
hora que o novo_evento*/
int EventosAMesmaHora(Evento novo_evento, Evento agenda[][MAX_EVENTOS_SALA], Evento eventos_sobrepostos[])
{
	int sala, evento, nr_ev_sobrepostos = 0;
	for(sala = 0; sala < TOTAL_SALAS; sala++)
	{
		for (evento = 0; evento < EscolheSala(sala)+1; evento++)
		{
			if (novo_evento.data == agenda[sala][evento].data && MesmaHora(novo_evento, agenda[sala][evento]))
				eventos_sobrepostos[nr_ev_sobrepostos++] = agenda[sala][evento];
		}
	}
	return nr_ev_sobrepostos;
}

/*=====================================================================================

									REMOVE PARTICIPANTE
								
		Aqui estao as funcoes que definem a instrucao "R" (ou Remove participante)
															
=======================================================================================

	A instrucao "remove participante" procura o evento com a descricao igual a do input e, se
nao houver participantes sobrepostos e o participante que queremos remover nao ser o unico do evento
, mudamos a duracao do evento.
	Procuramos o evento com a descricao desejada, vemos se tem apenas um participante E se e o
participante que queremos remover. Caso contrario colocamos os participantes, cujo nome e diferente
do participante que queremos remover. No final, copiamos esta nova string para a string de participantes
do evento*/

void Instrucao_remove_participante(Evento agenda[][MAX_EVENTOS_SALA])
{
	int i;

	/*nr_p_agenda = nr de partic. no evento da agenda que estamos a analisar*/
	int sala, evento, nr_p_agenda;

	/*Vetor onde vao ser guardadas as "coordenadas" do evento que queremos ir buscar, na agenda*/
	int indices[MAX_VETOR_INDICES];

	char input[MAX_CARACTERES_INPUT], descricao[MAX_DESCRICAO], participante[MAX_UM_PARTICIPANTE],
	 	tab[MAX_PARTICIPANTES_EVENTO][MAX_UM_PARTICIPANTE], participantes_temp[MAX_PARTICIPANTES] = "";

	leInput(input);
	leDescEParticipante(input, descricao, participante);

	if (ProcuraEventoQ(descricao, agenda, indices))
	{
		sala = indices[SALA]; 		/*O numero da sala-1 do evento que procuramos fica guardado em sala*/
		evento = indices[EVENTO];	/*E a posicao, no array dessa sala, do evento fica guardado em evento*/
		
		/*Se o participante estiver no evento e for o unico participante*/
		if ((nr_p_agenda = ContaParticipantes(agenda[sala][evento], tab)) == MIN_PARTICIPANTES_EVENTO
			&& ParticipanteNoEvento(tab, participante, nr_p_agenda))
			printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", participante, descricao);
		else
		{
			/*Na posicao 0 esta o responsavel, que nao queremos eliminar*/
			for (i = 1; i < nr_p_agenda; i++)
			{
				if (strcmp(tab[i], participante))
					AcrescentaParticipante(participantes_temp, tab[i]);
			}
			strcpy(agenda[sala][evento].participantes, participantes_temp);
		}
	}
	/*Se nao houver o evento pedido*/
	else
		printf("Evento %s inexistente.\n", descricao);
}

int ParticipanteNoEvento(char tab[][MAX_UM_PARTICIPANTE], char participante[], int len)
{
	int i;

	for (i = 1; i < len; ++i)
	{
		if (!strcmp(tab[i], participante))
			return TRUE;
	}
	return FALSE;
}
/*------------------------------------------------------
Funcoes reutilizadas
--------------------------------------------------------*/
/*Guarda uma linha de input do teclado num vetor de char*/
void leInput(char string[])
{
	int i;
	char c;

	getchar(); /*Necessario devido ao caracter ' ' entre a letra da instrucao e a informacao da
	intrucao*/
	
	for (i = 0; (c = getchar()) != '\n'; i++) /*Percorre o input ate encontrar um ':'*/
		string[i] = c;
	string[i] = '\0';
}

/*Recebe dois eventos e devolve 1 se acontecerem a mesma hora e 0 caso contrario.

Para dois eventos acontecerem a mesma hora, o tempo de inicio de ambos precisa de ser igual
OU a hora de inicio do evento1 esta contida dentro do intervalo ]ini_ev2, fim_ev2[ OU 
a hora de fim do evento1 esta contida dentro do intervalo ]ini_ev2, fim_ev2[ OU 
a hora de inicio do evento2 esta contida dentro do intervalo ]ini_ev1, fim_ev1[ OU 
a hora de fim do evento2 esta contida dentro do intervalo ]ini_ev1, fim_ev1[.*/
int MesmaHora(Evento evento1, Evento evento2)
{
	/*Converte a duracao do evento1 de minutos para o formato hhmm*/
	int duracao_ev1 = ConverteMinutosHoras(evento1.duracao);
	/*Determina a hora do fim de evento1*/
	int fim_ev1 = SomaHoras(evento1.tempo_ini, duracao_ev1);
	/*Converte a duracao do evento2 de minutos para o formato hhmm*/
	int duracao_ev2 = ConverteMinutosHoras(evento2.duracao);
	/*Determina a hora do fim de evento2*/
	int fim_ev2 = SomaHoras(evento2.tempo_ini, duracao_ev2);

	if (evento1.tempo_ini == evento2.tempo_ini 
		|| ContemIntervalo(evento1.tempo_ini, evento2.tempo_ini, fim_ev2)
		|| ContemIntervalo(fim_ev1, evento2.tempo_ini, fim_ev2) 
		|| ContemIntervalo(evento2.tempo_ini, evento1.tempo_ini, fim_ev1) 
		|| ContemIntervalo(fim_ev2, evento1.tempo_ini, fim_ev1))
		return TRUE;
	else
		return FALSE;
}

/*Devolve 1 se x pertencer ao intervalo ]inicio, fim[ e 0 caso contrario*/
int ContemIntervalo(int x, int inicio, int fim)
{
	if (x > inicio && x < fim)
		return TRUE;
	return FALSE;
}

/*Recebe um numero inteiro entre 1 e 1440 e devolve as horas e minutos correspondentes,
no formato hhmm*/
int ConverteMinutosHoras(int tempo)
{
	int horas = tempo/60, minutos = tempo%60;
	return horas*100 + minutos;
}

/*Recebe duas horas no formato hhmm e devolve a soma delas tambem no formato hhmm.
Para isto, isolamos os dois digitos mais significativos, referentes as horas, e os dois digitos
menos significativos, referentes aos minutos. Somamos as horas de um com as horas do outro e
o mesmo com os minutos. Se a soma dos minutos for maior ou igual a 60, somamos mais uma hora ao
resultado final*/
int SomaHoras(int hora1, int hora2)
{
	int soma = 0, hora = (hora1/100 + hora2/100)*100, minutos = hora1%100 +  hora2%100;
	soma += hora + minutos%60;
	if (minutos >= 60)
		soma += 100;
	return soma;
}

/*Devolve o numero total de eventos que a sala, dada como argumento, tem*/
int EscolheSala(int sala)
{
	switch(sala)
	{
			case SALA1:
				return eventos_sala1;
			break;

			case SALA2:
				return eventos_sala2;
			break;

			case SALA3:
				return eventos_sala3;
			break;

			case SALA4:
				return eventos_sala4;
			break;

			case SALA5:
				return eventos_sala5;
			break;

			case SALA6:
				return eventos_sala6;
			break;

			case SALA7:
				return eventos_sala7;
			break;

			case SALA8:
				return eventos_sala8;
			break;

			case SALA9:
				return eventos_sala9;
			break;

			default:
				return eventos_sala10;
			break;
	}
}

/*Recebe um numero que corresponde ao numero (sala do evento-1) e incrementa o valor
da variavel estatica que contem o numero total de eventos nessa sala*/
void AumentaCapacidadeSala(int sala)
{
	switch(sala)
	{
			case SALA1:
				eventos_sala1++;
			break;

			case SALA2:
				eventos_sala2++;
			break;

			case SALA3:
				eventos_sala3++;
			break;

			case SALA4:
				eventos_sala4++;
			break;

			case SALA5:
				eventos_sala5++;
			break;

			case SALA6:
				eventos_sala6++;
			break;

			case SALA7:
				eventos_sala7++;
			break;

			case SALA8:
				eventos_sala8++;
			break;

			case SALA9:
				eventos_sala9++;
			break;

			default:
				eventos_sala10++;
			break;

	}
}

/*Guarda a descricao do evento num vetor de char e devolve o numero correspondente a duracao do evento, em minutos*/
int leDescricao(char input[], char descricao[])
{
	char *token;

	token = strtok(input, ":"); 

	/*Guardamos a descricao*/
	strcpy(descricao, token);

	token = strtok(NULL, ":");

	/*Devolvemos o numero correspondente a duracao, e nao o seu codigo ASCII*/
	return TransformaEmNumero(token);
}

/*Devolve 1 se dia1 for maior que dia2 ou 0, caso contrario

Data1 e maior que data2 se o ano de data1 maior que data2 OU ano1 == ano2 E mes de data1 maior que 
mes de data2 OU ano1 == ano2 E mes1 == mes2 E dia de data1 maior que data2.*/
int DataMaior(int dia1, int dia2)
{
	/*A informacao das datas recebidas e guardada num array de 
	inteiros (dataX) com a seguinte estrutura:
			dataX[0] = ano
			dataX[1] = mes
			dataX[2] = dia*/

	int data1[3], data2[3];

	data1[ANO] = dia1%10000;
	dia1 /= 10000;
	data1[MES] = dia1%100;
	data1[DIA] = dia1/100;

	data2[ANO] = dia2%10000;
	dia2 /= 10000;
	data2[MES] = dia2%100;
	data2[DIA] = dia2/100;

	if (data1[ANO] >  data2[ANO] || (data1[ANO] == data2[ANO] && data1[MES] > data2[MES])
		|| (data1[ANO] == data2[ANO] && data1[MES] == data2[MES] && data1[DIA] > data2[DIA]))
		return TRUE;
	else 
		return FALSE;
}

/*Devolve 1 se a sala de novo_evento for igual a alguma sala dos eventos do array
eventos_sobrepostos*/
int SalaOcupada(Evento novo_evento, Evento eventos_sobrepostos[], int len_array)
{
	int i;

	for (i = 0; i < len_array; ++i)
	{
		if (strcmp(novo_evento.descricao, eventos_sobrepostos[i].descricao) 
			&& novo_evento.sala == eventos_sobrepostos[i].sala)
			return TRUE;
	}
	return FALSE;
}

/*Retorna 0 se podemos adicionar o evento na agenda, -1 se a sala ja esta ocupada ou um numero entre 1
e 4, que representa o numero de participantes ocupados*/
int AdicionaQ(Evento novo_evento, Evento agenda[][MAX_EVENTOS_SALA], char p_ocupados[][MAX_UM_PARTICIPANTE])
{
	/*Array com os eventos na mesma data que "novo_evento"*/
	Evento eventos_sobrepostos[MAX_EVENTOS];

	/*nr_ev_sobrepostos = comprimento do vetor "eventos_sobrepostos"; nr_ocupado = comprimento do vetor
	"p_ocupados", dado como argumento*/
	int nr_ev_sobrepostos, nr_ocupado; 

	nr_ev_sobrepostos = EventosAMesmaHora(novo_evento, agenda, eventos_sobrepostos);

	if (SalaOcupada(novo_evento, eventos_sobrepostos, nr_ev_sobrepostos))
		return SALA_OCUPADA; /*A sala ja esta ocupada*/

	else if ((nr_ocupado = ParticipanteOcupado(novo_evento, eventos_sobrepostos, nr_ev_sobrepostos, p_ocupados)))
	{

		return nr_ocupado; /*Ha participantes nesse evento que ja estao ocupados*/
	}
	else
		return ADICIONA;
}

/*------------------------
	Funcoes usadas
--------------------------*/

/*Adiciona Evento*/
Evento Cria_evento(char input[]);
int TransformaEmNumero(char linha[]);
int EventosAMesmaHora(Evento novo_evento, Evento agenda[][MAX_EVENTOS_SALA], Evento eventos_sobrepostos[]);
void Instrucao_adiciona_eventos(Evento agenda[][MAX_EVENTOS_SALA]);
int ParticipanteOcupado(Evento novo_evento, Evento array[], int nr_ev_sobrepostos, char p_ocupados[][MAX_UM_PARTICIPANTE]);
int ContaParticipantes(Evento new_ev, char tab[][MAX_UM_PARTICIPANTE]);

/*Muda duracao*/
void Instrucao_altera_duracao(Evento agenda[][MAX_EVENTOS_SALA]);
int ProcuraEventoQ(char descricao[], Evento agenda[][MAX_EVENTOS_SALA], int indices[2]);

/*Muda hora*/
void Instrucao_altera_hora(Evento agenda[][MAX_EVENTOS_SALA]);

/*Muda sala*/
void Instrucao_muda_sala(Evento agenda[][MAX_EVENTOS_SALA]);
void Diminui_capacidade_sala(int sala);

/*Lista Salas*/
void Instrucao_lista_salas(Evento agenda[][MAX_EVENTOS_SALA]);
void OrdenaPorSala(Evento a[], int inicio , int len);
int PartitionSala(Evento a[], int inicio, int len);
void ImprimeParticipantes(char string[], Evento evento);
void Exchange(Evento a[], int i, int j);

/*Apaga Evento*/
void Instrucao_apaga_evento(Evento agenda[][MAX_EVENTOS_SALA]);
void ApagaEventoDaAgenda(int sala, int nr_evento, Evento agenda[][MAX_EVENTOS_SALA]);
void leApenasDescricao(char input[], char descricao[]);

/*Lista_todos_eventos*/
void Instrucao_lista_eventos(Evento agenda[][MAX_EVENTOS_SALA]);
int TransfereAgenda(Evento array[], Evento agenda[][MAX_EVENTOS_SALA]);
void OrdenaTodos(Evento a[], int inicio , int len);
int PartitionTodos(Evento a[],int inicio ,int len);
void ImprimeEvento(Evento array[], int len_ordenada);


/*Acrescenta Participante*/
void Instrucao_acrescenta_participante(Evento agenda[][MAX_EVENTOS_SALA]);
void AcrescentaParticipante(char participantes_atuais[], char participante[]);
void leDescEParticipante(char input[], char descricao[], char participante[]);

/*Remove Participante*/
void Instrucao_remove_participante(Evento agenda[][MAX_EVENTOS_SALA]);
int ParticipanteNoEvento(char tab[][MAX_UM_PARTICIPANTE], char participante[], int len);

/*Funcoes reutilizadas*/
void leInput(char string[]);
int MesmaHora(Evento novo_evento, Evento evento_agenda);
int ContemIntervalo(int x, int inicio, int fim);
int ConverteMinutosHoras(int tempo);
int SomaHoras(int hora1, int hora2);
int EscolheSala(int sala);
void AumentaCapacidadeSala(int sala);
int leDescricao(char input[], char descricao[]);
int DataMaior(int dia1, int dia2);
int SalaOcupada(Evento novo_evento, Evento eventos_sobrepostos[], int len_array);
int AdicionaQ(Evento novo_evento, Evento agenda[][MAX_EVENTOS_SALA], char p_ocupados[][MAX_UM_PARTICIPANTE]);

#ifndef AEROPORTO_H
#define AEROPORTO_H

#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include "aviao.h"
#include "fila.h"

typedef size_t tempo_t;

typedef struct {
	size_t n_pistas;
	size_t n_portoes;
	size_t n_esteiras;
	size_t n_max_avioes_esteira;
	tempo_t t_pouso_decolagem;
	tempo_t t_remover_bagagens;
	tempo_t t_inserir_bagagens;
	tempo_t t_bagagens_esteira;
	tempo_t t_aproximacao_aero;

	// Ponteiros para varias pistas e portoes
	// Cada pista e portao só pode ter um aviao associado
	pthread_mutex_t *pistas, *portoes;

	// Semaforo para indicar os portoes livres antes de
	// um aviao pegar um portao
	sem_t portoesLivres;

	// Ponteiros para varias esteiras
	// Cada esteira pode ser associado com "nMax" avioes
	sem_t *esteiras;

	// Filas para as pistas do aeroporto e semaforo para indicar
	// se alguma destas pistas esta livre para pouso

	// PONTEIRO PARA PONTEIROS// TA ERRADU	
	fila_ordenada_t **filasPousoDecolagem;
	sem_t pistasLivres;
} aeroporto_t;

aeroporto_t* iniciar_aeroporto (size_t* args);
size_t aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao);
//Funções auxiliares da funcão aproximação aeroporto.
size_t aproximarNaMelhorFila (aeroporto_t *aeroporto, aviao_t *aviao, size_t *index);
size_t acharFilaComMenosAvioes(aeroporto_t *aeroporto);
void trancaTodasFilas(aeroporto_t *aeroporto);
void liberaTodasFilas(aeroporto_t *aeroporto);

// O idFilaDe aproximação corresponde ao id de uma fila de avioes que
// aguarda para pousar em uma pista expecifica, e o id desta pista corresponde
// também ao IdFilaDeAproximacao. Assim ao ser chamada, esta funcao
// faz lock do mutex correspondente ao IdfilaDeAproximacao do array
// de mutexes. Feito assim para otimizar, melhor do que
// ficar verificando mutex por mutex para ver qual esta livre.
// E para nao implementar algo simples como somente passar pelo semaforo
// de pistas que é inicializado com o numero de pistas
void pousar_aviao (aeroporto_t* aeroporto, size_t idAviao, size_t idFilaAproximacao);

/**
 * Esta função deve acoplar um avião a um portão de embarque livre.
 * Ela não deve ser chamada caso não hajam portões livres. Em seguida, o
 * o avião deve transportar suas bagagens para fora e receber novas bagagens.
 * Um avião não pode decolar sem receber novas bagagens.
 **/
void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve levar as bagagens de um avião para uma esteira e
 * colocar novas bagagens num avião para que ele seja liberado para
 * decolagem. Um avião não pode decolar sem receber novas bagagens.
 * Um avião não pode receber novas bagagens sem colocar as antigas
 * em uma esteira.
 **/
void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve colocar novas bagagens numa esteira, caso ela
 * esteja disponível. A esteira ficará ocupada enquanto houverem
 * bagagens nela.
 **/
void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve fazer com que um avião decole, caso haja
 * uma pista disponível para tal. Um avião que decolar deve
 * ter sua execução terminada.
 **/
void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve desalocar todos os recursos previamente
 * alocados pelo aeroporto. Retorna 1 caso bem sucedido.
 **/
int finalizar_aeroporto (aeroporto_t* aeroporto);

#endif

/**
 * \brief Definição do escalonador.
 *
 * O escalonador do projeto é não preemptivo com prioridades dinâmicas.
 * A máquina de estados das threads contém cinco estados, sendo estes:
 * criação, apto, executando, bloqueado e término. A prioridade das
 * threads dentro de cada fila é definida pelo tempo que que a thread
 * passou no estado executando, onde valores menores representam maior
 * prioridade.
 *
 * @author jfguimaraes
 */

#ifndef LIBCTHREAD_A_SCHEDULER_H
#define LIBCTHREAD_A_SCHEDULER_H

#include "support.h"
#include "cdata.h"

/*--------------------------------------------------------------------
Função: Insere um nodo na lista indicada, segundo o campo "prio" do TCB_t
	A fila deve estar ordenada (ou ter sido construída usado apenas essa funcao)
	O primeiro elemento da lista (first) é aquele com menor valor de "prio"
Entra:	pfila -> objeto FILA2
	pnodo -> objeto a ser colocado na FILA2
Ret:	==0, se conseguiu
	!=0, caso contrário (erro)
--------------------------------------------------------------------*/
// TODO Verificar se precisa expôr esta função, se não remover a declaração
// int	InsertByPrio(PFILA2 pfila, TCB_t *tcb);

/**
 * \brief Retorna um ponteiro para o contexto da função de processo finalizado.
 *
 * Usada para dar acesso read-only à função ccreate, que cria novos processos.
 * @return Ponteiro para o contexto da função de processo finalizado - deve ser inicializado antes.
 */
ucontext_t *GetFinishProcessContext();

/**
 * \brief Retorna um novo identificador para adição de uma thread.
 *
 * A variável new_id é incrementada e devolvida, garantindo que seja retornado
 * um número diferente e maior do que zero, pois o identificador zero é exclusivo
 * da função main.
 *
 * @return Novo identificador de thread - nunca se repete.
 */
int GetNewId();

/**
 * \brief Retorna um ponteiro para a thread em execução.
 *
 * @return Ponteiro para a thread em execução.
 */
TCB_t *GetExecutingThread();

/**
 * \brief Limpa o ponteiro para a thread em execução.
 */
void SetExecutingThreadToNull();

/**
 * \brief Módulo responsável por colocar a próxima thread da fila de aptos em execução.
 */
void Dispatcher();

/**
 * \brief Inicializa o escalonador salvando o contexto da função main.
 *
 * A função verifica se o escalonador já foi inicializado, se sim a função retorna 1, se não
 * executa a inicialização e retorna 0.
 *
 * @return Retorna 0 se obteve sucesso, retorna 1 se já ocorreu a inicialização e um valor negativo em caso de erro.
 */
int InitScheduler();

/**
 * \brief Adiciona uma nova thread à lista de aptos.
 *
 * @param new_thread Nova thread a ser adicionada à lista.
 * @return Retorna 0 se obteve sucesso, retorna um valor negativo em caso de erro.
 */
int IncludeInReadyList(TCB_t *new_thread);

#endif //LIBCTHREAD_A_SCHEDULER_H
/**
 * \brief Implementação do escalonador.
 *
 * O escalonador do projeto é não preemptivo com prioridades dinâmicas.
 * A máquina de estados das threads contém cinco estados, sendo estes:
 * criação, apto, executando, bloqueado e término. A prioridade das
 * threads dentro de cada fila é definida pelo tempo que que a thread
 * passou no estado executando, onde valores menores representam maior
 * prioridade.
 */

#include <stdlib.h>
#include <ucontext.h>

#include "../include/scheduler.h"

/// Constantes que indicam o estado de inicialização do escalonador
#define SCHEDULER_NOT_INITIALIZED 0
#define SCHEDULER_INITIALIZED 1

/// Indica se o escalonador já está inicializado
static int isInitialized = SCHEDULER_NOT_INITIALIZED;

/// Contexto da função de finalização de processos
ucontext_t finish_thread_context;

/// Variável usada para geração de novo indicador de thread
static int new_id = 0;

/// Fila de processos no estado apto.
static FILA2 ready_list;

/// Fila de processos no estado bloqueado.
static FILA2 blocked_list;

/// Processo em execução.
static TCB_t *executing_thread = NULL;

/**
 * \brief Retorna um ponteiro para o contexto da função de processo finalizado.
 *
 * Usada para dar acesso read-only à função ccreate, que cria novos processos.
 * @return Ponteiro para o contexto da função de processo finalizado - deve ser inicializado antes.
 */
ucontext_t *GetFinishProcessContext() {
    return &finish_thread_context;
}

/**
 * \brief Retorna um novo identificador para adição de uma thread.
 *
 * A variável new_id é incrementada e devolvida, garantindo que seja retornado
 * um número diferente e maior do que zero, pois o identificador zero é exclusivo
 * da função main.
 *
 * @return Novo identificador de thread - nunca se repete.
 */
int GetNewId() {
    return ++new_id;
}

/**
 * \brief Retorna um ponteiro para a thread em execução.
 *
 * @return Ponteiro para a thread em execução.
 */
TCB_t *GetExecutingThread() {
    return executing_thread;
}

/**
 * \brief Limpa o ponteiro para a thread em execução.
 */
void SetExecutingThreadToNull() {
    executing_thread = NULL;
}

/**
 * \brief Indica se a lista de aptos está vazia.
 * @return Se a lista estiver vazia retorna 1, do contrário retorna 0.
 */
int IsReadyListEmpty() {
    return FirstFila2(&ready_list) != 0 ? 1 : 0;
}

/**
 * \brief Indica se a lista de bloqueados está vazia.
 * @return Se a lista estiver vazia retorna 1, do contrário retorna 0.
 */
int IsBlockedListEmpty() {
    return FirstFila2(&blocked_list) != 0 ? 1 : 0;
}

/**
 * \brief Insere um nodo na lista indicada segundo o campo "prio" do TCB_t.
 *
 * A fila deve estar ordenada, o que pode ser garantido utilizando somente essa função
 * para manipulá-la. O primeiro elemento da lista é o de maior prioridade, ou seja, o
 * que tem o menor valor no campo "prio".
 *
 * @param pfila Ponteiro para uma fila do tipo FILA2.
 * @param tcb Nodo representando a thread a ser inserida na fila.
 * @return Retorna 0 se obteve sucesso, outro valor em caso de falha.
 */
static int InsertByPrio(PFILA2 pfila, TCB_t *tcb) {
	TCB_t *tcb_it;
	
	// pfila vazia?
	if (FirstFila2(pfila) == 0) {
		do {
			tcb_it = (TCB_t *) GetAtIteratorFila2(pfila);
			if (tcb->prio < tcb_it->prio)
				return InsertBeforeIteratorFila2(pfila, tcb);
		} while (NextFila2(pfila) == 0);
	}

	return AppendFila2(pfila, (void *) tcb);
}

/**
 * \brief Adiciona uma nova thread à lista de aptos.
 *
 * @param new_thread Nova thread a ser adicionada à lista.
 * @return Retorna 0 se obteve sucesso, retorna um valor negativo em caso de erro.
 */
int IncludeInReadyList(TCB_t *new_thread) {
    return InsertByPrio(&ready_list, new_thread);
}

/**
 * \brief Adiciona uma nova thread à lista de bloquados.
 *
 * @param new_thread Nova thread a ser adicionada à lista.
 * @return Retorna 0 se obteve sucesso, retorna um valor negativo em caso de erro.
 */
int IncludeInBlockedList(TCB_t *new_thread) {
    return InsertByPrio(&blocked_list, new_thread);
}

/**
 * \brief Busca a thread de id thread_id na lista de aptos.
 *
 * @param thread_id Id da thread a ser buscada.
 * @return Ponteiro para a thread encontrada ou NULL se não encontrada.
 */
TCB_t *GetThreadFromReadyList(int thread_id) {
    TCB_t *searched_thread = NULL;

    FirstFila2(&ready_list);
    searched_thread = (TCB_t *) GetAtIteratorFila2(&ready_list);

    while (searched_thread != NULL && searched_thread->tid != thread_id) {
        if (NextFila2(&ready_list) == -NXTFILA_ENDQUEUE) {
            searched_thread = (TCB_t *) GetAtIteratorFila2(&ready_list);

            if (searched_thread != NULL && searched_thread->tid != thread_id)
                searched_thread = NULL;

            break;
        }

        searched_thread = (TCB_t *) GetAtIteratorFila2(&ready_list);
    }

    return searched_thread;
}

/**
 * \brief Busca a thread de id thread_id na lista de bloqueados.
 *
 * @param thread_id Id da thread a ser buscada.
 * @return Ponteiro para a thread encontrada ou NULL se não encontrada.
 */
TCB_t *GetThreadFromBlockedList(int thread_id) {
    TCB_t *searched_thread = NULL;

    FirstFila2(&blocked_list);
    searched_thread = (TCB_t *) GetAtIteratorFila2(&blocked_list);

    while (searched_thread != NULL && searched_thread->tid != thread_id) {
        if (NextFila2(&blocked_list) == -NXTFILA_ENDQUEUE) {
            searched_thread = (TCB_t *) GetAtIteratorFila2(&blocked_list);

            if (searched_thread != NULL && searched_thread->tid != thread_id)
                searched_thread = NULL;

            break;
        }

        searched_thread = (TCB_t *) GetAtIteratorFila2(&blocked_list);
    }

    return searched_thread;
}

/**
 * \brief Remove a thread da lista de bloqueados e a insere na lista de aptos.
 *
 * @param thread_id Id da thread a ser desbloqueada.
 * @return Retorna 0 se obteve sucesso, retorna um valor negativo em caso de erro.
 */
int UnblockThread(int thread_id) {
    TCB_t *unblock_thread = GetThreadFromBlockedList(thread_id);

    if (unblock_thread != NULL) {
        unblock_thread->state = PROCST_APTO;

        //Iterador da lista de bloqueados está na posição da thread que buscou
        if (DeleteAtIteratorFila2(&blocked_list) != 0)
            return -1;
        if (IncludeInReadyList(unblock_thread) < 0)
            return -1;
    } else
        return -1;

    return 0;
}

/**
 * \brief Módulo responsável por colocar a próxima thread da fila de aptos em execução.
 *
 * @attention É necessário que o ponteiro para a thread em execução esteja nulo.
 */
void Dispatcher() {
    if (executing_thread == NULL) {
        FirstFila2(&ready_list);
        TCB_t *next_ready = (TCB_t *) GetAtIteratorFila2(&ready_list);

        if (next_ready != NULL) {
            executing_thread = next_ready;
            executing_thread->state = PROCST_EXEC;
            executing_thread->is_suspended = PROCESS_NOT_SUSPENDED;

            DeleteAtIteratorFila2(&ready_list);

            // Inicia a contagem de tempo em execução da thread
            startTimer();

            setcontext(&(executing_thread->context));
        }
    }
}

/**
 * \brief Remove a thread da memória e chama a função de seleção da próxima thread a executar.
 *
 * O contexto dessa função é passado como parâmetro para o uc_link do contexto de todas as threads,
 * garantindo que essa função seja executada ao final da execução de todas as threads. Seu contexto
 * é inicializado pela função InitScheduler(). Antes de finalizar a execução da função verifica se
 * alguma thread esperava pelo fim da atual, se sim a coloca novamente na lista de aptos.
 */
static void FinishThread() {
    if (executing_thread->joined_thread_id >= 0)
        UnblockThread(executing_thread->joined_thread_id);

    free(executing_thread);
    executing_thread = NULL;
    stopTimer();
    Dispatcher();
}

/**
 * \brief Inicializa o escalonador salvando o contexto da função main.
 *
 * A função verifica se o escalonador já foi inicializado, se sim a função retorna 1, se não
 * executa a inicialização e retorna 0.
 *
 * @return Retorna 0 se obteve sucesso, retorna 1 se já ocorreu a inicialização e um valor negativo em caso de erro.
 */
int InitScheduler() {
    if (isInitialized == SCHEDULER_NOT_INITIALIZED) {
        // Inicializa o contexto da função de finalização de threads
        getcontext(&finish_thread_context);
        finish_thread_context.uc_link = NULL;
        finish_thread_context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
        finish_thread_context.uc_stack.ss_size = SIGSTKSZ;
        makecontext(&finish_thread_context, (void (*)(void)) FinishThread, 0);


        // Inicializa a representação da função main
        TCB_t *main_function = (TCB_t *) malloc(sizeof(TCB_t));
        main_function->tid = 0;
        main_function->state = PROCST_EXEC;
        main_function->prio = 0;
        main_function->is_suspended = PROCESS_NOT_SUSPENDED;
        main_function->joined_thread_id = -1;

        getcontext(&(main_function->context));
        main_function->context.uc_link = &finish_thread_context;

        executing_thread = main_function;

        // Inicializa o contador de tempo para controle de prioridade
        startTimer();

        isInitialized = SCHEDULER_INITIALIZED;
        return 0;
    }

    return 1;
}

/**
 * \brief Coloca a thread atual no estado de bloqueio.
 *
 * @return Retorna 0 se obteve sucesso, retorna um valor negativo em caso de erro.
 */
int BlockCurrentThread() {
    if (IncludeInBlockedList(executing_thread) != 0)
        return -1;

    executing_thread->state = PROCST_BLOQ;
    executing_thread = NULL;
    return 0;
}

/**
 * \brief Reseta o escalonador para execução dos testes unitários.
 * @warning Função desenvolvida para fins de teste, não a utilize fora disso.
 */
void ResetScheduler() {
    FirstFila2(&ready_list);
    FirstFila2(&blocked_list);

    while (!IsReadyListEmpty())
        DeleteAtIteratorFila2(&ready_list);
    while (!IsBlockedListEmpty())
        DeleteAtIteratorFila2(&blocked_list);

    isInitialized = SCHEDULER_NOT_INITIALIZED;

    InitScheduler();
}

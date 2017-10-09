/**
 * \brief Implementação do escalonador.
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

#include <stdlib.h>
#include <ucontext.h>

#include "../include/scheduler.h"

/// Constantes que indicam o estado de inicialização do escalonador
#define SCHEDULER_NOT_INITIALIZED 0
#define SCHEDULER_INITIALIZED 1

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
	
	// pfile vazia?
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
 * \brief Módulo responsável por colocar a próxima thread da fila de aptos em execução.
 */
void Dispatcher() {
    // TODO Sempre que o dispatcher for chamado a thread em execução estará nula?

    // Verifica se o ponteiro para a thread em execução está vazio
    if (executing_thread == NULL) {
        // Verifica se a fila de aptos está vazia
        FirstFila2(&ready_list);

        TCB_t *next_ready = (TCB_t *) GetAtIteratorFila2(&ready_list);

        if (next_ready != NULL) {
            // Fila não vazia, coloca o primeiro elemento em execução
            executing_thread = next_ready;
            executing_thread->state = PROCST_EXEC;

            // Remove o elemento da lista de aptos
            DeleteAtIteratorFila2(&ready_list);

            // Inicia a contagem de tempo em execução da thread
            startTimer();

            // Inicia sua execução
            setcontext(&(executing_thread->context));
        }
    }
}

/**
 * \brief Remove a thread da memória e chama a função de seleção da próxima thread a executar.
 *
 * O contexto dessa função é passado como parâmetro para o uc_link do contexto de todas as threads,
 * garantindo que essa função seja executada ao final da execução de todas as threads. Seu contexto
 * é inicializado pela função InitScheduler().
 */
static void FinishThread() {
    // Libera a memória do processo em execução
    free(executing_thread);

    // Reseta o ponteiro
    executing_thread = NULL;

    // Reseta o timer de execução
    stopTimer();

    // Chama a função de seleção da próxima thread a executar
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
    // Variável indicando se o escalonador já foi inicializado
    // Por ser estática a variável preserva seu valor em todas as chamadas dessa função
    static int isInitialized = SCHEDULER_NOT_INITIALIZED;

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

        getcontext(&(main_function->context));
        main_function->context.uc_link = &finish_thread_context;

        // Salva a thread principal como a thread em execução
        executing_thread = main_function;

        // Inicializa o contador de tempo para controle de prioridade
        startTimer();

        // Indica que o escalonador foi inicializado e retorna
        isInitialized = SCHEDULER_INITIALIZED;
        return 0;
    }

    // Retorna 1 caso o escalonador já estivesse inicializado antes
    return 1;
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

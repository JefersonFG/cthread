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

#include <ucontext.h>
#include <stdlib.h>

#include "../include/scheduler.h"

/// Constantes que indicam o estado de inicialização do escalonador
#define SCHEDULER_NOT_INITIALIZED 0
#define SCHEDULER_INITIALIZED 1

/// Variável usada para geração de novo indicador de thread
static int new_id = 0;

/// Fila de processos no estado apto.
static FILA2 fila_aptos;

/// Fila de processos no estado bloqueado.
static FILA2 fila_bloqueados;

/// Processo em execução.
static TCB_t *thread_em_execucao;

/**
 * \brief Retorna um novo identificador para adição de uma thread.
 *
 * A variável new_id é incrementada e devolvida, garantindo que seja retornado
 * um número diferente e maior do que zero, pois o identificador zero é exclusivo
 * da função main.
 * @return Novo identificador de thread - nunca se repete.
 */
int GetNewId() {
    return ++new_id;
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
int	InsertByPrio(PFILA2 pfila, TCB_t *tcb) {
	TCB_t *tcb_it;
	
	// pfile vazia?
	if (FirstFila2(pfila)==0) {
		do {
			tcb_it = (TCB_t *) GetAtIteratorFila2(pfila);
			if (tcb->prio < tcb_it->prio) {
				return InsertBeforeIteratorFila2(pfila, tcb);
			}
		} while (NextFila2(pfila)==0);
	}	
	return AppendFila2(pfila, (void *)tcb);
}

/**
 * \brief Inicializa o escalonador salvando o contexto da função main.
 *
 * A função verifica se o escalonador já foi inicializado, se sim a função retorna 1, se não
 * executa a inicialização e retorna 0.
 *
 * @return Retorna 0 se obteve sucesso, retorna 1 se já ocorreu a inicialização e outro valor em caso de erro.
 */
int Init() {
    // Variável indicando se o escalonador já foi inicializado
    // Por ser estática a variável preserva seu valor em todas as chamadas dessa função
    static int isInitialized = SCHEDULER_NOT_INITIALIZED;

    if (isInitialized == SCHEDULER_NOT_INITIALIZED) {
        // Inicializa a representação da função main
        TCB_t *main_function = malloc(sizeof(TCB_t));
        main_function->tid = 0;
        main_function->state = PROCST_EXEC;
        main_function->prio = 0;
        getcontext(&main_function->context);

        // Salva a thread principal como a thread em execução
        thread_em_execucao = main_function;

        // Inicializa o contador de tempo para controle de prioridade
        startTimer();

        // Indica que o escalonador foi inicializado e retorna
        isInitialized = SCHEDULER_INITIALIZED;
        return 0;
    }

    // Retorna 1 caso não tenha inicializado
    return 1;
}

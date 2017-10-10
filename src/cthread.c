/**
 * \brief Implementação das funções da interface da biblioteca cthread.
 *
 * @author jfguimaraes
 */

#include <stdlib.h>
#include <ucontext.h>

#include "../include/cthread.h"
#include "../include/scheduler.h"

/**
 * \brief Retorna a identificação do grupo que desenvolveu a biblioteca.
 *
 * @param name Ponteiro onde devem ser escritos os nomes e números dos integrantes do grupo.
 * @param size Quantidade máxima de caracteres que podem ser usados na string de identificação.
 * @return Retorna 0 se executou corretamente, retorna um número negativo caso contrário.
 */
int cidentify (char *name, int size){
    // TODO Implementar cidentify
    return 0;
}

/**
 * \brief Cria um nova thread e a coloca na fila de aptos.
 *
 * @param start Ponteiro para a função que a thread executará.
 * @param arg Parâmetro a ser passado para a thread.
 * @param prio Prioridade da nova thread.
 * @return Retorna um valor positivo se executou com sucesso ou negativo caso contrário.
 */
int ccreate (void* (*start)(void*), void *arg, int prio) {
    // Inicializa o escalonador
    InitScheduler();

    // Obtém o id da thread
    int new_id = GetNewId();

    // Cria a representação da nova thread
    TCB_t *new_thread = (TCB_t *) malloc(sizeof(TCB_t));
    new_thread->tid = new_id;
    new_thread->state = PROCST_APTO;
    new_thread->prio = 0;
    new_thread->is_suspended = PROCESS_NOT_SUSPENDED;

    // Cria o contexto da nova thread
    getcontext(&(new_thread->context));
    new_thread->context.uc_link = GetFinishProcessContext();
    new_thread->context.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
    new_thread->context.uc_stack.ss_size = SIGSTKSZ;
    makecontext(&(new_thread->context), (void (*)(void)) start, 1, arg);

    // Inclui a thread na lista de aptos
    IncludeInReadyList(new_thread);

    // Retorna o id da thread crada
    return new_id;
}

/**
 * \brief Libera CPU da thread que executou essa função.
 *
 * A thread retorna ao estado de apto e o escalonador é chamado para selecionar a
 * próxima thread que irá executar.
 *
 * @return Retorna 0 se executou corretamente, retorna um valor negativo caso contrário.
 */
int cyield(void){
    // Inicializa o escalonador
    InitScheduler();

    // Verifica se a lista de aptos está vazia, se estiver nada a fazer
    if (IsReadyListEmpty() == 0) {
        // Obtém o ponteiro para thread em execução
        TCB_t *executing_thread = GetExecutingThread();

        // Salva o tempo em que a thread permaneceu em execução
        executing_thread->prio += stopTimer();

        // Indica que a thread será suspensa
        executing_thread->is_suspended = PROCESS_SUSPENDED;

        // Salva o contexto atual da thread
        getcontext(&(executing_thread->context));

        // Verifica se a thread está suspensa
        // Quando a thread tiver seu contexto restaurado is_suspended será falso
        if (executing_thread->is_suspended == PROCESS_SUSPENDED) {
            // Configura a thread em execução como apta
            executing_thread->state = PROCST_APTO;

            // Coloca a thread na lista de aptos
            IncludeInReadyList(executing_thread);

            // Limpa o ponteiro de thread em execução
            SetExecutingThreadToNull();

            // Chama o escalonador para colocar a próxima thread em execução
            Dispatcher();
        }
    }

    return 0;
}

/**
 * \brief Bloqueia a thread que executou a função até que a thread com o tid passado termine.
 *
 * Quando a thread que está sendo aguardada terminar, a função cjoin retorna indicando se houve sucesso.
 * Somente uma thread pode aguardar uma determinada thread terminar, se mais de uma thread tentar aguardar
 * a mesma thread terminar a função cjoin retorna um erro e a thread segue sua execução sem bloquear. Se o
 * identificador passado não existir a função cjoin retorna um erro. Quando a função aguardada finaliza a
 * que aguardava segue sua execução normalmente sem receber informações da thread que terminou.
 *
 * @param tid Identificador da thread que deve ser aguardada.
 * @return Retorna 0 se executou corretamente, retorna um valor negativo caso contrário.
 */
int cjoin(int tid) {
    // TODO Implementar cjoin
    return 0;
}

/**
 * \brief Inicializa csem_t com a quantidade de recursos a serem controlados.
 *
 * Para implementar o semáforo como exclusão mútua (mutex) o valor de count deve ser 1. Cada variável
 * do tipo csem_t deve ter uma fila de threads bloqueadas aguardando a liberação dos recursos controlados
 * e inicialmente essa fila deve estar vazia.
 *
 * @param sem Ponteiro para a estrutura de dados que representa o semáforo.
 * @param count Quantidade do recurso controlado pelo semáforo.
 * @return Retorna 0 se executou corretamente, retorna um valor negativo caso contrário.
 */
int csem_init(csem_t *sem, int count) {
    // TODO Implementar csem_init
    return 0;
}

/**
 * \brief Solicita um recurso do semáforo passado.
 *
 * Se houver um recurso disponível ele é atribuído à thread que continua sua execução normalmente, se não
 * a thread é bloqueada e colocada na fila de espera do recurso. Para cada chamada da função cwait o
 * valor de count deve ser decrementado.
 *
 * @param sem Semáforo que controla o recurso solicitado.
 * @return Retorna 0 se executou corretamente, retorna um valor negativo caso contrário.
 */
int cwait(csem_t *sem) {
    // TODO Implementar cwait
    return 0;
}

/**
 * \brief Libera um recurso do semáforo passado.
 *
 * A cada chamada da função csignal o valor de count do semáforo deverá ser incrementado. Se houver
 * mais de uma thread na fila de espera do recurso liberado a primeira da fila, seguindo uma política
 * FIFO deverá receber o recurso e passar para o estado apto e as demais threads da fila devem
 * permanecer no estado bloqueado.
 *
 * @param sem Semáforo que controla o recurso liberado.
 * @return Retorna 0 se executou corretamente, retorna um valor negativo caso contrário.
 */
int csignal(csem_t *sem) {
    // TODO Implementar csignal
    return 0;
}



/**
 * Testes unitários da biblioteca cthread.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/cthread.h"
#include "../include/scheduler.h"

#ifdef __cplusplus
}
#endif

#include "gtest/gtest.h"

/// Variáveis globais para testes.
int global_var = 0;
csem_t global_semaphore;

/**
 * Classe de testes com funções de inicialização e limpeza para os testes.
 */
class SchedulerTest: public ::testing::Test {
public:
    /**
     * Reseta o escalonador e a variável global.
     */
    SchedulerTest() {
        ResetScheduler();
        global_var = 0;
    }
};

/**
 * Função de testes.
 */
void* TestFunc(void *arg) {
    global_var += 1;
    cyield();
}

/**
 * Função de testes.
 */
void* TestFunc2(void *arg) {
    global_var += 2;
}

/**
 * Função de testes.
 */
void* TestFunc3(void *arg) {
    global_var += 3;
    cyield();

    EXPECT_FALSE(IsBlockedListEmpty());
}

/**
 * Função de testes.
 */
void* TestFunc4(void *arg){
    cwait(&global_semaphore);
    cyield();
}

/**
 * Verifica que a função ccreate gera diferentes ids para cada thread.
 */
TEST_F(SchedulerTest, ccreate_different_ids) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    ASSERT_NE(id1, ccreate(TestFunc2, (void *) NULL, 0));
}

/**
 * Verifica que a função ccreate coloca uma nova thread na lista de aptos.
 */
TEST_F(SchedulerTest, ccreate_list_not_empty) {
    ASSERT_TRUE(IsReadyListEmpty());

    ccreate(TestFunc, (void *) NULL, 0);

    ASSERT_FALSE(IsReadyListEmpty());
}

/**
 * Verifica que a função cyield permite que outras threads executem e que
 * se não houver outras threads para executar a thread que executou o
 * cyield continua sua execução normalmente.
 */
TEST_F(SchedulerTest, cyield_changing_thread) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    int id2 = ccreate(TestFunc2, (void *) NULL, 0);

    cyield();

    cyield();

    ASSERT_EQ(3, global_var);
}

/**
 * Verifica que a função cjoin aguarda a execução de outras threads e
 * que quando executada em uma thread inexistente ou já concluída a
 * thread que executou o cjoin segue sua execução normalmente.
 */
TEST_F(SchedulerTest, cjoin_changing_thread) {
    int id2 = ccreate(TestFunc2, (void *) NULL, 0);

    cjoin(id2);

    ASSERT_EQ(2, global_var);
    EXPECT_TRUE(IsBlockedListEmpty());

    int id1 = ccreate(TestFunc, (void *) NULL, 0);

    cjoin(id1);

    ASSERT_EQ(3, global_var);
    EXPECT_TRUE(IsBlockedListEmpty());

    int id3 = ccreate(TestFunc3, (void *) NULL, 0);
    int id4 = ccreate(TestFunc2, (void *) NULL, 0);

    cjoin(id3);

    ASSERT_EQ(8, global_var);
    EXPECT_TRUE(IsBlockedListEmpty());

    cjoin(id4);
    cjoin(-1);

    EXPECT_EQ(8, global_var);
    EXPECT_TRUE(IsBlockedListEmpty());
}

/**
 * Verifica que a função csem_init inicializa a estrutura csem_t corretamente.
 */
TEST_F(SchedulerTest, csem_init) {
    csem_t new_semaphor;

    csem_init(&new_semaphor, 2);

    ASSERT_EQ(2, new_semaphor.count);
    ASSERT_EQ(-NXTFILA_VAZIA, NextFila2(new_semaphor.fila));
}


/**
 * Verifica se o cwait decrementa o semáforo
 */
TEST_F(SchedulerTest, cwait) {
    csem_t new_semaphore;
    csem_init(&new_semaphore, 1);

    // TODO @lmsbatista Olha o que eu escrevi no telegram e corrige esses testes

    cwait(&new_semaphore);
    ASSERT_EQ(new_semaphore.count, 0);

    cwait(&new_semaphore);
    ASSERT_EQ(new_semaphore.count, -1);
}

//TODO: descobrir como testar essa porra
TEST_F(SchedulerTest, cwait_insertatqueue) {
    csem_init(&global_semaphore,1);

    int id1 = ccreate(TestFunc4, (void*)NULL, 0);
    cjoin(id1);
    EXPECT_TRUE(FirstFila2(global_semaphore.fila));

    int id2 = ccreate(TestFunc4, (void*)NULL, 0);
    cjoin(id2);
    EXPECT_FALSE(FirstFila2(global_semaphore.fila));

    int id3 = ccreate(TestFunc4, (void*)NULL, 0);
    cjoin(id3);
    EXPECT_FALSE(FirstFila2(global_semaphore.fila));
}

/**
 * Verifica se o csginal incrementa o semáforo
 */
TEST_F(SchedulerTest, csignal) {
    csem_t new_semaphore;
    csem_init(&new_semaphore, 1);

    cwait(&new_semaphore);
    csignal(&new_semaphore);
    ASSERT_EQ(new_semaphore.count, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

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

/// Variável global para testes.
int global_var = 0;

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
 * Função de testes
 */
void* TestFunc2(void *arg) {
    global_var += 2;
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

    int id1 = ccreate(TestFunc, (void *) NULL, 0);

    cjoin(id1);

    ASSERT_EQ(3, global_var);

    int id3 = ccreate(TestFunc, (void *) NULL, 0);
    int id4 = ccreate(TestFunc2, (void *) NULL, 0);

    cjoin(id3);

    ASSERT_EQ(6, global_var);

    cjoin(id4);
    cjoin(-1);

    ASSERT_EQ(6, global_var);
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

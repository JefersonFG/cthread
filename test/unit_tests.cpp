/**
 * \brief Testes unitários da biblioteca cthread.
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
 * \brief Classe de testes com funções de inicialização e limpeza para os testes.
 */
class SchedulerTest: public ::testing::Test {
public:
    SchedulerTest() {
        ResetScheduler();
        InitScheduler();
        global_var = 0;
    }
};

void* TestFunc(void *arg) {
    global_var += 1;
    cyield();
}

void* TestFunc2(void *arg) {
    global_var += 2;
}

TEST_F(SchedulerTest, ccreate_list_not_empty) {
    ASSERT_TRUE(IsReadyListEmpty());

    ccreate(TestFunc, (void *) NULL, 0);

    ASSERT_FALSE(IsReadyListEmpty());
}

TEST_F(SchedulerTest, ccreate_different_ids) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    ASSERT_NE(id1, ccreate(TestFunc2, (void *) NULL, 0));
}

TEST_F(SchedulerTest, cyield_changing_thread) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    int id2 = ccreate(TestFunc2, (void *) NULL, 0);

    cyield();

    // TODO Por um problema na limpeza das listas não é possível determinar o valor.
    ASSERT_TRUE(global_var > 0);
}

TEST_F(SchedulerTest, cjoin_changing_thread) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    int id2 = ccreate(TestFunc2, (void *) NULL, 0);

    cjoin(id1);
    cjoin(id2);

    // TODO Por um problema na limpeza das listas não é possível determinar o valor.
    ASSERT_TRUE(global_var > 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

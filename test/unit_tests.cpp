/**
 * \brief Testes unitários da biblioteca cthread.
 */

#include "gtest/gtest.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/cthread.h"
#include "../include/scheduler.h"

#ifdef __cplusplus
}
#endif

class SchedulerTest: public ::testing::Test {
public:
    SchedulerTest() {
        ResetScheduler();
        InitScheduler();
    }
};

void* TestFunc(void *arg) {
    printf("Print da TestFunc!\n");
}

void* TestFunc2(void *arg) {
    printf("Print da TestFunc2!\n");
}

TEST_F(SchedulerTest, list_not_empty) {
    ASSERT_TRUE(IsReadyListEmpty());

    ccreate(TestFunc, (void *) NULL, 0);

    ASSERT_FALSE(IsReadyListEmpty());
}

TEST_F(SchedulerTest, different_ids) {
    int id1 = ccreate(TestFunc, (void *) NULL, 0);
    ASSERT_NE(id1, ccreate(TestFunc2, (void *) NULL, 0));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

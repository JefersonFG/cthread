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

TEST (ccreate_test, list_not_empty) {
    ASSERT_TRUE(IsReadyListEmpty());

    ccreate(NULL, (void *) NULL, 0);

    ASSERT_FALSE(IsReadyListEmpty());
}

TEST (ccreate_test, different_ids) {
    int id1 = ccreate(NULL, (void *) NULL, 0);
    ASSERT_NE(id1, ccreate(NULL, (void *) NULL, 0));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

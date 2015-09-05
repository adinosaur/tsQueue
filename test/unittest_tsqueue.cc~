#include "../include/tsQueue.h"
#include "gtest/gtest.h"

#include <pthread.h>

#include <vector>
#include <iostream>

using namespace std;

#define NTHREADS 10

TSQueue<int> q;

static int counter = 0;

void* do_it(void* arg)
{
    for (int i = 0; i != 10; ++i)
    {
        q.push_back(++counter);
    }
    return NULL;
}

TEST(DINOSAUR_TSQUEUE, TSQUEUE)
{
    vector<pthread_t> tids(NTHREADS);
    
    for (int i = 0; i != NTHREADS; ++i)
        pthread_create(&tids.at(i), NULL, do_it, NULL);
        
    for (auto tid : tids)
        pthread_join(tid, NULL);
    
    int l = q.pop_front();
    int r;
    while (!q.empty())
    {
        r = q.pop_front();
        EXPECT_TRUE(r >= l);
        l = r;
    }
}

int main(int ac, char** av)
{
    ::testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}

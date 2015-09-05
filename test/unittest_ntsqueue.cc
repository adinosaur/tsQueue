#include "../include/tsQueue.h"

#include <pthread.h>

#include <vector>
#include <iostream>

using namespace std;

#define NTHREADS 10

NTSQueue<int> nq(100);

static int counter = 0;

void* do_it(void* arg)
{
    for (int i = 0; i != 3; ++i)
    {
        nq.push_back(++counter);
    }
    return NULL;
}

void* do_it2(void* arg)
{
    while (!nq.empty())
    {
        nq.pop_front();
        cout << "ntsqueue's size: " << nq.size() << endl;
    }
    return NULL;
}

int main(int ac, char** av)
{
    vector<pthread_t> tids(NTHREADS);
    pthread_t get_tid;
    
    for (int i = 0; i != NTHREADS; ++i)
        pthread_create(&tids.at(i), NULL, do_it, NULL);
    
    pthread_create(&get_tid, NULL, do_it2, NULL);
}

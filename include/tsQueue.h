// Date: 2015/09/04
// Author: dinosaur
//
// This module implement thread-safe queues by pushing and poping with a
// mutex control. The base class TSQueue implement base functions. 
// Like push_back and pop_front with a mutex control.

#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_
#include <pthread.h>
#include <time.h>

#include <deque>
#include <ostream>

// TSQueue is a thread-safe queue implemented the base funcions.
// With push_back and pop_front operations, each thread can modify
// queue safely.
template <typename T>
class TSQueue
{
    public:
        typedef T value_type;
        
        void push_back(const T& val)
        {
            pthread_mutex_lock(&_d_mutex);
            _deque.push_back(val);
            pthread_mutex_unlock(&_d_mutex);
        }
        
        T pop_front()
        {
            pthread_mutex_lock(&_d_mutex);
            value_type tmp = _deque.front();
            _deque.pop_front();
            pthread_mutex_unlock(&_d_mutex);
            return tmp;
        }
        
        bool empty()
        {
            return _deque.empty();
        }
        
        typename std::deque<value_type>::size_type size()
        {
            return _deque.size();
        }
        
        void print(std::ostream& os)
        {
            for (auto x : _deque)
                os << x << std::endl;
        }
    protected:
        pthread_mutex_t _d_mutex;
        std::deque<value_type> _deque;
};


// NTSQUEUE maintain a N elements queue.
// If queue's size == N, push_back will blocking
// If queue's size == 0, pop_front will blocking
template <typename T>
class NTSQueue : public TSQueue<T>
{
    public:
        using typename TSQueue<T>::value_type;
        using TSQueue<T>::_d_mutex;
        using TSQueue<T>::_deque;
        using TSQueue<T>::size;
        using TSQueue<T>::empty;
        using TSQueue<T>::print;
        
        explicit NTSQueue(int size): _deque_size(size) {}
        
        // override TSQueue<T>::push_back
        void push_back(const T& val)
        {
            pthread_mutex_lock(&_d_mutex);
            
            // if queue is full, wait for calling pop_front()
            if (size() >= _deque_size)
                pthread_cond_wait(&_d_put_cond, &_d_mutex);
            _deque.push_back(val);
            
            // when a new element is pushed in the queue, send a signal
            // to wake up those threads which's waiting _d_get_cond condition
            pthread_cond_signal(&_d_get_cond);
            pthread_mutex_unlock(&_d_mutex);
        }
        
        // override TSQueue<T>::pop_front
        T pop_front()
        {
            pthread_mutex_lock(&_d_mutex);
            
            // if queue is empty, wait for calling push_back()
            if (empty())
                pthread_cond_wait(&_d_get_cond, &_d_mutex);
            value_type tmp = _deque.front();
            _deque.pop_front();
            
            // when a element is poped from queue, send a signal to
            // wake up those threads which's waiting _d_put_cond condition
            pthread_cond_signal(&_d_put_cond);
            pthread_mutex_unlock(&_d_mutex);
            return tmp;
        }
    protected:
        size_t _deque_size;
        
        // condition _d_put_cond means queue is not full.
        // when queue is full, push_back() operations will wait for 
        // condition _d_put_cond. pop_front() operations will 
        pthread_cond_t _d_put_cond;
        pthread_cond_t _d_get_cond;
};

#endif

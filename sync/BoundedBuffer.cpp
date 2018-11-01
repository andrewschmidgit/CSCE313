#include "BoundedBuffer.h"

#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>

using namespace std;

BoundedBuffer::BoundedBuffer(int capacity)
{
    _capacity = capacity;
    pthread_mutex_init(&_lock, nullptr);
    pthread_cond_init(&_max, nullptr);
    pthread_cond_init(&_min, nullptr);
}

BoundedBuffer::~BoundedBuffer()
{
    pthread_mutex_destroy(&_lock);
    pthread_cond_destroy(&_max);
    pthread_cond_destroy(&_min);
}

int BoundedBuffer::size()
{
    return q.size();
}

void BoundedBuffer::push(string str)
{
    while(q.size() >= _capacity) {
        cout << "Waiting to push: " << q.size() << _capacity << endl;
        pthread_cond_wait(&_max, &_lock);
    }
    pthread_mutex_lock(&_lock);
    q.push(str);
    pthread_mutex_unlock(&_lock);
    pthread_cond_signal(&_min);
}

string BoundedBuffer::pop()
{
    while(q.size() == 0) {
        cout << "Waiting to pop: " << q.size() << endl;
        pthread_cond_wait(&_min, &_lock);
    }
    pthread_mutex_lock(&_lock);
    string s = q.front();
    q.pop();
    pthread_mutex_unlock(&_lock);
    pthread_cond_signal(&_max);
    return s;
}

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
    pthread_cond_init(&_canPush, nullptr);
    pthread_cond_init(&_canPop, nullptr);
}

BoundedBuffer::~BoundedBuffer()
{
    pthread_mutex_destroy(&_lock);
    pthread_cond_destroy(&_canPush);
    pthread_cond_destroy(&_canPop);
}

int BoundedBuffer::size()
{
    return q.size();
}

void BoundedBuffer::push(string str)
{
    pthread_mutex_lock(&_lock);
    while(q.size() >= _capacity)
        pthread_cond_wait(&_canPush, &_lock);
    q.push(str);
    pthread_cond_signal(&_canPop);
    pthread_mutex_unlock(&_lock);
}

string BoundedBuffer::pop()
{
    pthread_mutex_lock(&_lock);
    while(q.size() == 0)
        pthread_cond_wait(&_canPop, &_lock);
    string s = q.front();
    q.pop();
    pthread_cond_signal(&_canPush);
    pthread_mutex_unlock(&_lock);
    return s;
}

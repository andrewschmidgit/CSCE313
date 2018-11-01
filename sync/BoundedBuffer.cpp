#include "BoundedBuffer.h"

#include <iostream>
#include <queue>
#include <string>

using namespace std;

BoundedBuffer::BoundedBuffer(int capacity)
{
    _capacity = capacity || 1;
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
    pthread_mutex_lock(&_lock);
    while(q.size() >= _capacity)
        pthread_cond_wait(&_max, &_lock);
    q.push(str);
    pthread_cond_signal(&_min);
    pthread_mutex_unlock(&_lock);
}

string BoundedBuffer::pop()
{
    pthread_mutex_lock(&_lock);
    while(q.size() <= 0)
        pthread_cond_wait(&_min, &_lock);
    string s = q.front();
    q.pop();
    pthread_cond_signal(&_max);
    pthread_mutex_unlock(&_lock);
    return s;
}

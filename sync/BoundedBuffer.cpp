#include "BoundedBuffer.h"
#include <string>
#include <queue>
#include <iostream>
using namespace std;

BoundedBuffer::BoundedBuffer(int capacity)
{
    _capacity = capacity;
    pthread_mutex_init(&_lock, nullptr);
}

BoundedBuffer::~BoundedBuffer()
{
    pthread_mutex_destroy(&_lock);
}

int BoundedBuffer::size()
{
    return q.size();
}

void BoundedBuffer::push(string str)
{
    pthread_mutex_lock(&_lock);
    while(q.size() > _capacity)
        pthread_cond_wait(&_max, &_lock);
    cout << "push" << endl;
    q.push(str);
    pthread_cond_signal(&_min);
    pthread_mutex_unlock(&_lock);
}

string BoundedBuffer::pop()
{
    pthread_mutex_lock(&_lock);
    while(q.size() <= 0)
        pthread_cond_wait(&_min, &_lock);
    cout << "pop" << endl;
    string s = q.front();
    q.pop();
    pthread_cond_signal(&_max);
    pthread_mutex_unlock(&_lock);
    return s;
}

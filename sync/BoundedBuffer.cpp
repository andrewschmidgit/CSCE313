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
    /*
	Is this function thread-safe??? Does this automatically wait for the pop() to make room 
	when the buffer if full to capacity???
	*/
    cout << "push" << endl;
    pthread_mutex_lock(&_lock);
    while(q.size() > _capacity)
        pthread_cond_wait(&_max, &_lock);
    q.push(str);
    pthread_cond_signal(&_min);
    pthread_mutex_unlock(&_lock);
}

string BoundedBuffer::pop()
{
    cout << "pop" << endl;
    pthread_mutex_lock(&_lock);
    while(q.size() <= 0)
        pthread_cond_wait(&_min, &_lock);
    string s = q.front();
    q.pop();
    pthread_cond_signal(&_max);
    pthread_mutex_unlock(&_lock);
    return s;
}

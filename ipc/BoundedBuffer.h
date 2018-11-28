#ifndef BoundedBuffer_h
#define BoundedBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>
using namespace std;

class BoundedBuffer
{
  private:
    queue<string> q;
    int _capacity;
    pthread_mutex_t _lock;
    pthread_cond_t _canPush;
    pthread_cond_t _canPop;
  public:
    BoundedBuffer(int);
    ~BoundedBuffer();
    int size();
    void push(string);
    string pop();
};

#endif /* BoundedBuffer_ */

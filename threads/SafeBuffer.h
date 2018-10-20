#ifndef SafeBuffer_h
#define SafeBuffer_h

#include <stdio.h>
#include <queue>
#include <string>
#include <pthread.h>
using namespace std;

class SafeBuffer {
private:
	queue<string> q;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
public:
    SafeBuffer();
	~SafeBuffer();
	int size();
    void push (string str);
    string pop();
};

#endif /* SafeBuffer_ */

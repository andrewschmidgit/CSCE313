#include "SafeBuffer.h"
#include <string>
#include <queue>
#include <iostream>
using namespace std;

SafeBuffer::SafeBuffer() {
	
}

SafeBuffer::~SafeBuffer() {
	
}

int SafeBuffer::size() {
	/*
	Is this function thread-safe???
	Make necessary modifications to make it thread-safe
	*/
    pthread_mutex_lock(&lock);
    int ret = q.size();
    pthread_mutex_unlock(&lock);
    return ret;
}

void SafeBuffer::push(string str) {
	/*
	Is this function thread-safe???
	Make necessary modifications to make it thread-safe
	*/
    pthread_mutex_lock(&lock);
	q.push (str);
    pthread_mutex_unlock(&lock);
}

string SafeBuffer::pop() {
	/*
	Is this function thread-safe???
	Make necessary modifications to make it thread-safe
	*/
    pthread_mutex_lock(&lock);
	string s = q.front();
	q.pop();
    pthread_mutex_unlock(&lock);
	return s;
}

void SafeBuffer::print() {
    queue<string> temp;
    while(q.empty() == false) {
        string s = q.front();
        q.pop();
        cout << s << " ";
    }
    cout << endl;
    q = temp;
}

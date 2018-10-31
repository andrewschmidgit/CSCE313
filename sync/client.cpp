/*
    Based on original assignment by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

#include <sys/time.h>
#include <cassert>
#include <assert.h>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <list>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "reqchannel.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
using namespace std;

struct RequestArguments
{
    int RequestCount;
    string Name;
    BoundedBuffer *Buffer;
    RequestArguments(int requestCount, string name, BoundedBuffer *buffer)
    {
        RequestCount = requestCount;
        Name = name;
        Buffer = buffer;
    }
};

void *request_thread_function(void *arg)
{
    /*
		Fill in this function.

		The loop body should require only a single line of code.
		The loop conditions should be somewhat intuitive.

		In both thread functions, the arg parameter
		will be used to pass parameters to the function.
		One of the parameters for the request thread
		function MUST be the name of the "patient" for whom
		the data requests are being pushed: you MAY NOT
		create 3 copies of this function, one for each "patient".
	 */
    RequestArguments *args = (RequestArguments *)arg;
    for (int i = 0; i < args->RequestCount; i++)
        args->Buffer->push(args->Name);
}

struct WorkerArguments
{
    RequestChannel *Channel;
    BoundedBuffer *Buffer;

    BoundedBuffer *JohnBuffer;
    BoundedBuffer *JaneBuffer;
    BoundedBuffer *JoeBuffer;
    WorkerArguments(RequestChannel *channel, BoundedBuffer *buffer, BoundedBuffer *johnBuffer, BoundedBuffer *janeBuffer, BoundedBuffer *joeBuffer) : Channel(channel), Buffer(buffer), JohnBuffer(johnBuffer), JaneBuffer(janeBuffer), JoeBuffer(joeBuffer) {}
};

void *worker_thread_function(void *arg)
{
    WorkerArguments *args = (WorkerArguments *)arg;
    while (true)
    {
        string request = args->Buffer->pop();
        args->Channel->cwrite(request);

        if (request == "quit")
        {
            delete args->Channel;
            break;
        }
        else
        {
            string response = args->Channel->cread();
            if (request.find("John") != string::npos)
                args->JohnBuffer->push(response);
            else if (request.find("Jane") != string::npos)
                args->JaneBuffer->push(response);
            else if (request.find("Joe") != string::npos)
                args->JoeBuffer->push(response);
        }
    }
}

struct StatArguments
{
    int Count;
    string Name;
    BoundedBuffer *Buffer;
    Histogram *Hist;
    StatArguments(int count, string name, BoundedBuffer *buffer, Histogram *hist) : Count(count), Name(name), Buffer(buffer), Hist(hist) {}
};

void *stat_thread_function(void *arg)
{
    /*
		Fill in this function. 

		There should 1 such thread for each person. Each stat thread 
        must consume from the respective statistics buffer and update
        the histogram. Since a thread only works on its own part of 
        histogram, does the Histogram class need to be thread-safe????

     */
    StatArguments *args = (StatArguments *)arg;
    for (int i = 0; i < args->Count; i++)
    {
        auto response = args->Buffer->pop();
        args->Hist->update(args->Name, response);
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int n = 100;     //default number of requests per "patient"
    int w = 10;     //default number of worker threads
    int b = 3 * n; // default capacity of the request buffer, you should change this default
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:w:b:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'w':
            w = atoi(optarg); //This won't do a whole lot until you fill in the worker thread function
            break;
        case 'b':
            b = atoi(optarg);
            break;
        }
    }

    int pid = fork();
    if (pid == 0)
    {
        execl("dataserver", (char *)NULL);
    }
    else
    {
        cout << "n == " << n << endl;
        cout << "w == " << w << endl;
        cout << "b == " << b << endl;

        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        BoundedBuffer request_buffer(b);
        Histogram hist;

        //3 threads
        pthread_t john, jane, joe;

        RequestArguments *johnRequestArgs = new RequestArguments(n, "data John Smith", &request_buffer);
        RequestArguments *janeRequestArgs = new RequestArguments(n, "data Jane Smith", &request_buffer);
        RequestArguments *joeRequestArgs = new RequestArguments(n, "data Joe Smith", &request_buffer);

        pthread_create(&john, nullptr, request_thread_function, johnRequestArgs);
        pthread_create(&jane, nullptr, request_thread_function, janeRequestArgs);
        pthread_create(&joe, nullptr, request_thread_function, joeRequestArgs);

        BoundedBuffer johnBuffer(b / 3);
        BoundedBuffer janeBuffer(b / 3);
        BoundedBuffer joeBuffer(b / 3);
        vector<pthread_t> workers;
        for (int i = 0; i < w; i++)
        {
            chan->cwrite("newchannel");
            string s = chan->cread();
            RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
            WorkerArguments *workerArguments = new WorkerArguments(workerChannel, &request_buffer, &johnBuffer, &janeBuffer, &joeBuffer);
            pthread_t worker;
            pthread_create(&worker, nullptr, worker_thread_function, workerArguments);
            workers.push_back(worker);
        }

        pthread_t johnStat, janeStat, joeStat;
        StatArguments *johnStatArgs = new StatArguments(b / 3, "data John Smith", &johnBuffer, &hist);
        StatArguments *janeStatArgs = new StatArguments(b / 3, "data Jane Smith", &janeBuffer, &hist);
        StatArguments *joeStatArgs = new StatArguments(b / 3, "data Joe Smith", &joeBuffer, &hist);

        pthread_create(&johnStat, nullptr, stat_thread_function, johnStatArgs);
        pthread_create(&janeStat, nullptr, stat_thread_function, janeStatArgs);
        pthread_create(&joeStat, nullptr, stat_thread_function, joeStatArgs);

        // Waiting
        pthread_join(john, nullptr);
        pthread_join(jane, nullptr);
        pthread_join(joe, nullptr);

        cout << "Done populating request buffer" << endl;

        // Pushing Quit Requests
        cout << "Pushing quit requests... ";
        for (int i = 0; i < w; ++i)
        {
            request_buffer.push("quit");
        }
        cout << "done." << endl;

        for (auto worker : workers)
            pthread_join(worker, nullptr);

        pthread_join(johnStat, nullptr);
        pthread_join(janeStat, nullptr);
        pthread_join(joeStat, nullptr);

        chan->cwrite("quit");
        delete chan;
        cout << "All Done!!!" << endl;

        hist.print();
    }
}

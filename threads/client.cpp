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
#include "SafeBuffer.h"
#include "Histogram.h"
using namespace std;

struct RequestArguments
{
    int RequestCount;
    string Name;
    SafeBuffer *Buffer;
    RequestArguments(int requestCount, string name, SafeBuffer *buffer)
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
    {
        args->Buffer->push(args->Name);
    }
}

struct WorkerArguments
{
    RequestChannel *Channel;
    SafeBuffer *Buffer;
    Histogram *Hist;
    WorkerArguments(RequestChannel *channel, SafeBuffer *buffer, Histogram *hist)
    {
        Channel = channel;
        Buffer = buffer;
        Hist = hist;
    }
};

void *worker_thread_function(void *arg)
{
    /*
		Fill in this function. 

		Make sure it terminates only when, and not before,
		all the requests have been processed.

		Each thread must have its own dedicated
		RequestChannel. Make sure that if you
		construct a RequestChannel (or any object)
		using "new" that you "delete" it properly,
		and that you send a "quit" request for every
		RequestChannel you construct regardless of
		whether you used "new" for it.
    */
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
            args->Hist->update(request, response);
        }
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int n = 100; //default number of requests per "patient"
    int w = 1;   //default number of worker threads
    int opt = 0;
    bool output = false;
    while ((opt = getopt(argc, argv, "n:w:t")) != -1)
    {
        switch (opt)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'w':
            w = atoi(optarg); //This won't do a whole lot until you fill in the worker thread function
            break;
        case 't':
            output = true;
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
        if (!output)
        {
            cout << "n == " << n << endl;
            cout << "w == " << w << endl;

            cout << "CLIENT STARTED:" << endl;
            cout << "Establishing control channel... " << flush;
        }
        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        if (!output)
            cout << "done." << endl
                 << flush;

        SafeBuffer request_buffer;
        Histogram hist;

        // TIMING
        struct timeval t1, t2;
        gettimeofday(&t1, nullptr);

        //3 threads
        pthread_t john;
        pthread_t jane;
        pthread_t joe;

        RequestArguments *johnArgs = new RequestArguments(n, "data John Smith", &request_buffer);
        RequestArguments *janeArgs = new RequestArguments(n, "data Jane Smith", &request_buffer);
        RequestArguments *joeArgs = new RequestArguments(n, "data Joe Smith", &request_buffer);

        pthread_create(&john, NULL, request_thread_function, johnArgs);
        pthread_create(&jane, NULL, request_thread_function, janeArgs);
        pthread_create(&joe, NULL, request_thread_function, joeArgs);

        pthread_join(john, NULL);
        pthread_join(jane, NULL);
        pthread_join(joe, NULL);

        if (!output)
        {
            cout << "Done populating request buffer" << endl;

            cout << "Pushing quit requests... ";
        }
        for (int i = 0; i < w; i++)
        {
            request_buffer.push("quit");
        }

        if (!output)
            cout << "done." << endl;

        // Creates worker threads

        vector<pthread_t> workers;
        for (int i = 0; i < w; i++)
        {
            chan->cwrite("newchannel");
            string s = chan->cread();
            RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);

            pthread_t worker;
            WorkerArguments *args = new WorkerArguments(workerChannel, &request_buffer, &hist);
            pthread_create(&worker, NULL, worker_thread_function, args);

            workers.push_back(worker);
        }

        // Waiting on them all
        for (auto worker : workers)
        {
            pthread_join(worker, nullptr);
        }

        // Ending timer
        gettimeofday(&t2, nullptr);

        if (output == true)
        {
            double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
            cout << elapsedTime << endl;
        }

        chan->cwrite("quit");
        delete chan;
        if (!output)
        {
            cout << "All Done!!!" << endl;
            hist.print();
        }
    }
}

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

struct RequestChannelPair
{
    RequestChannel *Channel;
    string Request;
    RequestChannelPair(RequestChannel *channel, string request) : Channel(channel), Request(request) {}
};

struct EventArguments
{
    int N;
    int W;
    RequestChannel *Chan;
    BoundedBuffer *Buffer;
    BoundedBuffer *JohnBuffer;
    BoundedBuffer *JaneBuffer;
    BoundedBuffer *JoeBuffer;

    EventArguments(int n, int w, RequestChannel *chan, BoundedBuffer *buffer, BoundedBuffer *john, BoundedBuffer *jane, BoundedBuffer *joe)
        : N(n), W(w), Chan(chan), Buffer(buffer), JohnBuffer(john), JaneBuffer(jane), JoeBuffer(joe) {}
};

int getMaxFileDescriptor(vector<RequestChannelPair> &pairs)
{
    int ret = 0;
    for (auto pair : pairs)
        ret = max(ret, pair.Channel->read_fd());
    return ret;
}

bool processChannels(EventArguments *args, fd_set fds, vector<RequestChannelPair> pairs, int &setCounter, int& writeCounter)
{
    int readyFds = select(getMaxFileDescriptor(pairs) + 1, &fds, nullptr, nullptr, nullptr);
    int i;
    if(readyFds > 0)
        for (i = 0; i < pairs.size(); i++)
        {
            cout << "i = " << i << "set counter = " << setCounter << endl;
            RequestChannelPair pair = pairs[i];
            if (FD_ISSET(pair.Channel->read_fd(), &fds))
            {
                string response = pair.Channel->cread();
                if (pair.Request.find("John") != string::npos)
                    args->JohnBuffer->push(response);
                else if (pair.Request.find("Jane") != string::npos)
                    args->JaneBuffer->push(response);
                else if (pair.Request.find("Joe") != string::npos)
                    args->JoeBuffer->push(response);

                if(writeCounter < 3 * args->N) {
                    pair.Request = args->Buffer->pop();
                    pair.Channel->cwrite(pair.Request);
                }
                setCounter++;
                cout << "Set counter incremented to: " << setCounter << endl;
            }
        }
    return 3 * args->N == setCounter;
}

void *event_thread_function(void *arg)
{
    EventArguments *args = (EventArguments *)arg;
    vector<RequestChannelPair> pairs;

    // Priming the pump
    for (int i = 0; i < min(args->W, 3 * args->N); i++)
    {
        args->Chan->cwrite("newchannel");
        string s = args->Chan->cread();
        RequestChannelPair pair(new RequestChannel(s, RequestChannel::CLIENT_SIDE), args->Buffer->pop());
        pair.Channel->cwrite(pair.Request);
        pairs.push_back(pair);
    }
    cout << "Done priming the pump" << endl;

    bool isQuittingTime = false;
    int setCounter = 0, writeCounter = 0;
    while (isQuittingTime == false)
    {
        fd_set fds;
        FD_ZERO(&fds);
        for (auto pair : pairs)
            FD_SET(pair.Channel->read_fd(), &fds);

        isQuittingTime = processChannels(args, fds, pairs, setCounter, writeCounter);
    }

    for (auto pair : pairs)
    {
        pair.Channel->cwrite("quit");
        delete pair.Channel;
    }
}

struct StatArguments
{
    int Count;
    string Name;
    BoundedBuffer *Buffer;
    Histogram *Hist;
    StatArguments(int count, string name, BoundedBuffer *buffer, Histogram *hist) : Name(name), Buffer(buffer), Hist(hist)
    {
        Count = count;
    }
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
        string response = args->Buffer->pop();
        cout << args->Name << " response: " << response << endl;
        args->Hist->update(args->Name, response);
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int n = 100;   //default number of requests per "patient"
    int w = 10;    //default number of worker threads
    int b = 3 * n; // default capacity of the request buffer, you should change this default
    int opt = 0;
    bool output = false;
    while ((opt = getopt(argc, argv, "n:w:b:t")) != -1)
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
            cout << "b == " << b << endl;
        }

        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        BoundedBuffer request_buffer(b);
        Histogram hist;

        // Start Timing
        struct timeval t1, t2;
        gettimeofday(&t1, nullptr);

        //3 threads
        pthread_t john, jane, joe;

        RequestArguments *johnRequestArgs = new RequestArguments(n, "data John Smith", &request_buffer);
        RequestArguments *janeRequestArgs = new RequestArguments(n, "data Jane Smith", &request_buffer);
        RequestArguments *joeRequestArgs = new RequestArguments(n, "data Joe Smith", &request_buffer);

        pthread_create(&john, nullptr, request_thread_function, johnRequestArgs);
        pthread_create(&jane, nullptr, request_thread_function, janeRequestArgs);
        pthread_create(&joe, nullptr, request_thread_function, joeRequestArgs);

        BoundedBuffer johnBuffer((b + 3 - 1) / 3);
        BoundedBuffer janeBuffer((b + 3 - 1) / 3);
        BoundedBuffer joeBuffer((b + 3 - 1) / 3);

        // Event Handler stuff
        EventArguments *eventArgs = new EventArguments(n, w, chan, &request_buffer, &johnBuffer, &janeBuffer, &joeBuffer);
        pthread_t eventHandler;
        pthread_create(&eventHandler, nullptr, event_thread_function, eventArgs);

        pthread_t johnStat, janeStat, joeStat;
        StatArguments *johnStatArgs = new StatArguments(n, "data John Smith", &johnBuffer, &hist);
        StatArguments *janeStatArgs = new StatArguments(n, "data Jane Smith", &janeBuffer, &hist);
        StatArguments *joeStatArgs = new StatArguments(n, "data Joe Smith", &joeBuffer, &hist);

        pthread_create(&johnStat, nullptr, stat_thread_function, johnStatArgs);
        pthread_create(&janeStat, nullptr, stat_thread_function, janeStatArgs);
        pthread_create(&joeStat, nullptr, stat_thread_function, joeStatArgs);

        // Waiting
        pthread_join(john, nullptr);
        pthread_join(jane, nullptr);
        pthread_join(joe, nullptr);

        if (!output)
            cout << "Done populating request buffer" << endl;

        // Pushing Quit Requests
        if (!output)
            cout << "Pushing quit requests... ";
        //for(int i = 0; i < initial; i++)
        request_buffer.push("quit");
        if (!output)
            cout << "done." << endl;

        // Waiting for event handler to finish (after pushing quit requests)
        pthread_join(eventHandler, nullptr);

        if (!output)
            cout << "All Workers finished" << endl;
        pthread_join(johnStat, nullptr);
        pthread_join(janeStat, nullptr);
        pthread_join(joeStat, nullptr);

        gettimeofday(&t2, nullptr);

        if (output == true)
        {
            double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
            cout << elapsedTime << endl;
        }

        chan->cwrite("quit");
        delete chan;
        
        if (output == false)
        {
            cout << "All Done!!!" << endl;
            hist.print();
        }
    }
}

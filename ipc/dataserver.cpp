#include <cassert>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "reqchannel.h"
#include "GetRequestChannelProcessor.h"
#include <pthread.h>
using namespace std;

int nchannels = 0;
pthread_mutex_t newchannel_lock;
void *handle_process_loop(void *_channel);

struct ProcessLoopInfo
{
	RequestChannel *Channel;
	GetRequestChannelProcessor *Processor;
};

void process_newchannel(ProcessLoopInfo *info)
{
	nchannels++;
	string new_channel_name = "data" + to_string(nchannels) + "_";
	info->Channel->cwrite(new_channel_name);

	ProcessLoopInfo processLoopInfo;

	processLoopInfo.Channel = info->Processor->Get(new_channel_name, RequestChannel::SERVER_SIDE);
	processLoopInfo.Processor = info->Processor;
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, handle_process_loop, &processLoopInfo) < 0)
	{
		EXITONERROR("");
	}
}

void process_request(ProcessLoopInfo *info, string _request)
{

	if (_request.compare(0, 5, "hello") == 0)
	{
		info->Channel->cwrite("hello to you too");
	}
	else if (_request.compare(0, 4, "data") == 0)
	{
		usleep(1000 + (rand() % 5000));
		info->Channel->cwrite(to_string(rand() % 100));
	}
	else if (_request.compare(0, 10, "newchannel") == 0)
	{
		process_newchannel(info);
	}
	else
	{
		info->Channel->cwrite("unknown request");
	}
}

void *handle_process_loop(void *info)
{
	ProcessLoopInfo *Info = (ProcessLoopInfo *)info;
	for (;;)
	{
		string request = Info->Channel->cread();
		if (request.compare("quit") == 0)
		{
			cout << "Quitting" << endl;
			break; // break out of the loop;
		}
		process_request(Info, request);
	}
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	newchannel_lock = PTHREAD_MUTEX_INITIALIZER;
	RequestChannelType type;

	switch (argv[1][0])
	{
	case 'f':
		type = RequestChannelType::FIFO;
		break;
	case 'q':
		type = RequestChannelType::MQ;
		break;
	case 's':
		type = RequestChannelType::MEMORY;
		break;
	}
	GetRequestChannelProcessor getRequestChannelProcessor(type);
	RequestChannel *control_channel = getRequestChannelProcessor.Get("control", RequestChannel::SERVER_SIDE);
	ProcessLoopInfo info{control_channel, &getRequestChannelProcessor};
	handle_process_loop(&info);
	delete control_channel;
}

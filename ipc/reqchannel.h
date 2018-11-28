
#ifndef _reqchannel_H_
#define _reqchannel_H_

#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#define MAX_MESSAGE 255

using namespace std;

void EXITONERROR(string msg)
{
	perror(msg.c_str());
	exit(-1);
}

class RequestChannel
{

public:
	typedef enum
	{
		SERVER_SIDE,
		CLIENT_SIDE
	} Side;

	typedef enum
	{
		READ_MODE,
		WRITE_MODE
	} Mode;

protected:
	string my_name;
	string side_name;
	Side my_side;

public:
	RequestChannel(const string _name, const Side _side) : my_name(_name), my_side(_side), side_name((_side == RequestChannel::SERVER_SIDE) ? "SERVER" : "CLIENT") {}

	virtual string cread() = 0;
	/* Blocking read of data from the channel. Returns a string of characters
	 read from the channel. Returns NULL if read failed. */

	virtual int cwrite(string msg) = 0;
	/* Write the data to the channel. The function returns the number of characters written
	 to the channel. */

	string name()
	{
		return my_name;
	}
};

#endif
#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>

#include "reqchannel.h"
#include "SMBoundedBuffer.h"

using namespace std;

class SMRequestChannel : public RequestChannel
{
    SMBoundedBuffer *readBuffer;
    SMBoundedBuffer *writeBuffer;
    string filename;

  public:
    SMRequestChannel(const string _name, const Side _side) : RequestChannel(_name, _side)
    {
        filename = "schmid_" + my_name;
        FILE *file = fopen(filename.c_str(), "w");
        fclose(file);

        key_t rKey, rFull, rEmpty, rMutex, wKey, wFull, wEmpty, wMutex;
        if (my_side == SERVER_SIDE)
        {
            rKey = ftok(filename.c_str(), 1);
            rFull = ftok(filename.c_str(), 2);
            rEmpty = ftok(filename.c_str(), 3);
            rMutex = ftok(filename.c_str(), 4);
            wKey = ftok(filename.c_str(), 5);
            wFull = ftok(filename.c_str(), 6);
            wEmpty = ftok(filename.c_str(), 7);
            wMutex = ftok(filename.c_str(), 8);
        }
        else
        {
            rKey = ftok(filename.c_str(), 5);
            rFull = ftok(filename.c_str(), 6);
            rEmpty = ftok(filename.c_str(), 7);
            rMutex = ftok(filename.c_str(), 8);
            wKey = ftok(filename.c_str(), 1);
            wFull = ftok(filename.c_str(), 2);
            wEmpty = ftok(filename.c_str(), 3);
            wMutex = ftok(filename.c_str(), 4);
        }
        readBuffer = new SMBoundedBuffer(rKey, rFull, rEmpty, rMutex);
        writeBuffer = new SMBoundedBuffer(wKey, wFull, wEmpty, wMutex);
    }

    ~SMRequestChannel()
    {
        if (my_side == SERVER_SIDE)
        {
            delete readBuffer;
            delete writeBuffer;
        }
        remove(filename.c_str());
    }

    string cread()
    {
        return readBuffer->pop();
    }

    void cwrite(string msg)
    {
        writeBuffer->push(msg);
    }
};
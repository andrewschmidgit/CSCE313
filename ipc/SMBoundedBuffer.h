#pragma once

#include <string>
#include <string.h>
#include <sys/shm.h>

#include "KernelSemaphore.h"

using namespace std;

class SMBoundedBuffer
{
    KernelSemaphore full, empty, mutex;
    char *buffer;
    int schmid;

  public:
    SMBoundedBuffer(key_t bbKey, key_t fullKey, key_t emptyKey, key_t mutexKey) : full(0, fullKey), empty(1, emptyKey), mutex(1, mutexKey)
    {
        if ((schmid = shmget(bbKey, 512, IPC_CREAT | 0644)) == -1)
        {
            perror("shmget");
            exit(-1);
        }
        buffer = reinterpret_cast<char *>(shmat(schmid, 0, 0));
    }

    ~SMBoundedBuffer()
    {
        shmdt(buffer);
        shmctl(schmid, IPC_RMID, 0);
    }

    void push(const string &msg)
    {
        empty.less();
        mutex.less();

        strcpy(buffer, msg.c_str());

        mutex.more();
        full.more();
    }

    string pop()
    {
        string ret;
        full.less();
        mutex.less();

        ret.assign(buffer);

        mutex.more();
        empty.more();

        return ret;
    }
};
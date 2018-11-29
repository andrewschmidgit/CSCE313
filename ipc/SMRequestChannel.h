#pragma once

#include <sys/shm.h>

#include "reqchannel.h"
#include "KernalSemaphore.h"

using namespace std;

class SMRequestChannel : public RequestChannel {

    public:
    SMRequestChannel(const string _name, const Side _side) : RequestChannel(_name, _side) {}
    ~SMRequestChannel() {}

    string cread() {

    }

    void cwrite(string msg) {

    }
};
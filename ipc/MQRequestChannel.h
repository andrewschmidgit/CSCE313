#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "reqchannel.h"

using namespace std;

struct MessageBuffer
{
    long MessageType;
    char MessageText[255];
};

class MQRequestChannel : public RequestChannel
{
    int readMessageQueueId;
    int writeMessageQueueId;

    string getFileName(Mode mode)
    {
        string filename = "mq_" + my_name;

        if (my_side == CLIENT_SIDE)
        {
            if (mode == READ_MODE)
                filename += "1";
            else
                filename += "2";
        }
        else
        {
            /* SERVER_SIDE */
            if (mode == READ_MODE)
                filename += "2";
            else
                filename += "1";
        }
        return filename;
    }

  public:
    MQRequestChannel(const string _name, const Side _side) : RequestChannel(_name, _side)
    {
        string filename = "mq_" + my_name;
        FILE* file = fopen(filename.c_str(), "w");
        
        key_t readKey;
        key_t writeKey;

        if(my_side == SERVER_SIDE) {
            readKey = ftok(filename.c_str(), 1);
            writeKey = ftok(filename.c_str(), 2);
        }
        else {
            readKey = ftok(filename.c_str(), 2);
            writeKey = ftok(filename.c_str(), 1);
        }

        readMessageQueueId = msgget(readKey, 0666 | IPC_CREAT);
        writeMessageQueueId = msgget(writeKey, 0666 | IPC_CREAT);

        fclose(file);
        remove(filename.c_str());
    }

    ~MQRequestChannel()
    {
        msgctl(readMessageQueueId, IPC_RMID, nullptr);
        msgctl(writeMessageQueueId, IPC_RMID, nullptr);
    }
    
    string cread()
    {
        MessageBuffer buffer;
        msgrcv(readMessageQueueId, &buffer, sizeof(MessageBuffer), 1, 0);
        return buffer.MessageText;
    }

    void cwrite(string msg)
    {
        MessageBuffer buffer;
        buffer.MessageType = 1;
        strcpy(buffer.MessageText, msg.c_str());
        msgsnd(writeMessageQueueId, &buffer, sizeof(MessageBuffer), 0);
    }
};
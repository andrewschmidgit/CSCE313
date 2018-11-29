#pragma once

#include <string>
#include <fstream>

#include "RequestChannelType.h"
#include "reqchannel.h"
#include "FIFORequestChannel.h"
#include "MQRequestChannel.h"

using namespace std;

class GetRequestChannelProcessor
{
    RequestChannelType _type;

  public:
    GetRequestChannelProcessor(RequestChannelType type) : _type(type) {}

    RequestChannel *Get(string name, RequestChannel::Side side)
    {
        switch (_type)
        {
        case RequestChannelType::FIFO:
            return new FIFORequestChannel(name, side);
        case RequestChannelType::MQ:
            return new MQRequestChannel(name, side);
        }
        return nullptr;
    }
};
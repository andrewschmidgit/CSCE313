#pragma once

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "reqchannel.h"

class FIFORequestChannel : public RequestChannel
{
    int wfd;
    int rfd;

    std::string pipe_name(Mode _mode)
    {
        std::string pname = "fifo_" + my_name;

        if (my_side == CLIENT_SIDE)
        {
            if (_mode == READ_MODE)
                pname += "1";
            else
                pname += "2";
        }
        else
        {
            /* SERVER_SIDE */
            if (_mode == READ_MODE)
                pname += "2";
            else
                pname += "1";
        }
        return pname;
    }

    void create_pipe(std::string _pipe_name)
    {
        mkfifo(_pipe_name.c_str(), 0666);
    }

    void open_write_pipe(std::string _pipe_name)
    {
        create_pipe(_pipe_name);

        wfd = open(_pipe_name.c_str(), O_WRONLY);
        if (my_side == SERVER_SIDE)
        {
            ofstream ouf;
            ouf.open("fifo.txt", ofstream::app);
            ouf << " " << wfd << endl;
        }
        else
            cout << " " << wfd << endl;

        if (wfd < 0)
        {
            EXITONERROR(_pipe_name);
        }
    }

    void open_read_pipe(std::string _pipe_name)
    {
        create_pipe(_pipe_name);
        rfd = open(_pipe_name.c_str(), O_RDONLY);
        if (my_side == SERVER_SIDE)
        {
            ofstream ouf;
            ouf.open("fifo.txt", ofstream::app);
            ouf << " " << rfd << endl;
        }
        else
            cout << " " << rfd << endl;
        if (rfd < 0)
        {
            perror("");
            exit(0);
        }
    }

  public:
    FIFORequestChannel(const std::string _name, const Side _side) : RequestChannel(_name, _side)
    {
        if (_side == SERVER_SIDE)
        {
            open_write_pipe(pipe_name(WRITE_MODE).c_str());
            open_read_pipe(pipe_name(READ_MODE).c_str());
        }
        else
        {
            open_read_pipe(pipe_name(READ_MODE).c_str());
            open_write_pipe(pipe_name(WRITE_MODE).c_str());
        }
    }

    ~FIFORequestChannel()
    {
        close(wfd);
        close(rfd);

        remove(pipe_name(READ_MODE).c_str());
        remove(pipe_name(WRITE_MODE).c_str());
    }

    int read_fd()
    {
        return rfd;
    }

    int write_fd()
    {
        return wfd;
    }

    string cread()
    {
        char buf[MAX_MESSAGE];
        if (read(rfd, buf, MAX_MESSAGE) <= 0)
        {
            EXITONERROR("cread");
        }
        string s = buf;
        return s;
    }

    void cwrite(string msg)
    {
        if (msg.size() > MAX_MESSAGE)
        {
            EXITONERROR("cwrite");
        }
        if (write(wfd, msg.c_str(), msg.size() + 1) < 0)
        { // msg.size() + 1 to include the NULL byte
            EXITONERROR("cwrite");
        }
    }
};
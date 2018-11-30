#pragma once

#include <sys/ipc.h>
#include <sys/sem.h>

class KernelSemaphore
{
    int semid;

  public:
    KernelSemaphore(int value, key_t key)
    {
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
        if(semid < 0) {
            semid = semget(key, 1, 0666);
        }
        else
            if (value != 0)
            {
                struct sembuf sb = {0, value, 0};
                semop(semid, &sb, 1);
            }
    }
    ~KernelSemaphore() {
        semctl(semid, 1, 0);
    }

    void less()
    {
        struct sembuf sb = {0, -1, 0};
        semop(semid, &sb, 1);
    }

    void more()
    {
        struct sembuf sb = {0, 1, 0};
        semop(semid, &sb, 1);
    }
};
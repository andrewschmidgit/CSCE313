#include <sys/ipc.h>
#include <sys/shm.h>

class KernalSemaphore {
    int semid;
    public:
    KernalSemaphore(int value, int seed) {
        key_t key = ftok("a.txt", seed);
        semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
        struct sembuf sb = {0, value, 0};
        semop(semid, &sb, 1);
    }
    ~KernalSemaphore() {}

    void P() {
        struct sembuf sb = {0, -1, 0};
        semop(semid, &sb, 1);
    }

    void V() {
        struct sembuf sb = {0, 1, 0};
        semop(semid, &sb, 1);
    }
};
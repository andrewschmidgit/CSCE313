#include "unistd.h"
#include "Ackerman.h"
#include "BuddyAllocator.h"

int main(int argc, char **argv)
{
    uint blockSize = 128;
    uint memorySize = 1024 * 512;

    int argDelimiter;

    while ((argDelimiter = getopt(argc, argv, "b:s:")) != -1)
    {
        switch (argDelimiter)
        {
            case 'b':
            {
                auto in = strtol(optarg, nullptr, 0);
                if (in > blockSize)
                    blockSize = in;
                break;
            }
            case 's':
            {
                auto in = strtol(optarg, nullptr, 0);
                memorySize = in;
                break;
            }
            case '?':
                if (optopt == 'b' || optopt == 's')
                    cout << "-" << optopt << " requires a value" << endl;
                exit(1);
                break;
            default:
                break;
        }
    }
    
    // create memory manager
    BuddyAllocator *allocator = new BuddyAllocator(blockSize, memorySize);

    allocator->debug();
    // test memory manager
    vector<char*> xs;
    for(int i = 0; i < 10; i++)
        xs.push_back(allocator->alloc(i));
    allocator->debug();
    for(int i = 0; i < 10; i++)
        allocator->free(xs[i]);
    allocator->debug();

    // Ackerman *am = new Ackerman();
    // am->test(allocator); // this is the full-fledged test.

    // destroy memory manager
    delete allocator;
    return 0;
}

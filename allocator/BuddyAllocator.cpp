/* 
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator(uint blockSize, uint memorySize)
{
    _blockSize = _getNextPowerOfTwo(blockSize);
    _memorySize = _getNextPowerOfTwo(memorySize);
    
    // Setting up each list for each level
    for(int i = 0; i <= log2(_memorySize / _blockSize); i++)
        _freeList.push_back(LinkedList(_blockSize << i));
    
    // Creating first block of memory
    _start = new char[_memorySize];
    BlockHeader* block = reinterpret_cast<BlockHeader*>(_start);
    block->Size = _memorySize;
    _freeList.back().Insert(block);
}

BuddyAllocator::~BuddyAllocator()
{
    delete _start;
}

char *BuddyAllocator::alloc(uint length)
{
    uint requestedSize = _getNextPowerOfTwo(length + sizeof(BlockHeader));
    if(requestedSize < _blockSize) requestedSize = _blockSize;
    BlockHeader* block = nullptr;
    for(int i = log2(requestedSize / _blockSize); i < _freeList.size(); i++)
    {
        if(isvalid(_freeList[i].GetHead())) {
            block = _freeList[i].GetHead();
            break;
        }
    }
    if(block == nullptr) return nullptr;
    removeFromFreeList(block);
    block->Free = false;

    while(block->Size != requestedSize) {
        block = split(block);
        BlockHeader* buddy = getbuddy(block);
        buddy->Free = true;
        addToFreeList(buddy);
    }

    // +1 to move over one BlockHeader
    return (char*)(block + 1);
}

int BuddyAllocator::free(char *_a)
{
    // -1 to move over one BlockHeader
    if(_a == nullptr) return 1;
    BlockHeader* block = reinterpret_cast<BlockHeader*>(_a) - 1;
    if(isvalid(block) == false) return 1;
    block->Free = true;
    BlockHeader* buddy = getbuddy(block);
    removeFromFreeList(block);
    while(isvalid(buddy) && block->Free && buddy->Free)
    {
        block = merge(block, buddy);
        removeFromFreeList(buddy);
        buddy = getbuddy(block);
    }

    addToFreeList(block);

    return 0;
}

void BuddyAllocator::debug()
{
    cout << "=====================================================================" << endl;
    cout << "Block Size: " << _blockSize;
    cout << "\t \tMemory Size: " << _memorySize;

    cout << "\t \tSize of BlockHeader: " << sizeof(BlockHeader);
    cout << "\t \tStarting Address: " << _start << endl;
<<<<<<< HEAD:BuddyAllocator.cpp

=======
>>>>>>> 9cab6dd0c279efc1b0b7c63f90630e8a3019c2e4:allocator/BuddyAllocator.cpp
    for (int i = 0; i < _freeList.size(); i++) {
        cout << i << "\t| (" << _freeList[i].GetSize() << "): ";
        auto* cur = _freeList[i].GetHead();
        while (cur != nullptr) {
            cout << "{" << cur->Size << "," << cur << "} ";
            cur = cur->next;
        }
        cout << endl;
    }

    cout << endl;
}
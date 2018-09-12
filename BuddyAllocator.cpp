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
    _start = (BlockHeader*)malloc(_memorySize);
    _start->Size = _memorySize;
    _freeList.back().Insert(_start);
}

BuddyAllocator::~BuddyAllocator()
{
    _start->Size = _memorySize;
    delete _start;
}

char *BuddyAllocator::alloc(uint length)
{
    uint requestedSize = _getNextPowerOfTwo(length + sizeof(BlockHeader));
    if(requestedSize < _blockSize) requestedSize = _blockSize;
    BlockHeader* block = nullptr;
    for(int i = log2(requestedSize / _blockSize); i < _freeList.size(); i++)
        if(isvalid(_freeList[i].GetHead())) {
            block = _freeList[i].GetHead();
            break;
        }

    if(block == nullptr) return (char*)block;
    while(block->Size != requestedSize) {
        cout << "{blockSize, requestedSize}: {" << block->Size << "," << requestedSize << "} -> ";
        removeFromFreeList(block);
        block = split(block);
        addToFreeList(getbuddy(block));
        cout << "{" << block->Size << "," << requestedSize << "}" << endl;
    }

    block->Free = false;
    // +1 to move over one BlockHeader
    return (char*)(block + 1);
}

int BuddyAllocator::free(char *_a)
{
    // -1 to move over one BlockHeader
    BlockHeader* block = ((BlockHeader*)_a) - 1;
    if(isvalid(block) == false) return 1;
    
    block->Free = true;
    BlockHeader* buddy = getbuddy(block);

    while(isvalid(buddy) && block->Free && buddy->Free)
    {
        if (buddy > block) {
            BlockHeader* temp = block;
            block = buddy;
            buddy = block;
        }
        block = merge(block, buddy);
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

    for (int i = 0; i < _freeList.size(); i++) {
        cout << i << " (" << _freeList.at(i).GetSize() << "): ";
        BlockHeader* curr = _freeList.at(i).GetHead();
        while (curr != nullptr) {
            cout << "{" << curr->Size << "," << curr->next << "} ";
            curr = curr->next;
        }
        cout << endl;
    }

    BlockHeader* block = _start;
    block = split(block);
    cout << "Start: " << _start << endl;
    cout << "Block Address: " << block << endl;
    cout << "Buddy Address: " << getbuddy(block) << endl;

    cout << endl;
}
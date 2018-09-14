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
    cout << "=====================================================================" << endl;
    cout << "Block Size: " << _blockSize;
    cout << "\t \tMemory Size: " << _memorySize;

    cout << "\t \tSize of BlockHeader: " << sizeof(BlockHeader);
    cout << "\t \tStarting Address: " << block << endl;
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
    BlockHeader* buddy;
    while(block->Size != requestedSize) {
        removeFromFreeList(block);
        block = split(block);
        block->Free = false;
        buddy = getbuddy(block);
        buddy->Free = true;
        addToFreeList(buddy);
    }

    block->Free = false;
    // +1 to move over one BlockHeader
    return (char*)(block + 1);
}

int BuddyAllocator::free(char *_a)
{
    // -1 to move over one BlockHeader
    if(_a == nullptr) return 1;
    BlockHeader* block = (reinterpret_cast<BlockHeader*>(_a)) - 1;
    if(isvalid(block) == false) return 1;
    block->Free = true;
    BlockHeader* buddy = getbuddy(block);
    removeFromFreeList(block);
    while(isvalid(buddy) && block->Free && buddy->Free)
    {
        block = merge(block, buddy);
        removeFromFreeList(buddy);
        if(block->Size != _memorySize) buddy = getbuddy(block);
        else buddy = nullptr;
    }

    addToFreeList(block);

    return 0;
}

void BuddyAllocator::debug()
{
    for (int i = 0; i < _freeList.size(); i++) {
        cout << i << " (" << _freeList.at(i).GetSize() << "): ";
        BlockHeader* curr = _freeList.at(i).GetHead();
        while (curr != nullptr) {
            cout << "{" << curr->Size << "," << curr << "} ";
            curr = curr->next;
        }
        cout << endl;
    }

    cout << endl;
}
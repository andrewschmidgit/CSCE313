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
    for(int i = log2(requestedSize / _blockSize); i < _freeList.size(); i++) {
        if(isvalid(_freeList.at(i).GetHead())) {
            block = _freeList.at(i).GetHead();
            break;
        }
    }

    if(block == nullptr) return (char*)block;
    while(block->Size != requestedSize)
        block = split(block);

    block->Free = false;
    // +1 to move over one BlockHeader
    return (char*)(block + 1);
}

int BuddyAllocator::free(char *_a)
{
    // -1 to move over one BlockHeader
    BlockHeader* block = (BlockHeader*)(_a - sizeof(BlockHeader));
    if(isvalid(block) == false) {
        cout << "!!!! There was a major problem !!!!" << endl;
        return 1;
    }
    
    block->Free = true;
    BlockHeader* blockBuddy = getbuddy(block);

    intptr_t startAddress = (intptr_t)_start;
    while(isvalid(blockBuddy) && block->Free && blockBuddy->Free)
    {
        if ((intptr_t)block - (intptr_t)blockBuddy - startAddress < 0)
        {
            BlockHeader* temp = block;
            block = blockBuddy;
            blockBuddy = temp;
        }
        block = merge(block, blockBuddy);
        blockBuddy = getbuddy(block);
    }

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

    cout << endl;
}
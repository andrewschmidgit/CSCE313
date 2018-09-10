/* 
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator(uint blockSize, uint memorySize)
{
    _blockSize = _getNextPowerOfTwo(blockSize + sizeof(BlockHeader));
    _memorySize = _getNextPowerOfTwo(memorySize);
    
    // Setting up each list for each level
    for(int i = log2(_memorySize / _blockSize); i >= 0; i--)
        _freeList.push_back(LinkedList(_blockSize << i));

    // Creating first block of memory
    BlockHeader* init = (BlockHeader*)malloc(_memorySize);
    init->Size = _memorySize;
    _freeList[0].Insert(init);
}

BuddyAllocator::~BuddyAllocator()
{
}

char *BuddyAllocator::alloc(uint _length)
{
    /*  This preliminary implementation simply 
        hands the call over the the C standard library! 
        Of course this needs to be replaced by your implementation.
    */
    return new char[_length];
}

int BuddyAllocator::free(char *_a)
{
    /* Same here! */
    delete _a;
    return 0;
}

void BuddyAllocator::debug()
{
    cout << "\t\t\t\t\t\t\t\t\t\t\tDEBUG_MODE_ACTIVE = true;" << endl;
    cout << "Block Size: " << _blockSize << endl;
    cout << "Memory Size: " << _memorySize << endl;

    cout << "Size of BlockHeader: " << sizeof(BlockHeader) << endl;
    // // Inserting
    // int freeListLength = 0;
    
    // for(int i = _blockSize; i <= _memorySize; i *= 2, freeListLength++);
    
    // cout << "Free list length: " << freeListLength << endl;

    // for(int i = 0; i < freeListLength; i++) {
    //     cout << "Iteration: " << i << endl;
    //     uint size = _freeList[i]->GetSize();
        
    //     for(int j = -3; j < i; j++)
    //     {
    //         _freeList[i]->Insert(new BlockHeader(size));
    //     }
    // }
    // cout << "Static list of lengths:" << endl;
    // for(int i = 0; i < freeListLength; i++) {
    //     cout << _freeList[i]->GetSize() << ": " << _freeList[i]->Length() << endl;
    // }

    // // TESTING: LinkedList.Remove()
    // for(int i = 0; i < freeListLength; i++) {
    //     BlockHeader* cur = _freeList[i]->GetHead();
    //     while(cur != nullptr) {
    //         _freeList[i]->Remove(cur);
    //         cur = cur->next;
    //     }
    // }
    
    // cout << "Static list of lengths: " << endl;
    // for(int i = 0; i < freeListLength; i++) {
    //     cout << _freeList[i]->GetSize() << ": " << _freeList[i]->Length() << endl;
    // }
    
    // // TESTING: BuddyAllocator.isvalid()
    // cout << "Testing isvalid " << endl;
    // cout << "Should be true:  " << isvalid((char*)(new BlockHeader(128))) << endl;
    // cout << "Should be false: " << isvalid(new char) << endl;

    // TESTING: BuddyAllocator.merge()


    cout << endl;
}
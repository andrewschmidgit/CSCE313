/* 
    File: my_allocator.h

    Original Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _BuddyAllocator_h_ // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader
{
  public:
    // Linked List Requirements
    BlockHeader *next;
    uint Size;
    bool Free;

    // Structors
    BlockHeader(uint size) : Size(size), Free(true) {}
};

class LinkedList
{
  private:
    BlockHeader *_head; // you need a head of the list
    uint _size;

  public:
    LinkedList(uint size) : _size(size) {}

    uint GetSize() { return _size; }

    BlockHeader *GetHead()
    {
        return _head;
    }

    // adds a block to the list
    void Insert(BlockHeader *b)
    {
        BlockHeader *cur = _head;
        b->next = cur;
        _head = b;
    }

    void Remove(BlockHeader *b)
    {
        if(_head == b) {
            _head = _head->next;
            return;
        }
        BlockHeader *cur = _head;
        while (cur != nullptr)
        {
            if (cur->next == b)
            {
                // Tie adjacent pointers together
                cur->next = cur->next->next;
                break;
            }
            else
            {
                cur = cur->next;
            }
        }
    }

    int Length()
    {
        BlockHeader *cur = _head;
        int length = 0;
        while (cur != nullptr)
        {
            length++;
            cur = cur->next;
        }
        return length;
    }
};

class BuddyAllocator
{
  private:
    /* private function you are required to implement
	 this will allow you and us to do unit test */
    uint _blockSize;
    uint _memorySize;
    char *_start;
    vector<LinkedList> _freeList;

    char *getbuddy(char *addr)
    {
        uint size = ((BlockHeader)*addr).Size;
        uintptr_t zeroedAddress = (uintptr_t)addr - (uintptr_t)_start;
        uintptr_t buddyAddress = zeroedAddress ^ (uintptr_t)size + (uintptr_t)_start;
        return (char *)buddyAddress;
    }
    // given a block address, this function returns the address of its buddy

    bool isvalid(char *addr)
    {
        try
        {
            uint size = ((BlockHeader *)addr)->Size;

            for (uint blockSize = _blockSize; blockSize <= _memorySize; blockSize *= 2)
            {
                if (blockSize == size)
                    return true;
            }

            return false;
        }
        catch (...)
        {
            return false;
        }
    }
    // Is the memory starting at addr is a valid block
    // This is used to verify whether the a computed block address is actually correct

    bool arebuddies(char *block1, char *block2)
    {
        return getbuddy(block1) == block2;
    }
    // checks whether the two blocks are buddies are not

    char *merge(char *block1, char *block2)
    {
        BlockHeader* b1 = (BlockHeader*)block1;
        BlockHeader* b2 = (BlockHeader*)block2;
        intptr_t startAddress = (intptr_t)_start;
        // TODO: Modify Freelist
        while(b1->Free && b2->Free && b1->Size == b2->Size) {
            if ((intptr_t)b2 - (intptr_t)b1 - startAddress < 0)
                swap(b1, b2);
            
            int removeIndex = log2(b2->Size / _blockSize);
            _freeList.at(removeIndex).Remove(b1);
            _freeList.at(removeIndex).Remove(b2);
            _freeList.at(removeIndex - 1).Insert(b1);

            b1->Size *= 2;
            b2 = (BlockHeader*)getbuddy((char*)b1);
        }

        return (char*)b1;
    }
    // this function merges the two blocks returns the beginning address of the merged block
    // note that either block1 can be to the left of block2, or the other way around

    char *split(char *block)
    {
        BlockHeader* blockAddress = (BlockHeader*)block;

        if(blockAddress->Size == _blockSize) return block;
        blockAddress->Size /= 2;
        
        int removeIndex = log2(blockAddress->Size / _blockSize);
        _freeList.at(removeIndex).Remove(blockAddress);
        _freeList.at(removeIndex + 1).Insert(blockAddress);
        _freeList.at(removeIndex + 1).Insert((BlockHeader*)getbuddy((char*)blockAddress));

        return block;
    }
    // splits the given block by putting a new header halfway through the block
    // also, the original header needs to be corrected

    uint _getNextPowerOfTwo(uint value)
    {
        uint power = 1;
        while (power < value)
            power *= 2;
        return power;
    }

  public:
    BuddyAllocator(uint _basic_block_size, uint _total_memory_length);
    /* This initializes the memory allocator and makes a portion of 
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	   its minimal unit of allocation. The function returns the amount of 
	   memory made available to the allocator. If an error occurred, 
	   it returns 0. 
	*/

    ~BuddyAllocator();
    /* Destructor that returns any allocated memory back to the operating system. 
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/

    char *alloc(uint _length);
    /* Allocate _length number of bytes of free memory and returns the 
		address of the allocated portion. Returns 0 when out of memory. */

    int free(char *_a);
    /* Frees the section of physical memory previously allocated 
	   using ’my_malloc’. Returns 0 if everything ok. */

    void debug();
};

#endif
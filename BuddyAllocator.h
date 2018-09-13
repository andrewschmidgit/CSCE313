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
    BlockHeader(uint size) : Size(size), Free(false) {}
};

class LinkedList
{
  private:
    BlockHeader* _head; // you need a head of the list
    uint _size;

  public:
    LinkedList(uint size) : _size(size), _head(nullptr) {}

    uint GetSize() { return _size; }

    BlockHeader* GetHead()
    {
        return _head;
    }

    // adds a block to the list
    void Insert(BlockHeader *b)
    {
        b->next = _head;
        _head = b;
    }

    void Remove(BlockHeader *b)
    {
        if(b == nullptr) return;

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
                return;
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
    char* _start;
    vector<LinkedList> _freeList;

    BlockHeader* getbuddy(BlockHeader* block) { return reinterpret_cast<BlockHeader*>(((reinterpret_cast<char*>(block) - _start) ^ block->Size) + _start); }
    // given a block address, this function returns the address of its buddy

    bool isvalid(BlockHeader* block) { return block != nullptr 
        && block->Size >= _blockSize
        && block->Size <= _memorySize
        && block->Size % _blockSize == 0; }
    // Is the memory starting at addr is a valid block
    // This is used to verify whether the a computed block address is actually correct

    bool arebuddies(BlockHeader* block1, BlockHeader* block2)
    {
        return getbuddy(block1) == block2;
    }
    // checks whether the two blocks are buddies are not

    BlockHeader* merge(BlockHeader* block, BlockHeader* buddy)
    {
        if (block > buddy) {
            BlockHeader* temp = block;
            block = buddy;
            buddy = block;
        }
        block->Size *= 2;
        return block;
    }
    // this function merges the two blocks returns the beginning address of the merged block
    // note that either block1 can be to the left of block2, or the other way around

    BlockHeader* split(BlockHeader* block)
    {
        block->Size /= 2;
        BlockHeader* blockBuddy = getbuddy(block);
        blockBuddy->Size = block->Size;
        blockBuddy->Free = true;

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

    void addToFreeList(BlockHeader* block) {
        for(auto& list: _freeList)
            if(block->Size == list.GetSize()) {
                list.Insert(block);
                break;
            }
    }
    void removeFromFreeList(BlockHeader* block) {
        for(auto& list: _freeList)
            if(block->Size == list.GetSize()) {
                list.Remove(block);
                break;
            }
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
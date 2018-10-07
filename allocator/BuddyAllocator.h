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
    uint _blockSize;
    uint _memorySize;
    char* _start;
    vector<LinkedList> _freeList;

    BlockHeader* getbuddy(BlockHeader* block) 
    {
        if(block->Size >= _memorySize) return nullptr;
        return reinterpret_cast<BlockHeader*>(((reinterpret_cast<char*>(block) - _start) ^ block->Size) + _start);
    }

    bool isvalid(BlockHeader* block) { return block != nullptr 
        && block->Size >= _blockSize
        && block->Size <= _memorySize
        && block->Size % _blockSize == 0; }

    bool arebuddies(BlockHeader* block1, BlockHeader* block2)
    {
        return getbuddy(block1) == block2;
    }

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

    BlockHeader* split(BlockHeader* block)
    {
        block->Size /= 2;
        BlockHeader* blockBuddy = getbuddy(block);
        blockBuddy->Size = block->Size;
        blockBuddy->Free = true;

        return block;
    }

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

    ~BuddyAllocator();

    char *alloc(uint _length);

    int free(char *_a);

    void debug();
};

#endif
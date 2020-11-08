#pragma once

#include <atomic>

//----------------------------------------------
//  The World's Simplest Lock-Free HashTable
//
//  Maps 32-bit integers to 32-bit integers.
//  Uses open addressing with linear probing.
//  You can call SetItem and GetItem from several threads simultaneously.
//  GetItem is wait-free.
//  SetItem is lock-free.
//  You can't assign any value to key = 0.
//  In the m_cells array, key = 0 is reserved to indicate an unused cell.
//  You can't assign value = 0 to any key.
//  value = 0 means the key is unused.
//  The hash table never grows in size.
//  You can't delete individual items from the hash table.
//  You can Clear the hash table, but only at a time when there are no other calls being made from other threads.
//----------------------------------------------
class HashTable1
{
public:
    struct Entry
    {
        std::atomic<int> key;
        std::atomic<int> value;
    };

private:
    Entry* m_entries;
    uint32_t m_arraySize;

public:
    HashTable1(uint32_t arraySize);
    ~HashTable1();

    // Basic operations
    void SetItem(uint32_t key, uint32_t value);
    uint32_t GetItem(uint32_t key);
    uint32_t GetItemCount();
    void Clear();
};
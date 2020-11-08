#pragma once

#include <atomic>
//#include <memory>

class parallel_hash_table
{
public:
    struct entry
    {
        std::atomic<unsigned int> key;
        std::atomic<unsigned int> value;
    };

private:
    entry* m_entries;
    int m_arraySize;

public:
    parallel_hash_table(unsigned int arraySize);
    ~parallel_hash_table();

    // Basic operations
    void SetItem(unsigned int key, unsigned int value);
    unsigned int GetItem(unsigned int key);
    unsigned int GetItemCount();
    void Clear();
};

class simple_hash_table
{
    struct entry {
        int key;
        int value;
    };


private:
    entry* m_entries;
    int m_arraySize;

    int hashCode(int key) {
        return key % m_arraySize;
    }

public:
    simple_hash_table(unsigned int arraySize);
    ~simple_hash_table();

    // Basic operations
    void SetItem(unsigned int key, unsigned int value);
    unsigned int GetItem(unsigned int key);
    unsigned int GetItemCount();
    void Clear();

};
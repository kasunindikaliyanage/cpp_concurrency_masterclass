#include "hash_table_x.h"
#include <assert.h>
#include <memory.h>


#define USE_FAST_SETITEM 0


//----------------------------------------------
// from code.google.com/p/smhasher/wiki/MurmurHash3
inline static int integerHash(unsigned int h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}


//----------------------------------------------
parallel_hash_table::parallel_hash_table(unsigned int arraySize)
{
    // Initialize cells
    assert((arraySize & (arraySize - 1)) == 0);   // Must be a power of 2
    m_arraySize = arraySize;
    m_entries = new entry[arraySize];
    Clear();
}


//----------------------------------------------
parallel_hash_table::~parallel_hash_table()
{
    // Delete cells
    delete[] m_entries;
}

void parallel_hash_table::SetItem(unsigned int key, unsigned int value)
{
    assert(key != 0);
    assert(value != 0);

    for (uint32_t idx = integerHash(key);; idx++)
    {
        idx &= m_arraySize - 1;
        unsigned int prevKey = 0;
        
        //int prevKey = std::atomic_compare_exchange_strong(&m_entries[idx].key, 0, key);
        m_entries[idx].key.compare_exchange_strong(prevKey, key, std::memory_order_relaxed);

        if ((prevKey == 0) || (prevKey == key))
        {
            m_entries[idx].value.store(value, std::memory_order_relaxed);
            return;
        }
    }
}

//----------------------------------------------
unsigned int parallel_hash_table::GetItem(unsigned int key)
{
    assert(key != 0);

    for (uint32_t idx = integerHash(key);; idx++)
    {
        idx &= m_arraySize - 1;

        //uint32_t probedKey = mint_load_32_relaxed(&m_entries[idx].key);
        int probedKey = m_entries[idx].key.load(std::memory_order_relaxed);
        if (probedKey == key)
            //return mint_load_32_relaxed(&m_entries[idx].value);
            return m_entries[idx].value.load(std::memory_order_relaxed);
        if (probedKey == 0)
            return 0;
    }
}


//----------------------------------------------
unsigned int parallel_hash_table::GetItemCount()
{
    uint32_t itemCount = 0;
    for (uint32_t idx = 0; idx < m_arraySize; idx++)
    {
        if ((m_entries[idx].key.load(std::memory_order_relaxed) != 0)
            && (m_entries[idx].value.load(std::memory_order_relaxed) != 0))
            itemCount++;
    }
    return itemCount;
}


//----------------------------------------------
void parallel_hash_table::Clear()
{
    memset(m_entries, 0, sizeof(entry) * m_arraySize);
}
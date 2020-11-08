#include <assert.h>
#include <memory.h>

#include "parallel_hashtable.h"


#define USE_FAST_SETITEM 1


//----------------------------------------------
// from code.google.com/p/smhasher/wiki/MurmurHash3
inline static uint32_t integerHash(uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}


//----------------------------------------------
HashTable1::HashTable1(uint32_t arraySize)
{
    // Initialize cells
    assert((arraySize & (arraySize - 1)) == 0);   // Must be a power of 2
    m_arraySize = arraySize;
    m_entries = new Entry[arraySize];
    Clear();
}


//----------------------------------------------
HashTable1::~HashTable1()
{
    // Delete cells
    delete[] m_entries;
}


//----------------------------------------------
#if USE_FAST_SETITEM
void HashTable1::SetItem(uint32_t key, uint32_t value)
{
    assert(key != 0);
    assert(value != 0);

    for (uint32_t idx = integerHash(key);; idx++)
    {
        idx &= m_arraySize - 1;

        // Load the key that was there.
        uint32_t probedKey = std::atomic_load(&m_entries[idx].key);
        if (probedKey != key)
        {
            // The entry was either free, or contains another key.
            if (probedKey != 0)
                continue;           // Usually, it contains another key. Keep probing.

            // The entry was free. Now let's try to take it using a CAS.
            uint32_t prevKey = std::atomic_compare_exchange_strong(&m_entries[idx].key, 0, key);
            if ((prevKey != 0) && (prevKey != key))
                continue;       // Another thread just stole it from underneath us.

            // Either we just added the key, or another thread did.
        }

        // Store the value in this array entry.
        std::atomic_store(&m_entries[idx].value, value);
        return;
    }
}
#else
void HashTable1::SetItem(uint32_t key, uint32_t value)
{
    assert(key != 0);
    assert(value != 0);

    for (uint32_t idx = integerHash(key);; idx++)
    {
        idx &= m_arraySize - 1;

        uint32_t prevKey = mint_compare_exchange_strong_32_relaxed(&m_entries[idx].key, 0, key);
        if ((prevKey == 0) || (prevKey == key))
        {
            mint_store_32_relaxed(&m_entries[idx].value, value);
            return;
        }
    }
}
#endif

//----------------------------------------------
uint32_t HashTable1::GetItem(uint32_t key)
{
    assert(key != 0);

    for (uint32_t idx = integerHash(key);; idx++)
    {
        idx &= m_arraySize - 1;

        uint32_t probedKey = std::atomic_load(&m_entries[idx].key);
        if (probedKey == key)
            return atomic_load(&m_entries[idx].value);
        if (probedKey == 0)
            return 0;
    }
}


//----------------------------------------------
uint32_t HashTable1::GetItemCount()
{
    uint32_t itemCount = 0;
    for (uint32_t idx = 0; idx < m_arraySize; idx++)
    {
        if ((atomic_load(&m_entries[idx].key) != 0)
            && (atomic_load(&m_entries[idx].value) != 0))
            itemCount++;
    }
    return itemCount;
}


//----------------------------------------------
void HashTable1::Clear()
{
    memset(m_entries, 0, sizeof(Entry) * m_arraySize);
}
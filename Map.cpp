#include "Map.h"
#include "MapIterator.h"
#include "iostream"

Map::Map() {
    capacity = 3;
    tableSize = 0;
    table1 = new Bucket[capacity];
    table2 = new Bucket[capacity];
}

Map::~Map() {
    delete[] table1;
    delete[] table2;
}

/// adds a pair (key,value) to the map
///if the key already exists in the map, then the value associated to the key is replaced by the new value and the old value is returned
///if the key does not exist, a new pair is added and the value null is returned
TValue Map::add(TKey c, TValue v)
{

    if (search(c) != NULL_TVALUE)
    {
        int h1 = hashFunction1(c);
        int h2 = hashFunction2(c);
        TValue old_v;
        if (table1[h1].element.first == c)
        {
            old_v = table1[h1].element.second;
            table1[h1].element.second = v;
        }
        else
        {
            old_v = table2[h2].element.second;
            table2[h2].element.second = v;
        }
        return old_v;
    }

    int rehashesCount = 0;
    int possibleAddress = hashFunction1(c);
    TKey currentKey = c;
    TValue currentValue = v;
    while (rehashesCount < MAX_REHASHES)
    {
        if (rehashesCount % 2 == 0)
        {
            if (table1[possibleAddress].element == NULL_TELEM)
            {
                table1[possibleAddress].element.first = currentKey;
                table1[possibleAddress].element.second = currentValue;
                tableSize++;
                return NULL_TVALUE;
            }

            TElem oldElem = table1[possibleAddress].element;
            table1[possibleAddress].element.first = currentKey;
            table1[possibleAddress].element.second = currentValue;

            possibleAddress = hashFunction2(oldElem.first);
            currentKey = oldElem.first;
            currentValue = oldElem.second;
        }
        else
        {
            if (table2[possibleAddress].element == NULL_TELEM)
            {
                table2[possibleAddress].element.first = currentKey;
                table2[possibleAddress].element.second = currentValue;
                tableSize++;
                return NULL_TVALUE;
            }

            TElem oldElem = table2[possibleAddress].element;
            table2[possibleAddress].element.first = currentKey;
            table2[possibleAddress].element.second = currentValue;

            possibleAddress = hashFunction1(oldElem.first);
            currentKey = oldElem.first;
            currentValue = oldElem.second;
        }

        rehashesCount++;
    }

    if (rehashesCount == MAX_REHASHES)
    {
        resize();
        add(currentKey, currentValue);
    }

}


void Map::resize()
{
    int oldCapacity = capacity;
    capacity *= 2;

    Bucket* oldTable1 = table1;
    Bucket* oldTable2 = table2;

    table1 = new Bucket[capacity];
    table2 = new Bucket[capacity];

    for (int i = 0; i < capacity; i++)
    {
        table1[i].element = NULL_TELEM;
        table2[i].element = NULL_TELEM;
    }

    this->tableSize = 0;

    for (int i = 0; i < oldCapacity; i++)
    {
        if (oldTable1[i].element != NULL_TELEM)
        {
            TKey key = oldTable1[i].element.first;
            TValue value = oldTable1[i].element.second;
            add(key, value);
        }
        if (oldTable2[i].element != NULL_TELEM)
        {
            TKey key = oldTable2[i].element.first;
            TValue value = oldTable2[i].element.second;
            add(key, value);
        }
    }
    delete[] oldTable1;
    delete[] oldTable2;
}

///searches for the key and returns the value associated with the key if the map contains the key or null: NULL_TVALUE otherwise
TValue Map::search(TKey c) const
{
    int hash1 = hashFunction1(c);
    if (table1[hash1].element.first == c)
    {
        return table1[hash1].element.second;
    }

    int hash2 = hashFunction2(c);
    if (table2[hash2].element.first == c)
    {
        return table2[hash2].element.second;
    }

    return NULL_TVALUE;
}

///removes a key from the map and returns the value associated with the key if the key existed ot null: NULL_TVALUE otherwise
TValue Map::remove(TKey c)
{
    int hash1 = hashFunction1(c);
    if (table1[hash1].element.first == c)
    {
        TValue value = table1[hash1].element.second;
        table1[hash1].element = NULL_TELEM;
        this->tableSize--;
        return value;
    }

    int hash2 = hashFunction2(c);
    if (table2[hash2].element.first == c)
    {
        TValue value = table2[hash2].element.second;
        table2[hash2].element = NULL_TELEM;
        this->tableSize--;
        return value;
    }

    return NULL_TVALUE;
}


///returns the number of pairs (key,value) from the map
int Map::size() const
{
    return this->tableSize;
}

///checks whether the map is empty or not
bool Map::isEmpty() const
{
    return this-> tableSize == 0;
}

///returns an iterator for the map
MapIterator Map::iterator() const
{
    return MapIterator(*this);
}



#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 08: HASH MAP / HASH TABLE (Separate Chaining)
 * ============================================================================
 * 
 * A Hash Map is an associative key-value data structure offering O(1) average
 * lookup, insertion, and deletion using a hash function.
 * 
 * Key Interview Concepts:
 * 1. Hash Function: Maps arbitrary key types to bucket indices [0, capacity - 1].
 * 2. Collision Handling:
 *    - Separate Chaining (Linked Lists / Buckets) -> Used here.
 *    - Open Addressing (Linear Probing, Quadratic Probing, Double Hashing).
 * 3. Load Factor (LF) = size / capacity:
 *    - When LF > 0.75, the map automatically triggers REHASHING (doubling bucket
 *      count and re-indexing all entries) to maintain O(1) average time complexity.
 * 
 * Time Complexities:
 * - put(key, val)   : Average O(1), Worst-case O(N) [All keys collide in 1 bucket]
 * - get(key)        : Average O(1), Worst-case O(N)
 * - remove(key)     : Average O(1), Worst-case O(N)
 * - containsKey(key): Average O(1), Worst-case O(N)
 * Space Complexity: O(N + B) where B is bucket count
 */

template <typename K, typename V>
class HashMap {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        HashNode(const K& k, const V& v, HashNode* n = nullptr) : key(k), value(v), next(n) {}
    };

    vector<HashNode*> buckets;
    size_t numElements, numBuckets;
    const float MAX_LOAD_FACTOR = 0.75f;

    // Hash code generator (Polynomial rolling hash for strings, std::hash for generics)
    size_t getHashCode(const K& key) const { return std::hash<K>{}(key); }

    // Maps hash code to a valid bucket index: [0, numBuckets - 1]
    size_t getBucketIndex(const K& key) const { return getHashCode(key) % numBuckets; }

    // Rehash: Doubles bucket count and redistributes all nodes to avoid degradation to O(N)
    void rehash() {
        size_t oldBuckets = numBuckets;
        vector<HashNode*> oldBucketArray = buckets;
        numBuckets *= 2;
        buckets.assign(numBuckets, nullptr);
        numElements = 0; // Will be incremented during re-insertion

        for (size_t i = 0; i < oldBuckets; i++) {
            HashNode* curr = oldBucketArray[i];
            while (curr) {
                put(curr->key, curr->value);
                HashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

public:
    HashMap(size_t initialCapacity = 16) : numElements(0), numBuckets(initialCapacity == 0 ? 16 : initialCapacity) {
        buckets.resize(numBuckets, nullptr);
    }
    ~HashMap() { clear(); }

    void clear() {
        for (size_t i = 0; i < numBuckets; i++) {
            HashNode* curr = buckets[i];
            while (curr) {
                HashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        numElements = 0;
    }

    // Insert or update key-value pair: Average O(1)
    void put(const K& key, const V& value) {
        size_t bucketIdx = getBucketIndex(key);
        HashNode* curr = buckets[bucketIdx];
        while (curr) {
            if (curr->key == key) { curr->value = value; return; }
            curr = curr->next;
        }
        buckets[bucketIdx] = new HashNode(key, value, buckets[bucketIdx]);
        numElements++;
        if ((float)numElements / numBuckets > MAX_LOAD_FACTOR) rehash();
    }

    // Retrieve value by key: Average O(1)
    V get(const K& key) const {
        size_t bucketIdx = getBucketIndex(key);
        for (HashNode* curr = buckets[bucketIdx]; curr; curr = curr->next) {
            if (curr->key == key) return curr->value;
        }
        throw out_of_range("[HashMap Error] Key not found!");
    }

    // Check if key exists: Average O(1)
    bool containsKey(const K& key) const {
        size_t bucketIdx = getBucketIndex(key);
        for (HashNode* curr = buckets[bucketIdx]; curr; curr = curr->next) {
            if (curr->key == key) return true;
        }
        return false;
    }

    // Remove key from map: Average O(1)
    bool remove(const K& key) {
        size_t bucketIdx = getBucketIndex(key);
        HashNode *curr = buckets[bucketIdx], *prev = nullptr;
        while (curr) {
            if (curr->key == key) {
                if (!prev) buckets[bucketIdx] = curr->next;
                else prev->next = curr->next;
                delete curr;
                numElements--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // Access / Insert via subscript operator[]
    V& operator[](const K& key) {
        size_t bucketIdx = getBucketIndex(key);
        for (HashNode* curr = buckets[bucketIdx]; curr; curr = curr->next) {
            if (curr->key == key) return curr->value;
        }
        put(key, V{});
        for (HashNode* curr = buckets[getBucketIndex(key)]; curr; curr = curr->next) {
            if (curr->key == key) return curr->value;
        }
        throw runtime_error("[HashMap Error] Internal error.");
    }

    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    float loadFactor() const { return (float)numElements / numBuckets; }
    size_t bucketCount() const { return numBuckets; }
};

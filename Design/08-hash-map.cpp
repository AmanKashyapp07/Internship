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
        HashNode(const K& k, const V& v, HashNode* n = nullptr)
            : key(k), value(v), next(n) {}
    };

    vector<HashNode*> buckets;
    size_t numElements;
    size_t numBuckets;
    const float MAX_LOAD_FACTOR = 0.75f;

    // Hash code generator (Polynomial rolling hash for strings, std::hash for generics)
    size_t getHashCode(const K& key) const {
        return std::hash<K>{}(key);
    }

    // Maps hash code to a valid bucket index: [0, numBuckets - 1]
    size_t getBucketIndex(const K& key) const {
        return getHashCode(key) % numBuckets;
    }

    // Rehash: Doubles bucket count and redistributes all nodes to avoid degradation to O(N)
    void rehash() {
        size_t oldBuckets = numBuckets;
        vector<HashNode*> oldBucketArray = buckets;

        numBuckets *= 2;
        buckets.assign(numBuckets, nullptr);
        numElements = 0; // Will be incremented during re-insertion

        for (size_t i = 0; i < oldBuckets; i++) {
            HashNode* curr = oldBucketArray[i];
            while (curr != nullptr) {
                put(curr->key, curr->value);
                HashNode* temp = curr;
                curr = curr->next;
                delete temp; // Free old node as put() allocates a new node
            }
        }
    }

public:
    HashMap(size_t initialCapacity = 16)
        : numElements(0), numBuckets(initialCapacity) {
        if (numBuckets == 0) numBuckets = 16;
        buckets.resize(numBuckets, nullptr);
    }

    ~HashMap() {
        clear();
    }

    void clear() {
        for (size_t i = 0; i < numBuckets; i++) {
            HashNode* curr = buckets[i];
            while (curr != nullptr) {
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
        HashNode* head = buckets[bucketIdx];

        // 1. Search if key already exists in bucket chain -> Update value
        HashNode* curr = head;
        while (curr != nullptr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        // 2. Key does not exist -> Insert new node at HEAD of bucket chain: O(1)
        HashNode* newNode = new HashNode(key, value, head);
        buckets[bucketIdx] = newNode;
        numElements++;

        // 3. Check load factor and rehash if threshold exceeded
        if ((float)numElements / numBuckets > MAX_LOAD_FACTOR) {
            rehash();
        }
    }

    // Retrieve value by key: Average O(1)
    V get(const K& key) const {
        size_t bucketIdx = getBucketIndex(key);
        HashNode* curr = buckets[bucketIdx];

        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;
            }
            curr = curr->next;
        }
        throw out_of_range("[HashMap Error] Key not found!");
    }

    // Check if key exists: Average O(1)
    bool containsKey(const K& key) const {
        size_t bucketIdx = getBucketIndex(key);
        HashNode* curr = buckets[bucketIdx];

        while (curr != nullptr) {
            if (curr->key == key) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // Remove key from map: Average O(1)
    bool remove(const K& key) {
        size_t bucketIdx = getBucketIndex(key);
        HashNode* curr = buckets[bucketIdx];
        HashNode* prev = nullptr;

        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    // Removing head of bucket chain
                    buckets[bucketIdx] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                numElements--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false; // Key not found
    }

    // Access / Insert via subscript operator[]
    V& operator[](const K& key) {
        size_t bucketIdx = getBucketIndex(key);
        HashNode* curr = buckets[bucketIdx];

        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;
            }
            curr = curr->next;
        }

        // Insert default constructed value if key is missing
        put(key, V{});
        // Retrieve newly inserted node reference
        curr = buckets[getBucketIndex(key)];
        while (curr != nullptr) {
            if (curr->key == key) return curr->value;
            curr = curr->next;
        }
        throw runtime_error("[HashMap Error] Internal error.");
    }

    size_t size() const {
        return numElements;
    }

    bool empty() const {
        return numElements == 0;
    }

    float loadFactor() const {
        return (float)numElements / numBuckets;
    }

    size_t bucketCount() const {
        return numBuckets;
    }
};

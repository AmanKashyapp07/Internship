#include <iostream>
#include <unordered_map>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 16: LFU CACHE (Least Frequently Used Cache)
 * ============================================================================
 * 
 * An LFU Cache evicts the item with the LOWEST access frequency.
 * If there is a tie in minimum frequency, it evicts the LEAST RECENTLY USED (LRU)
 * item among those with that same frequency.
 * 
 * Key Interview Architecture:
 * 1. `keyTable`  : `unordered_map<int, Node*>` -> O(1) Key-to-Node lookup.
 * 2. `freqTable` : `unordered_map<int, DoublyLinkedList*>` -> Maps each frequency
 *                  count to a Doubly Linked List of nodes with that exact frequency.
 *                  (Most recently accessed nodes are placed at the HEAD of their freq list).
 * 3. `minFreq`   : Integer tracking the current global minimum frequency across all elements.
 * 
 * Time Complexities:
 * - get(key)      : O(1)
 * - put(key, val) : O(1)
 * Space Complexity: O(Capacity)
 */

class LFUCache {
private:
    struct Node {
        int key;
        int value;
        int freq;
        Node* prev;
        Node* next;
        Node(int k = 0, int v = 0)
            : key(k), value(v), freq(1), prev(nullptr), next(nullptr) {}
    };

    // Doubly Linked List with dummy sentinel nodes for each frequency level
    struct DoublyLinkedList {
        Node* head;
        Node* tail;
        int size;

        DoublyLinkedList() : size(0) {
            head = new Node();
            tail = new Node();
            head->next = tail;
            tail->prev = head;
        }

        ~DoublyLinkedList() {
            Node* curr = head;
            while (curr != nullptr) {
                Node* nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
        }

        // Add node right after head (marks node as MRU in this frequency bucket): O(1)
        void addNode(Node* node) {
            node->prev = head;
            node->next = head->next;
            head->next->prev = node;
            head->next = node;
            size++;
        }

        // Unlink node from this list: O(1)
        void removeNode(Node* node) {
            Node* prevNode = node->prev;
            Node* nextNode = node->next;
            prevNode->next = nextNode;
            nextNode->prev = prevNode;
            size--;
        }

        // Pop LRU node (node immediately preceding dummy tail): O(1)
        Node* popTail() {
            if (isEmpty()) return nullptr;
            Node* lruNode = tail->prev;
            removeNode(lruNode);
            return lruNode;
        }

        bool isEmpty() const {
            return size == 0;
        }
    };

    int capacity;
    int curSize;
    int minFreq;
    unordered_map<int, Node*> keyTable;
    unordered_map<int, DoublyLinkedList*> freqTable;

    // Helper: Promotes node to (freq + 1) bucket: O(1)
    void updateFreq(Node* node) {
        int oldFreq = node->freq;
        DoublyLinkedList* oldList = freqTable[oldFreq];
        oldList->removeNode(node);

        // If the minimum frequency list becomes empty, increment minFreq
        if (oldFreq == minFreq && oldList->isEmpty()) {
            minFreq++;
        }

        node->freq++;
        int newFreq = node->freq;

        // Allocate new frequency list if not already present
        if (freqTable.find(newFreq) == freqTable.end()) {
            freqTable[newFreq] = new DoublyLinkedList();
        }
        freqTable[newFreq]->addNode(node);
    }

public:
    LFUCache(int cap) : capacity(cap), curSize(0), minFreq(0) {}

    ~LFUCache() {
        for (auto& pair : freqTable) {
            delete pair.second;
        }
    }

    // Get value by key: O(1)
    int get(int key) {
        if (capacity <= 0) return -1;

        auto it = keyTable.find(key);
        if (it == keyTable.end()) {
            return -1; // Key not present
        }

        Node* node = it->second;
        updateFreq(node); // Elevate node frequency
        return node->value;
    }

    // Insert or update key-value pair: O(1)
    void put(int key, int value) {
        if (capacity <= 0) return;

        auto it = keyTable.find(key);

        if (it != keyTable.end()) {
            // Key already exists -> Update value & promote frequency
            Node* node = it->second;
            node->value = value;
            updateFreq(node);
        } else {
            // New Key insertion: Check capacity eviction
            if (curSize >= capacity) {
                // Evict LRU node from minFreq list
                DoublyLinkedList* minFreqList = freqTable[minFreq];
                Node* evictedNode = minFreqList->popTail();

                if (evictedNode) {
                    keyTable.erase(evictedNode->key);
                    delete evictedNode;
                    curSize--;
                }
            }

            // Create new node with frequency = 1
            Node* newNode = new Node(key, value);
            minFreq = 1; // Reset minFreq to 1 for newly inserted elements

            if (freqTable.find(1) == freqTable.end()) {
                freqTable[1] = new DoublyLinkedList();
            }
            freqTable[1]->addNode(newNode);
            keyTable[key] = newNode;
            curSize++;
        }
    }

    int getSize() const {
        return curSize;
    }

    int getCapacity() const {
        return capacity;
    }
};

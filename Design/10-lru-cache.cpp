#include <iostream>
#include <unordered_map>
#include <stdexcept>
using namespace std;

/**
 * ============================================================================
 * DESIGN 10: LRU CACHE (Least Recently Used Cache)
 * ============================================================================
 * 
 * An LRU Cache evicts the least recently accessed item when capacity is reached.
 * 
 * Frequently Asked in Interviews:
 * 1. Why Doubly Linked List + HashMap?
 *    - HashMap gives O(1) key-to-node lookup.
 *    - Doubly Linked List gives O(1) removal and insertion of nodes from any position.
 *    - (Singly linked list cannot delete a node in O(1) because it lacks `prev` pointer).
 * 2. Dummy Head & Dummy Tail Sentinel Nodes:
 *    - Eliminates null checks when inserting at head or removing from tail/middle.
 * 
 * Time Complexities:
 * - get(key)      : O(1)
 * - put(key, val) : O(1)
 * Space Complexity: O(Capacity)
 */

class LRUCache {
private:
    struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;
        Node(int k = 0, int v = 0) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    int size;
    unordered_map<int, Node*> map;
    Node* head; // Dummy head sentinel: Most Recently Used (MRU) adjacent to head
    Node* tail; // Dummy tail sentinel: Least Recently Used (LRU) adjacent to tail

    // Helper: Add node right after dummy head (mark as most recently used): O(1)
    void addNode(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    // Helper: Unlink an existing node from the doubly linked list: O(1)
    void removeNode(Node* node) {
        Node* prevNode = node->prev;
        Node* nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    // Helper: Move an accessed node to the front (head): O(1)
    void moveToHead(Node* node) {
        removeNode(node);
        addNode(node);
    }

    // Helper: Pop the least recently used item (node right before dummy tail): O(1)
    Node* popTail() {
        Node* lruNode = tail->prev;
        removeNode(lruNode);
        return lruNode;
    }

public:
    LRUCache(int cap) : capacity(cap), size(0) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }

    // Get value by key: O(1)
    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) {
            return -1; // Key does not exist
        }
        Node* node = it->second;
        moveToHead(node); // Mark as most recently used
        return node->value;
    }

    // Insert or update key-value pair: O(1)
    void put(int key, int value) {
        auto it = map.find(key);

        if (it != map.end()) {
            // Key already exists -> Update value and move to head
            Node* node = it->second;
            node->value = value;
            moveToHead(node);
        } else {
            // New key insertion
            Node* newNode = new Node(key, value);
            map[key] = newNode;
            addNode(newNode);
            size++;

            // If capacity exceeded -> Evict Least Recently Used (LRU) node
            if (size > capacity) {
                Node* lru = popTail();
                map.erase(lru->key);
                delete lru;
                size--;
            }
        }
    }

    // Remove specific key: O(1)
    bool remove(int key) {
        auto it = map.find(key);
        if (it == map.end()) return false;

        Node* node = it->second;
        removeNode(node);
        map.erase(key);
        delete node;
        size--;
        return true;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }
};

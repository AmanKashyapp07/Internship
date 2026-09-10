#include <bits/stdc++.h>
using namespace std;

// Standard singly linked list node
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x = 0, ListNode* n = nullptr) : val(x), next(n) {}
};

// Generic node with multi-directional pointers (doubly, random, child, tree)
struct Node {
    int val;
    Node* prev;
    Node* next;
    Node* child;
    Node* random;
    Node* left;
    Node* right;

    Node(int v = 0, Node* n = nullptr)
        : val(v), prev(nullptr), next(n), child(nullptr),
          random(nullptr), left(nullptr), right(nullptr) {}
};

/*
 ====================================================================================================
                  LINKED LIST — MICROSOFT TECHNICAL INTERVIEW & DESIGN PROBLEMS
 ====================================================================================================
 | #  | Problem Name                                     | Pattern / Technique                | Time      | Space  |
 |----|---------------------------------------------------|-------------------------------------|-----------|--------|
 | 01 | Reverse Linked List (LC 206)                      | Iterative Pointer Reversal          | O(N)      | O(1)   |
 | 02 | Merge Two Sorted Lists (LC 21)                    | Two-Pointer Linked Splice           | O(N+M)    | O(1)   |
 | 03 | Middle of the Linked List (LC 876)                | Slow/Fast Pointer                   | O(N)      | O(1)   |
 | 04 | Swap Nodes in Pairs (LC 24)                       | Adjacent Node Pointer Swap          | O(N)      | O(1)   |
 | 05 | Remove Duplicates from Sorted List (LC 83)        | Single Pass Neighbor Skip           | O(N)      | O(1)   |
 | 06 | Remove Linked List Elements (LC 203)              | Dummy Node + Value Filter           | O(N)      | O(1)   |
 | 07 | Delete Node in a Linked List (LC 237)             | Value/Next Overwrite Trick          | O(1)      | O(1)   |
 | 08 | Insert into a Sorted Circular List (LC 708)       | Circular Boundary & Inflection      | O(N)      | O(1)   |
 | 09 | Flatten Multilevel Doubly Linked List (LC 430)    | DFS Child Splice                   | O(N)      | O(1)   |
 | 10 | Convert BST to Sorted Doubly Linked List (LC 426) | In-order Traversal + Pointer Relink | O(N)      | O(H)   |
 | 11 | Design Linked List (LC 707)                       | Doubly-Linked Sentinel Nodes        | O(N)/op   | O(N)   |
 | 12 | LRU Cache (LC 146)                                | Hash Map + Doubly-Linked List        | O(1) ops  | O(C)   |
 | 13 | LFU Cache (LC 460)                                | Hash Map + Doubly-Linked Buckets    | O(1) ops  | O(C)   |
 | 14 | Design Browser History (LC 1472)                  | Doubly-Linked List Navigation       | O(1) avg  | O(N)   |
 ====================================================================================================
*/

// 1. Reverse Linked List
// Reverse a singly linked list and return the new head.
struct Solution1 {
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;

        while (curr != nullptr) {
            ListNode* nextNode = curr->next; // Store next node
            curr->next = prev;               // Invert pointer
            prev = curr;                     // Advance prev
            curr = nextNode;                 // Advance curr
        }

        return prev;
    }
};

// 2. Merge Two Sorted Lists
// Merge two sorted linked lists into one sorted list.
struct Solution2 {
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* dummy = new ListNode(0);
        ListNode* tail = dummy;

        while (list1 != nullptr && list2 != nullptr) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }

        // Attach remaining elements
        tail->next = (list1 != nullptr) ? list1 : list2;

        return dummy->next;
    }
};

// 3. Middle of the Linked List
// Return the middle node (second middle if two exist).
struct Solution3 {
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // Slow advances 1 step, fast advances 2 steps
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }
};

// 4. Swap Nodes in Pairs
// Swap every two adjacent nodes and return the head, without modifying values.
struct Solution4 {
    ListNode* swapPairs(ListNode* head) {
        ListNode* dummy = new ListNode(0, head);
        ListNode* prev = dummy;

        // Ensure there are at least two nodes to swap
        while (prev->next != nullptr && prev->next->next != nullptr) {
            ListNode* first = prev->next;
            ListNode* second = prev->next->next;

            // Swapping pointers
            first->next = second->next;
            second->next = first;
            prev->next = second;

            // Advance prev to the node before the next pair
            prev = first;
        }

        return dummy->next;
    }
};

// 5. Remove Duplicates from Sorted List
// Remove duplicate values so each value appears once.
struct Solution5 {
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;

        while (curr != nullptr && curr->next != nullptr) {
            if (curr->val == curr->next->val) {
                // Skip the duplicate node
                curr->next = curr->next->next;
            } else {
                // Advance only when no duplicate found
                curr = curr->next;
            }
        }

        return head;
    }
};

// 6. Remove Linked List Elements
// Remove all nodes whose value equals val.
struct Solution6 {
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* dummy = new ListNode(0, head);
        ListNode* curr = dummy;

        while (curr->next != nullptr) {
            if (curr->next->val == val) {
                // Bypass node with target value
                curr->next = curr->next->next;
            } else {
                curr = curr->next;
            }
        }

        return dummy->next;
    }
};

// 7. Delete Node in a Linked List (no head access)
// Delete the given node without access to the list head (guaranteed not the tail).
struct Solution7 {
    void deleteNode(ListNode* node) {
        // Copy the value of the next node into the current node
        node->val = node->next->val;
        // Bypass the next node
        node->next = node->next->next;
    }
};

// 8. Insert into a Sorted Circular Linked List
// Insert a value into a sorted circular linked list at its correct position.
struct Solution8 {
    Node* insert(Node* head, int insertVal) {
        // Case 1: Empty list -> create single self-pointing circular node
        if (head == nullptr) {
            Node* newNode = new Node(insertVal);
            newNode->next = newNode;
            return newNode;
        }

        Node* prev = head;
        Node* curr = head->next;

        while (true) {
            // Case 2: Normal insertion between two ordered values
            if (prev->val <= insertVal && insertVal <= curr->val) {
                break;
            }

            // Case 3: Inflection point (max -> min transition), value is either max or min
            if (prev->val > curr->val && (insertVal >= prev->val || insertVal <= curr->val)) {
                break;
            }

            prev = curr;
            curr = curr->next;

            // Case 4: Full cycle traversed (all elements in the list have equal value)
            if (prev == head) {
                break;
            }
        }

        // Splice new node between prev and curr
        prev->next = new Node(insertVal, curr);
        return head;
    }
};

// 9. Flatten a Multilevel Doubly Linked List
// Flatten a list with child pointers into a single-level doubly linked list.
struct Solution9 {
    Node* flatten(Node* head) {
        for (Node* curr = head; curr != nullptr; curr = curr->next) {
            // If current node has a child list
            if (!curr->child) {
                continue;
            }

            Node* nextNode = curr->next;
            Node* childTail = curr->child;

            // Find the tail of the child branch
            while (childTail->next != nullptr) {
                childTail = childTail->next;
            }

            // Splice the child list between curr and nextNode
            curr->next = curr->child;
            curr->child->prev = curr;
            curr->child = nullptr;

            if (nextNode != nullptr) {
                childTail->next = nextNode;
                nextNode->prev = childTail;
            }
        }

        return head;
    }
};

// 10. Convert BST to Sorted Doubly Linked List
// Convert a BST into a circular sorted doubly linked list, in-place.
struct Solution10 {
    Node* first = nullptr;
    Node* prev = nullptr;

    void inorder(Node* node) {
        if (!node) {
            return;
        }

        // Traverse left subtree
        inorder(node->left);

        // Process current node
        if (prev != nullptr) {
            prev->right = node;
            node->left = prev;
        } else {
            first = node; // Smallest element becomes head
        }
        prev = node;

        // Traverse right subtree
        inorder(node->right);
    }

    Node* treeToDoublyList(Node* root) {
        if (!root) {
            return nullptr;
        }

        first = nullptr;
        prev = nullptr;

        inorder(root);

        // Close circular doubly linked list (connect first and last)
        prev->right = first;
        first->left = prev;

        return first;
    }
};

// 11. Design Linked List
// Support get, addAtHead, addAtTail, addAtIndex, deleteAtIndex.
struct Solution11 {
    struct MyLinkedList {
        struct DNode {
            int val;
            DNode* prev;
            DNode* next;
            DNode(int v) : val(v), prev(nullptr), next(nullptr) {}
        };

        DNode* head;
        DNode* tail;
        int size = 0;

        MyLinkedList() {
            head = new DNode(0);
            tail = new DNode(0);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }

        int get(int index) {
            if (index < 0 || index >= size) {
                return -1;
            }

            DNode* curr = head->next;
            for (int i = 0; i < index; i++) {
                curr = curr->next;
            }

            return curr->val;
        }

        void addAtHead(int val) {
            addAtIndex(0, val);
        }

        void addAtTail(int val) {
            addAtIndex(size, val);
        }

        void addAtIndex(int index, int val) {
            if (index < 0 || index > size) {
                return;
            }

            DNode* pred = head;
            for (int i = 0; i < index; i++) {
                pred = pred->next;
            }
            DNode* succ = pred->next;

            DNode* node = new DNode(val);
            node->prev = pred;
            node->next = succ;
            pred->next = node;
            succ->prev = node;

            size++;
        }

        void deleteAtIndex(int index) {
            if (index < 0 || index >= size) {
                return;
            }

            DNode* pred = head;
            for (int i = 0; i < index; i++) {
                pred = pred->next;
            }

            DNode* toDelete = pred->next;
            DNode* succ = toDelete->next;

            pred->next = succ;
            succ->prev = pred;

            delete toDelete;
            size--;
        }
    };
};
using MyLinkedList = Solution11::MyLinkedList;

// 12. LRU Cache
// Support get/put in O(1) with least-recently-used eviction.
struct Solution12 {
    struct LRUCache {
        struct CacheNode {
            int key;
            int val;
            CacheNode* prev;
            CacheNode* next;

            CacheNode(int k, int v)
                : key(k), val(v), prev(nullptr), next(nullptr) {}
        };

        int cap;
        unordered_map<int, CacheNode*> mp;
        CacheNode* head;
        CacheNode* tail;

        // Helper: Remove an existing node from the doubly linked list
        void removeNode(CacheNode* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        // Helper: Insert a node right after the sentinel head (most recently used)
        void addToHead(CacheNode* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
        }

        // Helper: Move an accessed node to the front
        void moveToHead(CacheNode* node) {
            removeNode(node);
            addToHead(node);
        }

        LRUCache(int capacity) : cap(capacity) {
            head = new CacheNode(0, 0);
            tail = new CacheNode(0, 0);
            head->next = tail;
            tail->prev = head;
        }

        int get(int key) {
            if (!mp.count(key)) {
                return -1;
            }

            CacheNode* node = mp[key];
            moveToHead(node);
            return node->val;
        }

        void put(int key, int value) {
            if (mp.count(key)) {
                // Key exists: update value and move to front
                CacheNode* node = mp[key];
                node->val = value;
                moveToHead(node);
                return;
            }

            // Evict least recently used if at capacity
            if ((int)mp.size() >= cap) {
                CacheNode* lru = tail->prev;
                mp.erase(lru->key);
                removeNode(lru);
                delete lru;
            }

            // Insert new key-value pair
            CacheNode* newNode = new CacheNode(key, value);
            mp[key] = newNode;
            addToHead(newNode);
        }
    };
};
using LRUCache = Solution12::LRUCache;

// 13. LFU Cache
// Support get/put in O(1) with least-frequently-used eviction.
struct Solution13 {
    struct LFUCache {
        struct FNode {
            int key;
            int val;
            int freq;
        };

        int cap;
        int minFreq;
        unordered_map<int, list<FNode>::iterator> keyMap;
        unordered_map<int, list<FNode>> freqMap;

        // Helper: Increment frequency of an accessed node
        void updateFreq(list<FNode>::iterator it) {
            int key = it->key;
            int val = it->val;
            int freq = it->freq;

            // Remove from current frequency list
            freqMap[freq].erase(it);
            if (freqMap[freq].empty()) {
                freqMap.erase(freq);
                if (minFreq == freq) {
                    minFreq++;
                }
            }

            // Insert into (freq + 1) list at the front
            freqMap[freq + 1].push_front({key, val, freq + 1});
            keyMap[key] = freqMap[freq + 1].begin();
        }

        LFUCache(int capacity) : cap(capacity), minFreq(0) {}

        int get(int key) {
            if (cap == 0 || !keyMap.count(key)) {
                return -1;
            }

            auto it = keyMap[key];
            int val = it->val;
            updateFreq(it);
            return val;
        }

        void put(int key, int value) {
            if (cap == 0) {
                return;
            }

            // If key already exists, update value and bump frequency
            if (keyMap.count(key)) {
                keyMap[key]->val = value;
                updateFreq(keyMap[key]);
                return;
            }

            // Evict least frequently used (break ties with LRU at back of list)
            if ((int)keyMap.size() >= cap) {
                auto& minList = freqMap[minFreq];
                keyMap.erase(minList.back().key);
                minList.pop_back();

                if (minList.empty()) {
                    freqMap.erase(minFreq);
                }
            }

            // Insert new node with frequency = 1
            minFreq = 1;
            freqMap[1].push_front({key, value, 1});
            keyMap[key] = freqMap[1].begin();
        }
    };
};
using LFUCache = Solution13::LFUCache;

// 14. Design Browser History
// Support visit(url), back(steps), forward(steps) starting at homepage.
struct Solution14 {
    struct BrowserHistory {
        struct HNode {
            string url;
            HNode* prev;
            HNode* next;
            HNode(string u) : url(u), prev(nullptr), next(nullptr) {}
        };

        HNode* curr;

        BrowserHistory(string homepage) {
            curr = new HNode(homepage);
        }

        void visit(string url) {
            HNode* node = new HNode(url);
            curr->next = node;
            node->prev = curr;
            curr = node; // Clears all forward history
        }

        string back(int steps) {
            while (curr->prev != nullptr && steps > 0) {
                curr = curr->prev;
                steps--;
            }
            return curr->url;
        }

        string forward(int steps) {
            while (curr->next != nullptr && steps > 0) {
                curr = curr->next;
                steps--;
            }
            return curr->url;
        }
    };
};
using BrowserHistory = Solution14::BrowserHistory;
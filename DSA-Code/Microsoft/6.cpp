#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#include <list>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

// Definition for singly-linked list node
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Definition for a multi-purpose Node (Random, Doubly, Tree, Circular)
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
    Node* random;
    Node* left;
    Node* right;
    Node() : val(0), prev(nullptr), next(nullptr), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val) : val(_val), prev(nullptr), next(nullptr), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _next) : val(_val), prev(nullptr), next(_next), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _prev, Node* _next) : val(_val), prev(_prev), next(_next), child(nullptr), random(nullptr), left(nullptr), right(nullptr) {}
    Node(int _val, Node* _prev, Node* _next, Node* _child) : val(_val), prev(_prev), next(_next), child(_child), random(nullptr), left(nullptr), right(nullptr) {}
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

// =========================================================
// 1. REVERSE LINKED LIST (LC 206)
// =========================================================

class Solution1 {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }
        return prev;
    }
};
// Interview Explanation:
// - Problem Statement: Reverse a singly linked list and return the reversed list head.
// - Approach: Three-pointer iterative pointer reversal (`prev`, `curr`, `nextNode`).
// - Intuition: At each node, invert its `next` pointer to point to `prev`, then advance `prev` and `curr`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 2. MERGE TWO SORTED LISTS (LC 21)
// =========================================================

class Solution2 {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (list1 && list2) {
            if (list1->val <= list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }
        tail->next = list1 ? list1 : list2;
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Merge two sorted singly linked lists into one sorted linked list.
// - Approach: Two pointers with dummy head. Append the smaller node at each step and splice remaining list at the end.
// - Intuition: Splicing existing nodes preserves memory allocations and achieves linear time.
// - Complexity: Time: O(N + M), Space: O(1) auxiliary space.


// =========================================================
// 3. MIDDLE OF THE LINKED LIST (LC 876)
// =========================================================

class Solution3 {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
};
// Interview Explanation:
// - Problem Statement: Return middle node of linked list. If two middle nodes exist, return second middle node.
// - Approach: Two pointers where slow moves 1 step and fast moves 2 steps.
// - Intuition: When fast reaches end or one step past, slow lands on index floor(N/2).
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 4. SWAP NODES IN PAIRS (LC 24)
// =========================================================

class Solution4 {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;
            ListNode* second = first->next;

            first->next = second->next;
            second->next = first;
            prev->next = second;

            prev = first;
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Swap every two adjacent nodes and return head without modifying values.
// - Approach: Use dummy head and iteratively rewire pointers for pairs (A, B) into (B, A).
// - Intuition: For pair (A, B) after `prev`: link `prev->next = B`, `B->next = A`, `A->next = nextPair`.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 5. REMOVE DUPLICATES FROM SORTED LIST (LC 83)
// =========================================================

class Solution5 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;
        while (curr && curr->next) {
            if (curr->val == curr->next->val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Remove duplicate values from a sorted list such that each value appears once.
// - Approach: Single pass. If `curr->val == curr->next->val`, skip duplicate node; otherwise advance `curr`.
// - Intuition: Sorted order guarantees all identical elements are contiguous.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 6. REMOVE LINKED LIST ELEMENTS (LC 203)
// =========================================================

class Solution6 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode dummy(0, head);
        ListNode* curr = &dummy;

        while (curr->next) {
            if (curr->next->val == val) {
                ListNode* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            } else {
                curr = curr->next;
            }
        }
        return dummy.next;
    }
};
// Interview Explanation:
// - Problem Statement: Remove all nodes in linked list that have value equal to `val`.
// - Approach: Dummy node before head. Iterate and unlink matching nodes.
// - Intuition: Dummy node unifies head deletion with general interior node deletion.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 7. DELETE NODE IN A LINKED LIST (NO HEAD ACCESS) (LC 237)
// =========================================================

class Solution7 {
public:
    void deleteNode(ListNode* node) {
        ListNode* nextNode = node->next;
        node->val = nextNode->val;
        node->next = nextNode->next;
        delete nextNode;
    }
};
// Interview Explanation:
// - Problem Statement: Delete given node without access to list head (guaranteed not to be tail).
// - Approach: Overwrite current node's value with successor's value, then bypass successor.
// - Intuition: Value copying converts deletion of target into deletion of next node.
// - Complexity: Time: O(1), Space: O(1) auxiliary space.


// =========================================================
// 8. INSERT INTO A SORTED CIRCULAR LINKED LIST (LC 708)
// =========================================================

class Solution8 {
public:
    Node* insert(Node* head, int insertVal) {
        if (!head) {
            Node* newNode = new Node(insertVal);
            newNode->next = newNode;
            return newNode;
        }

        Node* prev = head;
        Node* curr = head->next;
        bool inserted = false;

        do {
            // Case 1: insertVal lies between prev and curr
            if (prev->val <= insertVal && insertVal <= curr->val) {
                inserted = true;
                break;
            }
            // Case 2: Boundary inflection point (max element wrapping to min element)
            if (prev->val > curr->val) {
                if (insertVal >= prev->val || insertVal <= curr->val) {
                    inserted = true;
                    break;
                }
            }
            prev = curr;
            curr = curr->next;
        } while (prev != head);

        // Case 3: Insertion spot found, or all elements identical / loop finished
        prev->next = new Node(insertVal, curr);
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Insert value into a sorted circular linked list in proper sorted position.
// - Approach: Traverse circular list. Check if `insertVal` fits monotonically between `prev` and `curr`, or spans across min/max boundary point.
// - Intuition: If no interior point matches, value must be either strictly >= max or <= min at the wrap-around edge.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 9. FLATTEN A MULTILEVEL DOUBLY LINKED LIST (LC 430)
// =========================================================

class Solution9 {
public:
    Node* flatten(Node* head) {
        if (!head) return nullptr;
        Node* curr = head;

        while (curr) {
            if (curr->child) {
                Node* nextNode = curr->next;
                Node* childTail = curr->child;

                while (childTail->next) {
                    childTail = childTail->next;
                }

                curr->next = curr->child;
                curr->child->prev = curr;
                curr->child = nullptr;

                if (nextNode) {
                    childTail->next = nextNode;
                    nextNode->prev = childTail;
                }
            }
            curr = curr->next;
        }
        return head;
    }
};
// Interview Explanation:
// - Problem Statement: Flatten a multilevel doubly linked list with child pointers into a single-level doubly linked list.
// - Approach: When encountering a child node, find child's tail, splice child list between `curr` and `curr->next`, reset child to null.
// - Intuition: In-place pointer re-linking flattens nested lists level-by-level without recursion overhead.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 10. CONVERT BST TO SORTED DOUBLY LINKED LIST (LC 426)
// =========================================================

class Solution10 {
    Node* first = nullptr;
    Node* prev = nullptr;

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);

        if (prev) {
            prev->right = node;
            node->left = prev;
        } else {
            first = node;
        }
        prev = node;

        inorder(node->right);
    }

public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        first = nullptr;
        prev = nullptr;

        inorder(root);

        // Make doubly linked list circular
        prev->right = first;
        first->left = prev;

        return first;
    }
};
// Interview Explanation:
// - Problem Statement: Convert a Binary Search Tree to a circular sorted Doubly Linked List in-place.
// - Approach: Recursive inorder traversal. Link `prev->right = curr` and `curr->left = prev`. Connect head and tail at the end.
// - Intuition: Inorder traversal naturally visits BST nodes in monotonically increasing order.
// - Complexity: Time: O(N), Space: O(H) recursion stack space (where H is tree height).


// =========================================================
// 11. DESIGN LINKED LIST (LC 707)
// =========================================================

class Solution11 {
public:
    class MyLinkedList {
        struct DNode {
            int val;
            DNode* prev;
            DNode* next;
            DNode(int v) : val(v), prev(nullptr), next(nullptr) {}
        };

        DNode* head;
        DNode* tail;
        int size;

    public:
        MyLinkedList() {
            head = new DNode(0);
            tail = new DNode(0);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }

        int get(int index) {
            if (index < 0 || index >= size) return -1;
            DNode* curr = head->next;
            for (int i = 0; i < index; ++i) curr = curr->next;
            return curr->val;
        }

        void addAtHead(int val) {
            addAtIndex(0, val);
        }

        void addAtTail(int val) {
            addAtIndex(size, val);
        }

        void addAtIndex(int index, int val) {
            if (index < 0 || index > size) return;
            DNode* pred = head;
            for (int i = 0; i < index; ++i) pred = pred->next;
            DNode* succ = pred->next;

            DNode* node = new DNode(val);
            node->prev = pred;
            node->next = succ;
            pred->next = node;
            succ->prev = node;
            size++;
        }

        void deleteAtIndex(int index) {
            if (index < 0 || index >= size) return;
            DNode* pred = head;
            for (int i = 0; i < index; ++i) pred = pred->next;
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

// Interview Explanation:
// - Problem Statement: Design singly/doubly linked list supporting get, addAtHead, addAtTail, addAtIndex, deleteAtIndex.
// - Approach: Doubly-linked list with sentinel head and tail nodes to simplify edge deletions/insertions.
// - Intuition: Sentinel nodes eliminate null checks at boundaries.
// - Complexity: Time: O(index) for get/addAtIndex/deleteAtIndex, O(1) for addAtHead/addAtTail, Space: O(N).


// =========================================================
// 12. LRU CACHE (LC 146)
// =========================================================

class Solution12 {
public:
    class LRUCache {
        struct CacheNode {
            int key;
            int val;
            CacheNode* prev;
            CacheNode* next;
            CacheNode(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
        };

        int cap;
        unordered_map<int, CacheNode*> mp;
        CacheNode* head;
        CacheNode* tail;

        void removeNode(CacheNode* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        void addToHead(CacheNode* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
        }

        void moveToHead(CacheNode* node) {
            removeNode(node);
            addToHead(node);
        }

    public:
        LRUCache(int capacity) : cap(capacity) {
            head = new CacheNode(0, 0);
            tail = new CacheNode(0, 0);
            head->next = tail;
            tail->prev = head;
        }

        int get(int key) {
            if (!mp.count(key)) return -1;
            CacheNode* node = mp[key];
            moveToHead(node);
            return node->val;
        }

        void put(int key, int value) {
            if (mp.count(key)) {
                CacheNode* node = mp[key];
                node->val = value;
                moveToHead(node);
            } else {
                if ((int)mp.size() >= cap) {
                    CacheNode* lru = tail->prev;
                    mp.erase(lru->key);
                    removeNode(lru);
                    delete lru;
                }
                CacheNode* newNode = new CacheNode(key, value);
                mp[key] = newNode;
                addToHead(newNode);
            }
        }
    };
};
using LRUCache = Solution12::LRUCache;

// Interview Explanation:
// - Problem Statement: Design Least Recently Used (LRU) Cache supporting get and put in O(1) time.
// - Approach: Hash map paired with doubly-linked list. Sentinel `head` (most recent) and `tail` (least recent).
// - Intuition: Hash map provides O(1) key-to-node lookup; doubly-linked list provides O(1) node relocation and eviction.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).


// =========================================================
// 13. LFU CACHE (LC 460)
// =========================================================

class Solution13 {
public:
    class LFUCache {
        struct Node {
            int key;
            int val;
            int freq;
        };

        int cap;
        int minFreq;
        unordered_map<int, list<Node>::iterator> keyMap;
        unordered_map<int, list<Node>> freqMap;

        void updateFreq(list<Node>::iterator it) {
            int key = it->key;
            int val = it->val;
            int freq = it->freq;

            freqMap[freq].erase(it);
            if (freqMap[freq].empty()) {
                freqMap.erase(freq);
                if (minFreq == freq) {
                    minFreq++;
                }
            }

            freqMap[freq + 1].push_front({key, val, freq + 1});
            keyMap[key] = freqMap[freq + 1].begin();
        }

    public:
        LFUCache(int capacity) : cap(capacity), minFreq(0) {}

        int get(int key) {
            if (cap == 0 || !keyMap.count(key)) return -1;
            auto it = keyMap[key];
            int val = it->val;
            updateFreq(it);
            return val;
        }

        void put(int key, int value) {
            if (cap == 0) return;

            if (keyMap.count(key)) {
                auto it = keyMap[key];
                it->val = value;
                updateFreq(it);
                return;
            }

            if ((int)keyMap.size() >= cap) {
                auto& minList = freqMap[minFreq];
                int evictKey = minList.back().key;
                minList.pop_back();
                if (minList.empty()) {
                    freqMap.erase(minFreq);
                }
                keyMap.erase(evictKey);
            }

            minFreq = 1;
            freqMap[1].push_front({key, value, 1});
            keyMap[key] = freqMap[1].begin();
        }
    };
};
using LFUCache = Solution13::LFUCache;

// Interview Explanation:
// - Problem Statement: Design Least Frequently Used (LFU) Cache supporting get and put in O(1) time.
// - Approach: Hash map of iterators + frequency-to-doubly-linked-list buckets (`freqMap`). Maintain `minFreq`.
// - Intuition: Access moves node from frequency bucket f to f+1. Eviction removes from back of `minFreq` bucket.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).


// =========================================================
// 14. DESIGN BROWSER HISTORY (LC 1472)
// =========================================================

class Solution14 {
public:
    class BrowserHistory {
        struct HistoryNode {
            string url;
            HistoryNode* prev;
            HistoryNode* next;
            HistoryNode(string u) : url(u), prev(nullptr), next(nullptr) {}
        };
        HistoryNode* curr;

    public:
        BrowserHistory(string homepage) {
            curr = new HistoryNode(homepage);
        }

        void visit(string url) {
            HistoryNode* node = new HistoryNode(url);
            curr->next = node;
            node->prev = curr;
            curr = node; // Naturally discards forward history
        }

        string back(int steps) {
            while (curr->prev && steps > 0) {
                curr = curr->prev;
                steps--;
            }
            return curr->url;
        }

        string forward(int steps) {
            while (curr->next && steps > 0) {
                curr = curr->next;
                steps--;
            }
            return curr->url;
        }
    };
};
using BrowserHistory = Solution14::BrowserHistory;

// Interview Explanation:
// - Problem Statement: Design browser history starting at homepage, with visit(url), back(steps), and forward(steps).
// - Approach: Doubly-linked list. Current page pointer navigates backward/forward; visit appends new node and drops forward branch.
// - Intuition: Doubly-linked list provides natural bidirectional linear navigation and O(1) branch truncation.
// - Complexity: Time: visit O(1), back/forward O(min(steps, len)), Space: O(N) total history nodes.

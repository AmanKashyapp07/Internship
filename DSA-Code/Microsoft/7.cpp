#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <list>
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
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Node with random pointer
class Node {
public:
    int val;
    Node* next;
    Node* random;
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

// =========================================================
// 1. LFU CACHE (LEAST FREQUENTLY USED CACHE)
// =========================================================

class LFUCache {
    int cap, minFreq;
    unordered_map<int, pair<int, int>> keyValFreq; // key -> {val, freq}
    unordered_map<int, list<int>> freqList;         // freq -> list of keys (MRU at front)
    unordered_map<int, list<int>::iterator> keyIter; // key -> iterator in freqList

public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        if (keyValFreq.find(key) == keyValFreq.end()) return -1;

        int val = keyValFreq[key].first;
        int freq = keyValFreq[key].second;

        // Erase key from current freq list
        freqList[freq].erase(keyIter[key]);
        if (freqList[freq].empty() && freq == minFreq) {
            minFreq++;
        }

        // Increment freq and push to front of new list
        keyValFreq[key].second++;
        freqList[freq + 1].push_front(key);
        keyIter[key] = freqList[freq + 1].begin();

        return val;
    }

    void put(int key, int value) {
        if (cap <= 0) return;

        if (get(key) != -1) {
            keyValFreq[key].first = value;
            return;
        }

        if ((int)keyValFreq.size() >= cap) {
            int evictKey = freqList[minFreq].back();
            freqList[minFreq].pop_back();
            keyValFreq.erase(evictKey);
            keyIter.erase(evictKey);
        }

        keyValFreq[key] = {value, 1};
        freqList[1].push_front(key);
        keyIter[key] = freqList[1].begin();
        minFreq = 1;
    }
};
// Interview Explanation:
// - Problem Statement: Implement an LFU Cache with O(1) get and put operations.
// - Approach: Frequency Doubly-Linked Lists + Hash Maps.
// - Intuition: Group keys by frequency. Maintain minFreq tracker. When evicting, pop from back of list at minFreq.
// - Complexity: Time: O(1) for get and put, Space: O(capacity).


// =========================================================
// 2. DESIGN IN-MEMORY FILE SYSTEM
// =========================================================

class FileSystem {
    struct FSNode {
        bool isFile = false;
        string content = "";
        map<string, FSNode*> children; // sorted for lexical output
    };
    FSNode* root;

    vector<string> splitPath(const string& path) {
        vector<string> parts;
        stringstream ss(path);
        string item;
        while (getline(ss, item, '/')) {
            if (!item.empty()) parts.push_back(item);
        }
        return parts;
    }

    FSNode* traverse(const string& path) {
        FSNode* curr = root;
        for (const string& p : splitPath(path)) {
            if (!curr->children.count(p)) {
                curr->children[p] = new FSNode();
            }
            curr = curr->children[p];
        }
        return curr;
    }

public:
    FileSystem() { root = new FSNode(); }

    vector<string> ls(string path) {
        FSNode* curr = traverse(path);
        if (curr->isFile) {
            auto parts = splitPath(path);
            return {parts.back()};
        }
        vector<string> res;
        for (auto& [name, _] : curr->children) res.push_back(name);
        return res;
    }

    void mkdir(string path) {
        traverse(path);
    }

    void addContentToFile(string filePath, string content) {
        FSNode* curr = traverse(filePath);
        curr->isFile = true;
        curr->content += content;
    }

    string readContentFromFile(string filePath) {
        return traverse(filePath)->content;
    }
};
// Interview Explanation:
// - Problem Statement: Design in-memory file system supporting ls, mkdir, addContentToFile, readContentFromFile.
// - Approach: Trie / N-ary Tree where each directory node has a map of child nodes.
// - Intuition: Path split by '/' serves as trie keys; std::map maintains alphabetical order automatically.
// - Complexity: Time: O(K log M) for path traversal where K is depth, Space: O(Total path tokens + file size).


// =========================================================
// 3. WORD LADDER II (ALL SHORTEST TRANSFORMATION PATHS)
// =========================================================

vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    vector<vector<string>> results;
    if (!dict.count(endWord)) return results;

    unordered_map<string, vector<string>> adj; // parent -> list of children in shortest DAG
    unordered_map<string, int> dist;           // node -> shortest distance from beginWord
    queue<string> q;

    q.push(beginWord);
    dist[beginWord] = 0;

    while (!q.empty()) {
        string u = q.front();
        q.pop();
        int d = dist[u];

        if (u == endWord) break;

        string nextWord = u;
        for (int i = 0; i < (int)nextWord.size(); ++i) {
            char orig = nextWord[i];
            for (char c = 'a'; c <= 'z'; ++c) {
                nextWord[i] = c;
                if (dict.count(nextWord)) {
                    if (!dist.count(nextWord)) {
                        dist[nextWord] = d + 1;
                        q.push(nextWord);
                        adj[u].push_back(nextWord);
                    } else if (dist[nextWord] == d + 1) {
                        adj[u].push_back(nextWord);
                    }
                }
            }
            nextWord[i] = orig;
        }
    }

    // DFS to reconstruct paths
    function<void(const string&, vector<string>&)> dfs = [&](const string& u, vector<string>& path) {
        if (u == endWord) {
            results.push_back(path);
            return;
        }
        for (const string& v : adj[u]) {
            path.push_back(v);
            dfs(v, path);
            path.pop_back();
        }
    };

    vector<string> path = {beginWord};
    dfs(beginWord, path);
    return results;
}
// Interview Explanation:
// - Problem Statement: Find all shortest transformation sequences from beginWord to endWord.
// - Approach: BFS level-order to build Shortest Path DAG + DFS Backtracking to enumerate paths.
// - Intuition: BFS finds exact shortest layer for each word; DFS only traverses forward edges where dist[v] == dist[u] + 1.
// - Complexity: Time: O(N * 26 * L + V + E), Space: O(N * L) for DAG and queue.


// =========================================================
// 4. MEDIAN OF TWO SORTED ARRAYS
// =========================================================

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
    int m = nums1.size(), n = nums2.size();
    int low = 0, high = m;

    while (low <= high) {
        int cut1 = low + (high - low) / 2;
        int cut2 = (m + n + 1) / 2 - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int r1 = (cut1 == m) ? INT_MAX : nums1[cut1];
        int r2 = (cut2 == n) ? INT_MAX : nums2[cut2];

        if (l1 <= r2 && l2 <= r1) {
            if ((m + n) % 2 == 0) {
                return (max(l1, l2) + min(r1, r2)) / 2.0;
            } else {
                return max(l1, l2);
            }
        } else if (l1 > r2) {
            high = cut1 - 1;
        } else {
            low = cut1 + 1;
        }
    }
    return 0.0;
}
// Interview Explanation:
// - Problem Statement: Find median of two sorted arrays in O(log(min(m, n))) time.
// - Approach: Binary Search on Partition Cut of the smaller array.
// - Intuition: Partition both arrays such that left half has (m+n+1)/2 elements and max(left) <= min(right).
// - Complexity: Time: O(log(min(m, n))), Space: O(1).


// =========================================================
// 5. TRAPPING RAIN WATER II (3D / 2D GRID)
// =========================================================

int trapRainWater2D(vector<vector<int>>& heightMap) {
    if (heightMap.empty() || heightMap[0].empty()) return 0;
    int m = heightMap.size(), n = heightMap[0].size();

    // {height, r, c}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (r == 0 || r == m - 1 || c == 0 || c == n - 1) {
                pq.push({heightMap[r][c], r, c});
                visited[r][c] = true;
            }
        }
    }

    int water = 0;
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!pq.empty()) {
        auto [h, r, c] = pq.top();
        pq.pop();

        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !visited[nr][nc]) {
                visited[nr][nc] = true;
                water += max(0, h - heightMap[nr][nc]);
                pq.push({max(h, heightMap[nr][nc]), nr, nc});
            }
        }
    }
    return water;
}
// Interview Explanation:
// - Problem Statement: Compute 3D trapped rain water in an m x n elevation matrix.
// - Approach: Min-Heap Priority Queue on boundary (Dijkstra-like inward sweep).
// - Intuition: The lowest boundary cell is the bottleneck; process it inward, filling water and lifting boundary height.
// - Complexity: Time: O(M * N * log(M * N)), Space: O(M * N) for heap and visited array.


// =========================================================
// 6. SLIDING WINDOW MAXIMUM
// =========================================================

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // stores indices of monotonically decreasing elements
    vector<int> res;

    for (int i = 0; i < (int)nums.size(); ++i) {
        // Evict elements outside window [i-k+1, i]
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();

        // Evict smaller elements from back
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();

        dq.push_back(i);

        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find maximum in each sliding window of size k.
// - Approach: Monotonic Decreasing Deque.
// - Intuition: Deque stores indices with descending values; front is always current window maximum.
// - Complexity: Time: O(N) since each element is pushed/popped at most once, Space: O(k).


// =========================================================
// 7. WORD SEARCH II (TRIE + 2D GRID BACKTRACKING)
// =========================================================

struct TrieNode {
    TrieNode* children[26] = {};
    string word = "";
};

void insertTrie(TrieNode* root, const string& w) {
    TrieNode* curr = root;
    for (char c : w) {
        int idx = c - 'a';
        if (!curr->children[idx]) curr->children[idx] = new TrieNode();
        curr = curr->children[idx];
    }
    curr->word = w;
}

void dfsWordSearch2(vector<vector<char>>& board, int r, int c, TrieNode* curr, vector<string>& res) {
    char ch = board[r][c];
    if (ch == '#' || !curr->children[ch - 'a']) return;

    curr = curr->children[ch - 'a'];
    if (!curr->word.empty()) {
        res.push_back(curr->word);
        curr->word = ""; // Prevent duplicate additions
    }

    board[r][c] = '#';
    const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        int nr = r + d[0], nc = c + d[1];
        if (nr >= 0 && nr < (int)board.size() && nc >= 0 && nc < (int)board[0].size()) {
            dfsWordSearch2(board, nr, nc, curr, res);
        }
    }
    board[r][c] = ch;
}

vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
    TrieNode* root = new TrieNode();
    for (const string& w : words) insertTrie(root, w);

    vector<string> res;
    for (int r = 0; r < (int)board.size(); ++r) {
        for (int c = 0; c < (int)board[0].size(); ++c) {
            dfsWordSearch2(board, r, c, root, res);
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find all words from a dictionary present in a 2D board.
// - Approach: Prefix Trie + DFS Grid Backtracking with visited masking.
// - Intuition: Building a Trie allows simultaneous matching of multiple words and early backtracking pruning.
// - Complexity: Time: O(M * N * 4^(L)), Space: O(Total characters in words) for Trie.


// =========================================================
// 8. TEXT JUSTIFICATION
// =========================================================

vector<string> fullJustify(vector<string>& words, int maxWidth) {
    vector<string> result;
    int i = 0, n = words.size();

    while (i < n) {
        int j = i, lineLen = 0;
        while (j < n && lineLen + (int)words[j].size() + (j - i) <= maxWidth) {
            lineLen += words[j].size();
            j++;
        }

        int numWords = j - i;
        int numSpaces = maxWidth - lineLen;
        string line = "";

        // If single word or last line -> left-justified
        if (numWords == 1 || j == n) {
            for (int k = i; k < j; ++k) {
                line += words[k];
                if (k < j - 1) line += " ";
            }
            while ((int)line.size() < maxWidth) line += " ";
        } else {
            // Fully justified
            int spacesBetween = numSpaces / (numWords - 1);
            int extraSpaces = numSpaces % (numWords - 1);

            for (int k = i; k < j; ++k) {
                line += words[k];
                if (k < j - 1) {
                    int s = spacesBetween + (k - i < extraSpaces ? 1 : 0);
                    line.append(s, ' ');
                }
            }
        }
        result.push_back(line);
        i = j;
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Format words into lines with full justification.
// - Approach: Greedy line packing + modular arithmetic space distribution.
// - Intuition: Determine line word count greedily. Distribute spaces evenly among gaps with extra modulo spaces placed on left gaps.
// - Complexity: Time: O(Total characters), Space: O(Total output characters).


// =========================================================
// 9. BUS ROUTES
// =========================================================

int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
    if (source == target) return 0;

    unordered_map<int, vector<int>> stopToBuses;
    for (int i = 0; i < (int)routes.size(); ++i) {
        for (int stop : routes[i]) stopToBuses[stop].push_back(i);
    }

    unordered_set<int> visitedStops;
    unordered_set<int> visitedBuses;
    queue<pair<int, int>> q; // {stop, busCount}

    q.push({source, 0});
    visitedStops.insert(source);

    while (!q.empty()) {
        auto [stop, count] = q.front();
        q.pop();

        if (stop == target) return count;

        for (int bus : stopToBuses[stop]) {
            if (visitedBuses.count(bus)) continue;
            visitedBuses.insert(bus);

            for (int nextStop : routes[bus]) {
                if (visitedStops.count(nextStop)) continue;
                visitedStops.insert(nextStop);
                q.push({nextStop, count + 1});
            }
        }
    }
    return -1;
}
// Interview Explanation:
// - Problem Statement: Find minimum bus transfers from source to target.
// - Approach: BFS on Bipartite Graph (Stops <-> Bus Routes).
// - Intuition: Mark buses as visited to avoid re-evaluating loops of already boarded bus lines.
// - Complexity: Time: O(Sum of routes lengths), Space: O(Sum of routes lengths).


// =========================================================
// 10. BINARY TREE CAMERAS
// =========================================================

class SolutionBinaryTreeCameras {
    int cameras = 0;
    // States: 0 = unmonitored, 1 = has camera, 2 = monitored
    int dfs(TreeNode* root) {
        if (!root) return 2; // Null nodes are considered monitored

        int left = dfs(root->left);
        int right = dfs(root->right);

        if (left == 0 || right == 0) {
            cameras++;
            return 1; // Must place camera
        }
        if (left == 1 || right == 1) {
            return 2; // Covered by child's camera
        }
        return 0; // Leaf or unmonitored node
    }

public:
    int minCameraCover(TreeNode* root) {
        if (dfs(root) == 0) cameras++;
        return cameras;
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum cameras to monitor all binary tree nodes.
// - Approach: Greedy Bottom-Up Post-Order DFS with 3-State DP.
// - Intuition: Always prefer placing cameras on parent of leaves rather than leaves to maximize coverage.
// - Complexity: Time: O(N) single tree pass, Space: O(H) recursion stack.


// =========================================================
// 11. VERTICAL ORDER TRAVERSAL OF BINARY TREE
// =========================================================

vector<vector<int>> verticalTraversal(TreeNode* root) {
    // col -> row -> multiset of values
    map<int, map<int, multiset<int>>> nodes;
    queue<tuple<TreeNode*, int, int>> q; // {node, row, col}
    if (root) q.push({root, 0, 0});

    while (!q.empty()) {
        auto [node, r, c] = q.front();
        q.pop();

        nodes[c][r].insert(node->val);

        if (node->left) q.push({node->left, r + 1, c - 1});
        if (node->right) q.push({node->right, r + 1, c + 1});
    }

    vector<vector<int>> result;
    for (auto& [col, rowMap] : nodes) {
        vector<int> colVals;
        for (auto& [row, mset] : rowMap) {
            colVals.insert(colVals.end(), mset.begin(), mset.end());
        }
        result.push_back(colVals);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Vertical columns ordered left to right, top to bottom, with value ties sorted.
// - Approach: BFS coordinate mapping + nested ordered maps/multisets.
// - Intuition: Assign coordinates (row, col). Col determines column bucket; row + multiset handles ordering and ties.
// - Complexity: Time: O(N log N), Space: O(N).


// =========================================================
// 12. POPULATING NEXT RIGHT POINTERS IN EACH NODE II
// =========================================================

struct NodeNext {
    int val;
    NodeNext *left, *right, *next;
    NodeNext(int x) : val(x), left(nullptr), right(nullptr), next(nullptr) {}
};

NodeNext* connect(NodeNext* root) {
    NodeNext* curr = root;

    while (curr != nullptr) {
        NodeNext dummy(0);
        NodeNext* tail = &dummy;

        while (curr != nullptr) {
            if (curr->left) {
                tail->next = curr->left;
                tail = tail->next;
            }
            if (curr->right) {
                tail->next = curr->right;
                tail = tail->next;
            }
            curr = curr->next;
        }
        curr = dummy.next; // Move to next level
    }
    return root;
}
// Interview Explanation:
// - Problem Statement: Populate next right pointers in any binary tree with O(1) extra memory.
// - Approach: Level-Order linked list traversal with dummy head tracker.
// - Intuition: Use established `next` pointers on current level to build next level's linked list in O(1) space.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 13. ACCOUNTS MERGE
// =========================================================

class DSUAccounts {
public:
    vi parent;
    DSUAccounts(int n) : parent(n) { iota(parent.begin(), parent.end(), 0); }
    int find(int i) { return parent[i] == i ? i : parent[i] = find(parent[i]); }
    void unite(int i, int j) {
        int rootI = find(i), rootJ = find(j);
        if (rootI != rootJ) parent[rootI] = rootJ;
    }
};

vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    DSUAccounts dsu(n);
    unordered_map<string, int> emailToId;

    for (int i = 0; i < n; ++i) {
        for (size_t j = 1; j < accounts[i].size(); ++j) {
            string email = accounts[i][j];
            if (emailToId.find(email) == emailToId.end()) {
                emailToId[email] = i;
            } else {
                dsu.unite(i, emailToId[email]);
            }
        }
    }

    unordered_map<int, vector<string>> merged;
    for (auto& [email, id] : emailToId) {
        merged[dsu.find(id)].push_back(email);
    }

    vector<vector<string>> result;
    for (auto& [id, emails] : merged) {
        sort(emails.begin(), emails.end());
        vector<string> row = {accounts[id][0]};
        row.insert(row.end(), emails.begin(), emails.end());
        result.push_back(row);
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Merge accounts sharing common emails.
// - Approach: Disjoint Set Union (DSU / Union-Find) on account indices.
// - Intuition: Map each email to first owner index; union subsequent owners. Group emails by root parent.
// - Complexity: Time: O(Total Emails * alpha(N) + E log E), Space: O(Total Emails).


// =========================================================
// 14. BASIC CALCULATOR II (EXPRESSION PARSER)
// =========================================================

int calculate(string s) {
    stack<long long> st;
    long long currNum = 0;
    char op = '+';

    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (isdigit(c)) {
            currNum = currNum * 10 + (c - '0');
        }
        if ((!isdigit(c) && !isspace(c)) || i == s.size() - 1) {
            if (op == '+') st.push(currNum);
            else if (op == '-') st.push(-currNum);
            else if (op == '*') {
                long long top = st.top(); st.pop();
                st.push(top * currNum);
            } else if (op == '/') {
                long long top = st.top(); st.pop();
                st.push(top / currNum);
            }
            op = c;
            currNum = 0;
        }
    }

    long long result = 0;
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    return result;
}
// Interview Explanation:
// - Problem Statement: Evaluate integer arithmetic expression with '+', '-', '*', '/'.
// - Approach: Stack with Operator Buffer.
// - Intuition: Multiply/divide immediately with stack top due to operator precedence; push +/- with signs for final sum.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 15. BASIC CALCULATOR (WITH PARENTHESES)
// =========================================================

int calculateWithParens(string s) {
    stack<int> st; // stores {result, sign}
    int result = 0, currNum = 0, sign = 1;

    for (char c : s) {
        if (isdigit(c)) {
            currNum = currNum * 10 + (c - '0');
        } else if (c == '+') {
            result += sign * currNum;
            currNum = 0;
            sign = 1;
        } else if (c == '-') {
            result += sign * currNum;
            currNum = 0;
            sign = -1;
        } else if (c == '(') {
            st.push(result);
            st.push(sign);
            result = 0;
            sign = 1;
        } else if (c == ')') {
            result += sign * currNum;
            currNum = 0;
            result *= st.top(); st.pop(); // multiply by sign before '('
            result += st.top(); st.pop(); // add result before '('
        }
    }
    result += sign * currNum;
    return result;
}
// Interview Explanation:
// - Problem Statement: Evaluate mathematical string with '+', '-', and nested parentheses '()'.
// - Approach: Result & Sign Stack Context Preservation.
// - Intuition: On '(', push current accumulated result and outer sign; on ')', resolve sub-expression and merge.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 16. BURST BALLOONS (INTERVAL / RANGE DP)
// =========================================================

int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> arr(n + 2, 1);
    for (int i = 0; i < n; ++i) arr[i + 1] = nums[i];

    // dp[i][j] = max coins from bursting all balloons strictly between i and j
    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));

    for (int len = 1; len <= n; ++len) {
        for (int left = 1; left <= n - len + 1; ++left) {
            int right = left + len - 1;
            for (int k = left; k <= right; ++k) {
                // k is the LAST balloon burst in (left-1, right+1)
                dp[left][right] = max(dp[left][right],
                    dp[left][k - 1] + dp[k + 1][right] + arr[left - 1] * arr[k] * arr[right + 1]);
            }
        }
    }
    return dp[1][n];
}
// Interview Explanation:
// - Problem Statement: Burst balloons to maximize coin gain.
// - Approach: Range DP by choosing the LAST balloon to burst.
// - Intuition: Choosing the first balloon creates interdependent subproblems; choosing the LAST makes subproblems independent.
// - Complexity: Time: O(N^3), Space: O(N^2).


// =========================================================
// 17. REGULAR EXPRESSION MATCHING ('.' AND '*')
// =========================================================

bool isMatchRegex(string s, string p) {
    int m = s.size(), n = p.size();
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;

    // Base cases with '*' matching empty string
    for (int j = 2; j <= n; j += 2) {
        if (p[j - 1] == '*') dp[0][j] = dp[0][j - 2];
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                // Zero occurrences of preceding char
                dp[i][j] = dp[i][j - 2];
                // One or more occurrences
                if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                    dp[i][j] = dp[i][j] || dp[i - 1][j];
                }
            }
        }
    }
    return dp[m][n];
}
// Interview Explanation:
// - Problem Statement: Implement regex matching with '.' (any single char) and '*' (zero or more preceding element).
// - Approach: 2D Dynamic Programming.
// - Intuition: '*' has two transitions: zero match (dp[i][j-2]) or multi-match if preceding char matches (dp[i-1][j]).
// - Complexity: Time: O(M * N), Space: O(M * N).


// =========================================================
// 18. REVERSE NODES IN K-GROUP
// =========================================================

ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head || k == 1) return head;

    // Check if at least k nodes exist
    ListNode* curr = head;
    for (int i = 0; i < k; ++i) {
        if (!curr) return head;
        curr = curr->next;
    }

    // Reverse k nodes
    ListNode* prev = nullptr;
    curr = head;
    for (int i = 0; i < k; ++i) {
        ListNode* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }

    // Reconnect head (which is now tail of reversed group) to next group
    head->next = reverseKGroup(curr, k);
    return prev;
}
// Interview Explanation:
// - Problem Statement: Reverse nodes of linked list k at a time; remaining nodes < k stay as-is.
// - Approach: Iterative/Recursive K-segment reversal.
// - Intuition: Count k nodes ahead. If valid, reverse k nodes and recurse for the remainder, connecting tail to next result.
// - Complexity: Time: O(N), Space: O(N / k) stack space (or O(1) iterative).


// =========================================================
// 19. COPY LIST WITH RANDOM POINTER
// =========================================================

Node* copyRandomList(Node* head) {
    if (!head) return nullptr;

    // Step 1: Interleave cloned nodes
    Node* curr = head;
    while (curr) {
        Node* copy = new Node(curr->val);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
    }

    // Step 2: Assign random pointers
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        curr = curr->next->next;
    }

    // Step 3: Separate cloned list from original
    curr = head;
    Node* copyHead = head->next;
    while (curr) {
        Node* copy = curr->next;
        curr->next = copy->next;
        if (copy->next) copy->next = copy->next->next;
        curr = curr->next;
    }
    return copyHead;
}
// Interview Explanation:
// - Problem Statement: Deep copy a linked list with next and random pointers in O(1) auxiliary space.
// - Approach: 3-Pass Node Interleaving.
// - Intuition: Place copy node directly after original: curr->next->random = curr->random->next, then decouple.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 20. MAXIMUM WIDTH OF BINARY TREE
// =========================================================

int widthOfBinaryTree(TreeNode* root) {
    if (!root) return 0;
    queue<pair<TreeNode*, unsigned long long>> q; // {node, 0-indexed column}
    q.push({root, 0});
    unsigned long long maxWidth = 0;

    while (!q.empty()) {
        int sz = q.size();
        unsigned long long minIdx = q.front().second;
        unsigned long long first = 0, last = 0;

        for (int i = 0; i < sz; ++i) {
            auto [node, idx] = q.front();
            q.pop();
            unsigned long long currIdx = idx - minIdx; // normalize to prevent overflow

            if (i == 0) first = currIdx;
            if (i == sz - 1) last = currIdx;

            if (node->left) q.push({node->left, 2 * currIdx + 1});
            if (node->right) q.push({node->right, 2 * currIdx + 2});
        }
        maxWidth = max(maxWidth, last - first + 1);
    }
    return maxWidth;
}
// Interview Explanation:
// - Problem Statement: Find maximum width between leftmost and rightmost non-null nodes at any level.
// - Approach: Level BFS with Binary Tree Array Indexing (2*i + 1, 2*i + 2).
// - Intuition: Normalize index per level (idx - minIdx) to avoid integer overflow across deep trees.
// - Complexity: Time: O(N), Space: O(W) where W is max tree width.


// =========================================================
// 21. NUMBER OF VISIBLE PEOPLE IN A QUEUE
// =========================================================

vector<int> canSeePersonsCount(vector<int>& heights) {
    int n = heights.size();
    vector<int> ans(n, 0);
    stack<int> st; // monotonic decreasing stack of heights

    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && heights[i] > st.top()) {
            st.pop();
            ans[i]++; // Can see the popped shorter person
        }
        if (!st.empty()) {
            ans[i]++; // Can see the first taller person blocking further view
        }
        st.push(heights[i]);
    }
    return ans;
}
// Interview Explanation:
// - Problem Statement: Count how many people to the right each person can see.
// - Approach: Monotonic Decreasing Stack from Right to Left.
// - Intuition: A person sees everyone shorter in increasing line of sight until blocked by someone taller.
// - Complexity: Time: O(N) amortized, Space: O(N).


// =========================================================
// 22. ALL O`ONE DATA STRUCTURE
// =========================================================

class AllOne {
    struct Bucket {
        int count;
        unordered_set<string> keys;
    };
    list<Bucket> buckets; // DLL of frequency buckets
    unordered_map<string, list<Bucket>::iterator> keyMap;

public:
    AllOne() {}

    void inc(string key) {
        if (keyMap.find(key) == keyMap.end()) {
            if (buckets.empty() || buckets.front().count > 1) {
                buckets.push_front({1, {key}});
            } else {
                buckets.front().keys.insert(key);
            }
            keyMap[key] = buckets.begin();
        } else {
            auto curr = keyMap[key];
            auto nxt = next(curr);
            int newCount = curr->count + 1;

            if (nxt == buckets.end() || nxt->count > newCount) {
                nxt = buckets.insert(nxt, {newCount, {key}});
            } else {
                nxt->keys.insert(key);
            }

            keyMap[key] = nxt;
            curr->keys.erase(key);
            if (curr->keys.empty()) buckets.erase(curr);
        }
    }

    void dec(string key) {
        auto curr = keyMap[key];
        if (curr->count == 1) {
            keyMap.erase(key);
        } else {
            int newCount = curr->count - 1;
            auto prv = (curr == buckets.begin()) ? buckets.end() : prev(curr);

            if (curr == buckets.begin() || prv->count < newCount) {
                prv = buckets.insert(curr, {newCount, {key}});
            } else {
                prv->keys.insert(key);
            }
            keyMap[key] = prv;
        }

        curr->keys.erase(key);
        if (curr->keys.empty()) buckets.erase(curr);
    }

    string getMaxKey() {
        return buckets.empty() ? "" : *buckets.back().keys.begin();
    }

    string getMinKey() {
        return buckets.empty() ? "" : *buckets.front().keys.begin();
    }
};
// Interview Explanation:
// - Problem Statement: Design data structure with inc, dec, getMaxKey, getMinKey all in O(1) time.
// - Approach: Doubly Linked List of Frequency Buckets + Hash Map to Bucket Iterators.
// - Intuition: Maintain buckets sorted by count. Increment/decrement shifts keys to adjacent bucket or creates one in O(1).
// - Complexity: Time: O(1) for all operations, Space: O(Total unique keys).


// =========================================================
// 23. TARJAN'S BRIDGES IN GRAPH (CRITICAL CONNECTIONS)
// =========================================================

class SolutionBridges {
    int timer = 0;
    void dfs(int u, int p, const vvi& adj, vi& tin, vi& low, vvi& bridges) {
        tin[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u, adj, tin, low, bridges);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    bridges.push_back({u, v});
                }
            }
        }
    }

public:
    vvi criticalConnections(int n, vvi& connections) {
        vvi adj(n);
        for (auto& e : connections) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        vi tin(n, 0), low(n, 0);
        vvi bridges;
        dfs(0, -1, adj, tin, low, bridges);
        return bridges;
    }
};
// Interview Explanation:
// - Problem Statement: Find critical connections (bridges) whose removal disconnects the server network.
// - Approach: Tarjan's Bridge-Finding Algorithm via DFS discovery time and lowest reachable timestamp.
// - Intuition: An edge (u, v) is a bridge if low[v] > tin[u] (no back-edge from v's subtree to u or above).
// - Complexity: Time: O(V + E), Space: O(V + E).


// =========================================================
// 24. SERIALIZE AND DESERIALIZE N-ARY TREE
// =========================================================

class NodeNary {
public:
    int val;
    vector<NodeNary*> children;
    NodeNary() {}
    NodeNary(int _val) : val(_val) {}
    NodeNary(int _val, vector<NodeNary*> _children) : val(_val), children(_children) {}
};

class CodecNary {
public:
    string serialize(NodeNary* root) {
        if (!root) return "#";
        string s = to_string(root->val) + " " + to_string(root->children.size()) + " ";
        for (auto child : root->children) {
            s += serialize(child);
        }
        return s;
    }

    NodeNary* deserializeHelper(stringstream& ss) {
        string valStr;
        if (!(ss >> valStr) || valStr == "#") return nullptr;

        int val = stoi(valStr);
        int childCount;
        ss >> childCount;

        NodeNary* root = new NodeNary(val);
        for (int i = 0; i < childCount; ++i) {
            root->children.push_back(deserializeHelper(ss));
        }
        return root;
    }

    NodeNary* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }
};
// Interview Explanation:
// - Problem Statement: Serialize and deserialize an N-ary tree.
// - Approach: Pre-order traversal recording node value and child count.
// - Intuition: Encoding child count allows exact deterministic reconstruction of subtree sizes without ambiguity.
// - Complexity: Time: O(N), Space: O(N).


// =========================================================
// 25. LONGEST VALID PARENTHESES
// =========================================================

int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1); // Base index sentinel
    int maxLen = 0;

    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i); // Reset base invalid boundary
            } else {
                maxLen = max(maxLen, i - st.top());
            }
        }
    }
    return maxLen;
}
// Interview Explanation:
// - Problem Statement: Find length of longest valid (well-formed) parentheses substring.
// - Approach: Stack of Unmatched Indices with base boundary sentinel -1.
// - Intuition: Distance between current index and stack.top() is the length of valid substring ending at i.
// - Complexity: Time: O(N), Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | LFU Cache                                   | Frequency Doubly-Linked Lists+Map | O(1)     | O(Cap)   |
 | 2  | Design In-Memory File System                | Trie / N-ary Tree Directory       | O(K logM)| O(N)     |
 | 3  | Word Ladder II                              | BFS Layered DAG + DFS Backtrack   | O(N*26L) | O(N * L) |
 | 4  | Median of Two Sorted Arrays                 | Binary Search on Partition Cut    | O(log min| O(1)     |
 | 5  | Trapping Rain Water II (3D Grid)            | Min-Heap Boundary Priority Queue  | O(MNlogMN| O(MN)    |
 | 6  | Sliding Window Maximum                      | Monotonic Decreasing Deque        | O(N)     | O(K)     |
 | 7  | Word Search II                              | Prefix Trie + 2D Grid Backtrack   | O(MN*4^L)| O(Trie)  |
 | 8  | Text Justification                          | Greedy Line Packing + Modulo Space| O(N)     | O(N)     |
 | 9  | Bus Routes                                  | BFS on Bipartite Bus Route Graph  | O(Sum R) | O(Sum R) |
 | 10 | Binary Tree Cameras                         | Greedy Post-Order DFS (3-State DP)| O(N)     | O(H)     |
 | 11 | Vertical Order Traversal of Binary Tree     | BFS Coordinates + Multiset Sorting| O(N logN)| O(N)     |
 | 12 | Populating Next Right Pointers II           | Level Linked List Splicing Sentinel| O(N)    | O(1)     |
 | 13 | Accounts Merge                              | Disjoint Set Union (DSU) on Emails| O(E a(N))| O(E)     |
 | 14 | Basic Calculator II                         | Stack Operator Precedence Buffer  | O(N)     | O(N)     |
 | 15 | Basic Calculator (with Parentheses)         | Sign & Result Context Stack       | O(N)     | O(N)     |
 | 16 | Burst Balloons                              | Range / Interval DP (Last Burst)  | O(N^3)   | O(N^2)   |
 | 17 | Regular Expression Matching                 | 2D Dynamic Programming            | O(M * N) | O(M * N) |
 | 18 | Reverse Nodes in k-Group                    | In-Place K-Segment Reversal       | O(N)     | O(1)     |
 | 19 | Copy List with Random Pointer               | 3-Pass Node Interleaving          | O(N)     | O(1)     |
 | 20 | Maximum Width of Binary Tree                | Level BFS + 64-bit Column Normal  | O(N)     | O(W)     |
 | 21 | Number of Visible People in a Queue         | Right-to-Left Monotonic Stack     | O(N)     | O(N)     |
 | 22 | All O`one Data Structure                    | Doubly-Linked Bucket List + Map   | O(1)     | O(N)     |
 | 23 | Tarjan's Bridges (Critical Connections)     | DFS Discovery Time & Lowest low   | O(V + E) | O(V + E) |
 | 24 | Serialize and Deserialize N-ary Tree        | Pre-Order DFS with Child Count    | O(N)     | O(N)     |
 | 25 | Longest Valid Parentheses                   | Stack Unmatched Index Sentinel    | O(N)     | O(N)     |
 ====================================================================================================
*/

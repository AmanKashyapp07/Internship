
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

struct Node {
    int child[2];
    Node() {
        child[0] = child[1] = -1;
    }
}; 

vector<Node> trie(1); 

void insert(int x) {
    int cur = 0;

    for (int b = 30; b >= 0; b--) {
        int bit = (x >> b) & 1; 

        if (trie[cur].child[bit] == -1) {
            trie[cur].child[bit] = trie.size(); 
            trie.emplace_back(); 
        } 

        cur = trie[cur].child[bit];
    }
}

int max_xor(int x) {
    int cur = 0;
    int ans = 0;

    for (int b = 30; b >= 0; b--) {
        int bit = (x >> b) & 1;
        int want = bit ^ 1;

        if (trie[cur].child[want] != -1) {
            ans |= (1 << b);
            cur = trie[cur].child[want];
        } else {
            cur = trie[cur].child[bit];
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    insert(0);

    int pref = 0;
    int answer = 0;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        pref ^= x;

        answer = max(answer, max_xor(pref));

        insert(pref);
    }

    cout << answer << '\n';
}
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
#include <cstring>
#include <cstdlib>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

struct nodde {
    char val;
    int pri, sz;
    bool rev;
    nodde *l, *r;

    nodde(char c) {
        val = c;
        pri = rand();
        sz = 1;
        rev = false;
        l = r = nullptr;
    }
};

struct ImplicitTreap {
    int size(nodde* root) {
        return root ? root->sz : 0;
    }

    void update(nodde* root) {
        if (root)
            root->sz = 1 + size(root->l) + size(root->r);
    }

    void push(nodde* root) {
        if (!root || !root->rev) return;

        swap(root->l, root->r);

        if (root->l) root->l->rev ^= 1;
        if (root->r) root->r->rev ^= 1;

        root->rev = false;
    }

    void split(nodde* root, int k, nodde* &l, nodde* &r) {
        if (!root) {
            l = r = nullptr;
            return;
        }

        push(root);

        if (size(root->l) < k) {
            split(root->r, k - size(root->l) - 1, root->r, r);
            l = root;
        } else {
            split(root->l, k, l, root->l);
            r = root;
        }

        update(root);
    }

    nodde* merge(nodde* l, nodde* r) {
        push(l);
        push(r);

        if (!l || !r)
            return l ? l : r;

        if (l->pri > r->pri) {
            l->r = merge(l->r, r);
            update(l);
            return l;
        }

        r->l = merge(l, r->l);
        update(r);
        return r;
    }

    void print_string(nodde* root) {
        if (!root) return;

        push(root);
        print_string(root->l);
        cout << root->val;
        print_string(root->r);
    }

    void insert(nodde* &root, int idx, char x) {
        nodde *l, *r;
        split(root, idx, l, r);
        nodde* cur = new nodde(x);
        root = merge(merge(l, cur), r);
    }

    void deletee(nodde* &root, int idx) {
        nodde *l, *mid, *r;
        split(root, idx, l, r);
        split(r, 1, mid, r);
        delete mid;
        root = merge(l, r);
    }

    void reverse_range(nodde* &root, int L, int R) {
        nodde *l, *mid, *r;
        split(root, L, l, r);
        split(r, R - L + 1, mid, r);

        if (mid)
            mid->rev ^= 1;

        root = merge(merge(l, mid), r);
    }

    void cut_range_paste(nodde* &root, int L, int R) {
        nodde *l, *mid, *r;
        split(root, L, l, r);
        split(r, R - L + 1, mid, r);
        root = merge(l, r); // Remove the segment [L, R] from the treap
        root = merge(root, mid); // Paste the segment [L, R] at the end of the treap
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string str;
    cin >> str;
    nodde *root = nullptr;
    ImplicitTreap treap;
    for(auto c : str) {
        root = treap.merge(root, new nodde(c));
    }
    while(m--) {
        int a,b;
        cin >> a >> b; a--; b--;
        treap.cut_range_paste(root, a, b);
    }
    treap.print_string(root);
    return 0;
}
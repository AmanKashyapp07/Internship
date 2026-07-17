#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Random number generator for randomized treap priorities
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct nodde {
    int val;
    long long sum;
    int pri, sz;
    bool rev;
    nodde *l, *r;

    nodde(int v) {
        val = v;
        sum = v;
        pri = rng();
        sz = 1;
        rev = false;
        l = r = nullptr;
    }
};

struct ImplicitTreap {
    int size(nodde* root) { return root ? root->sz : 0; }
    
    void update(nodde* root) {
        if (!root) return;
        root->sz = 1 + size(root->l) + size(root->r);
        root->sum = root->val + (root->l ? root->l->sum : 0) + (root->r ? root->r->sum : 0);
    }
    
    void push(nodde* root) {
        if (!root || !root->rev) return;
        swap(root->l, root->r);
        if (root->l) root->l->rev ^= 1;
        if (root->r) root->r->rev ^= 1;
        root->rev = false;
    } // this function propagates the reversal flag down to the children and swaps the left and right children of the current node because the subtree needs to be reversed.
    
    void split(nodde* root, int k, nodde* &l, nodde* &r) {
        if (!root) return void(l = r = nullptr);
        push(root);
        if (size(root->l) < k) {
            split(root->r, k - size(root->l) - 1, root->r, r);
            l = root;
        } else {
            split(root->l, k, l, root->l);
            r = root;
        }
        update(root);
    } // this function splits the treap into two parts: the left part contains the first k elements, and the right part contains the rest. It uses the size of the left subtree to determine whether to go left or right in the recursion.
    
    nodde* merge(nodde* l, nodde* r) {
        push(l); push(r);
        if (!l || !r) return l ? l : r;
        if (l->pri > r->pri) {
            l->r = merge(l->r, r);
            update(l);
            return l;
        } else {
            r->l = merge(l, r->l);
            update(r);
            return r;
        }
    } // this function merges two treaps into one, maintaining the heap property based on priority. It recursively merges the right child of the left treap with the right treap or the left child of the right treap with the left treap, depending on which root has a higher priority.
    
    void reverse_range(nodde* &root, int L, int R) {
        nodde *l, *mid, *r;
        split(root, L, l, r);
        split(r, R - L + 1, mid, r);
        if (mid) mid->rev ^= 1;
        root = merge(merge(l, mid), r);
    }
    
    long long sum_range(nodde* &root, int L, int R) {
        nodde *l, *mid, *r;
        split(root, L, l, r);
        split(r, R - L + 1, mid, r);
        long long res = mid ? mid->sum : 0;
        root = merge(merge(l, mid), r); // we are merging l, mid, and r back together to restore the original treap structure after the sum query.
        return res;
    }
};

int main() {
    // Optimizing input/output operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m; cin >> n >> m;

    nodde* root = nullptr;
    ImplicitTreap treap;

    for (int i = 0; i < n; i++) {
        int val;
        cin >> val;
        root = treap.merge(root, new nodde(val));
    }

    for (int i = 0; i < m; i++) {
        int t, a, b;
        cin >> t >> a >> b;
        // Convert to 0-based indices
        a--; b--;
        if (t == 1) {
            treap.reverse_range(root, a, b);
        } else if (t == 2) {
            cout << treap.sum_range(root, a, b) << "\n";
        }
    }

    return 0;
}

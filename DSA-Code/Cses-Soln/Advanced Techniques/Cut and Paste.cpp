// Link: https://cses.fi/problemset/task/2072
#include <bits/stdc++.h>
using namespace std;

struct Node {
    char val; int pri, sz; bool rev;
    Node *l, *r;
    Node(char c) : val(c), pri(rand()), sz(1), rev(false), l(nullptr), r(nullptr) {}
};

int sz(Node* t) { return t ? t->sz : 0; }
void upd(Node* t) { if (t) t->sz = 1 + sz(t->l) + sz(t->r); }
void push(Node* t) {
    if (!t || !t->rev) return;
    swap(t->l, t->r);
    if (t->l) t->l->rev ^= 1;
    if (t->r) t->r->rev ^= 1;
    t->rev = false;
}
void split(Node* t, int k, Node*& l, Node*& r) {
    if (!t) { l = r = nullptr; return; }
    push(t);
    if (sz(t->l) < k) { split(t->r, k - sz(t->l) - 1, t->r, r); l = t; }
    else              { split(t->l, k, l, t->l); r = t; }
    upd(t);
}
Node* merge(Node* l, Node* r) {
    push(l); push(r);
    if (!l || !r) return l ? l : r;
    if (l->pri > r->pri) { l->r = merge(l->r, r); upd(l); return l; }
    else                  { r->l = merge(l, r->l); upd(r); return r; }
}
void print(Node* t) {
    if (!t) return; push(t);
    print(t->l); cout << t->val; print(t->r);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    string s; cin >> s;
    Node* root = nullptr;
    for (char c : s) root = merge(root, new Node(c));
    while (m--) {
        int a, b; cin >> a >> b; a--; b--;
        Node *l, *mid, *r;
        split(root, a, l, r); split(r, b - a + 1, mid, r);
        root = merge(merge(l, r), mid); // cut [a,b] and paste at end
    }
    print(root);
    return 0;
}

// Interview Explanation:
// - Problem Statement: Apply m cut-and-paste operations (move substring [l,r] to end) on string s (CSES 2072).
// - Approach: Implicit Treap with lazy reversal support.
// - Intuition: Split into [0,a-1], [a,b], [b+1,n-1]; merge back as [0,a-1] + [b+1,n-1] + [a,b] to place the segment at the end.
// - Complexity: Time: O((N + M) \log N), Space: O(N).
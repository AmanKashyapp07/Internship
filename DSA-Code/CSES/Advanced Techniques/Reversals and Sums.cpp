// Link: https://cses.fi/problemset/task/2074
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    int val, pri, sz; long long sum; bool rev;
    Node *l, *r;
    Node(int v) : val(v), pri(rng()), sz(1), sum(v), rev(false), l(nullptr), r(nullptr) {}
};

int sz(Node* t) { return t ? t->sz : 0; }
long long sm(Node* t) { return t ? t->sum : 0; }
void upd(Node* t) { if (t) { t->sz = 1 + sz(t->l) + sz(t->r); t->sum = t->val + sm(t->l) + sm(t->r); } }
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

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    Node* root = nullptr;
    for (int i = 0; i < n; i++) { int v; cin >> v; root = merge(root, new Node(v)); }
    while (m--) {
        int t, a, b; cin >> t >> a >> b; a--; b--;
        Node *l, *mid, *r;
        split(root, a, l, r); split(r, b - a + 1, mid, r);
        if (t == 1) { if (mid) mid->rev ^= 1; }
        else        { cout << sm(mid) << '\n'; }
        root = merge(l, merge(mid, r));
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Process m operations: (1) reverse subarray [l,r], or (2) query sum of subarray [l,r] (CSES 2074).
// - Approach: Implicit Treap with lazy reversal flag + augmented subtree sum.
// - Intuition: Each node stores subtree sum updated lazily; split/merge in O(log N); flip lazy `rev` flag for reversals, read `sum` field for range queries.
// - Complexity: Time: O((N + M) \log N), Space: O(N).

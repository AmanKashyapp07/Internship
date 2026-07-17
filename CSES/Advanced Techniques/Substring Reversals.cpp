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

struct ImplicitTreap{
    int size(nodde* t) { return t ? t->sz : 0; }
    void update(nodde* t) {
        if (t) t->sz = 1 + size(t->l) + size(t->r);
    }
    void push(nodde* t) {
        if (t && t->rev) {
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= true;
            if (t->r) t->r->rev ^= true;
            t->rev = false;
        }
    }

    void split(nodde* t, int k, nodde* &l, nodde* &r) {
        if (!t) { l = r = nullptr; return; }
        push(t);
        if (size(t->l) < k) {
            split(t->r, k - size(t->l) - 1, t->r, r);
            l = t;
        } else {
            split(t->l, k, l, t->l);
            r = t;
        }
        update(t);
    }

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
    }

    void inorder(nodde* t) {
        if (!t) return;
        push(t);
        inorder(t->l);
        cout << t->val;
        inorder(t->r);
    }
};

// Given a string, your task is to process operations where you reverse a substring of the string. What is the final string after all the operations?


int main(){
    int n, m; cin >> n >> m;
    string str;
    cin >> str;
    ImplicitTreap treap;
    nodde* root = nullptr;
    for (char c : str) root = treap.merge(root, new nodde(c));

    while(m--){
        int l, r; cin >> l >> r;
        nodde *left, *mid, *right;
        treap.split(root, l - 1, left, mid);
        treap.split(mid, r - l + 1, mid, right);
        mid->rev ^= true;
        root = treap.merge(left, treap.merge(mid, right));
    }

    treap.inorder(root);

}
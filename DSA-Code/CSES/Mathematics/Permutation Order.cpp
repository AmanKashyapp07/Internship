// Link: https://cses.fi/problemset/task/3397
#include <bits/stdc++.h>
using namespace std;

vector<long long> fact(21);

void init() {
    fact[0] = 1;
    for (int i = 1; i <= 20; i++) fact[i] = fact[i - 1] * i;
}

vector<int> kthPerm(int n, long long k) {
    k--;
    vector<int> unused(n), ans;
    iota(unused.begin(), unused.end(), 1);
    for (int rem = n; rem >= 1; rem--) {
        long long block = fact[rem - 1];
        int idx = k / block;
        ans.push_back(unused[idx]);
        unused.erase(unused.begin() + idx);
        k %= block;
    }
    return ans;
}

long long permRank(const vector<int>& p) {
    int n = p.size();
    vector<int> unused(n);
    iota(unused.begin(), unused.end(), 1);
    long long rank = 0;
    for (int i = 0; i < n; i++) {
        int idx = find(unused.begin(), unused.end(), p[i]) - unused.begin();
        rank += idx * fact[n - i - 1];
        unused.erase(unused.begin() + idx);
    }
    return rank + 1;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    init();
    int t; cin >> t;
    while (t--) {
        int type, n; cin >> type >> n;
        if (type == 1) {
            long long k; cin >> k;
            auto res = kthPerm(n, k);
            for (int i = 0; i < n; i++) cout << res[i] << " \n"[i == n - 1];
        } else {
            vector<int> p(n);
            for (int &x : p) cin >> x;
            cout << permRank(p) << '\n';
        }
    }
    return 0;
}

// Interview Explanation:
// - Problem Statement: Convert between 1-based lexicographical rank k and nth permutation (CSES 3397).
// - Approach: Factorial Number System (Lehmer Code / Block Indexing).
// - Intuition: Each position fixes a block of size (rem-1)!; dividing by block size gives the index among unused elements.
// - Complexity: Time: O(N^2) per query, Space: O(N).
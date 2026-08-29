// Link: https://cses.fi/problemset/task/1188
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size(), m; cin >> m;
    set<pair<int, int>> seg;
    multiset<int> len;

    for (int i = 0, j; i < n; i = j) {
        for (j = i; j < n && s[j] == s[i]; j++);
        seg.insert({i, j - 1});
        len.insert(j - i);
    }

    auto removeSeg = [&](auto it) {
        len.erase(len.find(it->second - it->first + 1));
        return seg.erase(it);
    };

    auto addSeg = [&](int l, int r) {
        seg.insert({l, r});
        len.insert(r - l + 1);
    };

    while (m--) {
        int x; cin >> x; x--;
        auto it = prev(seg.upper_bound({x, n}));
        int l = it->first, r = it->second;

        removeSeg(it);
        if (l < x) addSeg(l, x - 1);
        if (x < r) addSeg(x + 1, r);
        addSeg(x, x);
        s[x] ^= 1;

        it = seg.find({x, x});
        if (it != seg.begin()) {
            auto p = prev(it);
            if (s[p->first] == s[x]) {
                int L = p->first, R = it->second;
                removeSeg(p); removeSeg(it);
                addSeg(L, R);
                it = seg.find({L, R});
            }
        }
        auto nxt = next(it);
        if (nxt != seg.end() && s[nxt->first] == s[it->first]) {
            int L = it->first, R = nxt->second;
            removeSeg(nxt); removeSeg(it);
            addSeg(L, R);
        }
        cout << *len.rbegin() << " ";
    }
    cout << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Track the length of the longest contiguous subsegment of identical bits after point flips (CSES 1188).
// - Approach: Dynamic Interval Set (`std::set`) + Multiset for segment lengths.
// - Intuition: Splitting segment at index x, flipping bit, and merging adjacent matching segments updates maximal segment length in O(log N).
// - Complexity: Time: O((N + M) \log N), Space: O(N).
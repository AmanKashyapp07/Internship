// Link: https://cses.fi/problemset/task/1188

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    set<pair<int,int>> seg;     // stores all maximal equal-character segments
    multiset<int> len;          // stores lengths of all segments
    string s;

    cin >> s;
    s = " " + s;                // 1-based indexing
    int n = s.size() - 1;

    // Build the initial segments
    for (int i = 1, j; i <= n; i = j + 1) {
        j = i;
        while (j < n && s[j] == s[j + 1]) j++;

        seg.insert({i, j});
        len.insert(j - i + 1);
    }

    int m;
    cin >> m;

    while (m--) {
        int x;
        cin >> x;

        // Find the segment containing position x
        auto it = prev(seg.upper_bound({x, INT_MAX}));
        int l = it->first;
        int r = it->second;

        // Remove the old segment
        len.erase(len.find(r - l + 1));
        seg.erase(it);

        // Left part after splitting
        if (l < x) {
            seg.insert({l, x - 1});
            len.insert(x - l);
        }

        // Middle single character
        seg.insert({x, x});
        len.insert(1);

        // Right part after splitting
        if (x < r) {
            seg.insert({x + 1, r});
            len.insert(r - x);
        }

        // Flip the character ('0' <-> '1')
        s[x] ^= 1;

        // Get iterator to the single-character segment
        it = seg.find({x, x});

        // Merge with left segment if characters become equal
        if (it != seg.begin()) {
    auto p = prev(it);

    if (s[p->first] == s[x]) {
        int L = p->first;
        int R = it->second;

        // Remove left segment
        len.erase(len.find(p->second - p->first + 1));
        seg.erase(p);

        // Remove current segment (length = 1)
        len.erase(len.find(it->second - it->first + 1));
        seg.erase(it);

        // Insert merged segment
        seg.insert({L, R});
        len.insert(R - L + 1);

        it = seg.find({L, R});
    }
}

        // Merge with right segment if possible
        auto nxt = next(it);

        if (nxt != seg.end() && s[nxt->first] == s[it->first]) {
            int L = it->first;
            int R = nxt->second;

            // Remove right segment
            len.erase(len.find(nxt->second - nxt->first + 1));
            seg.erase(nxt);

            // Remove current segment
            len.erase(len.find(it->second - it->first + 1));
            seg.erase(it);

            // Insert merged segment
            seg.insert({L, R});
            len.insert(R - L + 1);
        }

        // Print the maximum segment length
        cout << *len.rbegin() << " ";
    }

    return 0;
}
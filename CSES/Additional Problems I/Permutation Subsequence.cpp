// Link: https://cses.fi/problemset/task/3404

#include <bits/stdc++.h>
using namespace std;

/*
    LCS of two permutations

    Observation:
    ------------
    Since both arrays are permutations, every value appears exactly once.

    Let pos[x] = position of x in the first array.

    Replace every element of the second array by its position in the first array.

    Example:
        A = [4 1 5 2 3]
        pos:
            4->1, 1->2, 5->3, 2->4, 3->5

        B = [1 3 4 5 2]

        becomes

        P = [2 5 1 3 4]

    Now,
    - Any common subsequence must appear in increasing order in A.
    - Therefore, its positions in A are strictly increasing.
    - Hence LCS(A,B) == LIS(P).

    After finding LIS of positions, convert positions back to values
    using the first array.
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    vector<int> pos(max(n, m) + 1, -1);

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[a[i]] = i;
    }

    vector<int> p;          // positions in A
    vector<int> value;      // corresponding original values

    for (int i = 1; i <= m; i++) {
        int x;
        cin >> x;

        if (x <= max(n, m) && pos[x] != -1) {
            p.push_back(pos[x]);
            value.push_back(x);
        }
    }

    if (p.empty()) {
        cout << 0 << "\n";
        return 0;
    }

    vector<int> tail;
    vector<int> tailIndex;
    vector<int> parent(p.size(), -1);

    for (int i = 0; i < (int)p.size(); i++) {

        int j = lower_bound(tail.begin(), tail.end(), p[i]) - tail.begin();

        if (j == (int)tail.size()) {
            tail.push_back(p[i]);
            tailIndex.push_back(i);
        } else {
            tail[j] = p[i];
            tailIndex[j] = i;
        }

        if (j > 0)
            parent[i] = tailIndex[j - 1];
    }

    vector<int> ans;

    int cur = tailIndex.back();

    while (cur != -1) {
        ans.push_back(value[cur]);
        cur = parent[cur];
    }

    reverse(ans.begin(), ans.end());

    cout << ans.size() << "\n";
    for (int x : ans)
        cout << x << " ";
}
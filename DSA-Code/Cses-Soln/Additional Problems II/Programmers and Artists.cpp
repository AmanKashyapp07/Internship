// Link: https://cses.fi/problemset/task/2426
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Candidate {
    ll programmer, artist;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, n;
    if (!(cin >> a >> b >> n)) return 0;

    vector<Candidate> c(n);
    for (int i = 0; i < n; i++) cin >> c[i].programmer >> c[i].artist;

    sort(c.begin(), c.end(), [](const Candidate& x, const Candidate& y) {
        return x.programmer - x.artist > y.programmer - y.artist;
    });

    vector<ll> pref(n, 0), suff(n, 0);

    if (a > 0) {
        priority_queue<ll, vector<ll>, greater<ll>> pqP;
        ll sumP = 0;
        for (int i = 0; i < n; i++) {
            pqP.push(c[i].programmer);
            sumP += c[i].programmer;
            if ((int)pqP.size() > a) {
                sumP -= pqP.top();
                pqP.pop();
            }
            if ((int)pqP.size() == a) pref[i] = sumP;
        }
    }

    if (b > 0) {
        priority_queue<ll, vector<ll>, greater<ll>> pqA;
        ll sumA = 0;
        for (int i = n - 1; i >= 0; i--) {
            pqA.push(c[i].artist);
            sumA += c[i].artist;
            if ((int)pqA.size() > b) {
                sumA -= pqA.top();
                pqA.pop();
            }
            if ((int)pqA.size() == b) suff[i] = sumA;
        }
    }

    ll answer = 0;
    if (a == 0) answer = suff[0];
    else if (b == 0) answer = pref[n - 1];
    else {
        for (int i = a - 1; i < n - b; i++) answer = max(answer, pref[i] + suff[i + 1]);
    }

    cout << answer << "\n";
    return 0;
}

// Interview Explanation:
// - Problem Statement: Select exactly A programmers and B artists from N candidates to maximize total skill score (CSES 2426).
// - Approach: Skill Difference Sorting + Dual Prefix/Suffix Min-Heaps.
// - Intuition: Sorting by (programmer - artist) descending partitions optimal assignments; prefix heap tracks best A programmers, suffix heap tracks best B artists, and linear sweep finds optimal split.
// - Complexity: Time: O(N log N), Space: O(N).

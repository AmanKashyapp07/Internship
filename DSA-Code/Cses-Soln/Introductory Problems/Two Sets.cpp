// Link: https://cses.fi/problemset/task/1092
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long n; cin >> n;
    long long sum = n * (n + 1) / 2;
    if (sum % 2 != 0) { cout << "NO\n"; return 0; }

    cout << "YES\n";
    vector<int> s1, s2;
    long long target = sum / 2;
    for (int i = n; i >= 1; i--) {
        if (i <= target) { s1.push_back(i); target -= i; }
        else { s2.push_back(i); }
    }

    cout << s1.size() << '\n';
    for (int i = 0; i < (int)s1.size(); i++) cout << s1[i] << " \n"[i == (int)s1.size() - 1];
    cout << s2.size() << '\n';
    for (int i = 0; i < (int)s2.size(); i++) cout << s2[i] << " \n"[i == (int)s2.size() - 1];
    return 0;
}

// Interview Explanation:
// - Problem Statement: Divide 1..n into two sets of equal sum if possible (CSES 1092).
// - Approach: Greedy Reverse Selection (knapsack greedy strategy).
// - Intuition: Total sum n(n+1)/2 must be even; greedy selection from n down to 1 fills target sum target = totalSum/2 optimally.
// - Complexity: Time: O(N), Space: O(N).
// Link: https://cses.fi/problemset/task/3361

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

bool check(double average,
           const vector<long long>& prefixA,
           const vector<long long>& prefixB,
           int& bestPrefixA,
           int& bestPrefixB)
{
    int n = prefixA.size() - 1;

    double bestValueA = -1e100;
    double bestValueB = -1e100;

    for (int i = 1; i <= n; i++) {
        double value = prefixA[i] - average * i;
        if (value > bestValueA) {
            bestValueA = value;
            bestPrefixA = i;
        }
    }

    for (int i = 1; i <= n; i++) {
        double value = prefixB[i] - average * i;
        if (value > bestValueB) {
            bestValueB = value;
            bestPrefixB = i;
        }
    }

    return bestValueA + bestValueB >= 0;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int64> prefixA(n + 1, 0), prefixB(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        int64 x;
        cin >> x;
        prefixA[i] = prefixA[i - 1] + x;
    }

    for (int i = 1; i <= n; i++) {
        int64 x;
        cin >> x;
        prefixB[i] = prefixB[i - 1] + x;
    }

    int bestPrefixA = 1, bestPrefixB = 1;

    double low = 0;
    double high = 1e9;

    for (int iter = 0; iter < 70; iter++) {
        double mid = (low + high) / 2;

        if (check(mid, prefixA, prefixB, bestPrefixA, bestPrefixB))
            low = mid;
        else
            high = mid;
    }

    cout << bestPrefixA << " " << bestPrefixB << '\n';

    return 0;
}
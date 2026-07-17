#include <bits/stdc++.h>
using namespace std;

void generateSubsets(int idx,
                     vector<long long>& primes,
                     vector<pair<long long, int>>& subsets,
                     long long product,
                     int cnt,
                     long long n) {
    if (idx == primes.size()) {
        if (cnt > 0)
            subsets.push_back({product, cnt});
        return;
    }

    // Take current prime (avoid overflow)
    if (product <= n / primes[idx]) {
        generateSubsets(idx + 1, primes, subsets,
                        product * primes[idx], cnt + 1, n);
    }

    // Don't take current prime
    generateSubsets(idx + 1, primes, subsets,
                    product, cnt, n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    vector<long long> primes(k);
    for (int i = 0; i < k; i++) {
        cin >> primes[i];
    }

    vector<pair<long long, int>> subsets;
    generateSubsets(0, primes, subsets, 1, 0, n);

    long long answer = 0;

    for (auto [product, cnt] : subsets) {
        long long cur = n / product;

        if (cnt & 1)
            answer += cur;
        else
            answer -= cur;
    }

    cout << answer << '\n';
    return 0;
}
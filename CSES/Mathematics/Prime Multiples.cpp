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

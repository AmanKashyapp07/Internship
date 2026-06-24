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


vector<pair<long long, int>> primeFactorization(long long n) {
    vector<pair<long long, int>> factors;

    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) { // p is a factor, we will strip down 
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            factors.push_back({p, cnt});
        }
    }

    if (n > 1) {
        factors.push_back({n, 1});
    }

    return factors;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    cin >> n;

    auto factors = primeFactorization(n);
    for (const auto& factor : factors) {
        cout << factor.first << "^" << factor.second << " ";
    }
    cout << "\n";

    return 0;
}
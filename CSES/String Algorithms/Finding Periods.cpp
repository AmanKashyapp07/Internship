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

class FastHash {
private:
    static inline long long P1 = 0, P2 = 0;
    static constexpr long long M1 = 1e9 + 7, M2 = 1e9 + 9;
    vector<long long> pow1, pow2, pref1, pref2;

    static void init() {
        if (P1 != 0) return;
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<long long> dist(150, 500);
        P1 = dist(rng) | 1; 
        P2 = dist(rng) | 1;
        if (P1 == P2) P2 += 2; 
    }

public:
    FastHash(string_view s) {
        init();
        int n = s.length();
        pow1.assign(n + 1, 1); pow2.assign(n + 1, 1);
        pref1.assign(n + 1, 0); pref2.assign(n + 1, 0);
        
        for (int i = 0; i < n; i++) {
            pow1[i + 1] = (pow1[i] * P1) % M1;
            pow2[i + 1] = (pow2[i] * P2) % M2;
            long long val = s[i] - 'a' + 1;
            pref1[i + 1] = (pref1[i] * P1 + val) % M1;
            pref2[i + 1] = (pref2[i] * P2 + val) % M2;
        }
    }

    uint64_t get(int L, int R) const {
        if (L > R) return 0; // Safety check
        long long r1 = (pref1[R + 1] - (pref1[L] * pow1[R - L + 1]) % M1 + M1) % M1;
        long long r2 = (pref2[R + 1] - (pref2[L] * pow2[R - L + 1]) % M2 + M2) % M2;
        return ((uint64_t)r1 << 32) | r2;
    }
};
// ---------------------------------------------------------------------------

int main() {
    // Optimize Standard I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.length();

    FastHash string_hash(s);

    // Check every possible period length 'k' from 1 to n
    for (int k = 1; k <= n; k++) {
        
        // The full string itself is always a valid period
        if (k == n) {
            cout << n << " ";
            continue;
        }

        // A period of length 'k' leaves an overlapping segment of length (n - k)
        int overlap_len = n - k;

        // Extract the hashes in O(1)
        uint64_t prefix_hash = string_hash.get(0, overlap_len - 1);
        uint64_t suffix_hash = string_hash.get(k, n - 1);

        // If the overlapping prefix and suffix are identical, 'k' is a valid period
        if (prefix_hash == suffix_hash) {
            cout << k << " ";
        }
    }
    
    cout << "\n";

    return 0;
}

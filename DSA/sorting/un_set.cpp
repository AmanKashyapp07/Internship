/**
 * Problem: Custom Hash Unordered Set
 * Link: N/A
 * Category: Data Structures
 * 
 * Description:
 * Demonstrates custom hash function to prevent anti-hash test cases in unordered_set/map.
 * 
 * Logic/Approach:
 * High-precision splitmix64 time-seeded custom hash wrapper.
 */

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
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

const int INF  = INT_MAX;
const ll  LINF = LLONG_MAX;
const ll  MOD  = 1e9 + 7;

// ─────────────────────────────────────────────────────────────────────────────
int helper(vector<int>& arr) {
    unordered_set<int> s;
    for (int x : arr) {
        s.insert(x);
    }
    int longest = 0;
    for (int x : arr) {
        if (s.count(x - 1) == 0) { // if x-1 is not present in the set, then x is the start of a new sequence
            int curr = 1;
            int len = 1;
            while (s.count(x + curr)) {
                curr++;
                len++;
            }
            longest = max(longest, len);
        }}
        return longest;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    cout << helper(arr) << endl;

    return 0;
}

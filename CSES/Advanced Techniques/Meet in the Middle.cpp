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

#define int long long 
using namespace std;


const int MOD  = 1e9 + 7;
// ─────────────────────────────────────────────────────────────────────────────
vector<int> generateSums(const vector<int>& arr) {
    vector<int> sums;
    int n = arr.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += arr[i];
            }
        }
        sums.push_back(sum);
    }
    return sums;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,x;
    cin >> n >> x;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    vector<int> left, right;
    left=generateSums(vector<int>(a.begin(), a.begin() + n / 2));
    right=generateSums(vector<int>(a.begin() + n / 2, a.end()));
    sort(right.begin(), right.end());
    int count = 0;
    for (int sum : left) {
        int target = x - sum;
        count += upper_bound(right.begin(), right.end(), target) - lower_bound(right.begin(), right.end(), target);
    }
    cout << count << "\n";
    return 0;
}

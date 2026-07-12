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
#define input(v) for (auto & x: v) cin >> x
#define srt(v) sort((v).begin(), (v).end())
#define rev(v) reverse((v).begin(), (v).end())
#define maxx(v)( * max_element((v).begin(), (v).end()))
#define minn(v)( * min_element((v).begin(), (v).end()))
using namespace std;
#define MOD 1000000007
const int MAXN = 1e6 + 5;

void prt(vector < int > & v) {
    for (auto c: v) cout << c << " ";
    cout << "\n";
}

// ---------------------------
// Aman Kashyap
// ---------------------------

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    //initFactorials();
    //sieve(1000000);

    int t = 1;
    //cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector < int > a(n);
        input(a);
        long long ans = 0;
        int l = 0;
        unordered_map < int, int > freq;

        for (int r = 0; r < n; r++) {
            freq[a[r]]++;

            while ((int) freq.size() > k) {
                freq[a[l]]--;

                if (freq[a[l]] == 0) {
                    freq.erase(a[l]);
                }

                l++;
            }

            ans += r - l + 1;
        }
        cout << ans << "\n";


    }

    return 0;
}

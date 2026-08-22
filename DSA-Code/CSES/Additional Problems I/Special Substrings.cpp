// Link: https://cses.fi/problemset/task/2186

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
#include <cstring>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vector<int>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<bool> present(26, false);
    for (char c : s) {
        present[c - 'a'] = true;
    }

    vector<int> chars;
    for (int i = 0; i < 26; i++) {
        if (present[i]) {
            chars.push_back(i);
        }
    }

    map<vector<int>, ll> mp;
    vector<int> cnt(26, 0);

    vector<int> state(chars.size(), 0);
    mp[state] = 1;

    ll ans = 0;

    for (char ch : s) {
        cnt[ch - 'a']++;

        int mn = INT_MAX;
        for (int idx : chars) {
            mn = min(mn, cnt[idx]);
        }

        for (int i = 0; i < (int)chars.size(); i++) {
            state[i] = cnt[chars[i]] - mn;
        }

        ans += mp[state];
        mp[state]++;
    }

    cout << ans << '\n';
    return 0;
}
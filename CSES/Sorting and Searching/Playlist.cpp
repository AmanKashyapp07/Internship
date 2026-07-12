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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> songs(n);
    for (int i = 0; i < n; i++) {
        cin >> songs[i];
    }

    set<int> window;
    int left = 0;
    int longestUniqueSequence = 0;

    for (int right = 0; right < n; right++) {
        while (window.count(songs[right])) {
            window.erase(songs[left]);
            left++;
        }

        window.insert(songs[right]);
        longestUniqueSequence = max(longestUniqueSequence, right - left + 1);
    }

    cout << longestUniqueSequence << '\n';

    return 0;
}

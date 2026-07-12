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

const int MAXX = 1'000'000;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> freq(MAXX + 1, 0);

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        freq[x]++;
    }

    int ans = 1;

    for (int d = 1; d <= MAXX; d++)
    {
        int cnt = 0;

        for (int multiple = d; multiple <= MAXX; multiple += d)
        {
            cnt += freq[multiple];
        }

        if (cnt >= 2)
            ans = d;
    }

    cout << ans << '\n';

    return 0;
}

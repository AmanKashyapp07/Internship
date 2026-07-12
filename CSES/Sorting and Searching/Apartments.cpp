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

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    vector<int> b(m);
    for (int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    sort(all(a));
    sort(all(b));

    int count = 0;

    int i = 0, j = 0;
    while (i < n && j < m){
        if (abs(a[i] - b[j]) <= k)
        {
            count++;
            i++;
            j++;
        }
        else if (a[i] < b[j])
        {
            i++;
        }
        else
        {
            j++;
        }
    }
    cout << count << "\n";
    return 0;
}

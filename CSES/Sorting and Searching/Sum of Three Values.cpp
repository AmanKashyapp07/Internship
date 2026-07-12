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

struct Number {
    long long value;
    int index;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    vector<Number> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i].value;
        a[i].index = i + 1;
    }

    sort(a.begin(), a.end(), [](const Number& p, const Number& q) {
        return p.value < q.value;
    });

    for (int i = 0; i < n; i++) {
        long long target = x - a[i].value;

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            long long sum = a[left].value + a[right].value;

            if (sum == target) {
                cout << a[i].index << ' '
                     << a[left].index << ' '
                     << a[right].index << '\n';
                return 0;
            }

            if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}

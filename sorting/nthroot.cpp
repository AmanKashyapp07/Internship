/**
 * CSES Problem Set
 * 
 * Problem: CSES Solution Template
 * Link: https://cses.fi/
 * Category: Template
 * 
 * Description:
 * A template file containing boilerplate code, fast I/O setup, and common macros/imports for solving CSES problems.
 * 
 * Logic/Approach:
 * Provides basic imports, standard namespace, and empty main function with fast I/O configuration.
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
double nthRoot(double x, int n, double precision = 1e-7) {
    if (x < 0 && n % 2 == 0) {
        throw invalid_argument("Even root of negative number is not real.");
    }

    double low = (x < 1) ? x : 0;
    double high = (x < 1) ? 1 : x;
    double mid;

    while (high - low > precision) {
        mid = low + (high - low) / 2;
        double midPower = pow(mid, n);

        if (midPower < x) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return (low + high) / 2;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);  
    int x,n;
    cin>>x>>n;
    cout<<fixed<<setprecision(7)<<nthRoot(x,n)<<endl;



    return 0;
}

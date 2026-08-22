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

class Solution {
public:
    int nthUglyNumber(int n) {
        priority_queue<long long, vector<long long>, greater<long long>> pq;
        unordered_set<long long> seen;
        pq.push(1);
        seen.insert(1);
        long long ugly = 1;
        while(pq.size() && n!=0) {
            ugly = pq.top();
            pq.pop();
            n--;
            if(n == 0) return ugly;
            for(int factor : {2, 3, 5}) {
                long long nextUgly = ugly * factor;
                if(seen.find(nextUgly) == seen.end()) {
                    seen.insert(nextUgly);
                    pq.push(nextUgly);
                }
            }
        }
        return -1; // This line should never be reached if n is valid
    }
};
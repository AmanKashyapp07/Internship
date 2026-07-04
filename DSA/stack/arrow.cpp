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
    int findMinArrowShots(vector<vector<int>>& points) {
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0]<b[0] || (a[0]==b[0] && a[1]<b[1]); // sort by start point, and if equal, by end point
        });
        int arrows=0;
        ll end=LLONG_MIN; // initialize end to the minimum possible value
        for(auto& p:points){
            if(p[0]>end){ // if the start of the current balloon is greater than the end of the last balloon, we need a new arrow
                arrows++;
                end=p[1]; // update the end to the end of the current balloon
            }
            else { // if the start of the current balloon is less than or equal to the end of the last balloon, we can shoot the same arrow, so we update the end to the minimum of the two ends
                end=min(end,(ll)p[1]);
            }
        }
        return arrows;
    }
};
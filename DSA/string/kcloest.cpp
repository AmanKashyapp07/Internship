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
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
       int n = arr.size();
        int mini = INT_MAX;
        int maxi = INT_MIN;
        for(int i=0;i<n;i++) mini=min(mini, arr[i]), maxi=max(maxi, arr[i]);
        if(x<=mini) return vector<int>(arr.begin(), arr.begin()+k);
        if(x>=maxi) return vector<int>(arr.end()-k, arr.end());
        int low=0, high=n-1;
        while(high-low+1!=k){
            if(abs(arr[low]-x)>abs(arr[high]-x)) low++;
            else high--;
        }
        return vector<int>(arr.begin()+low, arr.begin()+high+1); // vector from low to high , both inclusive

        
    }
};

// total time complexity is O(n) and space complexity is O(1) as we are using only a few variables
// if we used priority queue, time compelxity will be O(nlogk) and space complexity will be O(k) as we are using a priority queue of size k
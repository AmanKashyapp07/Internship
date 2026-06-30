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
    int countLessEqual(int mid, int m, int n) {
        // for every sorted row , check how many numbers are less than or equal to mid
        int count=0;
        for(int i=0;i<m;i++){
            count+=min(mid/(i+1),n); // mid/(i+1) gives the number of elements in the i-th row that are less than or equal to mid
        }
        return count;
    }
    int findKthNumber(int m, int n, int k) {
        int low=1;
        int high=m*n;
        int ans=-1;
        while(low<=high){
            int mid=low+(high-low)/2;
            int count=countLessEqual(mid,m,n);
            if(count<k) low=mid+1;
            else{
                ans=mid;
                high=mid-1;
            }
        }
        return ans;

        
    }
};
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

#define int long long
using namespace std;
void generateSubsets(vector<int>& a, vector<vector<int>>& subsets, int index = 0, vector<int> current = {})
{
    if (index == a.size())
    {
        subsets.push_back(current);
        return;
    }

    // Include the current element
    current.push_back(a[index]);
    generateSubsets(a, subsets, index + 1, current);

    // Exclude the current element
    current.pop_back();
    generateSubsets(a, subsets, index + 1, current);
}
signed main()
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    vector<vector<int>> subsets;
    generateSubsets(a, subsets);
    int sum=0;
    for(auto v:a) sum+=v;
    int ans=INT_MAX;
    for (const auto& subset : subsets){
        int sum_subset=0;
        for (int num : subset) sum_subset+=num;
        ans=min(ans, abs(sum-2*sum_subset));
    }
    cout<<ans<<'\n';

}

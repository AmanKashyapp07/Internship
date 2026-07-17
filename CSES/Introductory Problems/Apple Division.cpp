
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
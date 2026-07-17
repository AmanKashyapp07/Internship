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
#include <cstring>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<ll, ll>;
using pll = pair<ll, ll>;
using vi = vector<ll>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const ll INF = LLONG_MAX;
const ll MOD = 1e9 + 7;
const ll P = 31;


int merge(vector<int>& nums, int left, int mid, int right) {
    int inversions = 0;
    vector<int> temp(right - left + 1);

    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (nums[i] <= nums[j]) {
            temp[k++] = nums[i++];
        } else {
            temp[k++] = nums[j++];
            inversions += (mid - i + 1);   // Count inversions
        }
    }

    while (i <= mid)
        temp[k++] = nums[i++];

    while (j <= right)
        temp[k++] = nums[j++];

    for (int p = 0; p < k; p++)
        nums[left + p] = temp[p];

    return inversions;
}

int mergeSort(vector<int>& nums, int left, int right) {
    if (left >= right)
        return 0;

    int mid = left + (right - left) / 2;

    int inversions = 0;

    inversions += mergeSort(nums, left, mid);
    inversions += mergeSort(nums, mid + 1, right);
    inversions += merge(nums, left, mid, right);

    return inversions;
}

int countInversions(vector<int>& nums) {
    return mergeSort(nums, 0, nums.size() - 1);
}

int main(){
    int n,k;
    cin>>n>>k;
    vector<int> a(n);
    for(int i=0;i<n;i++) cin>>a[i];
    vector<int> b(k);
    for(int i=0;i<k;i++) b.push_back(a[i]);
    int count= countInversions(b);
    cout<<count<<" ";
    set<int>s;
    for(int i=0;i<k;i++) s.insert(a[i]);
    for(int i=k;i<n;i++){
        int old = a[i-k];
        int neww = a[i];
        
    }
}
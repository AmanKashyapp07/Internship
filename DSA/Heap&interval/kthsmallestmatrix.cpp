/**
 * Problem: Kth Smallest Sum in a Sorted Matrix
 * 
 * Given an m x n matrix where each row is sorted in non-decreasing order,
 * return the kth smallest sum among all possible arrays formed by choosing
 * exactly one element from each row.
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
    int kthSmallest(vector<vector<int>>& mat, int k) {
        int m = mat.size();
        int n = mat[0].size();
        
        using T = pair<int, vector<int>>; // {current sum, indices}
        priority_queue<T, vector<T>, greater<T>> pq;
        
        // Initialize with first column
        int initSum = 0;
        vector<int> start(m, 0);
        for (int i = 0; i < m; ++i) {
            initSum += mat[i][0];
        }
        
        map<vector<int>, bool> visited;
        visited[start] = true;
        pq.push({initSum, start});
        
        while (!pq.empty()) {
            auto [sum, idx] = pq.top();
            pq.pop();
            k--;
            
            if (k == 0) return sum;
            
            // Try increasing index in each row
            for (int i = 0; i < m; ++i) {
                if (idx[i] + 1 < n) {
                    vector<int> nextIdx = idx;
                    nextIdx[i]++;
                    
                    int newSum = sum - mat[i][idx[i]] + mat[i][nextIdx[i]];
                    
                    if (!visited[nextIdx]) {
                        visited[nextIdx] = true;
                        pq.push({newSum, nextIdx});
                    }
                }
            }
        }
        
        return -1; // Should not reach here for valid inputs
    }
};
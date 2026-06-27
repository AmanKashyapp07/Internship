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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> arr(n);
    for (int i = 0; i < n; i++){
        cin >> arr[i];
    }
    
    int maxSum = arr[0];
    int currentSum = arr[0];
    
    int currentStart = 0;
    int bestStart = 0;
    int bestEnd = 0;
    
    for (int i = 1; i < n; i++){
        if (currentSum < 0){
            currentSum = arr[i];
            currentStart = i; // Reset the potential start position
        } else {
            currentSum += arr[i];
        }
        
        // Update the maximum sum and its boundaries lock-step
        if (currentSum > maxSum) {
            maxSum = currentSum;
            bestStart = currentStart;
            bestEnd = i;
        }
    }
    
    cout << "Max Sum: " << maxSum << '\n';
    cout << "Start Position: " << bestStart << ", End Position: " << bestEnd << '\n';
    
    cout << "Full subarray: ";
    for (int i = bestStart; i <= bestEnd; i++){
        cout << arr[i] << " ";
    }
    cout << '\n';
    
    return 0;
}
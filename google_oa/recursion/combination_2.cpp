/**
 * LeetCode 40 - Combination Sum II
 *
 * Description:
 * Given a collection of candidate numbers (candidates) and a target number (target), find all
 * unique combinations in candidates where the candidate numbers sum to target. Each number in
 * candidates may only be used once in the combination.
 *
 * Approach:
 * - Sort candidates to group duplicate elements and enable pruning.
 * - Backtracking / DFS traversal.
 * - To prevent duplicate combinations, skip identical elements at the same recursion depth
 *   using: `if (i > start && a[i] == a[i - 1]) continue;`.
 * - Pruning: Stop exploring if the current element exceeds the remaining target (`if (a[i] > target) break;`).
 *
 * Time Complexity: O(2^N) in the worst case.
 * Space Complexity: O(N) for the recursion stack and current path.
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

vector<vi> ans;
vi cur;
vi a;

void dfs(int start, int target) {
    if (target == 0) {
        ans.pb(cur);
        return;
    }
    int n = a.size();
    for (int i = start; i < n; i++) {
        if (i > start && a[i] == a[i - 1]) continue;
        if (a[i] > target) break;
        cur.pb(a[i]);
        dfs(i + 1, target - a[i]); // use each element only once
        cur.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, target;
    cin >> n >> target;

    a.resize(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    sort(all(a));

    dfs(0, target);

    for (auto &v : ans) {
        cout << "[ ";
        for (int x : v)
            cout << x << ' ';
        cout << "]\n";
    }

    return 0;
}

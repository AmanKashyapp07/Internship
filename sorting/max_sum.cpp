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

using MaxHeap = priority_queue<pair<int, pii>>;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> num1 = {1, 2, 3, 4, 5};
    vector<int> num2 = {5, 4, 3, 2};

    int k = 5;

    sort(rall(num1));
    sort(rall(num2));

    MaxHeap maxHeap;

    set<pii> visited;

    maxHeap.push({num1[0] + num2[0], {0, 0}});
    visited.insert({0, 0});

    vector<int> ans;

    while (!maxHeap.empty() && ans.size() < k) {

        auto top = maxHeap.top();
        maxHeap.pop();

        int sum = top.ff;
        int i = top.ss.ff;
        int j = top.ss.ss;

        ans.pb(sum);

        if (i + 1 < num1.size() &&
            !visited.count({i + 1, j})) {

            maxHeap.push({
                num1[i + 1] + num2[j],
                {i + 1, j}
            });

            visited.insert({i + 1, j});
        }

        if (j + 1 < num2.size() &&
            !visited.count({i, j + 1})) {

            maxHeap.push({
                num1[i] + num2[j + 1],
                {i, j + 1}
            });

            visited.insert({i, j + 1});
        }
    }

    for (int x : ans)
        cout << x << " ";

    return 0;
}
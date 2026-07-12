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

ll largestHistogram(vector<int>& h) {
    int m = h.size();
    stack<int> st;
    ll best = 0;

    for (int i = 0; i <= m; i++) {
        int curHeight = (i == m ? 0 : h[i]);

        while (!st.empty() && h[st.top()] >= curHeight) {
            int height = h[st.top()];
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int width = i - left - 1;

            best = max(best, 1LL * height * width);
        }

        st.push(i);
    }

    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> height(m, 0);
    ll answer = 0;

    for (int row = 0; row < n; row++) {
        string s;
        cin >> s;

        for (int col = 0; col < m; col++) {
            if (s[col] == '.')
                height[col]++;
            else
                height[col] = 0;
        }

        answer = max(answer, largestHistogram(height));
    }

    cout << answer << '\n';
}

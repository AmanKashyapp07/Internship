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

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    cin >> s;

    vector<int> freq(26, 0);
    for (char c : s) {
        freq[c - 'A']++;
    }

    int oddCount = 0;
    char middleChar = '\0'; // Track the single middle character if it exists

    for (int i = 0; i < 26; i++) {
        if (freq[i] % 2 != 0) {
            oddCount++;
            middleChar = char('A' + i);
            freq[i]--; // Reduce by 1 so the rest can be divided evenly into the halves
        }
    }

    // A palindrome can have at most one character with an odd frequency
    if (oddCount > 1) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    string left = "";
    for (int i = 0; i < 26; i++) {
        left.append(freq[i] / 2, char('A' + i));
    }

    string right = left;
    reverse(right.begin(), right.end());

    // Construct the final palindrome
    if (oddCount == 1) {
        cout << left << middleChar << right << '\n';
    } else {
        cout << left << right << '\n';
    }

    return 0;
}

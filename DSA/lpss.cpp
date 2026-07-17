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

ll hashWord(string s)
{
    ll h = 0;
    for (char c : s)
        h = (h * P + (c - 'a' + 1)) % MOD;
    return h;
}

struct FastHash
{
    vector<ll> p, h;

    FastHash(string s)
    {
        int n = s.size();
        p.assign(n + 1, 1);
        h.assign(n + 1, 0);

        for (int i = 0; i < n; i++)
        {
            p[i + 1] = p[i] * P % MOD;
            h[i + 1] = (h[i] * P + (s[i] - 'a' + 1)) % MOD;
        }
    }

    ll get(int l, int r)
    {
        return (h[r + 1] - h[l] * p[r - l + 1] % MOD + MOD) % MOD;
    }
};

pair<vi, vi> manacher(string s)
{
    int n = s.size();
    vi d1(n, 1), d2(n, 0);
    if (n == 0)
        return {d1, d2};

    FastHash fh(s);
    string s_rev = s;
    reverse(s_rev.begin(), s_rev.end());
    FastHash fh_rev(s_rev);

    auto is_pal = [&](int l, int r)
    {
        return l >= 0 && r < n && l <= r && fh.get(l, r) == fh_rev.get(n - 1 - r, n - 1 - l);
    };

    for (int i = 0; i < n; i++)
    {
        int low = 2, high = min(i + 1, n - i);
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid + 1, i + mid - 1))
                d1[i] = mid, low = mid + 1;
            else
                high = mid - 1;
        }

        low = 1, high = min(i, n - i);
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            if (is_pal(i - mid, i + mid - 1))
                d2[i] = mid, low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return {d1, d2};
}

int main()
{
    string s;
    cin >> s;

    auto [d1, d2] = manacher(s); // d1 is the radius of odd-length palindromes centered at each index, and d2 is the radius of even-length palindromes centered between each pair of indices.
    int maxiLength = 0;
    int startIndex = 0;

    for (int i = 0; i < s.size(); i++)
    {

        // odd palindrome
        int oddLength = 2 * d1[i] - 1;
        if (oddLength > maxiLength)
        {
            maxiLength = oddLength;
            startIndex = i - d1[i] + 1;
        }

        // even palindrome
        int evenLength = 2 * d2[i];
        if (evenLength > maxiLength)
        {
            maxiLength = evenLength;
            startIndex = i - d2[i];
        }
    }

    cout << s.substr(startIndex, maxiLength) << '\n';

    return 0;
}
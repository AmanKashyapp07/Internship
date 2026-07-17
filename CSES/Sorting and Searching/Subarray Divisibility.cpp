#include <bits/stdc++.h>
#define int long long
#define input(v)      \
    for (auto &x : v) \
    cin >> x
#define srt(v) sort((v).begin(), (v).end())
#define rev(v) reverse((v).begin(), (v).end())
#define maxx(v) (*max_element((v).begin(), (v).end()))
#define minn(v) (*min_element((v).begin(), (v).end()))
using namespace std;
#define MOD 1000000007
const int MAXN = 1e6 + 5;

void prt(vector<int> &v)
{
    for (auto c : v)
        cout << c << " ";
    cout << "\n";
}

// ---------------------------
// Aman Kashyap
// ---------------------------

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // initFactorials();
    // sieve(1000000);

    int t = 1;
    // cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<int> ish(n);
        input(ish);
        int sum = 0;
        unordered_map<int, int> hash;
        int ans = 0;
        hash[0] = 1;
        for (auto v : ish)
        {
            sum += v;
            sum %= n;

            if (sum < 0)
            {
                sum += n;
            }

            ans += hash[sum];
            hash[sum]++;
        }
        cout << ans << endl;
    }

    return 0;
}
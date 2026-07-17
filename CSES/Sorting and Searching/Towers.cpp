#include <algorithm>
#include <iostream>
#include <vector>
#define int long long
using namespace std;

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    vector<int> b;

    for (int i = 0; i < n; i++)
    {
        auto it = upper_bound(b.begin(), b.end(), a[i]);

        if (it == b.end())

        {

            b.push_back(a[i]);
        }

        else

        {

            *it = a[i];
        }
    }

    cout << b.size() << "\n";
}
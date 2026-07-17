#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int MAXX = 1'000'000;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> freq(MAXX + 1, 0);

    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        freq[x]++;
    }

    int ans = 1;

    for (int d = 1; d <= MAXX; d++)
    {
        int cnt = 0;

        for (int multiple = d; multiple <= MAXX; multiple += d)
        {
            cnt += freq[multiple];
        }

        if (cnt >= 2)
            ans = d;
    }

    cout << ans << '\n';

    return 0;
}
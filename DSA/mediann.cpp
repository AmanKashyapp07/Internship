#include <bits/stdc++.h>
using namespace std;

struct CoordinateCompressor
{
    vector<int> vals;

    void add(int x)
    {
        vals.push_back(x);
    }

    void build()
    {
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
    }

    int get(int x)
    {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    }

    int size()
    {
        return vals.size();
    }
};

struct Fenwick
{
    int n;
    vector<int> bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void update(int i, int val)
    {
        i++;
        while (i <= n)
        {
            bit[i] += val;
            i += i & -i;
        }
    } // update the frequency of index i by val (can be positive or negative)

    int query(int i)
    {
        i++;
        int sum = 0;
        while (i > 0)
        {
            sum += bit[i];
            i -= i & -i;
        }
        return sum;
    } // returns the sum of frequencies from index 0 to i (inclusive)

    int query(int l, int r)
    {
        if (l > r)
            return 0;
        return query(r) - (l ? query(l - 1) : 0);
    } // returns the sum of frequencies from index l to r (inclusive)
};

int kth(Fenwick &bit, int k)
{
    int l = 0, r = bit.n - 1;
    int ans = -1;
    while (l <= r)
    {
        int mid = (l + r) / 2;

        if (bit.query(mid) >= k) // if sum of frequencies from 0 to mid is greater than or equal to k, the k-th smallest element is in the left half
        {
            ans = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    return ans;
} // finds the index of the k-th smallest element in the current multiset represented by the Fenwick tree, which is median.

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    CoordinateCompressor cc;

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        cc.add(a[i]);
    }

    cc.build();

    Fenwick bit(cc.size());

    vector<int> id(n);
    for (int i = 0; i < n; i++)
        id[i] = cc.get(a[i]);

    // First window
    for (int i = 0; i < k; i++)
        bit.update(id[i], 1);

    int need = (k + 1) / 2;

    cout << cc.vals[kth(bit, need)];

    for (int i = k; i < n; i++)
    {
        bit.update(id[i - k], -1);
        bit.update(id[i], 1);

        cout << " " << cc.vals[kth(bit, need)];
    }

    cout << '\n';

    return 0;
}
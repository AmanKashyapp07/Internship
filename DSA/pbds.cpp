#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

using ll = long long;

typedef tree<
    pair<int,int>,
    null_type,
    less<pair<int,int>>,
    rb_tree_tag,
    tree_order_statistics_node_update
> ordered_set;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &x : a) cin >> x;

    ordered_set os;

    // Build first window
    for (int i = 0; i < k; i++)
        os.insert({a[i], i});

    // Initial inversion count using PBDS
    ll inv = 0;
    ordered_set temp;

    for (int i = 0; i < k; i++) {
        inv += temp.size() - temp.order_of_key({a[i], INT_MAX});
        temp.insert({a[i], i});
    }

    cout << inv;

    for (int i = k; i < n; i++) {
        int out = i - k;

        // Remove inversions contributed by outgoing element
        inv -= os.order_of_key({a[out], INT_MIN});

        os.erase({a[out], out});

        // Add inversions created by incoming element
        inv += os.size() - os.order_of_key({a[i], INT_MAX});

        os.insert({a[i], i});

        cout << " " << inv;
    }

    cout << '\n';
}
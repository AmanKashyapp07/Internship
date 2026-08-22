// Link: https://cses.fi/problemset/task/1071

#include <iostream>
#include <algorithm>

using namespace std;

void solve() {
    long long y, x;
    cin >> y >> x;
    
    long long z = max(y, x);
    long long ans = 0;
    
    if (z % 2 == 0) {
        if (y <= x) {
            ans = (z - 1) * (z - 1) + y;
        } else {
            ans = z * z - x + 1;
        }
    } else {
        if (y <= x) {
            ans = z * z - y + 1;
        } else {
            ans = (z - 1) * (z - 1) + x;
        }
    }
    
    cout << ans << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> freq(n + 1);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        freq[x]++;
    }

    // pw[i] = 2^i (mod MOD)
    vector<long long> pw(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; i++)
        pw[i] = pw[i - 1] * 2 % MOD;

    vector<int> cnt(n + 1);

    // cnt[i] = number of array elements divisible by i
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i)
            cnt[i] += freq[j];
    }

    vector<long long> ans(n + 1);

    // Let:
    // F(i) = number of non-empty subsets whose every element is divisible by i
    //      = 2^(cnt[i]) - 1
    //
    // G(i) = number of subsets whose gcd is exactly i (this is what we want)
    //
    // Every subset counted in F(i) has gcd equal to
    // i, 2i, 3i, ...
    //
    // Therefore,
    // F(i) = G(i) + G(2i) + G(3i) + ...
    //
    // Rearranging,
    // G(i) = F(i) - G(2i) - G(3i) - ...
    //
    // Process i from n down to 1 so that all multiples are already computed.
    for (int i = n; i >= 1; i--) {
        ans[i] = (pw[cnt[i]] - 1 + MOD) % MOD;

        for (int j = 2 * i; j <= n; j += i) {
            ans[i] -= ans[j];
            if (ans[i] < 0) ans[i] += MOD;
        }
    }

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
    cout << '\n';
}
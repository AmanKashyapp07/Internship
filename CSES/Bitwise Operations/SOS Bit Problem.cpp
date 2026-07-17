// Link: https://cses.fi/problemset/task/1654

#include <bits/stdc++.h>
using namespace std;

struct SOS {
    static constexpr int B = 20, N = 1 << B;
    vector<int> f, sub, sup;

    SOS() : f(N), sub(N), sup(N) {}

    void add(int x) { f[x]++; }

    void build() {
        sub = sup = f;
        for (int b = 0; b < B; b++)
            for (int m = 0; m < N; m++)
                if (m >> b & 1) sub[m] += sub[m ^ (1 << b)];
                else            sup[m] += sup[m | (1 << b)];
    }

    // x | y = x
    int subsets(int x) { return sub[x]; }

    // x & y = x
    int supersets(int x) { return sup[x]; }

    // x & y != 0
    int intersect(int x, int n) { return n - sub[(N - 1) ^ x]; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    SOS s;

    for (int &x : a) {
        cin >> x;
        s.add(x);
    }

    s.build();

    for (int x : a)
        cout << s.subsets(x) << ' '
             << s.supersets(x) << ' '
             << s.intersect(x, n) << '\n';
}
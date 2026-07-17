#include <bits/stdc++.h>
using namespace std;

struct Number {
    long long value;
    int index;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    vector<Number> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i].value;
        a[i].index = i + 1;
    }

    sort(a.begin(), a.end(), [](const Number& p, const Number& q) {
        return p.value < q.value;
    });

    for (int i = 0; i < n; i++) {
        long long target = x - a[i].value;

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            long long sum = a[left].value + a[right].value;

            if (sum == target) {
                cout << a[i].index << ' '
                     << a[left].index << ' '
                     << a[right].index << '\n';
                return 0;
            }

            if (sum < target) {
                left++;
            } else {
                right--;
            }
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}
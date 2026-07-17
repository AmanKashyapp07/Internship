#include <iostream>
#include <vector>

using namespace std;

int main() {
    long long n;
    cin >> n;

    long long sum = n * (n + 1) / 2;

    if (sum & 1) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";

    long long target = sum / 2;
    vector<int> a, b;

    for (long long i = n; i >= 1; i--) {
        if (i <= target) {
            a.push_back(i);
            target -= i;
        } else {
            b.push_back(i);
        }
    }

    cout << a.size() << '\n';
    for (int x : a) cout << x << ' ';
    cout << '\n';

    cout << b.size() << '\n';
    for (int x : b) cout << x << ' ';
    cout << '\n';

    return 0;
}
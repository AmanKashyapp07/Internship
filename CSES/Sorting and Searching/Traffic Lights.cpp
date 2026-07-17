#include <iostream>
#include <set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, n;
    cin >> x >> n;

    set<int> lights;
    multiset<int> lengths;

    lights.insert(0);
    lights.insert(x);
    lengths.insert(x);

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        auto it = lights.upper_bound(p);

        int r = *it;
        int l = *prev(it);

        lengths.erase(lengths.find(r - l));

        lengths.insert(p - l);
        lengths.insert(r - p);

        lights.insert(p);

        cout << *lengths.rbegin() << ' ';
    }

    cout << '\n';
    return 0;
}
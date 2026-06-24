#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll  = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi  = vector<int>;
using vll = vector<ll>;

#define all(x)   (x).begin(), (x).end()
#define rall(x)  (x).rbegin(), (x).rend()
#define pb       push_back
#define ff       first
#define ss       second

struct Item {
    int value;
    int weight;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    double W;
    cin >> N >> W;

    vector<Item> items(N);

    for (int i = 0; i < N; i++) {
        cin >> items[i].value >> items[i].weight;
    }

    sort(all(items), [](const Item &a, const Item &b) {
        return (double)a.value / a.weight >
               (double)b.value / b.weight;
    });

    double maxValue = 0.0;

    for (int i = 0; i < N && W > 0; i++) {
        if (items[i].weight <= W) {
            maxValue += items[i].value;
            W -= items[i].weight;
        } else {
            maxValue += ((double)items[i].value / items[i].weight) * W;
            W = 0;
        }
    }

    cout << fixed << setprecision(6) << maxValue << '\n';

    return 0;
}
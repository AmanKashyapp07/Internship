// Link: https://cses.fi/problemset/task/2162

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue> // <-- Added this missing header

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    queue<int> q;

    for (int i = 1; i <= n; i++) {
        q.push(i);
    }

    while (!q.empty()) {
        q.push(q.front());
        q.pop();

        cout << q.front() << ' ';
        q.pop();
    }

    cout << '\n';

    return 0;
}
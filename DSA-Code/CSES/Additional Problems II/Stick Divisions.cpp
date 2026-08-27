// Link: https://cses.fi/problemset/task/1161
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int x, n; cin >> x >> n;
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    while (n--) { long long d; cin >> d; pq.push(d); }

    long long totalCost = 0;
    while (pq.size() > 1) {
        long long a = pq.top(); pq.pop();
        long long b = pq.top(); pq.pop();
        totalCost += a + b;
        pq.push(a + b);
    }
    cout << totalCost << '\n';
    return 0;
}

// Interview Explanation:
// - Problem Statement: Divide a stick of length x into n sticks of specified lengths with minimum total cost (CSES 1161).
// - Approach: Huffman Coding Greedy Strategy via Min-Heap.
// - Intuition: Reversing division into joining sticks shows that greedily merging the two shortest sticks at each step minimizes cumulative sum additions.
// - Complexity: Time: O(N \log N), Space: O(N).
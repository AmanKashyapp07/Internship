/**
 * CSES Problem Set
 * 
 * Problem: Restaurant Customers
 * Link: https://cses.fi/problemset/task/1619
 * Category: Sorting and Searching
 * 
 * Description:
 * Find the maximum number of customers in a restaurant at any one time.
 * 
 * Logic/Approach:
 * Sweep-line: sort arrivals (+1) and departures (-1) and track maximum prefix sum.
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> events;

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;

        events.push_back({a, 1});   // arrival
        events.push_back({b, -1});  // leaving
    }

    sort(events.begin(), events.end());
    for(auto [time, change] : events) {
        // This loop is for debugging purposes to see the sorted events
         cout << "Time: " << time << ", Change: " << change << endl;
    }
    int current = 0;
    int answer = 0;

    for (auto [time, change] : events) {
        current += change;
        answer = max(answer, current);
    }

    cout << answer << '\n';
    return 0;
}
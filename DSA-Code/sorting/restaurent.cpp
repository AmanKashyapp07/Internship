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
 * if two intervals have same start and end time, we will consider the departure first, because if we consider the arrival first, then we will count the customer twice, once for arrival and once for departure, which is not correct. So we will consider the departure first, so that we can count the customer only once.
 * it means if (1,2) and (2,3) , then max answer will be 1, because at time 2, one customer will leave and another will arrive, so we will consider the departure first, so that we can count the customer only once.
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
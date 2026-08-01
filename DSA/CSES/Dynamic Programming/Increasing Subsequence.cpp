// Link: https://cses.fi/problemset/task/1145

#include <iostream>
#include <vector>
using namespace std;
// this is important 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> tails;
    // tails hold the smallest tail of all increasing subsequences of length i+1 in tails[i].
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;

        auto it = lower_bound(tails.begin(), tails.end(), x); // Find the first element in tails that is >= x , we can use binary search here because tails is always sorted in increasing order because we are always replacing elements with smaller values.

        if (it == tails.end()) {
            tails.push_back(x); // if x is greater than all elements in tails, append it to the end
        } else {
            *it = x; // Otherwise, replace the existing element with x. This maintains the property of tails that it is sorted and contains the smallest possible tail for increasing subsequences of that length.
        }
    }

    cout << tails.size() << '\n';
    return 0;
}


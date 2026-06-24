#include <iostream>
#include <vector>
using namespace std;

vector<int> arr = {1, 2, 3};
vector<int> perm;

void solve(int mask) {
    if (perm.size() == arr.size()) {
        for (int x : perm) cout << x << " ";
        cout << "\n";
        return;
    }

    for (int i = 0; i < arr.size(); i++) {
        if (mask & (1 << i)) continue;

        perm.push_back(arr[i]);
        solve(mask | (1 << i));
        perm.pop_back();
    }
} // bitmasking , time compelxity is O(n!*n) , as we are generating all permutations of n elements, and for each permutation, we are printing it which takes O(n) time, so total time complexity is O(n!*n), space complexity is O(n) for the recursion stack and O(n) for the perm vector, so total space complexity is O(n)

void solve2(int idx) {
    if (idx == arr.size()) {
        for (int x : perm) cout << x << " ";
        cout << "\n";
        return;
    }

    for (int i = idx; i < arr.size(); i++) {
        swap(arr[idx], arr[i]);
        perm.push_back(arr[idx]);
        solve2(idx + 1);
        perm.pop_back();
        swap(arr[idx], arr[i]);
    }
} // backtracking , time complexity is O(n!*n) , as we are generating all permutations of n elements, and for each permutation, we are printing it which takes O(n) time, so total time complexity is O(n!*n), space complexity is O(n) for the recursion stack and O(n) for the perm vector, so total space complexity is O(n)

int main() {
    solve(0);
}
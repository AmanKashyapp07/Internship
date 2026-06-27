/**
 * LeetCode 1125 - Smallest Sufficient Team
 *
 * Description:
 * Given a list of required skills req_skills, and a list of people, where people[i] contains the skills that person i has.
 * Find the smallest sufficient team (subset of people) such that every required skill is possessed by at least one team member.
 *
 * Approach:
 * - Bitmask Dynamic Programming with Memoization.
 * - Represent required skills as a bitmask of size `m` (skills covered).
 * - Let `solve(mask)` return the vector of indices representing the smallest sufficient team to cover the remaining skills.
 * - Transition: From state `mask`, try taking person `i` (adding their skills to `mask` if they add any new skill) and recursively solve for the new mask.
 *
 * Time Complexity: O(2^m * n) where m = req_skills.size(), n = people.size().
 * Space Complexity: O(2^m)
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
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
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

class Solution {
public:
    int n, m, FULL;
    vector<int> personMask;

    vector<vector<int>> dp;
    vector<bool> vis;

    vector<int> solve(int mask) {

        // All skills covered
        if (mask == FULL)
            return {};

        if (vis[mask])
            return dp[mask];

        vis[mask] = true;

        // Initialize with an impossible large team
        vector<int> best(100);

        // Try taking every person
        for (int i = 0; i < n; i++) {

            int newMask = mask | personMask[i];

            // Person adds no new skill
            if (newMask == mask)
                continue;

            vector<int> team = solve(newMask); // team vector for newMask
            team.push_back(i); // pushing because we are taking this person, initially team didn't have this person

            if (team.size() < best.size())
                best = team;
        }

        return dp[mask] = best;
    }


    vector<int> smallestSufficientTeam(vector<string>& req_skills,
                                       vector<vector<string>>& people) {

        m = req_skills.size();
        n = people.size();
        FULL = (1 << m) - 1;

        unordered_map<string, int> skillId;

        for (int i = 0; i < m; i++)
            skillId[req_skills[i]] = i; // mapping skills to a unique id

        personMask.resize(n);

        for (int i = 0; i < n; i++) {
            int mask = 0;
            for (string &skill : people[i])
                mask |= (1 << skillId[skill]);
            personMask[i] = mask;
        }

        dp.resize(1 << m);
        vis.assign(1 << m, false);

        return solve(0);
    }
};

int main(){
    Solution sol;
    return 0;
}
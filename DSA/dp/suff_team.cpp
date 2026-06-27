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
    int n_;
    int m_;
    int FULL_;
    vector<int> personMask_;
    vector<vector<int>> dp_;
    vector<bool> vis_;

    vector<int> solve(int mask) {
        // All skills covered
        if (mask == FULL_)
            return {};

        if (vis_[mask])
            return dp_[mask];

        vis_[mask] = true;

        // Initialize with an impossible large team
        vector<int> best(100);

        // Try taking every person
        for (int i = 0; i < n_; i++) {
            int newMask = mask | personMask_[i];

            // Person adds no new skill
            if (newMask == mask)
                continue;

            vector<int> team = solve(newMask); // team vector for newMask
            team.push_back(i); // pushing because we are taking this person, initially team didn't have this person

            if (team.size() < best.size())
                best = team;
        }

        return dp_[mask] = best;
    }

    vector<int> smallestSufficientTeam(vector<string>& req_skills,
                                       vector<vector<string>>& people) {
        m_ = req_skills.size();
        n_ = people.size();
        FULL_ = (1 << m_) - 1;

        unordered_map<string, int> skillId;

        for (int i = 0; i < m_; i++)
            skillId[req_skills[i]] = i; // mapping skills to a unique id

        personMask_.assign(n_, 0);

        for (int i = 0; i < n_; i++) {
            int mask = 0;
            for (string &skill : people[i])
                mask |= (1 << skillId[skill]);
            personMask_[i] = mask;
        }

        dp_.assign(1 << m_, vector<int>());
        vis_.assign(1 << m_, false);

        return solve(0);
    }
};

int main() {
    Solution sol;
    return 0;
}
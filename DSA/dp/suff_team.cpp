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

class Solution {
public:
    int size;
    int cols;
    int fullMask;
    vector<int> personMasks;
    vector<vector<int>> memo;
    vector<bool> visited;

    vector<int> solve(int mask) {
        // All skills covered
        if (mask == fullMask)
            return {};

        if (visited[mask])
            return memo[mask];

        visited[mask] = true;

        // Initialize with an impossible large team
        vector<int> best(100);

        // Try taking every person
        for (int i = 0; i < size; i++) {
            int newMask = mask | personMasks[i];

            // Person adds no new skill
            if (newMask == mask)
                continue;

            vector<int> team = solve(newMask); // team vector for newMask
            team.push_back(i); // pushing because we are taking this person, initially team didn't have this person

            if (team.size() < best.size())
                best = team;
        }

        return memo[mask] = best;
    }

    vector<int> smallestSufficientTeam(vector<string>& req_skills,
                                       vector<vector<string>>& people) {
        cols = req_skills.size();
        size = people.size();
        fullMask = (1 << cols) - 1;

        unordered_map<string, int> skillId;

        for (int i = 0; i < cols; i++)
            skillId[req_skills[i]] = i; // mapping skills to a unique id

        personMasks.assign(size, 0);

        for (int i = 0; i < size; i++) {
            int mask = 0;
            for (string &skill : people[i])
                mask |= (1 << skillId[skill]);
            personMasks[i] = mask;
        }

        memo.assign(1 << cols, vector<int>());
        visited.assign(1 << cols, false);

        return solve(0);
    }
};

int main() {
    return 0;
}
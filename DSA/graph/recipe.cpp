/**
 * LeetCode 2115 - Find All Possible Recipes from Given Supplies
 *
 * Description:
 * You are given a list of recipes, the ingredients required for each recipe,
 * and a list of initial supplies.
 *
 * A recipe can be prepared only if all of its ingredients are available.
 * Once a recipe is prepared, it can also be used as an ingredient for
 * preparing other recipes.
 *
 * Return all recipes that can eventually be prepared.
 *
 * Approach:
 * 1. Build a graph:
 *      ingredient -> recipes that depend on it.
 * 2. Store the number of missing ingredients (indegree) for each recipe.
 * 3. Start BFS from all initial supplies.
 * 4. Whenever an ingredient/recipe becomes available, reduce the indegree
 *    of dependent recipes.
 * 5. When a recipe's indegree becomes 0, it can be prepared:
 *      - Add it to the answer.
 *      - Push it into the queue so it can act as an ingredient.
 *
 * Time Complexity: O(R + I)
 *     R = number of recipes
 *     I = total number of ingredient entries
 *
 * Space Complexity: O(R + I)
 */


class Solution {
public:
    vector<string> findAllRecipes(vector<string>& recipes,
                                  vector<vector<string>>& ingredients,
                                  vector<string>& supplies) {

        unordered_map<string, vector<string>> graph;
        unordered_map<string, int> indegree;
        unordered_set<string> isRecipe(recipes.begin(), recipes.end());

        // ingredient -> recipes depending on it
        for (int i = 0; i < recipes.size(); i++) {
            indegree[recipes[i]] = ingredients[i].size();

            for (string &ing : ingredients[i]) {
                graph[ing].push_back(recipes[i]);
            }
        }

        queue<string> q;
        for (string &s : supplies)
            q.push(s);

        vector<string> ans;

        while (!q.empty()) {
            string curr = q.front();
            q.pop();

            // If we made a recipe, add it to answer
            if (isRecipe.count(curr))
                ans.push_back(curr);

            // Reduce dependency count of recipes using curr
            for (string &next : graph[curr]) {
                indegree[next]--;

                if (indegree[next] == 0)
                    q.push(next);
            }
        }

        return ans;
    }
};
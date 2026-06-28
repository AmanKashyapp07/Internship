class Solution {
public:
    vector<int> maxOfMins(vector<int>& arr) {
        int n = arr.size();

        vector<int> ans(n + 1, INT_MIN);
        stack<int> st;
        // strictly increasing monotonic stack by value.
        for (int i = 0; i <= n; i++) {

            while (!st.empty() &&
                   (i == n || arr[st.top()] >= arr[i])) { 
                int mid = st.top();
                st.pop();

                int left = st.empty() ? -1 : st.top();
                int right = i;

                int len = right - left - 1; // exlusive boundary
                // mid will be the minimum of the subarray from left+1 to right-1, which has length len
                ans[len] = max(ans[len], arr[mid]);
            }

            if (i < n) {
                st.push(i);
            }
        }

        // suffix maximum
        for (int len = n - 1; len >= 1; len--) {
            ans[len] = max(ans[len], ans[len + 1]);
        }

        vector<int> res;
        for (int len = 1; len <= n; len++) {
            res.push_back(ans[len]);
        }

        return res;
    }
};

// Monotonic Stack Cheat Sheet
//
// Pop While | Stack Order      | Previous          | Next
// ----------|------------------|-------------------|-------------------
// >=        | Strictly Inc (<) | Previous Smaller  | Next Smaller/Equal
// >         | Non-decreasing   | Previous <=       | Next Smaller
// <=        | Strictly Dec (>) | Previous Greater  | Next Greater/Equal
// <         | Non-increasing   | Previous >=       | Next Greater

// Pop equal (=) if you want STRICT previous.
// Keep equal if you want NON-STRICT previous.

// Previous Smaller      -> >=
// Previous Greater      -> <=

//>=  -> remove duplicates
//      Stack becomes strictly increasing

//>   -> keep duplicates
//      Stack becomes non-decreasing
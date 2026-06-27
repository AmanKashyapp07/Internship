class Solution {
public:
    vector<int> maxOfMins(vector<int>& arr) {
        int n = arr.size();

        vector<int> ans(n + 1, INT_MIN);
        stack<int> st;

        for (int i = 0; i <= n; i++) {

            while (!st.empty() &&
                   (i == n || arr[st.top()] >= arr[i])) { 
                int mid = st.top();
                st.pop();

                int left = st.empty() ? -1 : st.top();
                int right = i;

                int len = right - left - 1;

                ans[len] = max(ans[len], arr[mid]);
            }

            st.push(i);
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

// when you want previous smaller element, you need to use >= because we want to pop the elements which are greater than or equal to the current element, so that we can find the previous smaller element for the current element, if we use >, then we will not pop the elements which are equal to the current element, and we will not be able to find the previous smaller element for the current element, so we need to use >=
                // use >= when you want previous smaller element, use > when you want previous smaller or equal element, use < when you want next smaller element, use <= when you want next smaller or equal element

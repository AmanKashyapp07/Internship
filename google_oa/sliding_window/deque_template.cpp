/*
========================================================
                MONOTONIC DEQUE TEMPLATE
========================================================

Pattern:
- Sliding Window
- Range Minimum / Maximum
- DP Optimization

Idea:
Maintain candidate indices in a deque.

Invariant:
1. Indices are inside current window [L, R].
2. Values are monotonic.
3. Front always stores the answer.

--------------------------------------------------------
Algorithm
--------------------------------------------------------

1. Define current window [L, R].
2. Remove expired indices from the front.
3. Remove worse candidates from the back.
4. Insert current index R.
5. Front = answer.

Time  : O(n)
Space : O(n)

========================================================
                RANGE MINIMUM
========================================================
*/

deque<int> dq;

for (int R = 0; R < n; R++)
{
    int L = ...;      // Left boundary of current window

    // Remove expired indices
    while (!dq.empty() && dq.front() < L)
        dq.pop_front();

    // Maintain increasing deque
    while (!dq.empty() && value[dq.back()] >= value[R])
        dq.pop_back();

    dq.push_back(R);

    // Minimum in [L, R]
    int mn = value[dq.front()];

    // Use mn...
}

/*
========================================================
                RANGE MAXIMUM
========================================================

Only one line changes.

while (!dq.empty() && value[dq.back()] <= value[R])
    dq.pop_back();

int mx = value[dq.front()];

========================================================
                WHAT IS "value"?
========================================================

Sliding Window Minimum  -> value = arr
Sliding Window Maximum  -> value = arr

Maximum Subarray Sum II -> value = prefixSum
Shortest Subarray       -> value = prefixSum

DP Optimization         -> value = dp

========================================================
                REMEMBER
========================================================

Minimum:
    Increasing deque
    Pop while value[back] >= value[cur]

Maximum:
    Decreasing deque
    Pop while value[back] <= value[cur]

Front = Current Answer
Back  = Remove Worse Candidates

========================================================
*/
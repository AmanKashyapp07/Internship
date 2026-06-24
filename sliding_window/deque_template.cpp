/**
 * Pattern: Monotonic Deque
 * Category: Sliding Window / Range Queries
 *
 * Maintains the minimum value in the current range [L, R].
 *
 * Invariant:
 * - Indices are inside [L, R]
 * - Values are increasing in the deque
 *
 * Answer:
 * - value[dq.front()] = minimum in [L, R]
 */

deque<int> dq;

for (int R = 0; R < n; R++)
{
    int L = ...; // Left boundary of current range

    // Remove indices outside [L, R]
    while (!dq.empty() && dq.front() < L)
        dq.pop_front();

    // Insert R while maintaining increasing values
    while (!dq.empty() && value[dq.back()] >= value[R])
        dq.pop_back();

    dq.push_back(R);

    // Range minimum in [L, R]
    auto mn = value[dq.front()];

    // Use mn ...
}

// for range maximum : only one line changes while (!dq.empty() && value[dq.back()] <= value[R])
// dq.pop_back();
// auto mx = value[dq.front()];

/**
 * 1. Define current range [L, R]

2. Pop expired indices from front

3. Pop worse candidates from back

4. Push R

5. Answer is at front

Sliding Window Min      -> value = arr

Sliding Window Max      -> value = arr

Maximum Subarray Sum II -> value = prefix sums

DP Optimization         -> value = dp

Front = answer
Back  = cleanup

Minimum -> increasing deque (>=)
Maximum -> decreasing deque (<=)
 */
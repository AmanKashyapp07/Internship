/**
 * ============================================================================
 *                    MONOTONIC STACK - INTERVIEW/OA CHEAT SHEET
 * ============================================================================
 *
 * MONOTONIC STACK = Maintain elements in sorted order inside stack.
 *
 * ----------------------------------------------------------------------------
 * VARIANT 1 : PREVIOUS SMALLER ELEMENT (PSE)
 * ----------------------------------------------------------------------------
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 *     st.pop();
 *
 * pse[i] = st.empty() ? -1 : st.top();
 *
 * Stack Property:
 *     Strictly Increasing
 *
 * Questions:
 *     - Previous Smaller Element
 *     - Sum of Subarray Minimums (one side)
 *     - Histogram
 *     - Max of Minimum for Every Window Size
 *
 * ----------------------------------------------------------------------------
 * VARIANT 2 : PREVIOUS GREATER ELEMENT (PGE)
 * ----------------------------------------------------------------------------
 *
 * while (!st.empty() && arr[st.top()] <= arr[i])
 *     st.pop();
 *
 * pge[i] = st.empty() ? -1 : st.top();
 *
 * Stack Property:
 *     Strictly Decreasing
 *
 * Questions:
 *     - Previous Greater Element
 *     - Sum of Subarray Maximums
 *
 * ----------------------------------------------------------------------------
 * VARIANT 3 : NEXT SMALLER ELEMENT (NSE)
 * ----------------------------------------------------------------------------
 *
 * Traverse Right → Left
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 *     st.pop();
 *
 * nse[i] = st.empty() ? n : st.top();
 *
 * Stack Property:
 *     Strictly Increasing
 *
 * ----------------------------------------------------------------------------
 * VARIANT 4 : NEXT GREATER ELEMENT (NGE)
 * ----------------------------------------------------------------------------
 *
 * Traverse Right → Left
 *
 * while (!st.empty() && arr[st.top()] <= arr[i])
 *     st.pop();
 *
 * nge[i] = st.empty() ? n : st.top();
 *
 * Stack Property:
 *     Strictly Decreasing
 *
 * ============================================================================
 *                 DUPLICATE HANDLING (MOST IMPORTANT)
 * ============================================================================
 *
 * STRICT          : >  or <
 * NON-STRICT      : >= or <=
 *
 * Rule:
 *
 *     If duplicates exist,
 *     one side STRICT
 *     one side NON-STRICT
 *
 * Otherwise duplicate contribution gets counted twice.
 *
 * Example:
 *
 * Sum of Subarray Minimums
 *
 * PSE -> >
 * NSE -> >=
 *
 * OR
 *
 * PSE -> >=
 * NSE -> >
 *
 * Both are correct.
 *
 * Never:
 *
 * PSE -> >
 * NSE -> >
 *
 * Never:
 *
 * PSE -> >=
 * NSE -> >=
 *
 * ============================================================================
 *                 ONE PASS POP FORMULA (VERY IMPORTANT)
 * ============================================================================
 *
 * Histogram Pattern:
 *
 * while (!st.empty() && arr[st.top()] >= arr[i])
 * {
 *      int mid = st.top();
 *      st.pop();
 *
 *      int left  = st.empty() ? -1 : st.top();
 *      int right = i;
 *
 *      // contribution of arr[mid]
 * }
 *
 * Interpretation:
 *
 * left  = Previous Smaller
 * right = Next Smaller
 *
 * width = right - left - 1
 *
 * Used in:
 *
 *     Largest Rectangle Histogram
 *     Max Of Minimums
 *     Sum Of Subarray Minimums
 *     Sum Of Subarray Maximums
 *
 * ============================================================================
 *                CONTRIBUTION TECHNIQUE
 * ============================================================================
 *
 * If:
 *
 * left  = distance to previous boundary
 * right = distance to next boundary
 *
 * Then:
 *
 * Contribution Count
 *
 * count = left * right
 *
 * Because:
 *
 * left choices for start
 * right choices for end
 *
 * Used in:
 *
 *     Sum Of Subarray Minimums
 *     Sum Of Subarray Maximums
 *
 * Formula:
 *
 * contribution = arr[i] * left * right
 *
 * ============================================================================
 *               HISTOGRAM FAMILY
 * ============================================================================
 *
 * Largest Rectangle in Histogram
 *
 * width  = NSE - PSE - 1
 * area   = height * width
 *
 * Pattern:
 *
 * Pop when smaller appears.
 *
 * while(arr[st.top()] >= arr[i])
 *
 * ============================================================================
 *               MAX OF MINIMUM FOR EVERY WINDOW SIZE
 * ============================================================================
 *
 * width = NSE - PSE - 1
 *
 * Element becomes minimum for window size:
 *
 * len = width
 *
 * ans[len] = max(ans[len], arr[i]);
 *
 * Final:
 *
 * suffix maximum
 *
 * ============================================================================
 *               SUM OF SUBARRAY MINIMUMS
 * ============================================================================
 *
 * Previous Smaller Strict
 * Next Smaller Equal
 *
 * left  = i - pse[i]
 * right = nse[i] - i
 *
 * contribution
 *
 * arr[i] * left * right
 *
 * ============================================================================
 *               SUM OF SUBARRAY MAXIMUMS
 * ============================================================================
 *
 * Previous Greater Strict
 * Next Greater Equal
 *
 * left  = i - pge[i]
 * right = nge[i] - i
 *
 * contribution
 *
 * arr[i] * left * right
 *
 * ============================================================================
 *               NEXT GREATER ELEMENT (CLASSIC)
 * ============================================================================
 *
 * Traverse Right -> Left
 *
 * while(arr[st.top()] <= arr[i])
 *      st.pop();
 *
 * Answer:
 *
 * st.top()
 *
 * Questions:
 *
 *     Next Greater Element I
 *     Next Greater Element II
 *     Daily Temperatures
 *
 * ============================================================================
 *               DAILY TEMPERATURES
 * ============================================================================
 *
 * Maintain decreasing stack.
 *
 * Pop smaller temperatures.
 *
 * Remaining top:
 *
 * Next greater temperature.
 *
 * answer[i] = st.top() - i
 *
 * ============================================================================
 *               STOCK SPAN
 * ============================================================================
 *
 * Maintain decreasing stack.
 *
 * Pop:
 *
 * arr[st.top()] <= arr[i]
 *
 * Span:
 *
 * i - previous_greater_index
 *
 * ============================================================================
 *               TRAPPING RAIN WATER
 * ============================================================================
 *
 * Monotonic Decreasing Stack
 *
 * Whenever larger element arrives:
 *
 * Pop valley.
 *
 * Compute:
 *
 * width  = current - leftBoundary - 1
 *
 * height =
 * min(leftWall,rightWall) - valleyHeight
 *
 * ============================================================================
 *               MONOTONIC DEQUE (SLIDING WINDOW)
 * ============================================================================
 *
 * NOT STACK.
 *
 * Used for:
 *
 * Sliding Window Maximum
 * Sliding Window Minimum
 *
 * Maximum:
 *
 * while(arr[dq.back()] <= arr[i])
 *      dq.pop_back();
 *
 * Minimum:
 *
 * while(arr[dq.back()] >= arr[i])
 *      dq.pop_back();
 *
 * ============================================================================
 *               RECOGNITION PATTERNS
 * ============================================================================
 *
 * Keywords:
 *
 * 1. Nearest Greater
 * 2. Nearest Smaller
 * 3. First Greater on Left
 * 4. First Greater on Right
 * 5. First Smaller on Left
 * 6. First Smaller on Right
 * 7. Span
 * 8. Contribution
 * 9. Histogram
 * 10. Window Minimum/Maximum
 * 11. Range Influence
 * 12. Element Controls Interval
 *
 * => Think Monotonic Stack
 *
 * ============================================================================
 *               MASTER MEMORY TRICK
 * ============================================================================
 *
 * MINIMUM PROBLEM
 *
 *     pop >=
 *
 * MAXIMUM PROBLEM
 *
 *     pop <=
 *
 * DUPLICATES
 *
 *     One side strict
 *     One side non-strict
 *
 * CONTRIBUTION
 *
 *     left * right
 *
 * HISTOGRAM
 *
 *     width = NSE - PSE - 1
 *
 * ============================================================================
 */


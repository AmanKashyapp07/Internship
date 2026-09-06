# Microsoft OA Prep — High-Yield Consolidated Master List

> [!NOTE]
> **Load Reduction Notice:** Filtered down from 166 problems to **124 high-yield problems** by pruning **42 low-ROI entries** (10 non-DSA SQL/Shell queries + 32 trivial/no-brainer Easy questions such as reverse string, duplicate concatenation, basic Fibonacci, etc.). All core algorithmic patterns, tricky edge cases, and Microsoft-specific frequencies are preserved.

---

## Summary Statistics

| Dataset / Filter | Total Retained | Easy (High-Yield) | Medium | Hard |
| :--- | :---: | :---: | :---: | :---: |
| **Tier 1 (Last 30 Days)** | **39** | 9 | 27 | 3 |
| **Tier 2 Delta (30–90 Days)** | **85** | 5 | 57 | 23 |
| **Consolidated High-Yield Total** | **124** | **14** | **84** | **26** |

> [!TIP]
> **Summer Internship Strategy:** For internship OAs, focus 80% of your time on **Tier 1 (39 problems)** and **Tier 2 Mediums**. Hard questions occur far less frequently on intern assessments than on senior/full-time loops.

---

## Table of Contents

- [Intervals, Sorting & Greedy](#intervals-sorting--greedy-10-problems--5-tier-1-5-tier-2) (10)
- [Two Pointers & Sliding Window](#two-pointers--sliding-window-12-problems--6-tier-1-6-tier-2) (12)
- [Arrays, Prefix Sums & Hashing](#arrays-prefix-sums--hashing-12-problems--6-tier-1-6-tier-2) (12)
- [Matrices & Grids](#matrices--grids-7-problems--4-tier-1-3-tier-2) (7)
- [Strings & String Parsing](#strings--string-parsing-9-problems--5-tier-1-4-tier-2) (9)
- [Binary Search & Search on Answer](#binary-search--search-on-answer-7-problems--2-tier-1-5-tier-2) (7)
- [Linked Lists & Fast/Slow Pointers](#linked-lists--fastslow-pointers-10-problems--4-tier-1-6-tier-2) (10)
- [Linked Lists & Cache Design](#linked-lists--cache-design-2-problems--1-tier-1-1-tier-2) (2)
- [Trees & Binary Search Trees](#trees--binary-search-trees-10-problems--3-tier-1-7-tier-2) (10)
- [Graphs & Union-Find](#graphs--union-find-6-problems--1-tier-1-5-tier-2) (6)
- [Stacks, Queues & Monotonic Deque](#stacks-queues--monotonic-deque-12-problems--1-tier-1-11-tier-2) (12)
- [Dynamic Programming & Recursion](#dynamic-programming--recursion-15-problems--3-tier-1-12-tier-2) (15)
- [Backtracking & Combinatorics](#backtracking--combinatorics-5-problems--0-tier-1-5-tier-2) (5)
- [Math & Bit Manipulation](#math--bit-manipulation-7-problems--1-tier-1-6-tier-2) (7)
- [Appendix: Pruned Questions (42)](#appendix-pruned-questions-42-low-roi-entries)

---

## High-Yield Problem Bank

### Intervals, Sorting & Greedy (10 Problems — 5 Tier 1, 5 Tier 2)

- [ ] `[Tier 1]` [31. Next Permutation](https://leetcode.com/problems/next-permutation) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > A permutation of an array of integers is an arrangement of its members into a sequence or linear order.
  > Given an array of integers `nums`, find the next lexicographically greater permutation of its numbers.
  > If no greater permutation is possible (the array is sorted descending), rearrange it into the lowest possible order (ascending).
  > The replacement must be in-place and use only constant extra memory.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** None (in-place modification of `nums`)
  > - **Constraints:** `1 <= nums.length <= 100`, `0 <= nums[i] <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function nextPermutation(nums):
      n = length(nums)
      i = n - 2
      
      // Step 1: Scan right-to-left for the first decreasing element
      while i >= 0 and nums[i] >= nums[i + 1]:
          i = i - 1
          
      // Step 2: If found, find the smallest element to its right greater than nums[i]
      if i >= 0:
          j = n - 1
          while nums[j] <= nums[i]:
              j = j - 1
          swap(nums[i], nums[j])
          
      // Step 3: Reverse the suffix starting at i + 1 to make it ascending
      reverse(nums, start = i + 1, end = n - 1)
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [45. Jump Game II](https://leetcode.com/problems/jump-game-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a 0-indexed array of integers `nums` of length `n`. You are initially positioned at `nums[0]`.
  > Each element `nums[i]` represents the maximum length of a forward jump from index `i`.
  > Return the minimum number of jumps to reach `nums[n - 1]`. Test cases are generated such that you can always reach `nums[n - 1]`.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (minimum jumps)
  > - **Constraints:** `1 <= nums.length <= 10^4`, `0 <= nums[i] <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function jump(nums):
      n = length(nums)
      if n <= 1: return 0
      
      jumps = 0
      curr_end = 0
      farthest = 0
      
      for i from 0 to n - 2:
          farthest = max(farthest, i + nums[i])
          if i == curr_end:
              jumps = jumps + 1
              curr_end = farthest
              if curr_end >= n - 1: break
              
      return jumps
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [56. Merge Intervals](https://leetcode.com/problems/merge-intervals) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of `intervals` where `intervals[i] = [start_i, end_i]`, merge all overlapping intervals,
  > and return an array of the non-overlapping intervals that cover all the intervals in the input.
  >
  > - **Input:** `intervals: List[List[int]]`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= intervals.length <= 10^4`, `intervals[i].length == 2`, `0 <= start_i <= end_i <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function merge(intervals):
      sort intervals by start_time ascending
      merged = empty list
      
      for each interval in intervals:
          if merged is empty or merged.last[1] < interval[0]:
              append interval to merged
          else:
              merged.last[1] = max(merged.last[1], interval[1])
              
      return merged
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [134. Gas Station](https://leetcode.com/problems/gas-station) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > There are `n` gas stations along a circular route, where the amount of gas at the `i-th` station is `gas[i]`.
  > You have a car with an unlimited gas tank and it costs `cost[i]` of gas to travel from the `i-th` station to the `(i + 1)-th` station.
  > You begin the journey with an empty tank at one of the gas stations.
  > Return the starting gas station index if you can travel around the circuit once in the clockwise direction, otherwise return `-1`.
  > If a solution exists, it is guaranteed to be unique.
  >
  > - **Input:** `gas: List[int]`, `cost: List[int]`
  > - **Output:** `int` (starting index or -1)
  > - **Constraints:** `n == gas.length == cost.length`, `1 <= n <= 10^5`, `0 <= gas[i], cost[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function canCompleteCircuit(gas, cost):
      total_tank = 0
      curr_tank = 0
      start_index = 0
      
      for i from 0 to length(gas) - 1:
          diff = gas[i] - cost[i]
          total_tank = total_tank + diff
          curr_tank = curr_tank + diff
          
          // If tank drops negative, station i or earlier cannot be the start
          if curr_tank < 0:
              start_index = i + 1
              curr_tank = 0
              
      if total_tank >= 0: return start_index
      else: return -1
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [475. Heaters](https://leetcode.com/problems/heaters) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Winter is coming! During the contest, your first job is to design a standard horizontal radius of heaters to warm all the houses.
  > Given arrays `houses` and `heaters` positioned along a 1D line, return the minimum radius standard of heaters
  > such that those heaters could cover all houses.
  >
  > - **Input:** `houses: List[int]`, `heaters: List[int]`
  > - **Output:** `int` (minimum radius)
  > - **Constraints:** `1 <= houses.length, heaters.length <= 3 * 10^4`, `1 <= houses[i], heaters[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findRadius(houses, heaters):
      sort heaters ascending
      max_radius = 0
      
      for each house in houses:
          // Binary search heaters for closest heater to house
          idx = binary_search_insertion_point(heaters, house)
          dist_right = (heaters[idx] - house) if idx < length(heaters) else infinity
          dist_left = (house - heaters[idx - 1]) if idx > 0 else infinity
          
          closest_dist = min(dist_left, dist_right)
          max_radius = max(max_radius, closest_dist)
          
      return max_radius
  ```

  **Complexity:**
  - **Time:** $O(M log M + N log M)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 2]` [621. Task Scheduler](https://leetcode.com/problems/task-scheduler) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array of CPU tasks, represented by letters `A` to `Z`, and a cooling interval `n`.
  > Each task can be completed in one CPU interval. Identical tasks must be separated by at least `n` intervals.
  > Return the least number of CPU intervals that the CPU will take to finish all the given tasks.
  >
  > - **Input:** `tasks: List[str]`, `n: int`
  > - **Output:** `int` (minimum time intervals)
  > - **Constraints:** `1 <= tasks.length <= 10^4`, tasks are uppercase English letters, `0 <= n <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function leastInterval(tasks, n):
      count_map = frequency count of each task in tasks
      max_freq = maximum frequency among all tasks
      max_count = count of tasks that have frequency == max_freq
      
      // Schedule the most frequent tasks in frames of size (n + 1)
      slots_needed = (max_freq - 1) * (n + 1) + max_count
      
      // The answer is at least the total number of tasks
      return max(length(tasks), slots_needed)
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) (26 distinct tasks)$
  </details>
- [ ] `[Tier 1]` [763. Partition Labels](https://leetcode.com/problems/partition-labels) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a string `s`. We want to partition the string into as many parts as possible so that each letter appears in at most one part.
  > Return a list of integers representing the size of these parts in order.
  >
  > - **Input:** `s: str`
  > - **Output:** `List[int]` (lengths of each partition)
  > - **Constraints:** `1 <= s.length <= 500`, `s` consists of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function partitionLabels(s):
      last_seen = map storing each character's last index in s
      result = empty list
      
      start = 0
      curr_end = 0
      
      for i from 0 to length(s) - 1:
          curr_end = max(curr_end, last_seen[s[i]])
          if i == curr_end:
              append (curr_end - start + 1) to result
              start = i + 1
              
      return result
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) (26 characters)$
  </details>
- [ ] `[Tier 1]` [1288. Remove Covered Intervals](https://leetcode.com/problems/remove-covered-intervals) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array `intervals` where `intervals[i] = [l_i, r_i]`, remove all intervals covered by another interval in the list.
  > An interval `[a, b)` is covered by interval `[c, d)` if and only if `c <= a` and `b <= d`.
  > Return the number of remaining intervals.
  >
  > - **Input:** `intervals: List[List[int]]`
  > - **Output:** `int` (count of surviving intervals)
  > - **Constraints:** `1 <= intervals.length <= 1000`, `intervals[i].length == 2`, `0 <= l_i < r_i <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function removeCoveredIntervals(intervals):
      // Sort by start ascending; on tie, sort by end descending
      sort intervals with comparator:
          if a[0] != b[0]: return a[0] < b[0]
          else: return a[1] > b[1]
          
      count = 0
      max_end = 0
      
      for each interval in intervals:
          if interval[1] > max_end:
              count = count + 1
              max_end = interval[1]
              
      return count
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 2]` [1353. Maximum Number of Events That Can Be Attended](https://leetcode.com/problems/maximum-number-of-events-that-can-be-attended) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array of `events` where `events[i] = [startDay_i, endDay_i]`.
  > Every event `i` starts at `startDay_i` and ends at `endDay_i`.
  > You can attend at most one event at any day `d`. An event is attended if you attend it on any day between its start and end day.
  > Return the maximum number of events you can attend.
  >
  > - **Input:** `events: List[List[int]]`
  > - **Output:** `int` (maximum events attended)
  > - **Constraints:** `1 <= events.length <= 10^5`, `events[i].length == 2`, `1 <= startDay_i <= endDay_i <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxEvents(events):
      sort events by startDay ascending
      min_heap = empty priority queue (stores endDay of active events)
      
      day = 1
      event_idx = 0
      attended = 0
      n = length(events)
      
      while event_idx < n or min_heap is not empty:
          if min_heap is empty:
              day = events[event_idx][0]
              
          // Push all events that start on current day
          while event_idx < n and events[event_idx][0] <= day:
              push events[event_idx][1] into min_heap
              event_idx = event_idx + 1
              
          // Pop expired events (ended before day)
          while min_heap is not empty and min_heap.top() < day:
              pop min_heap
              
          // Greedily attend the event ending earliest
          if min_heap is not empty:
              pop min_heap
              attended = attended + 1
              day = day + 1
              
      return attended
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [3946. Maximum Number of Items From Sale I](https://leetcode.com/problems/maximum-number-of-items-from-sale-i) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array of items with regular prices and conditional discount rates, alongside a total budget `B`.
  > Determine the maximum number of items that can be purchased within budget `B`.
  >
  > - **Input:** `items: List[Item]`, `budget: int`
  > - **Output:** `int` (maximum items acquired)
  > - **Constraints:** `1 <= items.length <= 1000`, `1 <= budget <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxItemsFromSale(items, budget):
      // Compute effective discounted price for each item
      effective_costs = []
      for each item in items:
          effective_costs.append(item.discounted_price)
          
      sort effective_costs ascending
      count = 0
      
      for each cost in effective_costs:
          if budget >= cost:
              budget = budget - cost
              count = count + 1
          else: break
          
      return count
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N)$
  </details>

### Two Pointers & Sliding Window (12 Problems — 6 Tier 1, 6 Tier 2)

- [ ] `[Tier 1]` [3. Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s`, find the length of the longest substring without duplicate characters.
  >
  > - **Input:** `s: str`
  > - **Output:** `int` (maximum length)
  > - **Constraints:** `0 <= s.length <= 5 * 10^4`, `s` consists of English letters, digits, symbols and spaces.

  **Pseudocode (Language-Agnostic):**
  ```text
  function lengthOfLongestSubstring(s):
      last_seen = empty hash map (char -> last index)
      left = 0
      max_len = 0
      
      for right from 0 to length(s) - 1:
          c = s[right]
          if c in last_seen and last_seen[c] >= left:
              left = last_seen[c] + 1
              
          last_seen[c] = right
          max_len = max(max_len, right - left + 1)
          
      return max_len
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(min(N, Σ))$
  </details>
- [ ] `[Tier 1]` [11. Container With Most Water](https://leetcode.com/problems/container-with-most-water) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an integer array `height` of length `n`. There are `n` vertical lines drawn such that the two endpoints of the `i-th` line are `(i, 0)` and `(i, height[i])`.
  > Find two lines that together with the x-axis form a container, such that the container contains the most water.
  > Return the maximum amount of water a container can store.
  >
  > - **Input:** `height: List[int]`
  > - **Output:** `int` (maximum water capacity)
  > - **Constraints:** `n == height.length`, `2 <= n <= 10^5`, `0 <= height[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxArea(height):
      left = 0
      right = length(height) - 1
      max_water = 0
      
      while left < right:
          h = min(height[left], height[right])
          w = right - left
          max_water = max(max_water, h * w)
          
          // Result is bottlenecked by the shorter line — advance it
          if height[left] < height[right]:
              left = left + 1
          else:
              right = right - 1
              
      return max_water
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [15. 3Sum](https://leetcode.com/problems/3sum) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, return all the triplets `[nums[i], nums[j], nums[k]]` such that `i != j`, `i != k`, and `j != k`, and `nums[i] + nums[j] + nums[k] == 0`.
  > Notice that the solution set must not contain duplicate triplets.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `3 <= nums.length <= 3000`, `-10^5 <= nums[i] <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function threeSum(nums):
      sort nums ascending
      n = length(nums)
      result = empty list
      
      for i from 0 to n - 3:
          // Skip duplicate values for first element
          if i > 0 and nums[i] == nums[i - 1]: continue
          if nums[i] > 0: break // Cannot sum to 0 with all positive values
          
          left = i + 1
          right = n - 1
          while left < right:
              total = nums[i] + nums[left] + nums[right]
              if total == 0:
                  append [nums[i], nums[left], nums[right]] to result
                  while left < right and nums[left] == nums[left + 1]: left = left + 1
                  while left < right and nums[right] == nums[right - 1]: right = right - 1
                  left = left + 1
                  right = right - 1
              elif total < 0:
                  left = left + 1
              else:
                  right = right - 1
                  
      return result
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 2]` [18. 4Sum](https://leetcode.com/problems/4sum) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array `nums` of `n` integers and an integer `target`, return an array of all unique quadruplets `[nums[a], nums[b], nums[c], nums[d]]`
  > such that `nums[a] + nums[b] + nums[c] + nums[d] == target`.
  >
  > - **Input:** `nums: List[int]`, `target: int`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= nums.length <= 200`, `-10^9 <= nums[i], target <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function fourSum(nums, target):
      sort nums ascending
      n = length(nums)
      result = empty list
      
      for i from 0 to n - 4:
          if i > 0 and nums[i] == nums[i - 1]: continue
          for j from i + 1 to n - 3:
              if j > i + 1 and nums[j] == nums[j - 1]: continue
              
              left = j + 1
              right = n - 1
              while left < right:
                  sum4 = 64bit(nums[i]) + nums[j] + nums[left] + nums[right]
                  if sum4 == target:
                      append [nums[i], nums[j], nums[left], nums[right]] to result
                      while left < right and nums[left] == nums[left + 1]: left = left + 1
                      while left < right and nums[right] == nums[right - 1]: right = right - 1
                      left = left + 1
                      right = right - 1
                  elif sum4 < target: left = left + 1
                  else: right = right - 1
                  
      return result
  ```

  **Complexity:**
  - **Time:** $O(N^3)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 1]` [42. Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water) — **Hard** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given `n` non-negative integers representing an elevation map where the width of each bar is `1`, compute how much water it can trap after raining.
  >
  > - **Input:** `height: List[int]`
  > - **Output:** `int` (total trapped water units)
  > - **Constraints:** `n == height.length`, `1 <= n <= 2 * 10^4`, `0 <= height[i] <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function trap(height):
      left = 0
      right = length(height) - 1
      left_max = 0
      right_max = 0
      trapped_water = 0
      
      while left < right:
          if height[left] < height[right]:
              if height[left] >= left_max:
                  left_max = height[left]
              else:
                  trapped_water = trapped_water + (left_max - height[left])
              left = left + 1
          else:
              if height[right] >= right_max:
                  right_max = height[right]
              else:
                  trapped_water = trapped_water + (right_max - height[right])
              right = right - 1
              
      return trapped_water
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [75. Sort Colors](https://leetcode.com/problems/sort-colors) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array `nums` with `n` objects colored red, white, or blue, sort them in-place so that objects of the same color are adjacent,
  > with the colors in the order red (0), white (1), and blue (2).
  > You must solve this problem without using the library's sort function and in one pass with $O(1)$ extra space.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** None (in-place sort)
  > - **Constraints:** `n == nums.length`, `1 <= n <= 300`, `nums[i]` is either `0`, `1`, or `2`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function sortColors(nums):
      low = 0
      mid = 0
      high = length(nums) - 1
      
      while mid <= high:
          if nums[mid] == 0:
              swap(nums[low], nums[mid])
              low = low + 1
              mid = mid + 1
          elif nums[mid] == 1:
              mid = mid + 1
          else: // nums[mid] == 2
              swap(nums[mid], nums[high])
              high = high - 1
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [76. Minimum Window Substring](https://leetcode.com/problems/minimum-window-substring) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two strings `s` and `t` of lengths `m` and `n` respectively, return the minimum window substring of `s`
  > such that every character in `t` (including duplicates) is included in the window.
  > If there is no such substring, return the empty string `""`.
  >
  > - **Input:** `s: str`, `t: str`
  > - **Output:** `str` (minimum substring)
  > - **Constraints:** `m == s.length`, `n == t.length`, `1 <= m, n <= 10^5`, letters are English characters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function minWindow(s, t):
      need = frequency map of characters in t
      window = empty frequency map
      
      have = 0
      required = number of distinct characters in need
      
      best_len = infinity
      best_start = 0
      left = 0
      
      for right from 0 to length(s) - 1:
          c = s[right]
          window[c] = window.get(c, 0) + 1
          if c in need and window[c] == need[c]:
              have = have + 1
              
          // Shrink from left while window condition is satisfied
          while have == required:
              if (right - left + 1) < best_len:
                  best_len = right - left + 1
                  best_start = left
                  
              left_char = s[left]
              window[left_char] = window[left_char] - 1
              if left_char in need and window[left_char] < need[left_char]:
                  have = have - 1
              left = left + 1
              
      if best_len == infinity: return ""
      else: return s.substring(best_start, best_start + best_len)
  ```

  **Complexity:**
  - **Time:** $O(|s| + |t|)$
  - **Space:** $O(Σ)$
  </details>
- [ ] `[Tier 2]` [424. Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a string `s` and an integer `k`. You can choose any character of the string and change it to any other uppercase English character.
  > You can perform this operation at most `k` times.
  > Return the length of the longest substring containing the same letter you can get after performing the operations.
  >
  > - **Input:** `s: str`, `k: int`
  > - **Output:** `int` (maximum length)
  > - **Constraints:** `1 <= s.length <= 10^5`, `s` consists of only uppercase English letters, `0 <= k <= s.length`

  **Pseudocode (Language-Agnostic):**
  ```text
  function characterReplacement(s, k):
      count_map = empty map (char -> frequency)
      left = 0
      max_freq = 0
      max_len = 0
      
      for right from 0 to length(s) - 1:
          count_map[s[right]] = count_map.get(s[right], 0) + 1
          max_freq = max(max_freq, count_map[s[right]])
          
          // Valid window condition: window_size - max_freq <= k
          while (right - left + 1) - max_freq > k:
              count_map[s[left]] = count_map[s[left]] - 1
              left = left + 1
              
          max_len = max(max_len, right - left + 1)
          
      return max_len
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) (26 letters)$
  </details>
- [ ] `[Tier 1]` [438. Find All Anagrams in a String](https://leetcode.com/problems/find-all-anagrams-in-a-string) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two strings `s` and `p`, return an array of all the start indices of `p`'s anagrams in `s`.
  > An Anagram is a word or phrase formed by rearranging the letters of a different word or phrase, typically using all the original letters exactly once.
  >
  > - **Input:** `s: str`, `p: str`
  > - **Output:** `List[int]` (indices)
  > - **Constraints:** `1 <= s.length, p.length <= 3 * 10^4`, `s` and `p` consist of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function findAnagrams(s, p):
      ns = length(s), np = length(p)
      if ns < np: return []
      
      p_count = 26-element array initialized to 0
      s_count = 26-element array initialized to 0
      for char in p: p_count[char - 'a']++
      
      result = empty list
      for i from 0 to ns - 1:
          s_count[s[i] - 'a']++
          
          // Slide left edge once window exceeds length np
          if i >= np:
              s_count[s[i - np] - 'a']--
              
          if s_count == p_count:
              append (i - np + 1) to result
              
      return result
  ```

  **Complexity:**
  - **Time:** $O(|s|)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [567. Permutation in String](https://leetcode.com/problems/permutation-in-string) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two strings `s1` and `s2`, return `true` if `s2` contains a permutation of `s1`, or `false` otherwise.
  > In other words, return true if one of `s1`'s permutations is the substring of `s2`.
  >
  > - **Input:** `s1: str`, `s2: str`
  > - **Output:** `bool`
  > - **Constraints:** `1 <= s1.length, s2.length <= 10^4`, `s1` and `s2` consist of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function checkInclusion(s1, s2):
      n1 = length(s1), n2 = length(s2)
      if n1 > n2: return false
      
      c1 = frequency map of characters in s1
      c2 = frequency map of first n1 characters in s2
      if c1 == c2: return true
      
      for i from n1 to n2 - 1:
          c2[s2[i]]++
          c2[s2[i - n1]]--
          if c2[s2[i - n1]] == 0: remove s2[i - n1] from c2
          if c1 == c2: return true
          
      return false
  ```

  **Complexity:**
  - **Time:** $O(|s2|)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [992. Subarrays with K Different Integers](https://leetcode.com/problems/subarrays-with-k-different-integers) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` and an integer `k`, return the number of good subarrays of `nums`.
  > A good array is an array where the number of different integers in that array is exactly `k`.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `int` (count of subarrays)
  > - **Constraints:** `1 <= nums.length <= 2 * 10^4`, `1 <= nums[i], k <= nums.length`

  **Pseudocode (Language-Agnostic):**
  ```text
  function subarraysWithKDistinct(nums, k):
      // Exact(k) = AtMost(k) - AtMost(k - 1)
      return atMost(nums, k) - atMost(nums, k - 1)
  
  function atMost(nums, k):
      if k == 0: return 0
      freq = empty hash map
      left = 0
      total = 0
      
      for right from 0 to length(nums) - 1:
          freq[nums[right]] = freq.get(nums[right], 0) + 1
          
          while size(freq) > k:
              freq[nums[left]] = freq[nums[left]] - 1
              if freq[nums[left]] == 0: delete freq[nums[left]]
              left = left + 1
              
          total = total + (right - left + 1)
          
      return total
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(K)$
  </details>
- [ ] `[Tier 2]` [1838. Frequency of the Most Frequent Element](https://leetcode.com/problems/frequency-of-the-most-frequent-element) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > The frequency of an element is the number of times it occurs in an array.
  > You are given an integer array `nums` and an integer `k`. In one operation, you can choose an index of `nums` and increment the element by `1`.
  > Return the maximum possible frequency of an element after performing at most `k` operations.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `int` (maximum frequency)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `1 <= nums[i] <= 10^5`, `1 <= k <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxFrequency(nums, k):
      sort nums ascending
      left = 0
      running_sum = 0
      max_freq = 0
      
      for right from 0 to length(nums) - 1:
          running_sum = running_sum + nums[right]
          
          // Operations needed to make all elements in [left..right] equal to nums[right]:
          // cost = nums[right] * window_size - running_sum
          while nums[right] * (right - left + 1) - running_sum > k:
              running_sum = running_sum - nums[left]
              left = left + 1
              
          max_freq = max(max_freq, right - left + 1)
          
      return max_freq
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(1) auxiliary$
  </details>

### Arrays, Prefix Sums & Hashing (12 Problems — 6 Tier 1, 6 Tier 2)

- [ ] `[Tier 1]` [1. Two Sum](https://leetcode.com/problems/two-sum) — **Easy** (Freq: 100.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `nums` and an integer `target`, return indices of the two numbers such that they add up to `target`.
  > You may assume that each input would have exactly one solution, and you may not use the same element twice.
  > You can return the answer in any order.
  >
  > - **Input:** `nums: List[int]`, `target: int`
  > - **Output:** `List[int]` (two indices)
  > - **Constraints:** `2 <= nums.length <= 10^4`, `-10^9 <= nums[i], target <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function twoSum(nums, target):
      seen = empty hash map (value -> index)
      
      for i from 0 to length(nums) - 1:
          complement = target - nums[i]
          if complement in seen:
              return [seen[complement], i]
          seen[nums[i]] = i
          
      return []
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an unsorted array of integers `nums`, return the length of the longest consecutive elements sequence.
  > You must write an algorithm that runs in $O(N)$ time.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (length of longest streak)
  > - **Constraints:** `0 <= nums.length <= 10^5`, `-10^9 <= nums[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function longestConsecutive(nums):
      num_set = set of all elements in nums
      max_streak = 0
      
      for each x in num_set:
          // Only start counting if x is the beginning of a sequence
          if (x - 1) not in num_set:
              curr_num = x
              curr_streak = 1
              
              while (curr_num + 1) in num_set:
                  curr_num = curr_num + 1
                  curr_streak = curr_streak + 1
                  
              max_streak = max(max_streak, curr_streak)
              
      return max_streak
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [169. Majority Element](https://leetcode.com/problems/majority-element) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array `nums` of size `n`, return the majority element.
  > The majority element is the element that appears more than `⌊n / 2⌋` times. You may assume that the majority element always exists in the array.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (majority element)
  > - **Constraints:** `n == nums.length`, `1 <= n <= 5 * 10^4`, `-10^9 <= nums[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function majorityElement(nums):
      candidate = null
      count = 0
      
      // Boyer-Moore Voting Algorithm
      for each num in nums:
          if count == 0:
              candidate = num
          if num == candidate:
              count = count + 1
          else:
              count = count - 1
              
      return candidate
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [229. Majority Element II](https://leetcode.com/problems/majority-element-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array of size `n`, find all elements that appear more than `⌊n / 3⌋` times.
  > Could you solve the problem in linear time and in $O(1)$ space?
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[int]` (elements appearing > n/3 times)
  > - **Constraints:** `1 <= nums.length <= 5 * 10^4`, `-10^9 <= nums[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function majorityElementII(nums):
      // At most 2 elements can have frequency > n/3
      cand1 = null, cand2 = null
      count1 = 0, count2 = 0
      
      // Pass 1: Find candidates
      for each num in nums:
          if cand1 != null and num == cand1: count1++
          elif cand2 != null and num == cand2: count2++
          elif count1 == 0: cand1 = num, count1 = 1
          elif count2 == 0: cand2 = num, count2 = 1
          else: count1--, count2--
          
      // Pass 2: Verify frequency
      actual1 = count of cand1 in nums
      actual2 = count of cand2 in nums
      threshold = floor(length(nums) / 3)
      
      result = empty list
      if actual1 > threshold: append cand1 to result
      if cand2 != cand1 and actual2 > threshold: append cand2 to result
      return result
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [238. Product of Array Except Self](https://leetcode.com/problems/product-of-array-except-self) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, return an array `answer` such that `answer[i]` is equal to the product of all the elements of `nums` except `nums[i]`.
  > The product of any prefix or suffix of `nums` is guaranteed to fit in a 32-bit integer.
  > You must write an algorithm that runs in $O(N)$ time and without using the division operation.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[int]`
  > - **Constraints:** `2 <= nums.length <= 10^5`, `-30 <= nums[i] <= 30`

  **Pseudocode (Language-Agnostic):**
  ```text
  function productExceptSelf(nums):
      n = length(nums)
      answer = array of size n
      
      // Step 1: Compute prefix products
      answer[0] = 1
      for i from 1 to n - 1:
          answer[i] = answer[i - 1] * nums[i - 1]
          
      // Step 2: Multiply with running suffix products
      suffix = 1
      for i from n - 1 down to 0:
          answer[i] = answer[i] * suffix
          suffix = suffix * nums[i]
          
      return answer
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) auxiliary (output array does not count towards extra space)$
  </details>
- [ ] `[Tier 1]` [451. Sort Characters By Frequency](https://leetcode.com/problems/sort-characters-by-frequency) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s`, sort it in decreasing order based on the frequency of the characters.
  > The frequency of a character is the number of times it appears in the string.
  > Return the sorted string. If there are multiple answers, return any of them.
  >
  > - **Input:** `s: str`
  > - **Output:** `str`
  > - **Constraints:** `1 <= s.length <= 5 * 10^5`, `s` consists of uppercase/lowercase letters and digits.

  **Pseudocode (Language-Agnostic):**
  ```text
  function frequencySort(s):
      count_map = frequency count of each character in s
      n = length(s)
      buckets = array of n + 1 empty lists
      
      // Bucket sort by frequency
      for each (char, freq) in count_map:
          append char to buckets[freq]
          
      result = empty string builder
      for freq from n down to 1:
          for each char in buckets[freq]:
              append char repeated freq times to result
              
      return result.toString()
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [525. Contiguous Array](https://leetcode.com/problems/contiguous-array) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a binary array `nums`, return the maximum length of a contiguous subarray with an equal number of `0` and `1`.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (maximum length)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `nums[i]` is either `0` or `1`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function findMaxLength(nums):
      first_seen = empty hash map (prefix_sum -> earliest_index)
      first_seen[0] = -1 // Base case for prefix starting from index 0
      
      prefix_sum = 0
      max_len = 0
      
      for i from 0 to length(nums) - 1:
          // Treat 0 as -1, and 1 as +1
          prefix_sum = prefix_sum + (1 if nums[i] == 1 else -1)
          
          if prefix_sum in first_seen:
              max_len = max(max_len, i - first_seen[prefix_sum])
          else:
              first_seen[prefix_sum] = i
              
      return max_len
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [560. Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `nums` and an integer `k`, return the total number of subarrays whose sum equals to `k`.
  > A subarray is a contiguous non-empty sequence of elements within an array.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `int` (total valid subarrays)
  > - **Constraints:** `1 <= nums.length <= 2 * 10^4`, `-1000 <= nums[i] <= 1000`, `-10^7 <= k <= 10^7`

  **Pseudocode (Language-Agnostic):**
  ```text
  function subarraySum(nums, k):
      prefix_counts = empty hash map (prefix_sum -> frequency)
      prefix_counts[0] = 1 // Base case for prefix == k
      
      curr_sum = 0
      count = 0
      
      for each num in nums:
          curr_sum = curr_sum + num
          target = curr_sum - k
          
          if target in prefix_counts:
              count = count + prefix_counts[target]
              
          prefix_counts[curr_sum] = prefix_counts.get(curr_sum, 0) + 1
          
      return count
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [645. Set Mismatch](https://leetcode.com/problems/set-mismatch) — **Easy** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You have a set of integers `s`, which originally contains all the numbers from `1` to `n`.
  > Unfortunately, due to some error, one of the numbers in `s` got duplicated to another number in the set,
  > which results in repetition of one number and loss of another number.
  > Given an integer array `nums` representing the data status of this set after the error, find and return `[duplicate, missing]`.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[int]` of size 2
  > - **Constraints:** `2 <= nums.length <= 10^4`, `1 <= nums[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findErrorNums(nums):
      dup = -1, missing = -1
      
      // Pass 1: Mark visited indices using sign negation
      for each x in nums:
          val = abs(x)
          if nums[val - 1] < 0:
              dup = val
          else:
              nums[val - 1] = -nums[val - 1]
              
      // Pass 2: The index with positive value is the missing number
      for i from 0 to length(nums) - 1:
          if nums[i] > 0:
              missing = i + 1
              
      return [dup, missing]
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 2]` [2161. Partition Array According to Given Pivot](https://leetcode.com/problems/partition-array-according-to-given-pivot) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a 0-indexed integer array `nums` and an integer `pivot`.
  > Rearrange `nums` such that the following conditions are satisfied:
  > 1. Every element less than `pivot` appears before every element greater than `pivot`.
  > 2. Every element equal to `pivot` appears between elements less than and greater than `pivot`.
  > 3. The relative order of the elements less than `pivot` and elements greater than `pivot` is preserved.
  >
  > - **Input:** `nums: List[int]`, `pivot: int`
  > - **Output:** `List[int]`
  > - **Constraints:** `1 <= nums.length <= 10^5`, `-10^6 <= nums[i], pivot <= 10^6`

  **Pseudocode (Language-Agnostic):**
  ```text
  function pivotArray(nums, pivot):
      n = length(nums)
      result = array of size n
      
      // Count elements strictly smaller and equal to pivot
      less_count = 0, equal_count = 0
      for each x in nums:
          if x < pivot: less_count++
          elif x == pivot: equal_count++
          
      left_ptr = 0
      mid_ptr = less_count
      right_ptr = less_count + equal_count
      
      for each x in nums:
          if x < pivot: result[left_ptr++] = x
          elif x == pivot: result[mid_ptr++] = x
          else: result[right_ptr++] = x
          
      return result
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [3737. Count Subarrays With Majority Element I](https://leetcode.com/problems/count-subarrays-with-majority-element-i) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` and an integer `target`, count the number of subarrays where `target` is the majority element.
  > An element is the majority element of a subarray if it appears strictly more than `half` the length of that subarray (`count > length / 2`).
  >
  > - **Input:** `nums: List[int]`, `target: int`
  > - **Output:** `int` (total valid subarrays)
  > - **Constraints:** `1 <= nums.length <= 1000`, `1 <= nums[i], target <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function countSubarraysWithMajority(nums, target):
      n = length(nums)
      total = 0
      
      for start from 0 to n - 1:
          target_count = 0
          for end from start to n - 1:
              if nums[end] == target:
                  target_count = target_count + 1
              subarray_len = end - start + 1
              if target_count > subarray_len / 2:
                  total = total + 1
                  
      return total
  ```

  **Complexity:**
  - **Time:** $O(N^2) (or O(N log N) with Fenwick Tree)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 2]` [3761. Minimum Absolute Distance Between Mirror Pairs](https://leetcode.com/problems/minimum-absolute-distance-between-mirror-pairs) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, find the minimum absolute index distance `|i - j|` between any pair `(nums[i], nums[j])`
  > such that the string/number representation of `nums[i]` is the exact reverse mirror of `nums[j]`.
  > Return the minimum distance, or `-1` if no mirror pair exists.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (minimum index distance or -1)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `1 <= nums[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function minMirrorPairDistance(nums):
      last_seen = empty hash map (value -> most recent index)
      min_dist = infinity
      
      for i from 0 to length(nums) - 1:
          target_mirror = reverseDigits(nums[i])
          
          if target_mirror in last_seen:
              dist = i - last_seen[target_mirror]
              min_dist = min(min_dist, dist)
              
          last_seen[nums[i]] = i
          
      if min_dist == infinity: return -1
      else: return min_dist
  ```

  **Complexity:**
  - **Time:** $O(N * log10(max_val))$
  - **Space:** $O(N)$
  </details>

### Matrices & Grids (7 Problems — 4 Tier 1, 3 Tier 2)

- [ ] `[Tier 2]` [36. Valid Sudoku](https://leetcode.com/problems/valid-sudoku) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Determine if a `9 x 9` Sudoku board is valid. Only the filled cells need to be validated according to the following rules:
  > 1. Each row must contain the digits `1-9` without repetition.
  > 2. Each column must contain the digits `1-9` without repetition.
  > 3. Each of the nine `3 x 3` sub-boxes of the grid must contain the digits `1-9` without repetition.
  >
  > - **Input:** `board: List[List[str]]` (9x9 grid, empty cells denoted by `'.'`).
  > - **Output:** `bool`
  > - **Constraints:** `board.length == 9`, `board[i].length == 9`, `board[i][j]` is a digit `1-9` or `'.'`. 

  **Pseudocode (Language-Agnostic):**
  ```text
  function isValidSudoku(board):
      seen_row = 9 empty sets
      seen_col = 9 empty sets
      seen_box = 9 empty sets
      
      for r from 0 to 8:
          for c from 0 to 8:
              val = board[r][c]
              if val == '.': continue
              
              box_idx = (r / 3) * 3 + (c / 3)
              if val in seen_row[r] or val in seen_col[c] or val in seen_box[box_idx]:
                  return false
                  
              add val to seen_row[r]
              add val to seen_col[c]
              add val to seen_box[box_idx]
              
      return true
  ```

  **Complexity:**
  - **Time:** $O(1) (fixed 81 cells)$
  - **Space:** $O(1) (fixed sets)$
  </details>
- [ ] `[Tier 1]` [48. Rotate Image](https://leetcode.com/problems/rotate-image) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an `n x n` 2D matrix representing an image, rotate the image by 90 degrees (clockwise).
  > You have to rotate the image in-place, which means you have to modify the input 2D matrix directly.
  > DO NOT allocate another 2D matrix and do the rotation.
  >
  > - **Input:** `matrix: List[List[int]]`
  > - **Output:** None (in-place modification)
  > - **Constraints:** `n == matrix.length == matrix[i].length`, `1 <= n <= 20`, `-1000 <= matrix[i][j] <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function rotate(matrix):
      n = length(matrix)
      
      // Step 1: Transpose the matrix (swap matrix[i][j] with matrix[j][i])
      for i from 0 to n - 1:
          for j from i + 1 to n - 1:
              swap(matrix[i][j], matrix[j][i])
              
      // Step 2: Reverse each row horizontally
      for i from 0 to n - 1:
          reverse(matrix[i])
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [73. Set Matrix Zeroes](https://leetcode.com/problems/set-matrix-zeroes) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an `m x n` integer matrix `matrix`, if an element is `0`, set its entire row and column to `0`'s.
  > You must do it in place with $O(1)$ extra memory.
  >
  > - **Input:** `matrix: List[List[int]]`
  > - **Output:** None (in-place modification)
  > - **Constraints:** `m == matrix.length`, `n == matrix[0].length`, `1 <= m, n <= 200`, `-2^31 <= matrix[i][j] <= 2^31 - 1`

  **Pseudocode (Language-Agnostic):**
  ```text
  function setZeroes(matrix):
      m = length(matrix), n = length(matrix[0])
      first_col_zero = false
      
      // Use first row and first column as zero markers
      for i from 0 to m - 1:
          if matrix[i][0] == 0: first_col_zero = true
          for j from 1 to n - 1:
              if matrix[i][j] == 0:
                  matrix[i][0] = 0
                  matrix[0][j] = 0
                  
      // Fill inner matrix based on markers
      for i from 1 to m - 1:
          for j from 1 to n - 1:
              if matrix[i][0] == 0 or matrix[0][j] == 0:
                  matrix[i][j] = 0
                  
      // Zero out first row if marked
      if matrix[0][0] == 0:
          for j from 0 to n - 1: matrix[0][j] = 0
          
      // Zero out first column if marked
      if first_col_zero:
          for i from 0 to m - 1: matrix[i][0] = 0
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [74. Search a 2D Matrix](https://leetcode.com/problems/search-a-2d-matrix) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an `m x n` integer matrix `matrix` with the following two properties:
  > 1. Each row is sorted in non-decreasing order.
  > 2. The first integer of each row is greater than the last integer of the previous row.
  > Given an integer `target`, return `true` if `target` is in `matrix` or `false` otherwise. Solve in $O(\log(m \cdot n))$ time.
  >
  > - **Input:** `matrix: List[List[int]]`, `target: int`
  > - **Output:** `bool`
  > - **Constraints:** `m == matrix.length`, `n == matrix[i].length`, `1 <= m, n <= 100`, `-10^4 <= matrix[i][j], target <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function searchMatrix(matrix, target):
      m = length(matrix), n = length(matrix[0])
      low = 0
      high = m * n - 1
      
      // Treat 2D matrix as virtual 1D sorted array
      while low <= high:
          mid = low + (high - low) / 2
          row = mid / n
          col = mid % n
          val = matrix[row][col]
          
          if val == target: return true
          elif val < target: low = mid + 1
          else: high = mid - 1
          
      return false
  ```

  **Complexity:**
  - **Time:** $O(log(M * N))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [130. Surrounded Regions](https://leetcode.com/problems/surrounded-regions) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an `m x n` matrix `board` containing `'X'` and `'O'`, capture all regions that are 4-directionally surrounded by `'X'`.
  > A region is captured by flipping all `'O'`s into `'X'`s in that surrounded region.
  > Border `'O'`s and any `'O'`s connected to the border cannot be captured.
  >
  > - **Input:** `board: List[List[str]]`
  > - **Output:** None (in-place modification)
  > - **Constraints:** `m == board.length`, `n == board[i].length`, `1 <= m, n <= 200`, `board[i][j]` is `'X'` or `'O'`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function solve(board):
      m = length(board), n = length(board[0])
      if m <= 2 or n <= 2: return
      
      // Phase 1: Flood fill from boundary 'O's, marking them as safe '#'
      for r from 0 to m - 1:
          dfs_mark_safe(board, r, 0)
          dfs_mark_safe(board, r, n - 1)
      for c from 0 to n - 1:
          dfs_mark_safe(board, 0, c)
          dfs_mark_safe(board, m - 1, c)
          
      // Phase 2: Flip remaining 'O' to 'X' (captured), and '#' back to 'O'
      for r from 0 to m - 1:
          for c from 0 to n - 1:
              if board[r][c] == 'O': board[r][c] = 'X'
              elif board[r][c] == '#': board[r][c] = 'O'
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(M * N) recursion depth$
  </details>
- [ ] `[Tier 2]` [200. Number of Islands](https://leetcode.com/problems/number-of-islands) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an `m x n` 2D binary grid `grid` which represents a map of `'1'`s (land) and `'0'`s (water), return the number of islands.
  > An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.
  > You may assume all four edges of the grid are all surrounded by water.
  >
  > - **Input:** `grid: List[List[str]]`
  > - **Output:** `int` (number of islands)
  > - **Constraints:** `m == grid.length`, `n == grid[i].length`, `1 <= m, n <= 300`, `grid[i][j]` is `'0'` or `'1'`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function numIslands(grid):
      m = length(grid), n = length(grid[0])
      island_count = 0
      
      for r from 0 to m - 1:
          for c from 0 to n - 1:
              if grid[r][c] == '1':
                  island_count = island_count + 1
                  sinkIsland(grid, r, c)
                  
      return island_count
  
  function sinkIsland(grid, r, c):
      if r < 0 or r >= length(grid) or c < 0 or c >= length(grid[0]) or grid[r][c] != '1':
          return
      grid[r][c] = '0' // Sink visited land cell in-place
      sinkIsland(grid, r + 1, c)
      sinkIsland(grid, r - 1, c)
      sinkIsland(grid, r, c + 1)
      sinkIsland(grid, r, c - 1)
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(M * N) recursion stack$
  </details>
- [ ] `[Tier 1]` [994. Rotting Oranges](https://leetcode.com/problems/rotting-oranges) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an `m x n` grid where each cell can have one of three values:
  > - `0` representing an empty cell,
  > - `1` representing a fresh orange, or
  > - `2` representing a rotten orange.
  > Every minute, any fresh orange that is 4-directionally adjacent to a rotten orange becomes rotten.
  > Return the minimum number of minutes that must elapse until no cell has a fresh orange. If impossible, return `-1`.
  >
  > - **Input:** `grid: List[List[int]]`
  > - **Output:** `int` (minutes or -1)
  > - **Constraints:** `m == grid.length`, `n == grid[i].length`, `1 <= m, n <= 10`, `grid[i][j]` is `0`, `1`, or `2`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function orangesRotting(grid):
      m = length(grid), n = length(grid[0])
      queue = empty FIFO queue
      fresh_count = 0
      
      for r from 0 to m - 1:
          for c from 0 to n - 1:
              if grid[r][c] == 2: enqueue (r, c) into queue
              elif grid[r][c] == 1: fresh_count = fresh_count + 1
              
      if fresh_count == 0: return 0
      minutes = -1
      directions = [(1,0), (-1,0), (0,1), (0,-1)]
      
      // Multi-source BFS level-by-level
      while queue is not empty:
          level_size = length(queue)
          minutes = minutes + 1
          for i from 1 to level_size:
              (r, c) = dequeue(queue)
              for (dr, dc) in directions:
                  nr = r + dr, nc = c + dc
                  if 0 <= nr < m and 0 <= nc < n and grid[nr][nc] == 1:
                      grid[nr][nc] = 2
                      fresh_count = fresh_count - 1
                      enqueue (nr, nc) into queue
                      
      if fresh_count == 0: return minutes
      else: return -1
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(M * N)$
  </details>

### Strings & String Parsing (9 Problems — 2 Tier 1, 7 Tier 2)

- [ ] `[Tier 1]` [5. Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring) — **Medium** (Freq: 75.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s`, return the longest palindromic substring in `s`.
  >
  > - **Input:** `s: str`
  > - **Output:** `str` (longest palindrome)
  > - **Constraints:** `1 <= s.length <= 1000`, `s` consists of digits and English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function longestPalindrome(s):
      if length(s) <= 1: return s
      start = 0, max_len = 1
      
      for i from 0 to length(s) - 1:
          // Odd length center around i
          len1 = expandAroundCenter(s, i, i)
          // Even length center between i and i + 1
          len2 = expandAroundCenter(s, i, i + 1)
          
          best = max(len1, len2)
          if best > max_len:
              max_len = best
              start = i - (best - 1) / 2
              
      return s.substring(start, start + max_len)
  
  function expandAroundCenter(s, left, right):
      while left >= 0 and right < length(s) and s[left] == s[right]:
          left = left - 1
          right = right + 1
      return right - left - 1
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [6. Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > The string `"PAYPALISHIRING"` is written in a zigzag pattern on a given number of rows `numRows` like this:
  > P   A   H   N
  > A P L S I I G
  > Y   I   R
  > And then read line by line: `"PAHNAPLSIIGYIR"`. Write code that takes string and row count and returns converted string.
  >
  > - **Input:** `s: str`, `numRows: int`
  > - **Output:** `str`
  > - **Constraints:** `1 <= s.length <= 1000`, `1 <= numRows <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function convert(s, numRows):
      if numRows == 1 or numRows >= length(s): return s
      rows = array of numRows empty string builders
      curr_row = 0
      going_down = false
      
      for each char in s:
          append char to rows[curr_row]
          if curr_row == 0 or curr_row == numRows - 1:
              going_down = not going_down
          curr_row = curr_row + (1 if going_down else -1)
          
      return concatenate all rows
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [12. Integer to Roman](https://leetcode.com/problems/integer-to-roman) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Seven different symbols represent Roman numerals with values: I:1, V:5, X:10, L:50, C:100, D:500, M:1000.
  > Subtractive forms exist for 4, 9, 40, 90, 400, 900. Given an integer `num`, convert it to a Roman numeral.
  >
  > - **Input:** `num: int`
  > - **Output:** `str`
  > - **Constraints:** `1 <= num <= 3999`

  **Pseudocode (Language-Agnostic):**
  ```text
  function intToRoman(num):
      val_symbols = [
          (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
          (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
          (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I")
      ]
      result = empty string builder
      
      for (val, symbol) in val_symbols:
          while num >= val:
              append symbol to result
              num = num - val
              
      return result.toString()
  ```

  **Complexity:**
  - **Time:** $O(1) (bounded by 3999)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [17. Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string containing digits from `2-9` inclusive, return all possible letter combinations that the number could represent.
  > Return the answer in any order. The telephone digit-to-letter mapping matches standard phone keyboards.
  >
  > - **Input:** `digits: str`
  > - **Output:** `List[str]`
  > - **Constraints:** `0 <= digits.length <= 4`, `digits[i]` is in `['2', '9']`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function letterCombinations(digits):
      if digits is empty: return []
      phone_map = {
          '2': "abc", '3': "def", '4': "ghi", '5': "jkl",
          '6': "mno", '7': "pqrs", '8': "tuv", '9': "wxyz"
      }
      result = []
      
      function backtrack(index, current_str):
          if index == length(digits):
              append current_str to result
              return
          for each char in phone_map[digits[index]]:
              backtrack(index + 1, current_str + char)
              
      backtrack(0, "")
      return result
  ```

  **Complexity:**
  - **Time:** $O(4^N)$
  - **Space:** $O(N) recursion stack$
  </details>
- [ ] `[Tier 2]` [20. Valid Parentheses](https://leetcode.com/problems/valid-parentheses) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s` containing just the characters `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`,
  > determine if the input string is valid.
  > Open brackets must be closed by the same type of brackets in the correct order, and every close bracket has a corresponding open bracket.
  >
  > - **Input:** `s: str`
  > - **Output:** `bool`
  > - **Constraints:** `1 <= s.length <= 10^4`, `s` consists of parentheses only.

  **Pseudocode (Language-Agnostic):**
  ```text
  function isValid(s):
      stack = empty stack
      matching = {')': '(', '}': '{', ']': '['}
      
      for each char in s:
          if char in matching:
              if stack is empty or stack.pop() != matching[char]:
                  return false
          else:
              push char onto stack
              
      return stack is empty
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [22. Generate Parentheses](https://leetcode.com/problems/generate-parentheses) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given `n` pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
  >
  > - **Input:** `n: int`
  > - **Output:** `List[str]`
  > - **Constraints:** `1 <= n <= 8`

  **Pseudocode (Language-Agnostic):**
  ```text
  function generateParenthesis(n):
      result = []
      
      function backtrack(current_str, open_count, close_count):
          if length(current_str) == 2 * n:
              append current_str to result
              return
          if open_count < n:
              backtrack(current_str + '(', open_count + 1, close_count)
          if close_count < open_count:
              backtrack(current_str + ')', open_count, close_count + 1)
              
      backtrack("", 0, 0)
      return result
  ```

  **Complexity:**
  - **Time:** $O(4^N / sqrt(N)) (Catalan number)$
  - **Space:** $O(N) recursion stack$
  </details>
- [ ] `[Tier 2]` [30. Substring with Concatenation of All Words](https://leetcode.com/problems/substring-with-concatenation-of-all-words) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a string `s` and an array of strings `words`. All the strings of `words` are of the same length.
  > A concatenated string is a string that exactly contains all the strings of any permutation of `words` concatenated.
  > Return an array of the starting indices of all the concatenated substrings in `s`.
  >
  > - **Input:** `s: str`, `words: List[str]`
  > - **Output:** `List[int]`
  > - **Constraints:** `1 <= s.length <= 10^4`, `1 <= words.length <= 5000`, `1 <= words[i].length <= 30`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findSubstring(s, words):
      word_len = length(words[0])
      num_words = length(words)
      total_len = word_len * num_words
      word_counts = frequency map of words
      result = []
      
      // Slide with word_len different phase offsets
      for offset from 0 to word_len - 1:
          left = offset
          seen = empty map
          matched_words = 0
          
          for right from offset to length(s) - word_len step word_len:
              word = s.substring(right, right + word_len)
              if word in word_counts:
                  seen[word]++
                  matched_words++
                  while seen[word] > word_counts[word]:
                      left_word = s.substring(left, left + word_len)
                      seen[left_word]--
                      matched_words--
                      left = left + word_len
                  if matched_words == num_words:
                      append left to result
              else:
                  clear seen
                  matched_words = 0
                  left = right + word_len
                  
      return result
  ```

  **Complexity:**
  - **Time:** $O(|s| * word_len)$
  - **Space:** $O(num_words * word_len)$
  </details>
- [ ] `[Tier 2]` [49. Group Anagrams](https://leetcode.com/problems/group-anagrams) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of strings `strs`, group the anagrams together. You can return the answer in any order.
  >
  > - **Input:** `strs: List[str]`
  > - **Output:** `List[List[str]]`
  > - **Constraints:** `1 <= strs.length <= 10^4`, `0 <= strs[i].length <= 100`, `strs[i]` consists of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function groupAnagrams(strs):
      anagram_map = empty hash map (canonical_key -> list of strings)
      
      for each s in strs:
          // Key can be sorted string or 26-character frequency tuple
          key = sort_characters(s)
          append s to anagram_map[key]
          
      return all values in anagram_map
  ```

  **Complexity:**
  - **Time:** $O(N * K log K) where K = max word length$
  - **Space:** $O(N * K)$
  </details>
- [ ] `[Tier 1]` [205. Isomorphic Strings](https://leetcode.com/problems/isomorphic-strings) — **Easy** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two strings `s` and `t`, determine if they are isomorphic.
  > Two strings `s` and `t` are isomorphic if the characters in `s` can be replaced to get `t`.
  > All occurrences of a character must be replaced with another character while preserving the order of characters.
  > No two characters may map to the same character, but a character may map to itself.
  >
  > - **Input:** `s: str`, `t: str`
  > - **Output:** `bool`
  > - **Constraints:** `1 <= s.length <= 5 * 10^4`, `t.length == s.length`, `s` and `t` consist of valid ASCII characters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function isIsomorphic(s, t):
      s_to_t = empty hash map
      t_to_s = empty hash map
      
      for i from 0 to length(s) - 1:
          c1 = s[i]
          c2 = t[i]
          
          if c1 in s_to_t and s_to_t[c1] != c2: return false
          if c2 in t_to_s and t_to_s[c2] != c1: return false
          
          s_to_t[c1] = c2
          t_to_s[c2] = c1
          
      return true
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) (ASCII range <= 256)$
  </details>

### Binary Search & Search on Answer (7 Problems — 1 Tier 1, 6 Tier 2)

- [ ] `[Tier 2]` [4. Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two sorted arrays `nums1` and `nums2` of size `m` and `n` respectively, return the median of the two sorted arrays.
  > The overall run time complexity should be $O(\log (m+n))$.
  >
  > - **Input:** `nums1: List[int]`, `nums2: List[int]`
  > - **Output:** `float`
  > - **Constraints:** `nums1.length == m`, `nums2.length == n`, `0 <= m, n <= 1000`, `1 <= m + n <= 2000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findMedianSortedArrays(nums1, nums2):
      if length(nums1) > length(nums2):
          swap(nums1, nums2) // Ensure nums1 is shorter
      m = length(nums1), n = length(nums2)
      low = 0, high = m
      
      while low <= high:
          part1 = (low + high) / 2
          part2 = (m + n + 1) / 2 - part1
          
          max_left1 = nums1[part1 - 1] if part1 > 0 else -infinity
          min_right1 = nums1[part1] if part1 < m else +infinity
          max_left2 = nums2[part2 - 1] if part2 > 0 else -infinity
          min_right2 = nums2[part2] if part2 < n else +infinity
          
          if max_left1 <= min_right2 and max_left2 <= min_right1:
              if (m + n) % 2 == 0:
                  return (max(max_left1, max_left2) + min(min_right1, min_right2)) / 2.0
              else:
                  return max(max_left1, max_left2)
          elif max_left1 > min_right2:
              high = part1 - 1
          else:
              low = part1 + 1
  ```

  **Complexity:**
  - **Time:** $O(log(min(M, N)))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [33. Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > There is an integer array `nums` sorted in ascending order (with distinct values), rotated at an unknown pivot index `k`.
  > Given the rotated array `nums` and integer `target`, return the index of `target` if it is in `nums`, or `-1` if not.
  > Algorithm must run in $O(\log N)$ time.
  >
  > - **Input:** `nums: List[int]`, `target: int`
  > - **Output:** `int` (index or -1)
  > - **Constraints:** `1 <= nums.length <= 5000`, `-10^4 <= nums[i], target <= 10^4`, all values distinct.

  **Pseudocode (Language-Agnostic):**
  ```text
  function search(nums, target):
      low = 0, high = length(nums) - 1
      
      while low <= high:
          mid = low + (high - low) / 2
          if nums[mid] == target: return mid
          
          // Left half is sorted
          if nums[low] <= nums[mid]:
              if nums[low] <= target and target < nums[mid]:
                  high = mid - 1
              else:
                  low = mid + 1
          // Right half is sorted
          else:
              if nums[mid] < target and target <= nums[high]:
                  low = mid + 1
              else:
                  high = mid - 1
                  
      return -1
  ```

  **Complexity:**
  - **Time:** $O(log N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [410. Split Array Largest Sum](https://leetcode.com/problems/split-array-largest-sum) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` and an integer `k`, split `nums` into `k` non-empty subarrays
  > such that the largest sum among any of these subarrays is minimized.
  > Return the minimized largest sum of the split.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `int`
  > - **Constraints:** `1 <= nums.length <= 1000`, `0 <= nums[i] <= 10^6`, `1 <= k <= min(50, nums.length)`

  **Pseudocode (Language-Agnostic):**
  ```text
  function splitArray(nums, k):
      low = max(nums)   // Lower bound: max individual element
      high = sum(nums)  // Upper bound: total sum of all elements
      answer = high
      
      while low <= high:
          mid = low + (high - low) / 2
          if canSplit(nums, k, mid):
              answer = mid
              high = mid - 1 // Try smaller max sum
          else:
              low = mid + 1
              
      return answer
  
  function canSplit(nums, k, max_sum):
      subarrays = 1, current_sum = 0
      for x in nums:
          if current_sum + x > max_sum:
              subarrays++
              current_sum = x
          else:
              current_sum += x
      return subarrays <= k
  ```

  **Complexity:**
  - **Time:** $O(N * log(sum - max))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [540. Single Element in a Sorted Array](https://leetcode.com/problems/single-element-in-a-sorted-array) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a sorted array consisting of only integers where every element appears exactly twice, except for one element which appears exactly once.
  > Return the single element that appears only once in $O(\log N)$ time and $O(1)$ space.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (single unique element)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `0 <= nums[i] <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function singleNonDuplicate(nums):
      low = 0
      high = length(nums) - 1
      
      while low < high:
          mid = low + (high - low) / 2
          // Ensure mid is even to compare pair [mid, mid + 1]
          if mid % 2 == 1: mid = mid - 1
          
          if nums[mid] == nums[mid + 1]:
              // Disruption is in the right half
              low = mid + 2
          else:
              // Disruption is in the left half
              high = mid
              
      return nums[low]
  ```

  **Complexity:**
  - **Time:** $O(log N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [875. Koko Eating Bananas](https://leetcode.com/problems/koko-eating-bananas) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Koko loves to eat bananas. There are `n` piles of bananas, the `i-th` pile has `piles[i]` bananas. The guards will return in `h` hours.
  > Koko can decide her bananas-per-hour eating speed of `k`. Each hour, she chooses a pile and eats `k` bananas from it (if pile has less, she eats it all and doesn't eat more during that hour).
  > Return the minimum integer `k` such that she can eat all the bananas within `h` hours.
  >
  > - **Input:** `piles: List[int]`, `h: int`
  > - **Output:** `int` (minimum speed k)
  > - **Constraints:** `1 <= piles.length <= 10^4`, `piles.length <= h <= 10^9`, `1 <= piles[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function minEatingSpeed(piles, h):
      low = 1
      high = max(piles)
      ans = high
      
      while low <= high:
          mid = low + (high - low) / 2
          hours_needed = 0
          for p in piles:
              hours_needed += ceil(p / mid) // (p + mid - 1) / mid
              
          if hours_needed <= h:
              ans = mid
              high = mid - 1
          else:
              low = mid + 1
              
      return ans
  ```

  **Complexity:**
  - **Time:** $O(N * log(max(piles)))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [1011. Capacity To Ship Packages Within D Days](https://leetcode.com/problems/capacity-to-ship-packages-within-d-days) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > A conveyor belt has packages that must be shipped within `days` days. The `i-th` package has a weight of `weights[i]`.
  > Each day, we load packages on the belt in the order given by `weights`. We cannot load more weight than the maximum weight capacity of the ship.
  > Return the least weight capacity of the ship that will result in all packages being shipped within `days` days.
  >
  > - **Input:** `weights: List[int]`, `days: int`
  > - **Output:** `int` (minimum capacity)
  > - **Constraints:** `1 <= days <= weights.length <= 5 * 10^4`, `1 <= weights[i] <= 500`

  **Pseudocode (Language-Agnostic):**
  ```text
  function shipWithinDays(weights, days):
      low = max(weights)
      high = sum(weights)
      ans = high
      
      while low <= high:
          cap = low + (high - low) / 2
          needed_days = 1
          curr_load = 0
          
          for w in weights:
              if curr_load + w > cap:
                  needed_days++
                  curr_load = w
              else:
                  curr_load += w
                  
          if needed_days <= days:
              ans = cap
              high = cap - 1
          else:
              low = cap + 1
              
      return ans
  ```

  **Complexity:**
  - **Time:** $O(N * log(sum(weights)))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [1283. Find the Smallest Divisor Given a Threshold](https://leetcode.com/problems/find-the-smallest-divisor-given-a-threshold) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `nums` and an integer `threshold`, choose a positive integer `divisor`, divide all array by it, and sum the division results.
  > Each division is rounded up to nearest integer (e.g. 7/3 = 3). Find the smallest `divisor` such that the result is less than or equal to `threshold`.
  >
  > - **Input:** `nums: List[int]`, `threshold: int`
  > - **Output:** `int` (smallest divisor)
  > - **Constraints:** `1 <= nums.length <= 5 * 10^4`, `1 <= nums[i] <= 10^6`, `nums.length <= threshold <= 10^6`

  **Pseudocode (Language-Agnostic):**
  ```text
  function smallestDivisor(nums, threshold):
      low = 1
      high = max(nums)
      ans = high
      
      while low <= high:
          divisor = low + (high - low) / 2
          total_sum = 0
          for x in nums:
              total_sum += (x + divisor - 1) / divisor
              
          if total_sum <= threshold:
              ans = divisor
              high = divisor - 1
          else:
              low = divisor + 1
              
      return ans
  ```

  **Complexity:**
  - **Time:** $O(N * log(max(nums)))$
  - **Space:** $O(1)$
  </details>

### Linked Lists & Fast/Slow Pointers (10 Problems — 5 Tier 1, 5 Tier 2)

- [ ] `[Tier 1]` [2. Add Two Numbers](https://leetcode.com/problems/add-two-numbers) — **Medium** (Freq: 75.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order,
  > and each of their nodes contains a single digit. Add the two numbers and return the sum as a linked list.
  >
  > - **Input:** `l1: ListNode`, `l2: ListNode`
  > - **Output:** `ListNode`
  > - **Constraints:** Number of nodes in each list is in `[1, 100]`, `0 <= Node.val <= 9`, no leading zeros.

  **Pseudocode (Language-Agnostic):**
  ```text
  function addTwoNumbers(l1, l2):
      dummy = new ListNode(0)
      curr = dummy
      carry = 0
      
      while l1 is not null or l2 is not null or carry > 0:
          v1 = l1.val if l1 is not null else 0
          v2 = l2.val if l2 is not null else 0
          sum_val = v1 + v2 + carry
          
          carry = sum_val / 10
          curr.next = new ListNode(sum_val % 10)
          curr = curr.next
          
          if l1 is not null: l1 = l1.next
          if l2 is not null: l2 = l2.next
          
      return dummy.next
  ```

  **Complexity:**
  - **Time:** $O(max(N, M))$
  - **Space:** $O(max(N, M))$
  </details>
- [ ] `[Tier 1]` [21. Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given the heads of two sorted linked lists `list1` and `list2`.
  > Merge the two lists into one sorted list. The list should be made by splicing together the nodes of the first two lists.
  > Return the head of the merged linked list.
  >
  > - **Input:** `list1: ListNode`, `list2: ListNode`
  > - **Output:** `ListNode`
  > - **Constraints:** Number of nodes in both lists is in `[0, 50]`, `-100 <= Node.val <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function mergeTwoLists(list1, list2):
      dummy = new ListNode(-1)
      curr = dummy
      
      while list1 is not null and list2 is not null:
          if list1.val <= list2.val:
              curr.next = list1
              list1 = list1.next
          else:
              curr.next = list2
              list2 = list2.next
          curr = curr.next
          
      curr.next = list1 if list1 is not null else list2
      return dummy.next
  ```

  **Complexity:**
  - **Time:** $O(N + M)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [23. Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array of `k` linked-lists `lists`, each linked-list is sorted in ascending order.
  > Merge all the linked-lists into one sorted linked-list and return it.
  >
  > - **Input:** `lists: List[ListNode]`
  > - **Output:** `ListNode`
  > - **Constraints:** `k == lists.length`, `0 <= k <= 10^4`, `0 <= lists[i].length <= 500`, `-10^4 <= lists[i][j] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function mergeKLists(lists):
      min_heap = empty priority queue (ordered by node.val)
      for each head in lists:
          if head is not null: push head into min_heap
          
      dummy = new ListNode(0)
      curr = dummy
      
      while min_heap is not empty:
          smallest = pop min_heap
          curr.next = smallest
          curr = curr.next
          if smallest.next is not null:
              push smallest.next into min_heap
              
      return dummy.next
  ```

  **Complexity:**
  - **Time:** $O(N log K) where N is total nodes$
  - **Space:** $O(K)$
  </details>
- [ ] `[Tier 1]` [25. Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `head` of a linked list, reverse the nodes of the list `k` at a time, and return the modified list.
  > `k` is a positive integer and is less than or equal to the length of the linked list.
  > If the number of nodes is not a multiple of `k` then left-out nodes, in the end, should remain as it is.
  > You may not alter the values in the list's nodes, only nodes themselves may be changed.
  >
  > - **Input:** `head: ListNode`, `k: int`
  > - **Output:** `ListNode`
  > - **Constraints:** Number of nodes `n` in `[1, 5000]`, `0 <= Node.val <= 1000`, `1 <= k <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function reverseKGroup(head, k):
      // Verify at least k nodes exist
      curr = head
      for i from 1 to k:
          if curr is null: return head
          curr = curr.next
          
      // Reverse first k nodes
      prev = null
      curr = head
      for i from 1 to k:
          nxt = curr.next
          curr.next = prev
          prev = curr
          curr = nxt
          
      // head is now tail of reversed group; connect to recursively reversed rest
      head.next = reverseKGroup(curr, k)
      return prev
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N / K) call stack (or O(1) iterative)$
  </details>
- [ ] `[Tier 2]` [61. Rotate List](https://leetcode.com/problems/rotate-list) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `head` of a linked list, rotate the list to the right by `k` places.
  >
  > - **Input:** `head: ListNode`, `k: int`
  > - **Output:** `ListNode`
  > - **Constraints:** Number of nodes in `[0, 500]`, `-100 <= Node.val <= 100`, `0 <= k <= 2 * 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function rotateRight(head, k):
      if head is null or head.next is null or k == 0: return head
      
      // Step 1: Count length and connect tail to head (form ring)
      length = 1
      tail = head
      while tail.next is not null:
          tail = tail.next
          length = length + 1
      tail.next = head
      
      // Step 2: Find new tail at (length - k % length - 1)
      steps_to_new_tail = length - (k % length) - 1
      new_tail = head
      for i from 1 to steps_to_new_tail:
          new_tail = new_tail.next
          
      new_head = new_tail.next
      new_tail.next = null
      return new_head
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [92. Reverse Linked List II](https://leetcode.com/problems/reverse-linked-list-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `head` of a singly linked list and two integers `left` and `right` where `left <= right`,
  > reverse the nodes of the list from position `left` to position `right`, and return the reversed list in one pass.
  >
  > - **Input:** `head: ListNode`, `left: int`, `right: int`
  > - **Output:** `ListNode`
  > - **Constraints:** Number of nodes in `[1, 500]`, `-500 <= Node.val <= 500`, `1 <= left <= right <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function reverseBetween(head, left, right):
      dummy = new ListNode(0)
      dummy.next = head
      prev = dummy
      
      for i from 1 to left - 1:
          prev = prev.next
          
      curr = prev.next
      // Repeatedly move curr.next to front of sublist
      for i from 1 to right - left:
          forward = curr.next
          curr.next = forward.next
          forward.next = prev.next
          prev.next = forward
          
      return dummy.next
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [138. Copy List with Random Pointer](https://leetcode.com/problems/copy-list-with-random-pointer) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > A linked list of length `n` is given such that each node contains an additional random pointer,
  > which could point to any node in the list, or null. Construct a deep copy of the list.
  >
  > - **Input:** `head: Node` (val, next, random)
  > - **Output:** `Node` (deep copied list head)
  > - **Constraints:** `0 <= n <= 1000`, `-10^4 <= Node.val <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function copyRandomList(head):
      if head is null: return null
      
      // Step 1: Interweave cloned nodes right after originals (A -> A' -> B -> B')
      curr = head
      while curr is not null:
          clone = new Node(curr.val, curr.next, null)
          curr.next = clone
          curr = clone.next
          
      // Step 2: Assign random pointers to cloned nodes
      curr = head
      while curr is not null:
          if curr.random is not null:
              curr.next.random = curr.random.next
          curr = curr.next.next
          
      // Step 3: Decouple the interwoven list
      curr = head
      clone_head = head.next
      while curr is not null:
          clone = curr.next
          curr.next = clone.next
          clone.next = clone.next.next if clone.next is not null else null
          curr = curr.next
          
      return clone_head
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1) auxiliary$
  </details>
- [ ] `[Tier 1]` [141. Linked List Cycle](https://leetcode.com/problems/linked-list-cycle) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given `head`, the head of a linked list, determine if the linked list has a cycle in it.
  > There is a cycle in a linked list if there is some node in the list that can be reached again by continuously following the `next` pointer.
  > Return `true` if there is a cycle in the linked list. Otherwise, return `false`.
  >
  > - **Input:** `head: ListNode`
  > - **Output:** `bool`
  > - **Constraints:** Number of nodes in `[0, 10^4]`, `-10^5 <= Node.val <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function hasCycle(head):
      if head is null or head.next is null: return false
      slow = head
      fast = head
      
      while fast is not null and fast.next is not null:
          slow = slow.next
          fast = fast.next.next
          if slow == fast: return true
          
      return false
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [143. Reorder List](https://leetcode.com/problems/reorder-list) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given the head of a singly linked-list: `L0 -> L1 -> ... -> Ln - 1 -> Ln`.
  > Reorder the list to be on the following form: `L0 -> Ln -> L1 -> Ln - 1 -> L2 -> Ln - 2 -> ...`
  > You may not modify the values in the list's nodes. Only nodes themselves may be changed.
  >
  > - **Input:** `head: ListNode`
  > - **Output:** None (in-place modification)
  > - **Constraints:** Number of nodes in `[1, 5 * 10^4]`, `1 <= Node.val <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function reorderList(head):
      if head is null or head.next is null: return
      
      // Step 1: Find middle using slow and fast pointers
      slow = head, fast = head
      while fast.next is not null and fast.next.next is not null:
          slow = slow.next
          fast = fast.next.next
          
      // Step 2: Reverse second half
      prev = null, curr = slow.next
      slow.next = null // split lists
      while curr is not null:
          nxt = curr.next
          curr.next = prev
          prev = curr
          curr = nxt
      second = prev
      
      // Step 3: Interleave first and second halves
      first = head
      while second is not null:
          tmp1 = first.next
          tmp2 = second.next
          first.next = second
          second.next = tmp1
          first = tmp1
          second = tmp2
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [287. Find the Duplicate Number](https://leetcode.com/problems/find-the-duplicate-number) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `nums` containing `n + 1` integers where each integer is in the range `[1, n]` inclusive.
  > There is only one repeated number in `nums`, return this repeated number.
  > You must solve the problem without modifying the array `nums` and uses only constant extra space.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (duplicate value)
  > - **Constraints:** `1 <= n <= 10^5`, `nums.length == n + 1`, `1 <= nums[i] <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findDuplicate(nums):
      // Floyd's Tortoise and Hare on implicit linked list (i -> nums[i])
      slow = nums[0]
      fast = nums[0]
      
      // Phase 1: Detect intersection
      repeat:
          slow = nums[slow]
          fast = nums[nums[fast]]
      until slow == fast
      
      // Phase 2: Find cycle entrance
      ptr1 = nums[0]
      ptr2 = slow
      while ptr1 != ptr2:
          ptr1 = nums[ptr1]
          ptr2 = nums[ptr2]
          
      return ptr1
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>

### Linked Lists & Cache Design (2 Problems — 1 Tier 1, 1 Tier 2)

- [ ] `[Tier 1]` [146. LRU Cache](https://leetcode.com/problems/lru-cache) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Design a data structure that follows the constraints of a Least Recently Used (LRU) cache.
  > Implement `LRUCache(int capacity)`, `int get(int key)`, and `void put(int key, int value)` in $O(1)$ average time complexity.
  >
  > - **Input:** Operations and arguments: `get(key)`, `put(key, value)`
  > - **Output:** `int` for `get`, None for `put`
  > - **Constraints:** `1 <= capacity <= 3000`, `0 <= key <= 10^4`, `0 <= value <= 10^5`, at most `2 * 10^5` calls.

  **Pseudocode (Language-Agnostic):**
  ```text
  class LRUCache:
      // Uses Doubly Linked List (DLL) + Hash Map
      init(capacity):
          this.capacity = capacity
          this.map = empty hash map (key -> DLLNode)
          this.head = new DLLNode(0, 0) // MRU sentinel
          this.tail = new DLLNode(0, 0) // LRU sentinel
          head.next = tail, tail.prev = head
          
      get(key):
          if key not in map: return -1
          node = map[key]
          moveToHead(node)
          return node.value
          
      put(key, value):
          if key in map:
              node = map[key]
              node.value = value
              moveToHead(node)
          else:
              if size(map) == capacity:
                  lru = tail.prev
                  removeNode(lru)
                  delete map[lru.key]
              new_node = new DLLNode(key, value)
              map[key] = new_node
              addToHead(new_node)
  ```

  **Complexity:**
  - **Time:** $O(1) average for get and put$
  - **Space:** $O(capacity)$
  </details>
- [ ] `[Tier 2]` [460. LFU Cache](https://leetcode.com/problems/lfu-cache) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Design and implement a data structure for a Least Frequently Used (LFU) cache.
  > Implement `LFUCache(int capacity)`, `int get(int key)`, and `void put(int key, int value)` in $O(1)$ runtime complexity.
  > When cache reaches capacity, invalidate the least frequently used key. On a tie, evict the least recently used key among them.
  >
  > - **Input:** `capacity: int`, calls to `get` and `put`
  > - **Output:** `int` for `get`, None for `put`
  > - **Constraints:** `1 <= capacity <= 10^4`, `0 <= key <= 10^5`, `0 <= value <= 10^9`, at most `2 * 10^5` calls.

  **Pseudocode (Language-Agnostic):**
  ```text
  class LFUCache:
      init(capacity):
          this.capacity = capacity
          this.min_freq = 0
          this.key_to_node = hash map (key -> Node)
          this.freq_to_dll = hash map (freq -> DoublyLinkedList)
          
      get(key):
          if key not in key_to_node: return -1
          node = key_to_node[key]
          incrementFrequency(node)
          return node.value
          
      put(key, value):
          if capacity == 0: return
          if key in key_to_node:
              node = key_to_node[key]
              node.value = value
              incrementFrequency(node)
          else:
              if size(key_to_node) == capacity:
                  evict_node = freq_to_dll[min_freq].popTail()
                  delete key_to_node[evict_node.key]
              new_node = new Node(key, value, freq = 1)
              key_to_node[key] = new_node
              freq_to_dll[1].addToHead(new_node)
              min_freq = 1
  ```

  **Complexity:**
  - **Time:** $O(1) for get and put$
  - **Space:** $O(capacity)$
  </details>

### Trees & Binary Search Trees (10 Problems — 3 Tier 1, 7 Tier 2)

- [ ] `[Tier 2]` [105. Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two integer arrays `preorder` and `inorder` where `preorder` is the preorder traversal of a binary tree
  > and `inorder` is the inorder traversal of the same tree, construct and return the binary tree.
  >
  > - **Input:** `preorder: List[int]`, `inorder: List[int]`
  > - **Output:** `TreeNode` (root of reconstructed tree)
  > - **Constraints:** `1 <= preorder.length <= 3000`, `inorder.length == preorder.length`, `-3000 <= preorder[i], inorder[i] <= 3000`, all values are unique.

  **Pseudocode (Language-Agnostic):**
  ```text
  function buildTree(preorder, inorder):
      inorder_map = hash map (value -> index in inorder)
      preorder_idx = 0
      
      function helper(left, right):
          if left > right: return null
          
          root_val = preorder[preorder_idx]
          preorder_idx = preorder_idx + 1
          root = new TreeNode(root_val)
          
          mid = inorder_map[root_val]
          root.left = helper(left, mid - 1)
          root.right = helper(mid + 1, right)
          return root
          
      return helper(0, length(inorder) - 1)
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [110. Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree) — **Easy** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a binary tree, determine if it is height-balanced.
  > A height-balanced binary tree is a binary tree in which the depth of the two subtrees of every node never differs by more than one.
  >
  > - **Input:** `root: TreeNode`
  > - **Output:** `bool`
  > - **Constraints:** Number of nodes in `[0, 5000]`, `-10^4 <= Node.val <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function isBalanced(root):
      return checkHeight(root) != -1
  
  function checkHeight(node):
      if node is null: return 0
      
      left_height = checkHeight(node.left)
      if left_height == -1: return -1
      
      right_height = checkHeight(node.right)
      if right_height == -1: return -1
      
      if abs(left_height - right_height) > 1: return -1
      return 1 + max(left_height, right_height)
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(H) recursion stack$
  </details>
- [ ] `[Tier 2]` [124. Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > A path in a binary tree is a sequence of nodes where each pair of adjacent nodes has an edge connecting them.
  > A node can only appear in the sequence at most once. Note that the path does not need to pass through the root.
  > The path sum of a path is the sum of the node's values in the path.
  > Given the `root` of a binary tree, return the maximum path sum of any non-empty path.
  >
  > - **Input:** `root: TreeNode`
  > - **Output:** `int` (maximum path sum)
  > - **Constraints:** Number of nodes in `[1, 3 * 10^4]`, `-1000 <= Node.val <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxPathSum(root):
      global_max = -infinity
      
      function maxGain(node):
          if node is null: return 0
          
          // Discard negative gains from subtrees by clamping at 0
          left_gain = max(0, maxGain(node.left))
          right_gain = max(0, maxGain(node.right))
          
          // Price of the new path where 'node' is the turning root
          turning_path = node.val + left_gain + right_gain
          global_max = max(global_max, turning_path)
          
          // For parent recursion, only one branch can be continued
          return node.val + max(left_gain, right_gain)
          
      maxGain(root)
      return global_max
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(H) recursion stack$
  </details>
- [ ] `[Tier 1]` [230. Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `root` of a binary search tree, and an integer `k`, return the `k-th` smallest value (1-indexed) of all the values of the nodes in the tree.
  >
  > - **Input:** `root: TreeNode`, `k: int`
  > - **Output:** `int` (k-th smallest value)
  > - **Constraints:** Number of nodes in `[1, 10^4]`, `0 <= Node.val <= 10^4`, `1 <= k <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function kthSmallest(root, k):
      // Inorder traversal of BST yields elements in sorted ascending order
      stack = empty stack
      curr = root
      count = 0
      
      while curr is not null or stack is not empty:
          while curr is not null:
              push curr onto stack
              curr = curr.left
              
          curr = pop stack
          count = count + 1
          if count == k: return curr.val
          
          curr = curr.right
          
      return -1
  ```

  **Complexity:**
  - **Time:** $O(H + K)$
  - **Space:** $O(H)$
  </details>
- [ ] `[Tier 2]` [297. Serialize and Deserialize Binary Tree](https://leetcode.com/problems/serialize-and-deserialize-binary-tree) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Serialization is the process of converting a data structure or object into a sequence of bits so that it can be stored in a file or memory buffer.
  > Design an algorithm to serialize and deserialize a binary tree. There is no restriction on how your serialization/deserialization algorithm should work.
  >
  > - **Input:** `root: TreeNode`
  > - **Output:** `str` for serialize, `TreeNode` for deserialize
  > - **Constraints:** Number of nodes in `[0, 10^4]`, `-1000 <= Node.val <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function serialize(root):
      result = []
      function preorder(node):
          if node is null:
              append "#" to result
              return
          append string(node.val) to result
          preorder(node.left)
          preorder(node.right)
      preorder(root)
      return join(result, ",")
  
  function deserialize(data):
      tokens = split(data, ",")
      token_idx = 0
      
      function build():
          val = tokens[token_idx]
          token_idx = token_idx + 1
          if val == "#": return null
          node = new TreeNode(to_int(val))
          node.left = build()
          node.right = build()
          return node
          
      return build()
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [543. Diameter of Binary Tree](https://leetcode.com/problems/diameter-of-binary-tree) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `root` of a binary tree, return the length of the diameter of the tree.
  > The diameter of a binary tree is the length of the longest path between any two nodes in a tree. This path may or may not pass through the root.
  > The length of a path between two nodes is represented by the number of edges between them.
  >
  > - **Input:** `root: TreeNode`
  > - **Output:** `int` (diameter in edges)
  > - **Constraints:** Number of nodes in `[1, 10^4]`, `-100 <= Node.val <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function diameterOfBinaryTree(root):
      max_diameter = 0
      
      function maxDepth(node):
          if node is null: return 0
          left_depth = maxDepth(node.left)
          right_depth = maxDepth(node.right)
          
          // Longest path through this node has (left_depth + right_depth) edges
          max_diameter = max(max_diameter, left_depth + right_depth)
          
          return 1 + max(left_depth, right_depth)
          
      maxDepth(root)
      return max_diameter
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(H) recursion stack$
  </details>
- [ ] `[Tier 2]` [653. Two Sum IV - Input is a BST](https://leetcode.com/problems/two-sum-iv-input-is-a-bst) — **Easy** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `root` of a binary search tree and an integer `k`, return `true` if there exist two elements in the BST
  > such that their sum is equal to `k`, or `false` otherwise.
  >
  > - **Input:** `root: TreeNode`, `k: int`
  > - **Output:** `bool`
  > - **Constraints:** Number of nodes in `[1, 10^4]`, `-10^4 <= Node.val, k <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findTarget(root, k):
      seen = empty hash set
      
      function dfs(node):
          if node is null: return false
          if (k - node.val) in seen: return true
          add node.val to seen
          return dfs(node.left) or dfs(node.right)
          
      return dfs(root)
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [863. All Nodes Distance K in Binary Tree](https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `root` of a binary tree, the value of a target node `target`, and an integer `k`,
  > return an array of the values of all nodes that have a distance `k` from the target node.
  > You can return the answer in any order.
  >
  > - **Input:** `root: TreeNode`, `target: TreeNode`, `k: int`
  > - **Output:** `List[int]`
  > - **Constraints:** Number of nodes in `[1, 500]`, `0 <= Node.val <= 500`, all values unique, `0 <= k <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function distanceK(root, target, k):
      parent_map = empty hash map (node -> parent)
      
      // Step 1: Annotate parent pointers with BFS/DFS
      queue = [root]
      while queue is not empty:
          curr = dequeue(queue)
          if curr.left is not null:
              parent_map[curr.left] = curr
              enqueue curr.left into queue
          if curr.right is not null:
              parent_map[curr.right] = curr
              enqueue curr.right into queue
              
      // Step 2: Radial BFS starting from target node
      visited = set containing target
      bfs_queue = [target]
      dist = 0
      
      while bfs_queue is not empty:
          if dist == k:
              return list of node.val for node in bfs_queue
          for each node in current level:
              for neighbor in [node.left, node.right, parent_map.get(node)]:
                  if neighbor is not null and neighbor not in visited:
                      add neighbor to visited
                      enqueue neighbor
          dist = dist + 1
          
      return []
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [987. Vertical Order Traversal of a Binary Tree](https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the `root` of a binary tree, calculate the vertical order traversal of the binary tree.
  > For each node at `(row, col)`, its left child is at `(row + 1, col - 1)` and right child is at `(row + 1, col + 1)`.
  > Return the nodes column-by-column from leftmost column to rightmost. If two nodes are at the same `(row, col)`, sort them by value.
  >
  > - **Input:** `root: TreeNode`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** Number of nodes in `[1, 1000]`, `0 <= Node.val <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function verticalTraversal(root):
      nodes = [] // tuples of (col, row, val)
      
      function dfs(node, row, col):
          if node is null: return
          append (col, row, node.val) to nodes
          dfs(node.left, row + 1, col - 1)
          dfs(node.right, row + 1, col + 1)
          
      dfs(root, 0, 0)
      sort nodes ascending: primary by col, secondary by row, tertiary by val
      
      result = []
      group nodes having identical col into individual lists
      return result
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [1932. Merge BSTs to Create Single BST](https://leetcode.com/problems/merge-bsts-to-create-single-bst) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given `n` BST roots with 3 nodes each (a root with optional left and right leaves).
  > Merge all BSTs into a single valid BST by replacing leaf nodes with matching root values.
  > Return the root of the single resulting BST, or `null` if it is impossible to form a valid BST.
  >
  > - **Input:** `trees: List[TreeNode]`
  > - **Output:** `TreeNode` (root or null)
  > - **Constraints:** `1 <= trees.length <= 5 * 10^4`, `1 <= Node.val <= 5 * 10^4`, all roots have distinct values.

  **Pseudocode (Language-Agnostic):**
  ```text
  function canMerge(trees):
      root_map = hash map (root.val -> root TreeNode)
      in_degree = empty hash map
      
      for each tree in trees:
          root_map[tree.val] = tree
          if tree.left is not null: in_degree[tree.left.val]++
          if tree.right is not null: in_degree[tree.right.val]++
          
      // The global root must have in_degree == 0
      global_root = null
      for each tree in trees:
          if in_degree.get(tree.val, 0) == 0:
              if global_root != null: return null // multiple candidates
              global_root = tree
      if global_root == null: return null
      
      // Splicing DFS with BST boundary validation
      function validateAndMerge(node, min_val, max_val):
          if node is null: return true
          if node.val <= min_val or node.val >= max_val: return false
          
          // If node is a leaf and exists as a root in root_map, splice it in
          if node.left is null and node.right is null and node.val in root_map and node != global_root:
              sub = root_map[node.val]
              node.left = sub.left
              node.right = sub.right
              delete root_map[node.val]
              
          return validateAndMerge(node.left, min_val, node.val) and validateAndMerge(node.right, node.val, max_val)
          
      delete root_map[global_root.val]
      if validateAndMerge(global_root, -infinity, +infinity) and size(root_map) == 0:
          return global_root
      return null
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>

### Graphs & Union-Find (6 Problems — 1 Tier 1, 5 Tier 2)

- [ ] `[Tier 2]` [684. Redundant Connection](https://leetcode.com/problems/redundant-connection) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > In this problem, a tree is an undirected graph that is connected and has no cycles.
  > You are given a graph that started as a tree with `n` nodes labeled from `1` to `n`, with one additional edge added.
  > Return an edge that can be removed so that the resulting graph is a tree of `n` nodes.
  > If there are multiple answers, return the answer that occurs last in the input.
  >
  > - **Input:** `edges: List[List[int]]`
  > - **Output:** `List[int]` (the redundant edge `[u, v]`)
  > - **Constraints:** `n == edges.length`, `3 <= n <= 1000`, `edges[i].length == 2`, `1 <= u_i < v_i <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function findRedundantConnection(edges):
      parent = array of size n + 1 where parent[i] = i
      
      function find(i):
          if parent[i] == i: return i
          parent[i] = find(parent[i]) // path compression
          return parent[i]
          
      for each [u, v] in edges:
          root_u = find(u)
          root_v = find(v)
          if root_u == root_v:
              return [u, v] // Adding this edge creates a cycle
          parent[root_u] = root_v
          
      return []
  ```

  **Complexity:**
  - **Time:** $O(N * alpha(N))$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [721. Accounts Merge](https://leetcode.com/problems/accounts-merge) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a list of `accounts` where each element `accounts[i]` is a list of strings, where the first element `accounts[i][0]` is a name,
  > and the rest are emails representing emails of the account.
  > Merge these accounts. Two accounts definitely belong to the same person if there is some common email to both accounts.
  > Return the accounts in the format: name followed by sorted emails.
  >
  > - **Input:** `accounts: List[List[str]]`
  > - **Output:** `List[List[str]]`
  > - **Constraints:** `1 <= accounts.length <= 1000`, `2 <= accounts[i].length <= 10`, `accounts[i][j]` valid email.

  **Pseudocode (Language-Agnostic):**
  ```text
  function accountsMerge(accounts):
      // DSU across unique email strings
      parent = empty hash map (email -> parent_email)
      owner = empty hash map (email -> account_name)
      
      for each account in accounts:
          name = account[0]
          first_email = account[1]
          for i from 1 to length(account) - 1:
              email = account[i]
              owner[email] = name
              union(first_email, email)
              
      // Group emails by root representative
      groups = empty map (root_email -> list of emails)
      for each email in owner:
          root = find(email)
          append email to groups[root]
          
      result = []
      for each (root, email_list) in groups:
          sort email_list
          result.append([owner[root]] + email_list)
          
      return result
  ```

  **Complexity:**
  - **Time:** $O(N * K log(N * K)) where K = max emails per account$
  - **Space:** $O(N * K)$
  </details>
- [ ] `[Tier 2]` [802. Find Eventual Safe States](https://leetcode.com/problems/find-eventual-safe-states) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > There is a directed graph of `n` nodes with each node labeled from `0` to `n - 1`.
  > A node is a terminal node if there are no outgoing edges. A node is a safe node if every possible path starting from that node leads to a terminal node.
  > Return an array containing all the safe nodes of the graph sorted in ascending order.
  >
  > - **Input:** `graph: List[List[int]]`
  > - **Output:** `List[int]` (safe node indices)
  > - **Constraints:** `n == graph.length`, `1 <= n <= 10^4`, `0 <= graph[i].length <= n`

  **Pseudocode (Language-Agnostic):**
  ```text
  function eventualSafeNodes(graph):
      n = length(graph)
      color = array of size n filled with 0
      // 0 = unvisited, 1 = visiting (in recursion stack), 2 = safe
      
      function dfs(node):
          if color[node] > 0:
              return color[node] == 2
          color[node] = 1
          for neighbor in graph[node]:
              if not dfs(neighbor): return false
          color[node] = 2
          return true
          
      result = []
      for i from 0 to n - 1:
          if dfs(i): append i to result
          
      return result
  ```

  **Complexity:**
  - **Time:** $O(V + E)$
  - **Space:** $O(V)$
  </details>
- [ ] `[Tier 2]` [827. Making A Large Island](https://leetcode.com/problems/making-a-large-island) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an `n x n` binary matrix `grid`. You are allowed to change at most one `0` to be `1`.
  > Return the size of the largest island in `grid` after applying this operation.
  > An island is a 4-directionally connected group of `1`s.
  >
  > - **Input:** `grid: List[List[int]]`
  > - **Output:** `int` (maximum island size)
  > - **Constraints:** `n == grid.length == grid[i].length`, `1 <= n <= 500`, `grid[i][j]` is `0` or `1`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function largestIsland(grid):
      n = length(grid)
      island_area = hash map (island_id -> area)
      island_id = 2
      max_area = 0
      
      // Step 1: Label each connected island with unique id >= 2
      for r from 0 to n - 1:
          for c from 0 to n - 1:
              if grid[r][c] == 1:
                  area = dfs_mark(grid, r, c, island_id)
                  island_area[island_id] = area
                  max_area = max(max_area, area)
                  island_id = island_id + 1
                  
      // Step 2: Try flipping each 0 to 1 and sum distinct neighboring islands
      for r from 0 to n - 1:
          for c from 0 to n - 1:
              if grid[r][c] == 0:
                  neighbor_ids = set of neighboring island_ids
                  combined_area = 1
                  for id in neighbor_ids:
                      combined_area += island_area[id]
                  max_area = max(max_area, combined_area)
                  
      return max_area
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(N^2)$
  </details>
- [ ] `[Tier 2]` [2858. Minimum Edge Reversals So Every Node Is Reachable](https://leetcode.com/problems/minimum-edge-reversals-so-every-node-is-reachable) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > There is a directed tree with `n` nodes labeled `0` to `n - 1` and `n - 1` directed edges.
  > Return an array `answer` of size `n` where `answer[i]` is the minimum number of edge reversals required so that all nodes are reachable from node `i`.
  >
  > - **Input:** `n: int`, `edges: List[List[int]]`
  > - **Output:** `List[int]`
  > - **Constraints:** `2 <= n <= 10^5`, `edges.length == n - 1`, `edges[i] == [u_i, v_i]`

  **Pseudocode (Language-Agnostic):**
  ```text
  function minEdgeReversals(n, edges):
      adj = array of size n with pairs (neighbor, reversal_cost)
      for [u, v] in edges:
          adj[u].append((v, 0)) // traversal u -> v has cost 0
          adj[v].append((u, 1)) // traversal v -> u requires 1 reversal
          
      answer = array of size n
      
      // Pass 1: Compute reversals needed if root is node 0
      function dfs1(curr, parent):
          cost = 0
          for (neighbor, weight) in adj[curr]:
              if neighbor != parent:
                  cost += weight + dfs1(neighbor, curr)
          return cost
      answer[0] = dfs1(0, -1)
      
      // Pass 2: Re-rooting DP to transition answer from parent to child in O(1)
      function dfs2(curr, parent):
          for (neighbor, weight) in adj[curr]:
              if neighbor != parent:
                  // If edge was curr -> neighbor (0), making neighbor root requires reversing (neighbor -> curr) (+1)
                  // If edge was neighbor -> curr (1), making neighbor root avoids reversal (-1)
                  answer[neighbor] = answer[curr] + (1 if weight == 0 else -1)
                  dfs2(neighbor, curr)
                  
      dfs2(0, -1)
      return answer
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [3532. Path Existence Queries in a Graph I](https://leetcode.com/problems/path-existence-queries-in-a-graph-i) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an undirected graph with weighted edges and query pairs `[u, v]`, determine whether a valid path exists between `u` and `v`
  > satisfying edge connectivity constraints.
  >
  > - **Input:** `n: int`, `edges: List[List[int]]`, `queries: List[List[int]]`
  > - **Output:** `List[bool]`
  > - **Constraints:** `1 <= n <= 10^5`, `1 <= edges.length, queries.length <= 10^5`

  **Pseudocode (Language-Agnostic):**
  ```text
  function pathExistenceQueries(n, edges, queries):
      parent = array of size n where parent[i] = i
      
      function find(i):
          if parent[i] == i: return i
          parent[i] = find(parent[i])
          return parent[i]
          
      function union(u, v):
          root_u = find(u)
          root_v = find(v)
          if root_u != root_v:
              parent[root_u] = root_v
              
      // Build connected components
      for [u, v] in edges:
          union(u, v)
          
      result = []
      for [u, v] in queries:
          result.append(find(u) == find(v))
          
      return result
  ```

  **Complexity:**
  - **Time:** $O((E + Q) * alpha(V))$
  - **Space:** $O(V)$
  </details>

### Stacks, Queues & Monotonic Deque (12 Problems — 1 Tier 1, 11 Tier 2)

- [ ] `[Tier 2]` [32. Longest Valid Parentheses](https://leetcode.com/problems/longest-valid-parentheses) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string containing just the characters `'('` and `')'`, return the length of the longest valid (well-formed) parentheses substring.
  >
  > - **Input:** `s: str`
  > - **Output:** `int` (maximum length)
  > - **Constraints:** `0 <= s.length <= 3 * 10^4`, `s[i]` is either `'('` or `')'`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function longestValidParentheses(s):
      stack = [-1] // Base index for valid boundary
      max_len = 0
      
      for i from 0 to length(s) - 1:
          if s[i] == '(':
              push i onto stack
          else:
              pop stack
              if stack is empty:
                  push i onto stack // New baseline for next valid substring
              else:
                  max_len = max(max_len, i - stack.top())
                  
      return max_len
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [84. Largest Rectangle in Histogram](https://leetcode.com/problems/largest-rectangle-in-histogram) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `heights` representing the histogram's bar height where the width of each bar is `1`,
  > return the area of the largest rectangle in the histogram.
  >
  > - **Input:** `heights: List[int]`
  > - **Output:** `int` (maximum rectangular area)
  > - **Constraints:** `1 <= heights.length <= 10^5`, `0 <= heights[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function largestRectangleArea(heights):
      stack = empty stack (stores indices of monotonically increasing heights)
      max_area = 0
      n = length(heights)
      
      for i from 0 to n:
          // Append virtual 0-height bar at index n to flush stack
          curr_height = 0 if i == n else heights[i]
          
          while stack is not empty and curr_height < heights[stack.top()]:
              h = heights[pop stack]
              w = i if stack is empty else (i - stack.top() - 1)
              max_area = max(max_area, h * w)
              
          push i onto stack
          
      return max_area
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [85. Maximal Rectangle](https://leetcode.com/problems/maximal-rectangle) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a `rows x cols` binary matrix filled with `0`'s and `1`'s, find the largest rectangle containing only `1`'s and return its area.
  >
  > - **Input:** `matrix: List[List[str]]`
  > - **Output:** `int` (maximum area)
  > - **Constraints:** `rows == matrix.length`, `cols == matrix[i].length`, `1 <= rows, cols <= 200`, `matrix[i][j]` is `'0'` or `'1'`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function maximalRectangle(matrix):
      if matrix is empty: return 0
      cols = length(matrix[0])
      heights = array of size cols filled with 0
      max_area = 0
      
      for each row in matrix:
          for c from 0 to cols - 1:
              if row[c] == '1': heights[c] = heights[c] + 1
              else: heights[c] = 0
              
          // Run LC 84 Largest Rectangle in Histogram on heights
          max_area = max(max_area, largestRectangleArea(heights))
          
      return max_area
  ```

  **Complexity:**
  - **Time:** $O(rows * cols)$
  - **Space:** $O(cols)$
  </details>
- [ ] `[Tier 2]` [155. Min Stack](https://leetcode.com/problems/min-stack) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.
  > Implement `MinStack()`: `void push(val)`, `void pop()`, `int top()`, `int getMin()`.
  > Methods `pop`, `top` and `getMin` operations will always be called on non-empty stacks.
  >
  > - **Input:** Sequence of operations
  > - **Output:** Values from `top()` and `getMin()`
  > - **Constraints:** `-2^31 <= val <= 2^31 - 1`, at most `3 * 10^4` calls.

  **Pseudocode (Language-Agnostic):**
  ```text
  class MinStack:
      init():
          this.main_stack = empty stack
          this.min_stack = empty stack
          
      push(val):
          push val onto main_stack
          current_min = val if min_stack is empty else min(val, min_stack.top())
          push current_min onto min_stack
          
      pop():
          pop main_stack
          pop min_stack
          
      top():
          return main_stack.top()
          
      getMin():
          return min_stack.top()
  ```

  **Complexity:**
  - **Time:** $O(1) for all operations$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [224. Basic Calculator](https://leetcode.com/problems/basic-calculator) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s` representing a valid expression, implement a basic calculator to evaluate it, and return the result of the evaluation.
  > `s` consists of digits, `'+'`, `'-'`, `'('`, `')'`, and `' '`.
  >
  > - **Input:** `s: str`
  > - **Output:** `int` (evaluated result)
  > - **Constraints:** `1 <= s.length <= 3 * 10^5`, valid mathematical expression.

  **Pseudocode (Language-Agnostic):**
  ```text
  function calculate(s):
      stack = empty stack
      res = 0
      curr_num = 0
      sign = 1 // +1 for positive, -1 for negative
      
      for each char in s:
          if isDigit(char):
              curr_num = curr_num * 10 + to_int(char)
          elif char == '+':
              res = res + sign * curr_num
              curr_num = 0
              sign = 1
          elif char == '-':
              res = res + sign * curr_num
              curr_num = 0
              sign = -1
          elif char == '(':
              push res onto stack
              push sign onto stack
              res = 0
              sign = 1
          elif char == ')':
              res = res + sign * curr_num
              curr_num = 0
              prev_sign = pop stack
              prev_res = pop stack
              res = prev_res + prev_sign * res
              
      return res + sign * curr_num
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [239. Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum) — **Hard** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array of integers `nums`, there is a sliding window of size `k` which is moving from the very left to the very right.
  > You can only see the `k` numbers in the window. Each time the sliding window moves right by one position.
  > Return the max sliding window.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `List[int]` (maximums of each window)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `-10^4 <= nums[i] <= 10^4`, `1 <= k <= nums.length`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxSlidingWindow(nums, k):
      deque = empty double-ended queue (stores indices with decreasing values)
      result = []
      
      for i from 0 to length(nums) - 1:
          // 1. Remove indices outside of the current window
          while deque is not empty and deque.front() <= i - k:
              deque.popFront()
              
          // 2. Maintain monotonic decreasing order in deque
          while deque is not empty and nums[deque.back()] <= nums[i]:
              deque.popBack()
              
          deque.pushBack(i)
          
          // 3. Record maximum for valid window
          if i >= k - 1:
              result.append(nums[deque.front()])
              
      return result
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(K)$
  </details>
- [ ] `[Tier 2]` [373. Find K Pairs with Smallest Sums](https://leetcode.com/problems/find-k-pairs-with-smallest-sums) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given two integer arrays `nums1` and `nums2` sorted in ascending order and an integer `k`.
  > Define a pair `(u, v)` which consists of one element from `nums1` and one element from `nums2`.
  > Return the `k` pairs `(u1, v1), (u2, v2), ..., (uk, vk)` with the smallest sums.
  >
  > - **Input:** `nums1: List[int]`, `nums2: List[int]`, `k: int`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= nums1.length, nums2.length <= 10^5`, `-10^9 <= nums1[i], nums2[i] <= 10^9`, `1 <= k <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function kSmallestPairs(nums1, nums2, k):
      min_heap = empty priority queue (stores tuple: (sum, i, j))
      result = []
      
      // Initialize heap with first element of nums2 paired with first min(k, len(nums1)) of nums1
      for i from 0 to min(k, length(nums1)) - 1:
          push (nums1[i] + nums2[0], i, 0) into min_heap
          
      while min_heap is not empty and length(result) < k:
          (sum_val, i, j) = pop min_heap
          append [nums1[i], nums2[j]] to result
          
          if j + 1 < length(nums2):
              push (nums1[i] + nums2[j + 1], i, j + 1) into min_heap
              
      return result
  ```

  **Complexity:**
  - **Time:** $O(K log K)$
  - **Space:** $O(K)$
  </details>
- [ ] `[Tier 2]` [496. Next Greater Element I](https://leetcode.com/problems/next-greater-element-i) — **Easy** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > The next greater element of some element `x` in an array is the first greater element that is to the right of `x` in the same array.
  > You are given two distinct 0-indexed integer arrays `nums1` and `nums2`, where `nums1` is a subset of `nums2`.
  > For each `0 <= i < nums1.length`, find the index `j` such that `nums1[i] == nums2[j]` and determine the next greater element of `nums2[j]` in `nums2`.
  > If there is no next greater element, then the answer for this query is `-1`.
  >
  > - **Input:** `nums1: List[int]`, `nums2: List[int]`
  > - **Output:** `List[int]`
  > - **Constraints:** `1 <= nums1.length <= nums2.length <= 1000`, `0 <= nums1[i], nums2[i] <= 10^4`, all elements distinct.

  **Pseudocode (Language-Agnostic):**
  ```text
  function nextGreaterElement(nums1, nums2):
      next_greater = empty hash map (num -> next_greater_val)
      stack = empty stack (decreasing monotonic)
      
      for x in nums2:
          while stack is not empty and stack.top() < x:
              next_greater[pop stack] = x
          push x onto stack
          
      result = []
      for x in nums1:
          result.append(next_greater.get(x, -1))
          
      return result
  ```

  **Complexity:**
  - **Time:** $O(N1 + N2)$
  - **Space:** $O(N2)$
  </details>
- [ ] `[Tier 2]` [503. Next Greater Element II](https://leetcode.com/problems/next-greater-element-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a circular integer array `nums` (i.e., the next element of `nums[nums.length - 1]` is `nums[0]`),
  > return the next greater number for every element in `nums`.
  > If it doesn't exist, return `-1` for this number.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[int]`
  > - **Constraints:** `1 <= nums.length <= 10^4`, `-10^9 <= nums[i] <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function nextGreaterElements(nums):
      n = length(nums)
      result = array of size n filled with -1
      stack = empty stack (stores indices)
      
      // Loop twice (2 * n) to simulate circular array traversal
      for i from 0 to 2 * n - 1:
          num = nums[i % n]
          while stack is not empty and nums[stack.top()] < num:
              result[pop stack] = num
          if i < n:
              push i onto stack
              
      return result
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [703. Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream) — **Easy** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Design a class to find the `k-th` largest element in a stream.
  > Implement `KthLargest(int k, int[] nums)` and `int add(int val)`.
  > It is guaranteed that there will be at least `k` elements in the array when you search for the `k-th` element.
  >
  > - **Input:** `k: int`, `nums: List[int]`, series of calls to `add(val)`
  > - **Output:** `int` (k-th largest)
  > - **Constraints:** `1 <= k <= 10^4`, `0 <= nums.length <= 10^4`, `-10^4 <= nums[i], val <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  class KthLargest:
      init(k, nums):
          this.k = k
          this.min_heap = empty priority queue
          for x in nums:
              this.add(x)
              
      add(val):
          push val into min_heap
          if size(min_heap) > k:
              pop min_heap
          return min_heap.top()
  ```

  **Complexity:**
  - **Time:** $O(log K) per add operation$
  - **Space:** $O(K)$
  </details>
- [ ] `[Tier 2]` [1475. Final Prices With a Special Discount in a Shop](https://leetcode.com/problems/final-prices-with-a-special-discount-in-a-shop) — **Easy** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an integer array `prices` where `prices[i]` is the price of the `i-th` item in a shop.
  > There is a special discount: you will receive a discount equivalent to `prices[j]` where `j` is the minimum index such that `j > i` and `prices[j] <= prices[i]`.
  > If no such `j` exists, you pay full price. Return an array where the `i-th` element is the final price you will pay.
  >
  > - **Input:** `prices: List[int]`
  > - **Output:** `List[int]`
  > - **Constraints:** `1 <= prices.length <= 500`, `1 <= prices[i] <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function finalPrices(prices):
      stack = empty stack (stores indices)
      ans = copy of prices
      
      for i from 0 to length(prices) - 1:
          while stack is not empty and prices[stack.top()] >= prices[i]:
              prev_idx = pop stack
              ans[prev_idx] = ans[prev_idx] - prices[i]
          push i onto stack
          
      return ans
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [1696. Jump Game VI](https://leetcode.com/problems/jump-game-vi) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given a 0-indexed integer array `nums` and an integer `k`. You are initially standing at index `0`.
  > In one move, you can jump at most `k` steps forward without going outside the boundaries of the array.
  > Your score is the sum of all `nums[j]` for each index `j` you visited in the path. Return the maximum score you can get.
  >
  > - **Input:** `nums: List[int]`, `k: int`
  > - **Output:** `int` (maximum path score)
  > - **Constraints:** `1 <= nums.length, k <= 10^5`, `-10^4 <= nums[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxResult(nums, k):
      n = length(nums)
      dp = array of size n
      dp[0] = nums[0]
      
      // Monotonic decreasing deque storing indices of max dp values in window [i-k, i-1]
      deque = [0]
      
      for i from 1 to n - 1:
          // Evict elements outside sliding window of size k
          while deque is not empty and deque.front() < i - k:
              deque.popFront()
              
          dp[i] = nums[i] + dp[deque.front()]
          
          // Maintain decreasing order in deque
          while deque is not empty and dp[deque.back()] <= dp[i]:
              deque.popBack()
              
          deque.pushBack(i)
          
      return dp[n - 1]
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>

### Dynamic Programming & Recursion (15 Problems — 4 Tier 1, 11 Tier 2)

- [ ] `[Tier 2]` [10. Regular Expression Matching](https://leetcode.com/problems/regular-expression-matching) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an input string `s` and a pattern `p`, implement regular expression matching with support for `'.'` and `'*'` where:
  > - `'.'` Matches any single character.
  > - `'*'` Matches zero or more of the preceding element.
  > The matching should cover the entire input string (not partial).
  >
  > - **Input:** `s: str`, `p: str`
  > - **Output:** `bool`
  > - **Constraints:** `1 <= s.length <= 20`, `1 <= p.length <= 20`, `s` contains only lowercase English letters, `p` contains lowercase English letters, `'.'`, and `'*'`.

  **Pseudocode (Language-Agnostic):**
  ```text
  function isMatch(s, p):
      m = length(s), n = length(p)
      dp = (m + 1) x (n + 1) boolean table initialized to false
      dp[0][0] = true
      
      // Patterns matching empty string (e.g. a*, a*b*)
      for j from 2 to n:
          if p[j - 1] == '*':
              dp[0][j] = dp[0][j - 2]
              
      for i from 1 to m:
          for j from 1 to n:
              if p[j - 1] == '.' or p[j - 1] == s[i - 1]:
                  dp[i][j] = dp[i - 1][j - 1]
              elif p[j - 1] == '*':
                  // Option 1: 0 occurrences of preceding character
                  dp[i][j] = dp[i][j - 2]
                  // Option 2: 1 or more occurrences if preceding matches
                  if p[j - 2] == '.' or p[j - 2] == s[i - 1]:
                      dp[i][j] = dp[i][j] or dp[i - 1][j]
                      
      return dp[m][n]
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(M * N)$
  </details>
- [ ] `[Tier 1]` [53. Maximum Subarray](https://leetcode.com/problems/maximum-subarray) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, find the subarray with the largest sum, and return its sum.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (maximum subarray sum)
  > - **Constraints:** `1 <= nums.length <= 10^5`, `-10^4 <= nums[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxSubArray(nums):
      // Kadane's Algorithm
      max_so_far = nums[0]
      curr_max = nums[0]
      
      for i from 1 to length(nums) - 1:
          curr_max = max(nums[i], curr_max + nums[i])
          max_so_far = max(max_so_far, curr_max)
          
      return max_so_far
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 1]` [62. Unique Paths](https://leetcode.com/problems/unique-paths) — **Medium** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > There is a robot on an `m x n` grid. The robot is initially located at the top-left corner `(0, 0)`.
  > The robot tries to move to the bottom-right corner `(m - 1, n - 1)`. The robot can only move either down or right at any point in time.
  > Given the two integers `m` and `n`, return the number of possible unique paths that the robot can take to reach the bottom-right corner.
  >
  > - **Input:** `m: int`, `n: int`
  > - **Output:** `int` (total unique paths)
  > - **Constraints:** `1 <= m, n <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function uniquePaths(m, n):
      // 1D DP space optimization
      dp = array of size n initialized to 1
      
      for r from 1 to m - 1:
          for c from 1 to n - 1:
              dp[c] = dp[c] + dp[c - 1]
              
      return dp[n - 1]
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [72. Edit Distance](https://leetcode.com/problems/edit-distance) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given two strings `word1` and `word2`, return the minimum number of operations required to convert `word1` to `word2`.
  > You have the following three operations permitted on a word: Insert a character, Delete a character, Replace a character.
  >
  > - **Input:** `word1: str`, `word2: str`
  > - **Output:** `int` (minimum edit distance)
  > - **Constraints:** `0 <= word1.length, word2.length <= 500`, `word1` and `word2` consist of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function minDistance(word1, word2):
      m = length(word1), n = length(word2)
      dp = (m + 1) x (n + 1) table
      
      for i from 0 to m: dp[i][0] = i // delete all characters
      for j from 0 to n: dp[0][j] = j // insert all characters
      
      for i from 1 to m:
          for j from 1 to n:
              if word1[i - 1] == word2[j - 1]:
                  dp[i][j] = dp[i - 1][j - 1]
              else:
                  // Min among Insert, Delete, and Replace operations
                  dp[i][j] = 1 + min(dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1])
                  
      return dp[m][n]
  ```

  **Complexity:**
  - **Time:** $O(M * N)$
  - **Space:** $O(M * N)$
  </details>
- [ ] `[Tier 2]` [120. Triangle](https://leetcode.com/problems/triangle) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a `triangle` array, return the minimum path sum from top to bottom.
  > For each step, you may move to an adjacent number of the row below. More formally, if you are on index `i` on the current row,
  > you may move to either index `i` or index `i + 1` on the next row.
  >
  > - **Input:** `triangle: List[List[int]]`
  > - **Output:** `int` (minimum path sum)
  > - **Constraints:** `1 <= triangle.length <= 200`, `triangle[0].length == 1`, `triangle[i].length == triangle[i - 1].length + 1`, `-10^4 <= triangle[i][j] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function minimumTotal(triangle):
      // Bottom-up DP in-place or 1D array
      n = length(triangle)
      dp = copy of triangle[n - 1]
      
      for r from n - 2 down to 0:
          for c from 0 to r:
              dp[c] = triangle[r][c] + min(dp[c], dp[c + 1])
              
      return dp[0]
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 1]` [121. Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock) — **Easy** (Freq: 62.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an array `prices` where `prices[i]` is the price of a given stock on the `i-th` day.
  > You want to maximize your profit by choosing a single day to buy one stock and choosing a different day in the future to sell that stock.
  > Return the maximum profit you can achieve from this transaction. If you cannot achieve any profit, return `0`.
  >
  > - **Input:** `prices: List[int]`
  > - **Output:** `int` (max profit)
  > - **Constraints:** `1 <= prices.length <= 10^5`, `0 <= prices[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function maxProfit(prices):
      min_price = infinity
      max_profit = 0
      
      for each price in prices:
          if price < min_price:
              min_price = price
          elif price - min_price > max_profit:
              max_profit = price - min_price
              
      return max_profit
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [198. House Robber](https://leetcode.com/problems/house-robber) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed,
  > the only constraint stopping you from robbing each of them is that adjacent houses have security systems connected
  > and it will automatically contact the police if two adjacent houses were broken into on the same night.
  > Given an integer array `nums` representing the amount of money of each house, return the maximum amount of money you can rob tonight without alerting the police.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (maximum loot)
  > - **Constraints:** `1 <= nums.length <= 100`, `0 <= nums[i] <= 400`

  **Pseudocode (Language-Agnostic):**
  ```text
  function rob(nums):
      prev2 = 0 // max money up to 2 houses back
      prev1 = 0 // max money up to 1 house back
      
      for each money in nums:
          curr = max(prev1, prev2 + money)
          prev2 = prev1
          prev1 = curr
          
      return prev1
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [300. Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, return the length of the longest strictly increasing subsequence.
  > Solve in $O(N \log N)$ time complexity.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (length of LIS)
  > - **Constraints:** `1 <= nums.length <= 2500`, `-10^4 <= nums[i] <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function lengthOfLIS(nums):
      tails = [] // tails[i] stores smallest tail of all increasing subsequences of length i + 1
      
      for x in nums:
          idx = binary_search_lower_bound(tails, x)
          if idx == length(tails):
              append x to tails
          else:
              tails[idx] = x
              
      return length(tails)
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [416. Partition Equal Subset Sum](https://leetcode.com/problems/partition-equal-subset-sum) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, return `true` if you can partition the array into two subsets such that the sum of the elements in both subsets is equal or `false` otherwise.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `bool`
  > - **Constraints:** `1 <= nums.length <= 200`, `1 <= nums[i] <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function canPartition(nums):
      total = sum(nums)
      if total % 2 != 0: return false
      target = total / 2
      
      // 0/1 Knapsack subset sum DP
      dp = boolean array of size target + 1 initialized to false
      dp[0] = true
      
      for x in nums:
          for w from target down to x:
              dp[w] = dp[w] or dp[w - x]
              
      return dp[target]
  ```

  **Complexity:**
  - **Time:** $O(N * target)$
  - **Space:** $O(target)$
  </details>
- [ ] `[Tier 2]` [493. Reverse Pairs](https://leetcode.com/problems/reverse-pairs) — **Hard** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums`, return the number of reverse pairs in the array.
  > A reverse pair is a pair `(i, j)` where `0 <= i < j < nums.length` and `nums[i] > 2 * nums[j]`.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (count of reverse pairs)
  > - **Constraints:** `1 <= nums.length <= 5 * 10^4`, `-2^31 <= nums[i] <= 2^31 - 1`

  **Pseudocode (Language-Agnostic):**
  ```text
  function reversePairs(nums):
      return mergeSort(nums, 0, length(nums) - 1)
  
  function mergeSort(nums, left, right):
      if left >= right: return 0
      mid = (left + right) / 2
      count = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right)
      
      // Two pointer counting for nums[i] > 2 * nums[j]
      j = mid + 1
      for i from left to mid:
          while j <= right and 64bit(nums[i]) > 2 * 64bit(nums[j]):
              j = j + 1
          count += (j - (mid + 1))
          
      // Standard merge of two sorted halves
      merge(nums, left, mid, right)
      return count
  ```

  **Complexity:**
  - **Time:** $O(N log N)$
  - **Space:** $O(N) auxiliary$
  </details>
- [ ] `[Tier 2]` [516. Longest Palindromic Subsequence](https://leetcode.com/problems/longest-palindromic-subsequence) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s`, find the longest palindromic subsequence's length in `s`.
  > A subsequence is a sequence that can be derived from another sequence by deleting some or no elements without changing the order of the remaining elements.
  >
  > - **Input:** `s: str`
  > - **Output:** `int` (length of longest palindromic subsequence)
  > - **Constraints:** `1 <= s.length <= 1000`, `s` consists only of lowercase English letters.

  **Pseudocode (Language-Agnostic):**
  ```text
  function longestPalindromeSubseq(s):
      n = length(s)
      dp = n x n table initialized to 0
      
      for i from 0 to n - 1:
          dp[i][i] = 1
          
      for len from 2 to n:
          for i from 0 to n - len:
              j = i + len - 1
              if s[i] == s[j]:
                  dp[i][j] = 2 + (dp[i + 1][j - 1] if len > 2 else 0)
              else:
                  dp[i][j] = max(dp[i + 1][j], dp[i][j - 1])
                  
      return dp[0][n - 1]
  ```

  **Complexity:**
  - **Time:** $O(N^2)$
  - **Space:** $O(N^2)$
  </details>
- [ ] `[Tier 2]` [518. Coin Change II](https://leetcode.com/problems/coin-change-ii) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > You are given an integer array `coins` representing coins of different denominations and an integer `amount` representing a total amount of money.
  > Return the number of combinations that make up that amount. If that amount of money cannot be made up by any combination of the coins, return `0`.
  > You may assume that you have an infinite number of each kind of coin.
  >
  > - **Input:** `amount: int`, `coins: List[int]`
  > - **Output:** `int` (total combinations)
  > - **Constraints:** `1 <= coins.length <= 300`, `1 <= coins[i] <= 5000`, `0 <= amount <= 5000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function change(amount, coins):
      dp = array of size amount + 1 initialized to 0
      dp[0] = 1 // 1 way to make amount 0 (empty selection)
      
      // Outer loop over coins ensures ordered combinations (no permutations)
      for coin in coins:
          for a from coin to amount:
              dp[a] = dp[a] + dp[a - coin]
              
      return dp[amount]
  ```

  **Complexity:**
  - **Time:** $O(N * amount)$
  - **Space:** $O(amount)$
  </details>
- [ ] `[Tier 2]` [526. Beautiful Arrangement](https://leetcode.com/problems/beautiful-arrangement) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Suppose you have `n` integers labeled `1` to `n`. A permutation of these `n` integers `perm` is called a beautiful arrangement if for every `1 <= i <= n`:
  > - `perm[i]` is divisible by `i`, or
  > - `i` is divisible by `perm[i]`.
  > Given an integer `n`, return the number of the beautiful arrangements that you can construct.
  >
  > - **Input:** `n: int`
  > - **Output:** `int` (number of valid arrangements)
  > - **Constraints:** `1 <= n <= 15`

  **Pseudocode (Language-Agnostic):**
  ```text
  function countArrangement(n):
      count = 0
      visited = boolean array of size n + 1 initialized to false
      
      function backtrack(pos):
          if pos > n:
              count = count + 1
              return
          for num from 1 to n:
              if not visited[num] and (num % pos == 0 or pos % num == 0):
                  visited[num] = true
                  backtrack(pos + 1)
                  visited[num] = false
                  
      backtrack(1)
      return count
  ```

  **Complexity:**
  - **Time:** $O(k) where k is valid permutations << N!$
  - **Space:** $O(N) recursion stack$
  </details>
- [ ] `[Tier 1]` [779. K-th Symbol in Grammar](https://leetcode.com/problems/k-th-symbol-in-grammar) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > We build a table of `n` rows (1-indexed). We start by writing `0` in the 1st row.
  > Now in every subsequent row, we look at the previous row and replace each occurrence of `0` with `01`, and each occurrence of `1` with `10`.
  > Given two integers `n` and `k`, return the `k-th` (1-indexed) symbol in the `n-th` row of a table of `n` rows.
  >
  > - **Input:** `n: int`, `k: int`
  > - **Output:** `int` (0 or 1)
  > - **Constraints:** `1 <= n <= 30`, `1 <= k <= 2^(n - 1)`

  **Pseudocode (Language-Agnostic):**
  ```text
  function kthGrammar(n, k):
      if n == 1 and k == 1: return 0
      
      parent_k = (k + 1) / 2
      parent_val = kthGrammar(n - 1, parent_k)
      
      // Odd k is identical to parent; even k is inverted
      if k % 2 == 1: return parent_val
      else: return 1 - parent_val
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N) call stack$
  </details>
- [ ] `[Tier 2]` [3699. Number of ZigZag Arrays I](https://leetcode.com/problems/number-of-zigzag-arrays-i) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Count the number of arrays of length `n` with elements in `[1, m]` that alternate directions (zigzag):
  > `a[0] < a[1] > a[2] < a[3] ...` or `a[0] > a[1] < a[2] > a[3] ...`.
  > Return the answer modulo `10^9 + 7`.
  >
  > - **Input:** `n: int`, `m: int`
  > - **Output:** `int` (count modulo 10^9 + 7)
  > - **Constraints:** `1 <= n <= 1000`, `1 <= m <= 100`

  **Pseudocode (Language-Agnostic):**
  ```text
  function countZigZagArrays(n, m):
      MOD = 10^9 + 7
      // dp_inc[i][v]: zigzag arrays of length i ending in v on an increasing step
      // dp_dec[i][v]: zigzag arrays of length i ending in v on a decreasing step
      dp_inc = (n + 1) x (m + 1) initialized to 0
      dp_dec = (n + 1) x (m + 1) initialized to 0
      
      for v from 1 to m:
          dp_inc[1][v] = 1
          dp_dec[1][v] = 1
          
      for i from 2 to n:
          prefix_dec = 0
          for v from 1 to m:
              dp_inc[i][v] = prefix_dec % MOD
              prefix_dec = (prefix_dec + dp_dec[i - 1][v]) % MOD
              
          suffix_inc = 0
          for v from m down to 1:
              dp_dec[i][v] = suffix_inc % MOD
              suffix_inc = (suffix_inc + dp_inc[i - 1][v]) % MOD
              
      total = sum(dp_inc[n][v] + dp_dec[n][v] for v in 1..m) % MOD
      return total
  ```

  **Complexity:**
  - **Time:** $O(N * M)$
  - **Space:** $O(N * M) (or O(M) with rolling arrays)$
  </details>

### Backtracking & Combinatorics (5 Problems — 0 Tier 1, 5 Tier 2)

- [ ] `[Tier 2]` [39. Combination Sum](https://leetcode.com/problems/combination-sum) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of distinct integers `candidates` and a target integer `target`, return a list of all unique combinations of `candidates` where the chosen numbers sum to `target`.
  > You may return the combinations in any order. The same number may be chosen from `candidates` an unlimited number of times.
  >
  > - **Input:** `candidates: List[int]`, `target: int`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= candidates.length <= 30`, `2 <= candidates[i] <= 40`, `1 <= target <= 40`, all elements distinct.

  **Pseudocode (Language-Agnostic):**
  ```text
  function combinationSum(candidates, target):
      sort candidates ascending
      result = []
      
      function backtrack(start_idx, current_combo, remaining):
          if remaining == 0:
              append copy(current_combo) to result
              return
          for i from start_idx to length(candidates) - 1:
              if candidates[i] > remaining: break // Prune search branch
              append candidates[i] to current_combo
              backtrack(i, current_combo, remaining - candidates[i]) // allow reuse of same index i
              pop current_combo
              
      backtrack(0, [], target)
      return result
  ```

  **Complexity:**
  - **Time:** $O(2^target)$
  - **Space:** $O(target / min(candidates)) recursion depth$
  </details>
- [ ] `[Tier 2]` [51. N-Queens](https://leetcode.com/problems/n-queens) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > The n-queens puzzle is the problem of placing `n` queens on an `n x n` chessboard such that no two queens attack each other.
  > Given an integer `n`, return all distinct solutions to the n-queens puzzle. You may return the answer in any order.
  > Each solution contains a distinct board configuration of the n-queens' placement, where `'Q'` and `'.'` both indicate a queen and an empty space.
  >
  > - **Input:** `n: int`
  > - **Output:** `List[List[str]]`
  > - **Constraints:** `1 <= n <= 9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function solveNQueens(n):
      result = []
      cols = set(), diag1 = set() // r - c, diag2 = set() // r + c
      board = n x n board initialized with '.'
      
      function backtrack(row):
          if row == n:
              append copy_as_strings(board) to result
              return
          for col from 0 to n - 1:
              d1 = row - col, d2 = row + col
              if col in cols or d1 in diag1 or d2 in diag2: continue
              
              board[row][col] = 'Q'
              add col to cols, add d1 to diag1, add d2 to diag2
              backtrack(row + 1)
              remove col from cols, remove d1 from diag1, remove d2 from diag2
              board[row][col] = '.'
              
      backtrack(0)
      return result
  ```

  **Complexity:**
  - **Time:** $O(N!)$
  - **Space:** $O(N) sets and recursion stack$
  </details>
- [ ] `[Tier 2]` [78. Subsets](https://leetcode.com/problems/subsets) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` of unique elements, return all possible subsets (the power set).
  > The solution set must not contain duplicate subsets. Return the solution in any order.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= nums.length <= 10`, `-10 <= nums[i] <= 10`, all numbers unique.

  **Pseudocode (Language-Agnostic):**
  ```text
  function subsets(nums):
      result = []
      
      function backtrack(start_idx, current_subset):
          append copy(current_subset) to result
          for i from start_idx to length(nums) - 1:
              append nums[i] to current_subset
              backtrack(i + 1, current_subset)
              pop current_subset
              
      backtrack(0, [])
      return result
  ```

  **Complexity:**
  - **Time:** $O(N * 2^N)$
  - **Space:** $O(N) recursion depth$
  </details>
- [ ] `[Tier 2]` [90. Subsets II](https://leetcode.com/problems/subsets-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` that may contain duplicates, return all possible subsets (the power set).
  > The solution set must not contain duplicate subsets. Return the solution in any order.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `List[List[int]]`
  > - **Constraints:** `1 <= nums.length <= 10`, `-10 <= nums[i] <= 10`

  **Pseudocode (Language-Agnostic):**
  ```text
  function subsetsWithDup(nums):
      sort nums ascending
      result = []
      
      function backtrack(start_idx, current_subset):
          append copy(current_subset) to result
          for i from start_idx to length(nums) - 1:
              // Skip duplicate choices at the same recursion tree level
              if i > start_idx and nums[i] == nums[i - 1]: continue
              append nums[i] to current_subset
              backtrack(i + 1, current_subset)
              pop current_subset
              
      backtrack(0, [])
      return result
  ```

  **Complexity:**
  - **Time:** $O(N * 2^N)$
  - **Space:** $O(N) recursion stack$
  </details>
- [ ] `[Tier 2]` [140. Word Break II](https://leetcode.com/problems/word-break-ii) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a string `s` and a dictionary of strings `wordDict`, add spaces in `s` to construct a sentence where each word is a valid dictionary word.
  > Return all such possible sentences in any order. The same word in the dictionary may be reused multiple times in the segmentation.
  >
  > - **Input:** `s: str`, `wordDict: List[str]`
  > - **Output:** `List[str]`
  > - **Constraints:** `1 <= s.length <= 20`, `1 <= wordDict.length <= 1000`, `1 <= wordDict[i].length <= 10`

  **Pseudocode (Language-Agnostic):**
  ```text
  function wordBreak(s, wordDict):
      word_set = set of wordDict
      memo = empty hash map (substring -> list of segmented sentences)
      
      function dfs(remaining_str):
          if remaining_str in memo: return memo[remaining_str]
          if remaining_str is empty: return [""]
          
          sentences = []
          for word in word_set:
              if remaining_str starts with word:
                  sub_results = dfs(remaining_str.substring(length(word)))
                  for sub in sub_results:
                      space = " " if sub != "" else ""
                      sentences.append(word + space + sub)
                      
          memo[remaining_str] = sentences
          return sentences
          
      return dfs(s)
  ```

  **Complexity:**
  - **Time:** $O(2^N)$
  - **Space:** $O(2^N)$
  </details>

### Math & Bit Manipulation (7 Problems — 0 Tier 1, 7 Tier 2)

- [ ] `[Tier 2]` [7. Reverse Integer](https://leetcode.com/problems/reverse-integer) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given a signed 32-bit integer `x`, return `x` with its digits reversed.
  > If reversing `x` causes the value to go outside the signed 32-bit integer range `[-2^31, 2^31 - 1]`, then return `0`.
  > Assume the environment does not allow you to store 64-bit integers.
  >
  > - **Input:** `x: int`
  > - **Output:** `int` (reversed integer or 0)
  > - **Constraints:** `-2^31 <= x <= 2^31 - 1`

  **Pseudocode (Language-Agnostic):**
  ```text
  function reverse(x):
      INT_MAX = 2147483647, INT_MIN = -2147483648
      rev = 0
      
      while x != 0:
          pop = x % 10 // remainder
          x = x / 10
          
          // Guard against overflow before multiplying by 10
          if rev > INT_MAX / 10 or (rev == INT_MAX / 10 and pop > 7): return 0
          if rev < INT_MIN / 10 or (rev == INT_MIN / 10 and pop < -8): return 0
          
          rev = rev * 10 + pop
          
      return rev
  ```

  **Complexity:**
  - **Time:** $O(log10(|x|))$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [50. Pow(x, n)](https://leetcode.com/problems/powx-n) — **Medium** (Freq: 50.0%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Implement `pow(x, n)`, which calculates `x` raised to the power `n` (`x^n`).
  >
  > - **Input:** `x: float`, `n: int`
  > - **Output:** `float`
  > - **Constraints:** `-100.0 < x < 100.0`, `-2^31 <= n <= 2^31 - 1`, `n` is an integer, `-10^4 <= x^n <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function myPow(x, n):
      exp = 64bit(n)
      if exp < 0:
          x = 1.0 / x
          exp = -exp
          
      ans = 1.0
      curr_product = x
      
      // Binary exponentiation
      while exp > 0:
          if exp % 2 == 1:
              ans = ans * curr_product
          curr_product = curr_product * curr_product
          exp = exp / 2
          
      return ans
  ```

  **Complexity:**
  - **Time:** $O(log N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [137. Single Number II](https://leetcode.com/problems/single-number-ii) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer array `nums` where every element appears three times except for one, which appears exactly once.
  > Find the single element and return it. You must implement a solution with linear runtime and constant extra space.
  >
  > - **Input:** `nums: List[int]`
  > - **Output:** `int` (single unique element)
  > - **Constraints:** `1 <= nums.length <= 3 * 10^4`, `-2^31 <= nums[i] <= 2^31 - 1`

  **Pseudocode (Language-Agnostic):**
  ```text
  function singleNumber(nums):
      ones = 0
      twos = 0
      
      // Bitwise state machine: track modulo-3 occurrence of each bit
      for x in nums:
          ones = (ones ^ x) & (~twos)
          twos = (twos ^ x) & (~ones)
          
      return ones
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [204. Count Primes](https://leetcode.com/problems/count-primes) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer `n`, return the number of prime numbers that are strictly less than `n`.
  >
  > - **Input:** `n: int`
  > - **Output:** `int` (count of primes)
  > - **Constraints:** `0 <= n <= 5 * 10^6`

  **Pseudocode (Language-Agnostic):**
  ```text
  function countPrimes(n):
      if n <= 2: return 0
      is_prime = boolean array of size n initialized to true
      is_prime[0] = false, is_prime[1] = false
      
      // Sieve of Eratosthenes
      p = 2
      while p * p < n:
          if is_prime[p]:
              for multiple from p * p to n - 1 step p:
                  is_prime[multiple] = false
          p = p + 1
          
      return count of true values in is_prime
  ```

  **Complexity:**
  - **Time:** $O(N log(log N))$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [223. Rectangle Area](https://leetcode.com/problems/rectangle-area) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given the coordinates of two rectilinear rectangles in a 2D plane, return the total area covered by the two rectangles.
  > Rectangle 1 is defined by bottom-left `(ax1, ay1)` and top-right `(ax2, ay2)`.
  > Rectangle 2 is defined by bottom-left `(bx1, by1)` and top-right `(bx2, by2)`.
  >
  > - **Input:** `ax1, ay1, ax2, ay2, bx1, by1, bx2, by2: int`
  > - **Output:** `int` (total covered area)
  > - **Constraints:** `-10^4 <= all coordinates <= 10^4`

  **Pseudocode (Language-Agnostic):**
  ```text
  function computeArea(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2):
      areaA = (ax2 - ax1) * (ay2 - ay1)
      areaB = (bx2 - bx1) * (by2 - by1)
      
      // Calculate overlapping rectangle boundaries
      overlap_x = max(0, min(ax2, bx2) - max(ax1, bx1))
      overlap_y = max(0, min(ay2, by2) - max(ay1, by1))
      overlap_area = overlap_x * overlap_y
      
      return areaA + areaB - overlap_area
  ```

  **Complexity:**
  - **Time:** $O(1)$
  - **Space:** $O(1)$
  </details>
- [ ] `[Tier 2]` [274. H-Index](https://leetcode.com/problems/h-index) — **Medium** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an array of integers `citations` where `citations[i]` is the number of citations a researcher received for their `i-th` paper,
  > return the researcher's h-index.
  > The h-index is defined as the maximum value of `h` such that the given author has published at least `h` papers that have each been cited at least `h` times.
  >
  > - **Input:** `citations: List[int]`
  > - **Output:** `int` (h-index)
  > - **Constraints:** `1 <= citations.length <= 5000`, `0 <= citations[i] <= 1000`

  **Pseudocode (Language-Agnostic):**
  ```text
  function hIndex(citations):
      n = length(citations)
      // Counting sort bucket array of size n + 1
      buckets = array of size n + 1 filled with 0
      
      for c in citations:
          if c >= n: buckets[n]++
          else: buckets[c]++
          
      count = 0
      for h from n down to 0:
          count += buckets[h]
          if count >= h:
              return h
              
      return 0
  ```

  **Complexity:**
  - **Time:** $O(N)$
  - **Space:** $O(N)$
  </details>
- [ ] `[Tier 2]` [829. Consecutive Numbers Sum](https://leetcode.com/problems/consecutive-numbers-sum) — **Hard** (Freq: 37.5%)
  <details>
  <summary>Problem Statement & Pseudocode</summary>

  **Problem Statement:**
  > Given an integer `n`, return the number of ways you can write `n` as the sum of consecutive positive integers.
  >
  > - **Input:** `n: int`
  > - **Output:** `int` (number of ways)
  > - **Constraints:** `1 <= n <= 10^9`

  **Pseudocode (Language-Agnostic):**
  ```text
  function consecutiveNumbersSum(n):
      // Sum of k consecutive integers starting from x:
      // k * x + k * (k - 1) / 2 = n ==> k * x = n - k * (k - 1) / 2
      ways = 0
      k = 1
      
      while k * (k - 1) / 2 < n:
          remainder = n - k * (k - 1) / 2
          if remainder % k == 0:
              ways = ways + 1
          k = k + 1
          
      return ways
  ```

  **Complexity:**
  - **Time:** $O(sqrt(N))$
  - **Space:** $O(1)$
  </details>

---

## Appendix: Pruned Questions (42 Low-ROI Entries)

<details>
<summary>Click to view all 42 removed questions and why they were pruned</summary>

### Non-DSA Questions (10 Removed — SQL & Shell)
These appear in generic data/database roles, not Microsoft Software Engineering Online Assessments:

- [175. Combine Two Tables](https://leetcode.com/problems/combine-two-tables) (Easy) — *Reason: SQL / Shell task*
- [176. Second Highest Salary](https://leetcode.com/problems/second-highest-salary) (Medium) — *Reason: SQL / Shell task*
- [181. Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers) (Easy) — *Reason: SQL / Shell task*
- [193. Valid Phone Numbers](https://leetcode.com/problems/valid-phone-numbers) (Easy) — *Reason: SQL / Shell task*
- [584. Find Customer Referee](https://leetcode.com/problems/find-customer-referee) (Easy) — *Reason: SQL / Shell task*
- [586. Customer Placing the Largest Number of Orders](https://leetcode.com/problems/customer-placing-the-largest-number-of-orders) (Easy) — *Reason: SQL / Shell task*
- [620. Not Boring Movies](https://leetcode.com/problems/not-boring-movies) (Easy) — *Reason: SQL / Shell task*
- [1148. Article Views I](https://leetcode.com/problems/article-views-i) (Easy) — *Reason: SQL / Shell task*
- [1280. Students and Examinations](https://leetcode.com/problems/students-and-examinations) (Easy) — *Reason: SQL / Shell task*
- [1757. Recyclable and Low Fat Products](https://leetcode.com/problems/recyclable-and-low-fat-products) (Easy) — *Reason: SQL / Shell task*

### Trivial / No-Brainer Easy DSA Questions (32 Removed)
These problems test basic syntax, elementary standard library calls (`std::reverse`, `sort`, `lower_bound`), or 1-line formulas without algorithmic depth:

- [9. Palindrome Number](https://leetcode.com/problems/palindrome-number) — *Reason: Trivial syntax / elementary 1-liner*
- [13. Roman to Integer](https://leetcode.com/problems/roman-to-integer) — *Reason: Trivial syntax / elementary 1-liner*
- [14. Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix) — *Reason: Trivial syntax / elementary 1-liner*
- [26. Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array) — *Reason: Trivial syntax / elementary 1-liner*
- [28. Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string) — *Reason: Trivial syntax / elementary 1-liner*
- [35. Search Insert Position](https://leetcode.com/problems/search-insert-position) — *Reason: Trivial syntax / elementary 1-liner*
- [67. Add Binary](https://leetcode.com/problems/add-binary) — *Reason: Trivial syntax / elementary 1-liner*
- [70. Climbing Stairs](https://leetcode.com/problems/climbing-stairs) — *Reason: Trivial syntax / elementary 1-liner*
- [88. Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array) — *Reason: Trivial syntax / elementary 1-liner*
- [104. Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree) — *Reason: Trivial syntax / elementary 1-liner*
- [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle) — *Reason: Trivial syntax / elementary 1-liner*
- [136. Single Number](https://leetcode.com/problems/single-number) — *Reason: Trivial syntax / elementary 1-liner*
- [206. Reverse Linked List](https://leetcode.com/problems/reverse-linked-list) — *Reason: Trivial syntax / elementary 1-liner*
- [217. Contains Duplicate](https://leetcode.com/problems/contains-duplicate) — *Reason: Trivial syntax / elementary 1-liner*
- [242. Valid Anagram](https://leetcode.com/problems/valid-anagram) — *Reason: Trivial syntax / elementary 1-liner*
- [283. Move Zeroes](https://leetcode.com/problems/move-zeroes) — *Reason: Trivial syntax / elementary 1-liner*
- [344. Reverse String](https://leetcode.com/problems/reverse-string) — *Reason: Trivial syntax / elementary 1-liner*
- [383. Ransom Note](https://leetcode.com/problems/ransom-note) — *Reason: Trivial syntax / elementary 1-liner*
- [448. Find All Numbers Disappeared in an Array](https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array) — *Reason: Trivial syntax / elementary 1-liner*
- [509. Fibonacci Number](https://leetcode.com/problems/fibonacci-number) — *Reason: Trivial syntax / elementary 1-liner*
- [643. Maximum Average Subarray I](https://leetcode.com/problems/maximum-average-subarray-i) — *Reason: Trivial syntax / elementary 1-liner*
- [704. Binary Search](https://leetcode.com/problems/binary-search) — *Reason: Trivial syntax / elementary 1-liner*
- [796. Rotate String](https://leetcode.com/problems/rotate-string) — *Reason: Trivial syntax / elementary 1-liner*
- [1021. Remove Outermost Parentheses](https://leetcode.com/problems/remove-outermost-parentheses) — *Reason: Trivial syntax / elementary 1-liner*
- [1365. How Many Numbers Are Smaller Than the Current Number](https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number) — *Reason: Trivial syntax / elementary 1-liner*
- [1732. Find the Highest Altitude](https://leetcode.com/problems/find-the-highest-altitude) — *Reason: Trivial syntax / elementary 1-liner*
- [1768. Merge Strings Alternately](https://leetcode.com/problems/merge-strings-alternately) — *Reason: Trivial syntax / elementary 1-liner*
- [1929. Concatenation of Array](https://leetcode.com/problems/concatenation-of-array) — *Reason: Trivial syntax / elementary 1-liner*
- [2418. Sort the People](https://leetcode.com/problems/sort-the-people) — *Reason: Trivial syntax / elementary 1-liner*
- [2553. Separate the Digits in an Array](https://leetcode.com/problems/separate-the-digits-in-an-array) — *Reason: Trivial syntax / elementary 1-liner*
- [2784. Check if Array is Good](https://leetcode.com/problems/check-if-array-is-good) — *Reason: Trivial syntax / elementary 1-liner*
- [3754. Concatenate Non-Zero Digits and Multiply by Sum I](https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-i) — *Reason: Trivial syntax / elementary 1-liner*

</details>

---

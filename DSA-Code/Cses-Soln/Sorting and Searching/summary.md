# CSES Sorting and Searching - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 35 problems in the **Sorting and Searching** topic.

---

## 1. Distinct Numbers

- **Link**: [Distinct Numbers](https://cses.fi/problemset/task/1621)
- **Problem Statement**: Given an array of $N$ integers, count the number of distinct values in the array.
- **Interview Pattern**: Sorting / Hash Set.
- **Intuition**: Sort the array and count how many adjacent element pairs are different, plus 1 for the first element.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 2. Apartments

- **Link**: [Apartments](https://cses.fi/problemset/task/1084)
- **Problem Statement**: There are $N$ applicants and $M$ apartments. Applicant $i$ wants size $A_i$ (acceptable range $[A_i - K, A_i + K]$). Each apartment $j$ has size $B_j$. Assign maximum number of applicants to apartments.
- **Interview Pattern**: Two Pointers Greedy.
- **Intuition**: Sort both applicant desires and apartment sizes. Match the smallest valid apartment to the smallest applicant.
- **Complexity**: Time: $\mathcal{O}(N \log N + M \log M)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 3. Ferris Wheel

- **Link**: [Ferris Wheel](https://cses.fi/problemset/task/1090)
- **Problem Statement**: Given $N$ children with weights $P_i$ and a gondola weight limit $X$, each gondola can hold at most 2 children. Find minimum gondolas required.
- **Interview Pattern**: Two Pointers Greedy (Lightest + Heaviest Pairing).
- **Intuition**: Sort weights. Try to pair the lightest available child with the heaviest available child. If combined weight $\le X$, pair them; otherwise the heaviest child gets their own gondola.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 4. Concert Tickets

- **Link**: [Concert Tickets](https://cses.fi/problemset/task/1091)
- **Problem Statement**: Given $H$ ticket prices and $M$ customers arriving one by one, each customer $i$ will pay at most $T_i$. Assign each customer the highest available ticket price $\le T_i$ and remove it, or -1 if none available.
- **Interview Pattern**: Multiset Upper Bound Search.
- **Intuition**: Store ticket prices in a `std::multiset`. For each customer $T$, find `upper_bound(T)`. Decrement iterator to get largest price $\le T$, print it, and erase it from multiset.
- **Complexity**: Time: $\mathcal{O}((H + M) \log H)$, Space: $\mathcal{O}(H)$.

---

## 5. Restaurant Customers

- **Link**: [Restaurant Customers](https://cses.fi/problemset/task/1619)
- **Problem Statement**: Given arrival and departure times for $N$ customers in a restaurant, find the maximum number of customers present at any single moment.
- **Interview Pattern**: Sweep Line Event Processing.
- **Intuition**: Convert arrivals to $(a_i, +1)$ events and departures to $(d_i, -1)$ events. Sort events by time and maintain running customer count; record maximum.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 6. Movie Festival

- **Link**: [Movie Festival](https://cses.fi/problemset/task/1629)
- **Problem Statement**: Given $N$ movies with start time $a_i$ and end time $b_i$, find the maximum number of non-overlapping movies you can watch.
- **Interview Pattern**: Interval Scheduling Greedy (Sort by End Time).
- **Intuition**: Always choose the movie that finishes earliest. This leaves maximum time for remaining movies.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 7. Sum of Two Values

- **Link**: [Sum of Two Values](https://cses.fi/problemset/task/1640)
- **Problem Statement**: Given an array of $N$ positive integers and a target sum $X$, find 1-based indices of two distinct elements whose sum is $X$, or output `IMPOSSIBLE`.
- **Interview Pattern**: Two Pointers / Hash Map Complement Search.
- **Intuition**: Maintain a hash map of seen value $\to$ index. For each $A[i]$, check if $X - A[i]$ is in map.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 8. Maximum Subarray Sum

- **Link**: [Maximum Subarray Sum](https://cses.fi/problemset/task/1643)
- **Problem Statement**: Given an array of $N$ integers, find the maximum sum of a contiguous non-empty subarray.
- **Interview Pattern**: Kadane's Algorithm.
- **Intuition**: `current_sum = max(A[i], current_sum + A[i])`, update `max_sum = max(max_sum, current_sum)`.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 9. Stick Lengths

- **Link**: [Stick Lengths](https://cses.fi/problemset/task/1074)
- **Problem Statement**: Given $N$ sticks of lengths $P_i$, find the minimum cost (sum of absolute changes) to make all stick lengths equal.
- **Interview Pattern**: Median Minimization (L1 Distance).
- **Intuition**: The sum of absolute differences $\sum |P_i - X|$ is minimized when $X$ is the **median** of the stick lengths!
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 10. Missing Coin Sum

- **Link**: [Missing Coin Sum](https://cses.fi/problemset/task/2183)
- **Problem Statement**: Given $N$ positive integers, find the smallest positive integer sum that **cannot** be created using any subset of the numbers.
- **Interview Pattern**: Sorted Prefix Achievable Range.
- **Intuition**: Sort numbers. Maintain `reach = 1` (smallest unachievable sum). If next coin $x \le \text{reach}$, we can extend achievable sums to $\text{reach} + x$. If $x > \text{reach}$, then $\text{reach}$ can never be formed!
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 11. Collecting Numbers

- **Link**: [Collecting Numbers](https://cses.fi/problemset/task/2216)
- **Problem Statement**: Given a permutation of $1 \dots N$, you collect numbers in order $1, 2, 3, \dots, N$ in rounds from left to right. How many rounds are needed?
- **Interview Pattern**: Inverse Index Inversions.
- **Intuition**: Store position `pos[val]`. A new round is triggered whenever `pos[x] > pos[x+1]`. Total rounds = $1 + \sum_{x=1}^{N-1} [pos[x] > pos[x+1]]$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 12. Collecting Numbers II

- **Link**: [Collecting Numbers II](https://cses.fi/problemset/task/2217)
- **Problem Statement**: Given a permutation of $1 \dots N$, process $M$ swap operations $(u, v)$ and print total rounds needed after each swap.
- **Interview Pattern**: Local Inversion Delta Update.
- **Intuition**: Swapping values $x$ and $y$ only affects rounds count for pairs $(x-1, x), (x, x+1), (y-1, y), (y, y+1)$. Subtract their contributions before swap, perform swap, add updated contributions after swap.
- **Complexity**: Time: $\mathcal{O}(N + M)$, Space: $\mathcal{O}(N)$.

---

## 13. Playlist

- **Link**: [Playlist](https://cses.fi/problemset/task/1641)
- **Problem Statement**: Given a sequence of $N$ songs, find the maximum length of a contiguous sequence of unique songs.
- **Interview Pattern**: Sliding Window / Two Pointers with Set.
- **Intuition**: Expand right pointer $r$. If $A[r]$ is already in set, shrink left pointer $l$ until $A[r]$ is removed. `max_len = max(max_len, r - l + 1)`.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 14. Towers

- **Link**: [Towers](https://cses.fi/problemset/task/1073)
- **Problem Statement**: Given $N$ blocks arriving one by one, place each block on top of an existing tower whose top block is strictly larger, or start a new tower. Minimize total towers.
- **Interview Pattern**: `std::multiset` Upper Bound Greedy (Patience Sorting).
- **Intuition**: For each block $x$, find `upper_bound(x)` in multiset of tower tops. If exists, replace that tower top with $x$; else insert $x$ as a new tower top.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 15. Traffic Lights

- **Link**: [Traffic Lights](https://cses.fi/problemset/task/1163)
- **Problem Statement**: A street of length $X$ has traffic lights added at positions $P_i$ one by one. After each addition, print the length of the longest passage without traffic lights.
- **Interview Pattern**: `std::set` Position Maintenance + `std::multiset` Segment Lengths.
- **Intuition**:
  - `positions` set stores light positions (initially `{0, X}`).
  - `lengths` multiset stores passage lengths (initially `{X}`).
  - Adding light at $p$: find neighbor positions $L < p < R$. Erase segment $R - L$ from `lengths`, insert $p - L$ and $R - p$. Print `*lengths.rbegin()`.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 16. Josephus Problem I

- **Link**: [Josephus Problem I](https://cses.fi/problemset/task/2162)
- **Problem Statement**: $N$ children in a circle numbered $1 \dots N$. Every second child is removed until no children remain. Output removal order.
- **Interview Pattern**: Circular Simulation / Queue.
- **Intuition**: Use a queue. For each step: pop front, push to back; then pop front and print (removed child).
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 17. Josephus Problem II

- **Link**: [Josephus Problem II](https://cses.fi/problemset/task/2163)
- **Problem Statement**: $N$ children in a circle. Every $K$-th child is removed until no children remain ($K \le 10^9$). Output removal order.
- **Interview Pattern**: Order Statistic Tree (Policy-Based Data Structure `pbds`) / Fenwick Binary Lifting.
- **Intuition**:
  - Store active children in a `tree` supporting `find_by_order(idx)` in $\mathcal{O}(\log N)$.
  - Next index to remove: `curr_idx = (curr_idx + K) % remaining_count`.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 18. Nested Ranges Check

- **Link**: [Nested Ranges Check](https://cses.fi/problemset/task/2168)
- **Problem Statement**: Given $N$ ranges $[x_i, y_i]$, for each range determine: (1) if it contains another range, (2) if it is contained in another range.
- **Interview Pattern**: Interval Sorting + Min/Max Sweeping.
- **Intuition**:
  - Sort ranges by left $x_i$ asc, right $y_i$ desc.
  - Range $i$ is contained in a prior range iff prior max $y \ge y_i$.
  - Range $i$ contains a later range iff later min $y \le y_i$.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 19. Nested Ranges Count

- **Link**: [Nested Ranges Count](https://cses.fi/problemset/task/2169)
- **Problem Statement**: Given $N$ ranges $[x_i, y_i]$, count for each range how many ranges it contains, and how many ranges contain it.
- **Interview Pattern**: Coordinate Compression + Fenwick Tree on Sorted Intervals.
- **Intuition**:
  - Sort ranges by left $x_i$ asc, right $y_i$ desc.
  - Sweep through ranges and maintain right endpoints $y_i$ in a BIT to count smaller/larger right endpoints in $\mathcal{O}(\log N)$.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 20. Room Allocation

- **Link**: [Room Allocation](https://cses.fi/problemset/task/1164)
- **Problem Statement**: Given arrival and departure days for $N$ customers, allocate room numbers such that no two customers share a room on overlapping days, minimizing total rooms used.
- **Interview Pattern**: Min-Heap Event Processing (Departure Tracking).
- **Intuition**:
  - Sort customers by arrival day. Maintain min-heap of active rooms `(departure_day, room_number)`.
  - If `heap.top().departure < customer.arrival`, reuse room. Else allocate new room.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 21. Factory Machines

- **Link**: [Factory Machines](https://cses.fi/problemset/task/1620)
- **Problem Statement**: Given $N$ machines with production times $k_i$, find minimum time to produce at least $T$ items.
- **Interview Pattern**: Binary Search on Answer.
- **Intuition**:
  - Monotonic check function: in time $M$, total products = $\sum \lfloor M / k_i \rfloor$.
  - Binary search $M \in [1, 10^{18}]$.
- **Complexity**: Time: $\mathcal{O}(N \log(10^{18}))$, Space: $\mathcal{O}(1)$.

---

## 22. Tasks and Deadlines

- **Link**: [Tasks and Deadlines](https://cses.fi/problemset/task/1630)
- **Problem Statement**: Given $N$ tasks with duration $a_i$ and deadline $d_i$, order tasks to maximize total reward $\sum (d_i - \text{finish\_time}_i)$.
- **Interview Pattern**: Greedy Exchange Argument (Sort by Duration).
- **Intuition**:
  - Shorter duration tasks completed earlier minimize accumulated delay for all subsequent tasks. Sort tasks by duration $a_i$ ascending.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 23. Reading Books

- **Link**: [Reading Books](https://cses.fi/problemset/task/1631)
- **Problem Statement**: Two people read $N$ books of lengths $t_i$. No two people read same book simultaneously. Find minimum total time for both to read all books.
- **Interview Pattern**: Bottleneck Max Element vs Sum.
- **Intuition**:
  - Let $M = \max(t_i)$ and $S = \sum t_i$.
  - If $M > S - M$ (longest book takes more than half total time), answer is $2M$.
  - Else answer is $S$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 24. Sum of Three Values

- **Link**: [Sum of Three Values](https://cses.fi/problemset/task/1641)
- **Problem Statement**: Given $N$ integers and target $X$, find 1-based indices of 3 elements summing to $X$.
- **Interview Pattern**: Sorting + Fix First + Two Pointers ($\mathcal{O}(N^2)$).
- **Intuition**:
  - Sort element-index pairs. Fix first element at index $i$, then run Two Pointers on range $[i+1, N-1]$ to find sum $X - A[i]$.
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N)$.

---

## 25. Sum of Four Values

- **Link**: [Sum of Four Values](https://cses.fi/problemset/task/1642)
- **Problem Statement**: Given $N$ integers and target $X$, find 1-based indices of 4 elements summing to $X$.
- **Interview Pattern**: Pair-Sum Hash Map ($\mathcal{O}(N^2)$).
- **Intuition**:
  - Store all pair sums $(A[i] + A[j])$ in a hash map `pair_map[sum] = (i, j)`.
  - For candidate pair $(k, l)$, check if $X - (A[k] + A[l])$ exists in map with non-overlapping indices.
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N^2)$.

---

## 26. Nearest Smaller Values

- **Link**: [Nearest Smaller Values](https://cses.fi/problemset/task/1645)
- **Problem Statement**: For each position in an array of $N$ integers, find 1-based index of nearest smaller element to its left, or 0 if none.
- **Interview Pattern**: Monotonic Increasing Stack.
- **Intuition**: Maintain stack of indices with increasing values. Pop elements $\ge A[i]$. Stack top is nearest smaller left element.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 27. Subarray Sums I

- **Link**: [Subarray Sums I](https://cses.fi/problemset/task/1660)
- **Problem Statement**: Given array of $N$ **positive** integers, count contiguous subarrays summing to $X$.
- **Interview Pattern**: Two Pointers / Sliding Window.
- **Intuition**: Since all numbers are positive, expand right pointer $r$ and shrink left pointer $l$ when sum $> X$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 28. Subarray Sums II

- **Link**: [Subarray Sums II](https://cses.fi/problemset/task/1661)
- **Problem Statement**: Given array of $N$ integers (can be negative), count contiguous subarrays summing to $X$.
- **Interview Pattern**: Prefix Sum Hash Map.
- **Intuition**: Subarray sum $A[l..r] = P[r] - P[l-1] = X \implies P[l-1] = P[r] - X$. Maintain map of prefix sum frequencies.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 29. Subarray Divisibility

- **Link**: [Subarray Divisibility](https://cses.fi/problemset/task/1662)
- **Problem Statement**: Given array of $N$ integers, count contiguous subarrays whose sum is divisible by $N$.
- **Interview Pattern**: Prefix Remainder Frequency Map modulo $N$.
- **Intuition**: Subarray sum $A[l..r] \equiv 0 \pmod N \implies P[r] \equiv P[l-1] \pmod N$. Count frequency of remainder $(P[i] \bmod N + N) \bmod N$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 30. Distinct Values Subarrays

- **Link**: [Distinct Values Subarrays](https://cses.fi/problemset/task/2428)
- **Problem Statement**: Given array of $N$ integers and $K$, count contiguous subarrays with at most $K$ distinct values.
- **Interview Pattern**: Sliding Window Frequency Map.
- **Intuition**: Expand right pointer $r$. If distinct count $> K$, shrink left pointer $l$. Valid subarrays ending at $r$ count $= r - l + 1$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 31. Distinct Values Subarrays II

- **Link**: [Distinct Values Subarrays II](https://cses.fi/problemset/task/2429)
- **Problem Statement**: Count contiguous subarrays with **at least** $K$ distinct values.
- **Interview Pattern**: Complementary Counting (Total Subarrays - Subarrays with $< K$ distinct).
- **Intuition**: Total subarrays = $N(N+1)/2$. Subtract count of subarrays with $\le K-1$ distinct values using Sliding Window.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 32. Distinct Values Subsequences

- **Link**: [Distinct Values Subsequences](https://cses.fi/problemset/task/2430)
- **Problem Statement**: Count total non-empty subsequences with all distinct elements modulo $10^9+7$.
- **Interview Pattern**: Element Frequency Product $\prod (freq[x] + 1) - 1$.
- **Intuition**: For each unique value $x$, we can either pick 1 of its $freq[x]$ occurrences or pick none. Total combinations = $\prod (freq[x] + 1) - 1$.
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 33. Array Division

- **Link**: [Array Division](https://cses.fi/problemset/task/1085)
- **Problem Statement**: Partition array of $N$ integers into $K$ contiguous subarrays to minimize maximum subarray sum.
- **Interview Pattern**: Binary Search on Answer + Greedy Check.
- **Intuition**: Check if target max sum $M$ is possible: greedily form subarrays without exceeding $M$. Binary search $M \in [\max(A), \sum A]$.
- **Complexity**: Time: $\mathcal{O}(N \log(\sum A))$, Space: $\mathcal{O}(1)$.

---

## 34. Movie Festival II

- **Link**: [Movie Festival II](https://cses.fi/problemset/task/1984)
- **Problem Statement**: Given $N$ movies and $K$ people, find maximum total movies they can watch.
- **Interview Pattern**: Greedy Sort by End Time + `std::multiset` Upper Bound.
- **Intuition**: Sort movies by end time. Assign movie $(a_i, b_i)$ to person whose available time is $\le a_i$ and closest to $a_i$ (using `upper_bound`).
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

## 35. Maximum Subarray Sum II

- **Link**: [Maximum Subarray Sum II](https://cses.fi/problemset/task/1644)
- **Problem Statement**: Find maximum subarray sum for length in range $[A, B]$.
- **Interview Pattern**: Prefix Sum + Sliding Window Monotonic Deque / `std::multiset`.
- **Intuition**: Subarray sum $P[i] - P[j]$ with $i - B \le j \le i - A$. Maintain multiset of valid prefix sums $P[j]$ for window of candidate $j$.
- **Complexity**: Time: $\mathcal{O}(N \log(B - A))$, Space: $\mathcal{O}(B - A)$.

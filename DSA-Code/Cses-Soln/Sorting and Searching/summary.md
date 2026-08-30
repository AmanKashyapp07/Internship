# CSES Sorting and Searching - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 16 problems in the **Sorting and Searching** topic.

---

## 1. Concert Tickets

- **Tier**: Tier 2
- **Link**: [Concert Tickets](https://cses.fi/problemset/task/1091)
- **Problem Statement**: Given $H$ ticket prices and $M$ customers arriving one by one, each customer $i$ will pay at most $T_i$. Assign each customer the highest available ticket price $\le T_i$ and remove it, or -1 if none available.
- **Interview Pattern**: Multiset Upper Bound Search.
- **Intuition**: Store ticket prices in a `std::multiset`. For each customer $T$, find `upper_bound(T)`. Decrement iterator to get largest price $\le T$, print it, and erase it from multiset.
- **Step-by-Step Interview Walkthrough**:
  1. Insert all ticket prices into a multiset.
  2. For each customer max price $T$:
     - Query `it = multiset.upper_bound(T)`.
     - If `it == multiset.begin()`: no ticket $\le T$ exists, output -1.
     - Else: decrement `it = prev(it)`, print `*it`, and erase `it` from multiset.
- **Pseudocode**:
```text
function concert_tickets(tickets, customers):
    multiset = build_multiset(tickets)
    for T in customers:
        it = multiset.upper_bound(T)
        if it == multiset.begin():
            print -1
        else:
            it = prev(it)
            print *it
            multiset.erase(it)
```
- **Complexity**: Time: $\mathcal{O}((H + M) \log H)$, Space: $\mathcal{O}(H)$.

---

## 2. Collecting Numbers

- **Tier**: Tier 1
- **Link**: [Collecting Numbers](https://cses.fi/problemset/task/2216)
- **Problem Statement**: Given a permutation of $1 \dots N$, you collect numbers in order $1, 2, 3, \dots, N$ in rounds from left to right. How many rounds are needed?
- **Interview Pattern**: Inverse Index Inversions.
- **Intuition**: Store position `pos[val]`. A new round is triggered whenever `pos[x] > pos[x+1]`. Total rounds = $1 + \sum_{x=1}^{N-1} [pos[x] > pos[x+1]]$.
- **Step-by-Step Interview Walkthrough**:
  1. Store 0-based position `pos[A[i]] = i` for all elements.
  2. Initialize `rounds = 1`.
  3. Loop $x = 1 \dots N-1$: if `pos[x] > pos[x + 1]`, increment `rounds`.
  4. Return `rounds`.
- **Pseudocode**:
```text
function collecting_numbers(A, N):
    for i = 0 to N - 1: pos[A[i]] = i
    rounds = 1
    for x = 1 to N - 1:
        if pos[x] > pos[x + 1]:
            rounds += 1
    return rounds
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 3. Collecting Numbers II

- **Tier**: Tier 2
- **Link**: [Collecting Numbers II](https://cses.fi/problemset/task/2217)
- **Problem Statement**: Given a permutation of $1 \dots N$, process $M$ swap operations $(u, v)$ and print total rounds needed after each swap.
- **Interview Pattern**: Local Inversion Delta Update.
- **Intuition**: Swapping values $x$ and $y$ only affects rounds count for pairs $(x-1, x), (x, x+1), (y-1, y), (y, y+1)$. Subtract their contributions before swap, perform swap, add updated contributions after swap.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute `pos[val]` and initial total rounds.
  2. For each swap query $(u, v)$:
     - Let $x = A[u], y = A[v]$.
     - Collect unique adjacent pairs among $(x-1, x), (x, x+1), (y-1, y), (y, y+1)$.
     - For each pair $(a, b)$: if `pos[a] > pos[b]`, decrement `rounds`.
     - Swap values in array and update `pos[x] = v, pos[y] = u`.
     - For each pair $(a, b)$: if `pos[a] > pos[b]`, increment `rounds`.
     - Print `rounds`.
- **Pseudocode**:
```text
function collecting_numbers_ii(A, swaps):
    for i = 0 to N - 1: pos[A[i]] = i
    rounds = count_initial_rounds()
    for (u, v) in swaps:
        x = A[u], y = A[v]
        pairs = unique({(x-1,x), (x,x+1), (y-1,y), (y,y+1)})
        for (a, b) in pairs: if pos[a] > pos[b]: rounds -= 1
        swap(A[u], A[v])
        pos[x] = v, pos[y] = u
        for (a, b) in pairs: if pos[a] > pos[b]: rounds += 1
        print rounds
```
- **Complexity**: Time: $\mathcal{O}(N + M)$, Space: $\mathcal{O}(N)$.

---

## 4. Towers

- **Tier**: Tier 2
- **Link**: [Towers](https://cses.fi/problemset/task/1073)
- **Problem Statement**: Given $N$ blocks arriving one by one, place each block on top of an existing tower whose top block is strictly larger, or start a new tower. Minimize total towers.
- **Interview Pattern**: `std::multiset` Upper Bound Greedy (Patience Sorting).
- **Intuition**: For each block $x$, find `upper_bound(x)` in multiset of tower tops. If exists, replace that tower top with $x$; else insert $x$ as a new tower top.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain a multiset `tower_tops`.
  2. For each incoming block $x$:
     - Query `it = tower_tops.upper_bound(x)`.
     - If `it != tower_tops.end()`: erase `it` (place block $x$ on this tower).
     - Insert $x$ into `tower_tops`.
  3. Return size of `tower_tops`.
- **Pseudocode**:
```text
function towers(blocks):
    tower_tops = multiset()
    for x in blocks:
        it = tower_tops.upper_bound(x)
        if it != tower_tops.end():
            tower_tops.erase(it)
        tower_tops.insert(x)
    return length(tower_tops)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 5. Traffic Lights

- **Tier**: Tier 2
- **Link**: [Traffic Lights](https://cses.fi/problemset/task/1163)
- **Problem Statement**: A street of length $X$ has traffic lights added at positions $P_i$ one by one. After each addition, print the length of the longest passage without traffic lights.
- **Interview Pattern**: `std::set` Position Maintenance + `std::multiset` Segment Lengths.
- **Intuition**:
  - `positions` set stores light positions (initially `{0, X}`).
  - `lengths` multiset stores passage lengths (initially `{X}`).
  - Adding light at $p$: find neighbor positions $L < p < R$. Erase segment $R - L$ from `lengths`, insert $p - L$ and $R - p$. Print `*lengths.rbegin()`.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `pos_set = {0, X}` and `len_multiset = {X}`.
  2. For each added position $p$:
     - Locate boundaries $L < p < R$ using `it = pos_set.upper_bound(p)`.
     - Remove segment length $(R - L)$ from `len_multiset`.
     - Insert new segments $(p - L)$ and $(R - p)$ into `len_multiset`.
     - Insert $p$ into `pos_set`.
     - Output max element `*len_multiset.rbegin()`.
- **Pseudocode**:
```text
function traffic_lights(X, positions_to_add):
    pos_set = set({0, X})
    len_multiset = multiset({X})
    for p in positions_to_add:
        it = pos_set.upper_bound(p)
        R = *it, L = *prev(it)
        len_multiset.erase_one(R - L)
        len_multiset.insert(p - L)
        len_multiset.insert(R - p)
        pos_set.insert(p)
        print *len_multiset.rbegin()
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 6. Josephus Problem II

- **Tier**: Tier 2
- **Link**: [Josephus Problem II](https://cses.fi/problemset/task/2163)
- **Problem Statement**: $N$ children in a circle. Every $K$-th child is removed until no children remain ($K \le 10^9$). Output removal order.
- **Interview Pattern**: Order Statistic Tree (Policy-Based Data Structure `pbds`) / Fenwick Binary Lifting.
- **Intuition**:
  - Store active children in a `tree` supporting `find_by_order(idx)` in $\mathcal{O}(\log N)$.
  - Next index to remove: `curr_idx = (curr_idx + K) % remaining_count`.
- **Step-by-Step Interview Walkthrough**:
  1. Insert $1 \dots N$ into Policy-Based Order Statistic Tree.
  2. Maintain `curr = 0`.
  3. While tree size $> 0$:
     - `curr = (curr + K) % tree.size()`.
     - Query $k$-th element `val = tree.find_by_order(curr)`.
     - Print `val` and delete from tree.
- **Pseudocode**:
```text
function josephus_ii(N, K):
    tree = pbds_order_tree([1..N])
    curr = 0
    while not tree.empty():
        curr = (curr + K) % tree.size()
        val = tree.find_by_order(curr)
        print val
        tree.erase(val)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 7. Nested Ranges Check

- **Tier**: Tier 2
- **Link**: [Nested Ranges Check](https://cses.fi/problemset/task/2168)
- **Problem Statement**: Given $N$ ranges $[x_i, y_i]$, for each range determine: (1) if it contains another range, (2) if it is contained in another range.
- **Interview Pattern**: Interval Sorting + Min/Max Sweeping.
- **Intuition**:
  - Sort ranges by left $x_i$ asc, right $y_i$ desc.
  - Range $i$ is contained in a prior range iff prior max $y \ge y_i$.
  - Range $i$ contains a later range iff later min $y \le y_i$.
- **Step-by-Step Interview Walkthrough**:
  1. Tag ranges with original 0-based indices and sort: $x$ ascending, $y$ descending.
  2. **Contains check** (right to left sweep): track running `min_y`. If $y_i \ge \text{min\_y}$, range $i$ contains a smaller range. Update $\text{min\_y} = \min(\text{min\_y}, y_i)$.
  3. **Is-Contained check** (left to right sweep): track running `max_y`. If $y_i \le \text{max\_y}$, range $i$ is contained in a larger range. Update $\text{max\_y} = \max(\text{max\_y}, y_i)$.
- **Pseudocode**:
```text
function nested_ranges_check(ranges):
    sort(ranges, by_x_asc_y_desc)
    min_y = INF
    for i = N - 1 down to 0:
        if ranges[i].y >= min_y: contains[ranges[i].id] = 1
        min_y = min(min_y, ranges[i].y)
    
    max_y = 0
    for i = 0 to N - 1:
        if ranges[i].y <= max_y: is_contained[ranges[i].id] = 1
        max_y = max(max_y, ranges[i].y)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 8. Nested Ranges Count

- **Tier**: Tier 2
- **Link**: [Nested Ranges Count](https://cses.fi/problemset/task/2169)
- **Problem Statement**: Given $N$ ranges $[x_i, y_i]$, count for each range how many ranges it contains, and how many ranges contain it.
- **Interview Pattern**: Coordinate Compression + Fenwick Tree on Sorted Intervals.
- **Intuition**:
  - Sort ranges by left $x_i$ asc, right $y_i$ desc.
  - Sweep through ranges and maintain right endpoints $y_i$ in a BIT to count smaller/larger right endpoints in $\mathcal{O}(\log N)$.
- **Step-by-Step Interview Walkthrough**:
  1. Coordinate compress all $y$-values to range $[1, U]$.
  2. Sort ranges by $x$ ascending, $y$ descending.
  3. **Contains count** (sweep right to left): query `BIT.query(y)` to count points with $y' \le y$, then insert $y$ into BIT.
  4. **Is-Contained count** (sweep left to right): query `BIT.query_range(y, U)` to count points with $y' \ge y$, then insert $y$ into BIT.
- **Pseudocode**:
```text
function nested_ranges_count(ranges):
    compress_y_coordinates(ranges)
    sort(ranges, by_x_asc_y_desc)
    
    BIT1 = FenwickTree()
    for i = N - 1 down to 0:
        contains_count[ranges[i].id] = BIT1.query(ranges[i].compressed_y)
        BIT1.add(ranges[i].compressed_y, 1)
        
    BIT2 = FenwickTree()
    for i = 0 to N - 1:
        contained_count[ranges[i].id] = BIT2.query_range(ranges[i].compressed_y, MAX_Y)
        BIT2.add(ranges[i].compressed_y, 1)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 9. Room Allocation

- **Tier**: Tier 2
- **Link**: [Room Allocation](https://cses.fi/problemset/task/1164)
- **Problem Statement**: Given arrival and departure days for $N$ customers, allocate room numbers such that no two customers share a room on overlapping days, minimizing total rooms used.
- **Interview Pattern**: Min-Heap Event Processing (Departure Tracking).
- **Intuition**:
  - Sort customers by arrival day. Maintain min-heap of active rooms `(departure_day, room_number)`.
  - If `heap.top().departure < customer.arrival`, reuse room. Else allocate new room.
- **Step-by-Step Interview Walkthrough**:
  1. Store customers with original IDs and sort by arrival day ascending.
  2. Min-heap stores active rooms `(departure_day, room_id)`.
  3. For each customer $(a_i, d_i)$:
     - If heap is not empty and $\text{top.departure} < a_i$: reuse room, `room_id = heap.pop().room_id`.
     - Else: allocate new room, `room_id = ++room_count`.
     - Record `assigned_room[c.id] = room_id` and push `(d_i, room_id)` to heap.
  4. Return `room_count` and `assigned_room` array.
- **Pseudocode**:
```text
function room_allocation(customers):
    sort(customers, by_arrival)
    pq = min_heap() // (departure_day, room_id)
    room_count = 0
    for c in customers:
        if not pq.empty() and pq.top().dep < c.arr:
            room_id = pq.pop().room_id
        else:
            room_count += 1
            room_id = room_count
        assigned_room[c.id] = room_id
        pq.push((c.dep, room_id))
    return room_count, assigned_room
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 10. Reading Books

- **Tier**: Tier 2
- **Link**: [Reading Books](https://cses.fi/problemset/task/1631)
- **Problem Statement**: Two people read $N$ books of lengths $t_i$. No two people read same book simultaneously. Find minimum total time for both to read all books.
- **Interview Pattern**: Bottleneck Max Element vs Sum.
- **Intuition**:
  - Let $M = \max(t_i)$ and $S = \sum t_i$.
  - If $M > S - M$ (longest book takes more than half total time), answer is $2M$.
  - Else answer is $S$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute total sum $S = \sum t_i$ and maximum book time $M = \max(t_i)$.
  2. If $M > S - M$: return $2 \times M$.
  3. Else: return $S$.
- **Pseudocode**:
```text
function reading_books(times):
    total_sum = sum(times)
    max_time = max(times)
    if max_time > total_sum - max_time:
        return 2 * max_time
    else:
        return total_sum
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 11. Subarray Divisibility

- **Tier**: Tier 2
- **Link**: [Subarray Divisibility](https://cses.fi/problemset/task/1662)
- **Problem Statement**: Given array of $N$ integers, count contiguous subarrays whose sum is divisible by $N$.
- **Interview Pattern**: Prefix Remainder Frequency Map modulo $N$.
- **Intuition**: Subarray sum $A[l..r] \equiv 0 \pmod N \implies P[r] \equiv P[l-1] \pmod N$. Count frequency of remainder $(P[i] \bmod N + N) \bmod N$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `rem_count = {0: 1}, pref = 0, count = 0`.
  2. For each element $x \in A$:
     - $\text{pref} = (\text{pref} + x \bmod N + N) \bmod N$.
     - `count += rem_count[pref]`.
     - `rem_count[pref] += 1`.
  3. Return `count`.
- **Pseudocode**:
```text
function subarray_divisibility(A, N):
    rem_count = map({0: 1})
    pref = 0, count = 0
    for x in A:
        pref = (pref + x % N + N) % N
        count += rem_count[pref]
        rem_count[pref] += 1
    return count
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 12. Distinct Values Subarrays

- **Tier**: Tier 2
- **Link**: [Distinct Values Subarrays](https://cses.fi/problemset/task/2428)
- **Problem Statement**: Given array of $N$ integers and $K$, count contiguous subarrays with at most $K$ distinct values.
- **Interview Pattern**: Sliding Window Frequency Map.
- **Intuition**: Expand right pointer $r$. If distinct count $> K$, shrink left pointer $l$. Valid subarrays ending at $r$ count $= r - l + 1$.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain hash map `freq` for element counts in window $[l, r]$.
  2. For $r = 0 \dots N-1$:
     - `freq[A[r]] += 1`.
     - While `freq.size() > K`:
       - `freq[A[l]] -= 1`.
       - If `freq[A[l]] == 0`: erase `A[l]` from `freq`.
       - `l += 1`.
     - `count += (r - l + 1)`.
  3. Return `count`.
- **Pseudocode**:
```text
function count_at_most_k_distinct(A, K):
    freq = map(), l = 0, count = 0
    for r = 0 to length(A) - 1:
        freq[A[r]] += 1
        while length(freq) > K:
            freq[A[l]] -= 1
            if freq[A[l]] == 0: erase(freq[A[l]])
            l += 1
        count += (r - l + 1)
    return count
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 13. Distinct Values Subarrays II

- **Tier**: Tier 2
- **Link**: [Distinct Values Subarrays II](https://cses.fi/problemset/task/2429)
- **Problem Statement**: Count contiguous subarrays with **at least** $K$ distinct values.
- **Interview Pattern**: Complementary Counting (Total Subarrays - Subarrays with $< K$ distinct).
- **Intuition**: Total subarrays = $N(N+1)/2$. Subtract count of subarrays with $\le K-1$ distinct values using Sliding Window.
- **Step-by-Step Interview Walkthrough**:
  1. Total contiguous subarrays $= N(N + 1) / 2$.
  2. Compute `subarrays_less_than_k = count_at_most_k_distinct(A, K - 1)`.
  3. Result $= \text{Total} - \text{subarrays\_less\_than\_k}$.
- **Pseudocode**:
```text
function count_at_least_k_distinct(A, K, N):
    total = N * (N + 1) / 2
    return total - count_at_most_k_distinct(A, K - 1)
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 14. Distinct Values Subsequences

- **Tier**: Tier 2
- **Link**: [Distinct Values Subsequences](https://cses.fi/problemset/task/2430)
- **Problem Statement**: Count total non-empty subsequences with all distinct elements modulo $10^9+7$.
- **Interview Pattern**: Element Frequency Product $\prod (freq[x] + 1) - 1$.
- **Intuition**: For each unique value $x$, we can either pick 1 of its $freq[x]$ occurrences or pick none. Total combinations = $\prod (freq[x] + 1) - 1$.
- **Step-by-Step Interview Walkthrough**:
  1. Count frequency of each unique value $x$ in array $A$.
  2. Initialize `ans = 1, MOD = 1e9 + 7`.
  3. For each unique element: $\text{ans} = (\text{ans} \times (\text{freq}[x] + 1)) \pmod{\text{MOD}}$.
  4. Return $(\text{ans} - 1 + \text{MOD}) \pmod{\text{MOD}}$.
- **Pseudocode**:
```text
function distinct_values_subsequences(A, MOD=1000000007):
    freq = map()
    for x in A: freq[x] += 1
    ans = 1
    for (x, count) in freq:
        ans = (ans * (count + 1)) % MOD
    return (ans - 1 + MOD) % MOD
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 15. Movie Festival II

- **Tier**: Tier 3
- **Link**: [Movie Festival II](https://cses.fi/problemset/task/1984)
- **Problem Statement**: Given $N$ movies and $K$ people, find maximum total movies they can watch.
- **Interview Pattern**: Greedy Sort by End Time + `std::multiset` Upper Bound.
- **Intuition**: Sort movies by end time. Assign movie $(a_i, b_i)$ to person whose available time is $\le a_i$ and closest to $a_i$ (using `upper_bound`).
- **Step-by-Step Interview Walkthrough**:
  1. Sort movies by end time $b_i$ ascending.
  2. Multiset `members` stores finish times of all $K$ people (initialized with $K$ zeros).
  3. For each movie $(a_i, b_i)$:
     - `it = members.upper_bound(a_i)`.
     - If `it != members.begin()`:
       - `it = prev(it)` (person available before/at $a_i$).
       - `members.erase(it)`.
       - `members.insert(b_i)`.
       - `watched += 1`.
  4. Return `watched`.
- **Pseudocode**:
```text
function movie_festival_ii(movies, K):
    sort(movies, by_end_time_ascending)
    members = multiset of K zeros
    watched = 0
    for (start, end) in movies:
        it = members.upper_bound(start)
        if it != members.begin():
            it = prev(it)
            members.erase(it)
            members.insert(end)
            watched += 1
    return watched
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

## 16. Maximum Subarray Sum II

- **Tier**: Tier 2
- **Link**: [Maximum Subarray Sum II](https://cses.fi/problemset/task/1644)
- **Problem Statement**: Find maximum subarray sum for length in range $[A, B]$.
- **Interview Pattern**: Prefix Sum + Sliding Window Monotonic Deque / `std::multiset`.
- **Intuition**: Subarray sum $P[i] - P[j]$ with $i - B \le j \le i - A$. Maintain multiset of valid prefix sums $P[j]$ for window of candidate $j$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute prefix sums $P[0 \dots N]$ with $P[0] = 0$.
  2. Sliding multiset stores $P[j]$ for valid $j \in [i - B, i - A]$.
  3. For $i = A \dots N$:
     - Insert $P[i - A]$ into multiset.
     - If $i - B - 1 \ge 0$: remove $P[i - B - 1]$ from multiset.
     - $\text{max\_sum} = \max(\text{max\_sum}, P[i] - \min(\text{multiset}))$.
  4. Return `max_sum`.
- **Pseudocode**:
```text
function max_subarray_sum_ii(A, min_len, max_len):
    P = compute_prefix_sums(A)
    window = multiset()
    max_sum = -INF
    for i = min_len to length(A):
        window.insert(P[i - min_len])
        if i - max_len - 1 >= 0:
            window.erase_one(P[i - max_len - 1])
        max_sum = max(max_sum, P[i] - *window.begin())
    return max_sum
```
- **Complexity**: Time: $\mathcal{O}(N \log(B - A))$, Space: $\mathcal{O}(B - A)$.

---

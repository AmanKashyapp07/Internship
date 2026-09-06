# Timepass & No-Brainer Problems (Pruned from OA Prep)

> **Purpose:** This file archives the **42 low-ROI problems** pruned from the master Microsoft OA prep list. These questions are either **non-algorithmic database/shell tasks** or **elementary 1-liners / syntax warmups** that add needless bulk without teaching substantive algorithmic thinking.

---

## Summary Breakdown

| Category | Count | Primary Reason for Removal |
| :--- | :---: | :--- |
| **Non-DSA (SQL & Shell)** | **10** | Never tested in Microsoft Software Engineering Online Assessments (Codility/HackerRank). |
| **Trivial / No-Brainer Easy DSA** | **32** | 1-line standard library calls (`std::reverse`, `sort`), elementary math, or exact duplicates. |
| **Total Pruned Problems** | **42** | Cuts study fatigue and saves ~15–20 hours of redundant practice. |

---

## Part 1: Trivial / No-Brainer Easy DSA (32 Problems)

These problems can safely be skipped or solved in under 60 seconds mentally:

- [ ] `[Tier 2: 3-Month]` [9. Palindrome Number](https://leetcode.com/problems/palindrome-number) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Palindrome Number: 1-line string reverse comparison or basic integer digit reversal.
- [ ] `[Tier 1: 30-Day]` [13. Roman to Integer](https://leetcode.com/problems/roman-to-integer) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Roman to Integer: Elementary hash map lookup with subtractive condition `curr < next`.
- [ ] `[Tier 1: 30-Day]` [14. Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix) — **Easy** (Freq: 62.5%)
  - *Why it's timepass:* Longest Common Prefix: Basic horizontal or vertical string comparison loop.
- [ ] `[Tier 2: 3-Month]` [26. Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Remove Duplicates from Sorted Array: Standard 2-pointer overwrite or `std::unique`.
- [ ] `[Tier 1: 30-Day]` [28. Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Find the Index of the First Occurrence in a String: 1-line `s.find(needle)`.
- [ ] `[Tier 2: 3-Month]` [35. Search Insert Position](https://leetcode.com/problems/search-insert-position) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Search Insert Position: Standard library `std::lower_bound` call.
- [ ] `[Tier 2: 3-Month]` [67. Add Binary](https://leetcode.com/problems/add-binary) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Add Binary: Elementary grade-school carry addition loop from right to left.
- [ ] `[Tier 2: 3-Month]` [70. Climbing Stairs](https://leetcode.com/problems/climbing-stairs) — **Easy** (Freq: 62.5%)
  - *Why it's timepass:* Climbing Stairs: Exact identical duplicate of Fibonacci recurrence `dp[i] = dp[i-1] + dp[i-2]`.
- [ ] `[Tier 2: 3-Month]` [88. Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Merge Sorted Array: Standard backwards 3-pointer merge into array buffer.
- [ ] `[Tier 2: 3-Month]` [104. Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Maximum Depth of Binary Tree: 1-line recursive call `1 + max(depth(left), depth(right))`.
- [ ] `[Tier 1: 30-Day]` [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Pascal's Triangle: Basic nested simulation loop computing adjacent sum.
- [ ] `[Tier 1: 30-Day]` [136. Single Number](https://leetcode.com/problems/single-number) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Single Number: 1-line XOR accumulator `reduce(xor, nums)`.
- [ ] `[Tier 2: 3-Month]` [206. Reverse Linked List](https://leetcode.com/problems/reverse-linked-list) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Reverse Linked List: Textbook 3-pointer list traversal template (`prev, curr, next`).
- [ ] `[Tier 2: 3-Month]` [217. Contains Duplicate](https://leetcode.com/problems/contains-duplicate) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Contains Duplicate: 1-line hash set check `len(set(nums)) != len(nums)`.
- [ ] `[Tier 1: 30-Day]` [242. Valid Anagram](https://leetcode.com/problems/valid-anagram) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Valid Anagram: Direct 26-element character frequency array comparison.
- [ ] `[Tier 2: 3-Month]` [283. Move Zeroes](https://leetcode.com/problems/move-zeroes) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Move Zeroes: Elementary 2-pointer scan overwriting zeros then padding suffix.
- [ ] `[Tier 1: 30-Day]` [344. Reverse String](https://leetcode.com/problems/reverse-string) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Reverse String: 1-line standard library call `std::reverse(s.begin(), s.end())`.
- [ ] `[Tier 2: 3-Month]` [383. Ransom Note](https://leetcode.com/problems/ransom-note) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Ransom Note: Basic character count check using a 26-element array or `Counter`.
- [ ] `[Tier 2: 3-Month]` [448. Find All Numbers Disappeared in an Array](https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Find All Numbers Disappeared in an Array: Elementary index negation marker or hash set lookup.
- [ ] `[Tier 1: 30-Day]` [509. Fibonacci Number](https://leetcode.com/problems/fibonacci-number) — **Easy** (Freq: 62.5%)
  - *Why it's timepass:* Fibonacci Number: Elementary recurrence `F(n) = F(n-1) + F(n-2)`.
- [ ] `[Tier 2: 3-Month]` [643. Maximum Average Subarray I](https://leetcode.com/problems/maximum-average-subarray-i) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Maximum Average Subarray I: Elementary fixed-size sliding window accumulating sum.
- [ ] `[Tier 2: 3-Month]` [704. Binary Search](https://leetcode.com/problems/binary-search) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Binary Search: Textbook standard binary search template with `left <= right`.
- [ ] `[Tier 2: 3-Month]` [796. Rotate String](https://leetcode.com/problems/rotate-string) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Rotate String: 1-line string doubling trick `len(s) == len(goal) and goal in s + s`.
- [ ] `[Tier 2: 3-Month]` [1021. Remove Outermost Parentheses](https://leetcode.com/problems/remove-outermost-parentheses) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Remove Outermost Parentheses: Simple balance counter skipping outer layer.
- [ ] `[Tier 2: 3-Month]` [1365. How Many Numbers Are Smaller Than the Current Number](https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* How Many Numbers Are Smaller Than the Current Number: Counting sort or prefix frequency scan.
- [ ] `[Tier 2: 3-Month]` [1732. Find the Highest Altitude](https://leetcode.com/problems/find-the-highest-altitude) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Find the Highest Altitude: Running sum tracking maximum value seen so far.
- [ ] `[Tier 1: 30-Day]` [1768. Merge Strings Alternately](https://leetcode.com/problems/merge-strings-alternately) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Merge Strings Alternately: Simple alternating zipper loop joining two strings.
- [ ] `[Tier 1: 30-Day]` [1929. Concatenation of Array](https://leetcode.com/problems/concatenation-of-array) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Concatenation of Array: 1-line array concatenation `ans = nums + nums`.
- [ ] `[Tier 2: 3-Month]` [2418. Sort the People](https://leetcode.com/problems/sort-the-people) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Sort the People: Zip names with heights, sort descending by height.
- [ ] `[Tier 2: 3-Month]` [2553. Separate the Digits in an Array](https://leetcode.com/problems/separate-the-digits-in-an-array) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Separate the Digits in an Array: Convert numbers to strings or repeated modulo-10 division.
- [ ] `[Tier 2: 3-Month]` [2784. Check if Array is Good](https://leetcode.com/problems/check-if-array-is-good) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* Check if Array is Good: Count frequencies and verify permutation condition.
- [ ] `[Tier 2: 3-Month]` [3754. Concatenate Non-Zero Digits and Multiply by Sum I](https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-i) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Concatenate Non-Zero Digits and Multiply by Sum I: Trivial digit extraction and product multiplication.

---

## Part 2: Non-DSA SQL & Shell Tasks (10 Problems)

These belong to data engineering or IT screeners and do not appear in SWE coding assessments:

- [ ] `[Tier 2: 3-Month]` [175. Combine Two Tables](https://leetcode.com/problems/combine-two-tables) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* SQL: Basic LEFT JOIN between Person and Address tables.
- [ ] `[Tier 2: 3-Month]` [176. Second Highest Salary](https://leetcode.com/problems/second-highest-salary) — **Medium** (Freq: 50.0%)
  - *Why it's timepass:* SQL: SELECT max salary with OFFSET 1 or LIMIT 1 subquery.
- [ ] `[Tier 2: 3-Month]` [181. Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: Self-join to compare employee salary with manager salary.
- [ ] `[Tier 2: 3-Month]` [193. Valid Phone Numbers](https://leetcode.com/problems/valid-phone-numbers) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* Bash / Regex: grep -P standard phone number format `^(\([0-9]{3}\) |[0-9]{3}-)[0-9]{3}-[0-9]{4}$`.
- [ ] `[Tier 2: 3-Month]` [584. Find Customer Referee](https://leetcode.com/problems/find-customer-referee) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: Simple WHERE referee_id != 2 OR referee_id IS NULL.
- [ ] `[Tier 2: 3-Month]` [586. Customer Placing the Largest Number of Orders](https://leetcode.com/problems/customer-placing-the-largest-number-of-orders) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: GROUP BY customer_number ORDER BY count DESC LIMIT 1.
- [ ] `[Tier 2: 3-Month]` [620. Not Boring Movies](https://leetcode.com/problems/not-boring-movies) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: WHERE id % 2 = 1 AND description != "boring" ORDER BY rating DESC.
- [ ] `[Tier 2: 3-Month]` [1148. Article Views I](https://leetcode.com/problems/article-views-i) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: SELECT DISTINCT author_id WHERE author_id = viewer_id.
- [ ] `[Tier 2: 3-Month]` [1280. Students and Examinations](https://leetcode.com/problems/students-and-examinations) — **Easy** (Freq: 50.0%)
  - *Why it's timepass:* SQL: CROSS JOIN Students with Subjects, then LEFT JOIN with Examinations.
- [ ] `[Tier 2: 3-Month]` [1757. Recyclable and Low Fat Products](https://leetcode.com/problems/recyclable-and-low-fat-products) — **Easy** (Freq: 37.5%)
  - *Why it's timepass:* SQL: WHERE low_fats = "Y" AND recyclable = "Y".

---

## Part 3: Complete Pruned Problems Master Table

| Status | ID | Title | Difficulty | Tier | Freq % | Category | Quick Rationale |
| :---: | :---: | :--- | :---: | :---: | :---: | :--- | :--- |
| [ ] | 9 | [Palindrome Number](https://leetcode.com/problems/palindrome-number) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Palindrome Number: 1-line string reverse comparison or basic integer digit reversal. |
| [ ] | 13 | [Roman to Integer](https://leetcode.com/problems/roman-to-integer) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Roman to Integer: Elementary hash map lookup with subtractive condition `curr < next`. |
| [ ] | 14 | [Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix) | Easy | **Tier 1** | 62.5% | Trivial Easy DSA | Longest Common Prefix: Basic horizontal or vertical string comparison loop. |
| [ ] | 26 | [Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Remove Duplicates from Sorted Array: Standard 2-pointer overwrite or `std::unique`. |
| [ ] | 28 | [Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string) | Easy | **Tier 1** | 37.5% | Trivial Easy DSA | Find the Index of the First Occurrence in a String: 1-line `s.find(needle)`. |
| [ ] | 35 | [Search Insert Position](https://leetcode.com/problems/search-insert-position) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Search Insert Position: Standard library `std::lower_bound` call. |
| [ ] | 67 | [Add Binary](https://leetcode.com/problems/add-binary) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Add Binary: Elementary grade-school carry addition loop from right to left. |
| [ ] | 70 | [Climbing Stairs](https://leetcode.com/problems/climbing-stairs) | Easy | Tier 2 | 62.5% | Trivial Easy DSA | Climbing Stairs: Exact identical duplicate of Fibonacci recurrence `dp[i] = dp[i-1] + dp[i-2]`. |
| [ ] | 88 | [Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Merge Sorted Array: Standard backwards 3-pointer merge into array buffer. |
| [ ] | 104 | [Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Maximum Depth of Binary Tree: 1-line recursive call `1 + max(depth(left), depth(right))`. |
| [ ] | 118 | [Pascal's Triangle](https://leetcode.com/problems/pascals-triangle) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Pascal's Triangle: Basic nested simulation loop computing adjacent sum. |
| [ ] | 136 | [Single Number](https://leetcode.com/problems/single-number) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Single Number: 1-line XOR accumulator `reduce(xor, nums)`. |
| [ ] | 175 | [Combine Two Tables](https://leetcode.com/problems/combine-two-tables) | Easy | Tier 2 | 50.0% | Non-DSA (SQL/Shell) | SQL: Basic LEFT JOIN between Person and Address tables. |
| [ ] | 176 | [Second Highest Salary](https://leetcode.com/problems/second-highest-salary) | Medium | Tier 2 | 50.0% | Non-DSA (SQL/Shell) | SQL: SELECT max salary with OFFSET 1 or LIMIT 1 subquery. |
| [ ] | 181 | [Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: Self-join to compare employee salary with manager salary. |
| [ ] | 193 | [Valid Phone Numbers](https://leetcode.com/problems/valid-phone-numbers) | Easy | Tier 2 | 50.0% | Non-DSA (SQL/Shell) | Bash / Regex: grep -P standard phone number format `^(\([0-9]{3}\) |[0-9]{3}-)[0-9]{3}-[0-9]{4}$`. |
| [ ] | 206 | [Reverse Linked List](https://leetcode.com/problems/reverse-linked-list) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Reverse Linked List: Textbook 3-pointer list traversal template (`prev, curr, next`). |
| [ ] | 217 | [Contains Duplicate](https://leetcode.com/problems/contains-duplicate) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Contains Duplicate: 1-line hash set check `len(set(nums)) != len(nums)`. |
| [ ] | 242 | [Valid Anagram](https://leetcode.com/problems/valid-anagram) | Easy | **Tier 1** | 37.5% | Trivial Easy DSA | Valid Anagram: Direct 26-element character frequency array comparison. |
| [ ] | 283 | [Move Zeroes](https://leetcode.com/problems/move-zeroes) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Move Zeroes: Elementary 2-pointer scan overwriting zeros then padding suffix. |
| [ ] | 344 | [Reverse String](https://leetcode.com/problems/reverse-string) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Reverse String: 1-line standard library call `std::reverse(s.begin(), s.end())`. |
| [ ] | 383 | [Ransom Note](https://leetcode.com/problems/ransom-note) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Ransom Note: Basic character count check using a 26-element array or `Counter`. |
| [ ] | 448 | [Find All Numbers Disappeared in an Array](https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Find All Numbers Disappeared in an Array: Elementary index negation marker or hash set lookup. |
| [ ] | 509 | [Fibonacci Number](https://leetcode.com/problems/fibonacci-number) | Easy | **Tier 1** | 62.5% | Trivial Easy DSA | Fibonacci Number: Elementary recurrence `F(n) = F(n-1) + F(n-2)`. |
| [ ] | 584 | [Find Customer Referee](https://leetcode.com/problems/find-customer-referee) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: Simple WHERE referee_id != 2 OR referee_id IS NULL. |
| [ ] | 586 | [Customer Placing the Largest Number of Orders](https://leetcode.com/problems/customer-placing-the-largest-number-of-orders) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: GROUP BY customer_number ORDER BY count DESC LIMIT 1. |
| [ ] | 620 | [Not Boring Movies](https://leetcode.com/problems/not-boring-movies) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: WHERE id % 2 = 1 AND description != "boring" ORDER BY rating DESC. |
| [ ] | 643 | [Maximum Average Subarray I](https://leetcode.com/problems/maximum-average-subarray-i) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Maximum Average Subarray I: Elementary fixed-size sliding window accumulating sum. |
| [ ] | 704 | [Binary Search](https://leetcode.com/problems/binary-search) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Binary Search: Textbook standard binary search template with `left <= right`. |
| [ ] | 796 | [Rotate String](https://leetcode.com/problems/rotate-string) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Rotate String: 1-line string doubling trick `len(s) == len(goal) and goal in s + s`. |
| [ ] | 1021 | [Remove Outermost Parentheses](https://leetcode.com/problems/remove-outermost-parentheses) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Remove Outermost Parentheses: Simple balance counter skipping outer layer. |
| [ ] | 1148 | [Article Views I](https://leetcode.com/problems/article-views-i) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: SELECT DISTINCT author_id WHERE author_id = viewer_id. |
| [ ] | 1280 | [Students and Examinations](https://leetcode.com/problems/students-and-examinations) | Easy | Tier 2 | 50.0% | Non-DSA (SQL/Shell) | SQL: CROSS JOIN Students with Subjects, then LEFT JOIN with Examinations. |
| [ ] | 1365 | [How Many Numbers Are Smaller Than the Current Number](https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | How Many Numbers Are Smaller Than the Current Number: Counting sort or prefix frequency scan. |
| [ ] | 1732 | [Find the Highest Altitude](https://leetcode.com/problems/find-the-highest-altitude) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Find the Highest Altitude: Running sum tracking maximum value seen so far. |
| [ ] | 1757 | [Recyclable and Low Fat Products](https://leetcode.com/problems/recyclable-and-low-fat-products) | Easy | Tier 2 | 37.5% | Non-DSA (SQL/Shell) | SQL: WHERE low_fats = "Y" AND recyclable = "Y". |
| [ ] | 1768 | [Merge Strings Alternately](https://leetcode.com/problems/merge-strings-alternately) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Merge Strings Alternately: Simple alternating zipper loop joining two strings. |
| [ ] | 1929 | [Concatenation of Array](https://leetcode.com/problems/concatenation-of-array) | Easy | **Tier 1** | 50.0% | Trivial Easy DSA | Concatenation of Array: 1-line array concatenation `ans = nums + nums`. |
| [ ] | 2418 | [Sort the People](https://leetcode.com/problems/sort-the-people) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Sort the People: Zip names with heights, sort descending by height. |
| [ ] | 2553 | [Separate the Digits in an Array](https://leetcode.com/problems/separate-the-digits-in-an-array) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Separate the Digits in an Array: Convert numbers to strings or repeated modulo-10 division. |
| [ ] | 2784 | [Check if Array is Good](https://leetcode.com/problems/check-if-array-is-good) | Easy | Tier 2 | 37.5% | Trivial Easy DSA | Check if Array is Good: Count frequencies and verify permutation condition. |
| [ ] | 3754 | [Concatenate Non-Zero Digits and Multiply by Sum I](https://leetcode.com/problems/concatenate-non-zero-digits-and-multiply-by-sum-i) | Easy | Tier 2 | 50.0% | Trivial Easy DSA | Concatenate Non-Zero Digits and Multiply by Sum I: Trivial digit extraction and product multiplication. |

---

## Related Playbooks

- High-Yield Master List (124 Problems): [`consolidated_problems.md`](file:///Users/amankashyap/Documents/internship/DSA-Code/Microsoft/consolidated_problems.md)
- High-ROI Strategy & Sprint Guide: [`strategy.md`](file:///Users/amankashyap/Documents/internship/DSA-Code/Microsoft/strategy.md)
- Core 20 Invariant Cheatsheet: [`final.md`](file:///Users/amankashyap/Documents/internship/DSA-Code/Microsoft/final.md)
- Greedy & Interval Templates: [`greedy.md`](file:///Users/amankashyap/Documents/internship/DSA-Code/Microsoft/greedy.md)
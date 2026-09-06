# Antigravity Action Protocol & Standard Operating Procedure (SOP)

This document serves as the general Standard Operating Procedure (SOP) and engineering protocol for maintaining, implementing, reviewing, and extending algorithmic problem solutions across all repository suites.

---

## 1. Core Implementation & Coding Rules

Every C++ algorithmic implementation must strictly adhere to the following five rules:

### A. Strictly No Lambda Functions
- Never use lambda functions (`[&]`, `[=]`, `[]`, `[this]`, or `std::function`).
- Always declare standalone helper functions, named structs, or explicit comparator functions outside the main solution function.
- Rationale: Standalone functions provide clear call graphs, simplify debugging, and prevent hidden capture-overhead or stack-frame surprises during interviews.
- Example:
  ```cpp
  // INCORRECT (Lambda)
  sort(a.begin(), a.end(), [](const vi& x, const vi& y) { return x[1] < y[1]; });

  // CORRECT (Separate Named Comparator)
  bool compareByEnd(const vi& x, const vi& y) {
      return x[1] < y[1];
  }
  sort(a.begin(), a.end(), compareByEnd);
  ```

### B. Competitive Programming Variable Naming Standard
- Avoid verbose variable names inside algorithm logic; use concise, idiomatic competitive programming and interview identifiers:
  - Vectors / Arrays: `a`, `b`, `g` (grid), `m` (matrix)
  - Dimensions & Sizes: `n = a.size()`, `m = g.size()`, `sz`
  - Targets & Constraints: `t` (target), `k` (limit / count)
  - Search & Pointers: `l` / `r` (or `lo` / `hi`), `mid`, `cur`, `nxt`, `prev`
  - Linked Lists: `cur`, `prev`, `nxt`, `d` (sentinel dummy node)
  - Data Structures: `vis` (visited), `st` (set/seen), `mp` (map/hash table), `dq` (deque), `pq` (priority queue), `dll` (doubly linked list)
  - Accumulators & Extremes: `ans`, `res`, `cnt`, `tot`, `mx`, `mn`
  - Graphs: `u`, `v`, `w`, `deg`, `adj`, `par`, `dist`

### C. Standardized 4-Part Interview Explanation Block
Every problem must conclude with a standardized explanation block where the `Intuition` section is expanded into **3 to 5 structured bullet points** (`* ` prefix):
```cpp
// ============================================================
// N. PROBLEM TITLE (LEETCODE / CSES / GFG #)
// ============================================================
[Standalone helper functions / structs if needed]

ReturnType solutionFunction(...) {
    // Implementation with short variables
}
// Interview Explanation:
// - Problem Statement: Brief 1-line description of the problem.
// - Approach: High-level algorithmic pattern (e.g., Two Pointers, Monotonic Stack, Bitmask DP).
// - Intuition:
//   * Bottleneck / Core Observation: Why naive approaches fail and the critical insight.
//   * Mathematical / Structural Invariant: Underlying invariant (e.g., monotonicity, cycle LCM, DAG reachability).
//   * Step-by-Step Execution: How pointers, states, and data structures progress mechanistically.
//   * Edge Cases & Pruning: Handling boundary conditions, empty inputs, duplicate elements, or overflow guards.
// - Complexity: Time: O(...), Space: O(...).
```

### D. Zero Emojis Standard
- Strictly no emojis anywhere in code comments, banner headers, markdown documents, or commit messages.
- Maintain a clean, professional, interview-grade codebase.

### E. C++17 Standard & Compatibility
- Platform-safe include guards with fallback headers:
  ```cpp
  #if __has_include(<bits/stdc++.h>)
  #include <bits/stdc++.h>
  #else
  #include <iostream>
  #include <vector>
  // ... standard fallbacks
  #endif
  ```
- Standard type aliases and constants:
  ```cpp
  using ll = long long;
  using pii = pair<int, int>;
  using vi = vector<int>;
  using vl = vector<ll>;
  using vvi = vector<vector<int>>;
  using vvl = vector<vector<ll>>;
  const ll MOD = 1e9 + 7;
  ```

---

## 2. File Header Summary Tables

Every problem source file must maintain a 5-column Markdown complexity table at the top:

```
/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 1  | ...                                         | ...                               | ...      | ...      |
 ====================================================================================================
*/
```

### Invariants:
1. **Sequential Indexing**: Always maintain 1-based indexing (`1..N`) without gaps or duplicate numbers.
2. **Alignment**: Keep column pipes `|` neatly formatted.
3. **Exact Matching**: Problem numbers in the header banner `// N. PROBLEM_NAME` must match the table row `#`.

---

## 3. Deduplication & Categorization Guidelines

- **Core Modules**: Foundational, topic-wise standard algorithms (e.g., standard traversals, basic trees, classical DP).
- **Notebooks**: Advanced, niche, and competitive programming problems (e.g., CSES problem sets, Matrix Exponentiation, Slope Trick, Advanced Intervals, Low-Level System Design).
- **Company / Targeted Suites**: Curated, high-yield interview problems specific to company question banks and live interview tracks.
- **Deduplication Rule**: If a problem already exists in a core module, it must not be duplicated in another suite unless implemented with a distinctly different algorithmic technique.

---

## 4. Verification Protocol

Before finishing any change, verify the code using strict compilation and consistency checks:

1. **Compilation Check**:
   ```bash
   clang++ -std=c++17 -fsyntax-only <filepath>
   # or
   g++ -std=c++17 -fsyntax-only <filepath>
   ```

2. **Batch Compilation Check**:
   ```bash
   for f in path/to/*.cpp; do echo -n "$f: "; clang++ -std=c++17 -fsyntax-only "$f" && echo "OK" || echo "FAIL"; done
   ```

3. **Zero-Lambda Check**:
   ```bash
   # Must return exit code 1 (0 matches)
   grep -E '(\[&\]|\[=\]|\[\]|\[this\]|\[\w+\]\(|function<)' <filepath>
   ```

4. **Zero-Emoji Check**:
   ```bash
   python3 -c "
   import glob
   for f in glob.glob('**/*.cpp', recursive=True):
       with open(f, 'r', encoding='utf-8') as fp:
           for line_no, line in enumerate(fp, 1):
               for ch in line:
                   if 0x1F300 <= ord(ch) <= 0x1FAFF or 0x2600 <= ord(ch) <= 0x27BF:
                       print(f'{f}:{line_no}: Found emoji {ch}')
   "
   ```

---

## 5. Reusable Prompt Templates for Future Sessions

Copy and paste these standard prompt templates when working across sessions:

### Prompt 1: Add a New Problem
> "Add [Problem Name] (LeetCode [Number] / Source) to [target file]. Follow the repository SOP: (1) no lambda functions (write standalone helper/comparator functions), (2) short competitive programming variables, (3) 4-part Interview Explanation block with 3-5 point intuition bullets, (4) zero emojis, (5) update the top header table, and (6) verify with `clang++ -std=c++17 -fsyntax-only`."

### Prompt 2: Refactor Code to General SOP
> "Refactor [target file] to match our SOP: (1) convert variables to clean competitive programming names, (2) expand `// - Intuition:` into 3-5 structured speaking points with `* ` prefixes, (3) replace any lambdas with standalone functions, (4) remove any emojis, and (5) verify compilation with `clang++ -std=c++17 -fsyntax-only`."

### Prompt 3: Audit & Synchronize Summary Tables
> "Audit and synchronize the header summary table in [target file] and master catalogs. Ensure sequential 1..N numbering, correct column alignments, and verify compilation using `clang++ -std=c++17 -fsyntax-only`."

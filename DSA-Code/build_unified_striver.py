import re

# 1. Parse all 191 problems from SDE sheet payload
with open('/Users/amankashyap/.gemini/antigravity-ide/brain/53aa20df-9c9c-4157-a154-c42aed3b72e4/.system_generated/steps/2216/content.md') as f:
    raw = f.read()

chunks = re.findall(r'self\.__next_f\.push\(\[1,\"(.*?)\"\]\)', raw, re.DOTALL)
full_text = ''.join(chunks).encode('utf-8').decode('unicode_escape')
sec_blocks = re.findall(r'\"category_name\":\"([^\"]+)\",\"problems\":\[(.*?)\](?=,\{|\}\])', full_text)

sde_problems = []
for cat, p_block in sec_blocks:
    items = re.findall(r'\"problem_id\":\"([^\"]+)\",\"problem_name\":\"([^\"]+)\",\"article\":\"([^\"]*)\",\"youtube\":\"([^\"]*)\",\"leetcode\":\"([^\"]*)\"', p_block)
    for pid, pname, art, yt, lc in items:
        link = lc if lc and lc != '$undefined' else (art if art and art != '$undefined' else '')
        sde_problems.append((pname.strip(), link.strip(), cat.strip()))

# 2. Striver 79
striver_79 = [
    ('Next Permutation', 'https://leetcode.com/problems/next-permutation/'),
    ('3 Sum', 'https://leetcode.com/problems/3sum/'),
    ('Kadane\'s Algorithm - Maximum Subarray', 'https://leetcode.com/problems/maximum-subarray/'),
    ('Majority Element-II (> N/3)', 'https://leetcode.com/problems/majority-element-ii/'),
    ('Count Subarrays with Given XOR K', 'https://www.geeksforgeeks.org/problems/count-subarray-with-given-xor/1'),
    ('Find the Repeating and Missing Number', 'https://www.geeksforgeeks.org/problems/find-missing-and-repeating2512/1'),
    ('Count Inversions', 'https://www.geeksforgeeks.org/problems/inversion-of-array-1587115620/1'),
    ('Maximum Product Subarray in an Array', 'https://leetcode.com/problems/maximum-product-subarray/'),
    ('Search in Rotated Sorted Array-II (with duplicates)', 'https://leetcode.com/problems/search-in-rotated-sorted-array-ii/'),
    ('Find Minimum in Rotated Sorted Array', 'https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/'),
    ('Find Peak Element', 'https://leetcode.com/problems/find-peak-element/'),
    ('Koko Eating Bananas', 'https://leetcode.com/problems/koko-eating-bananas/'),
    ('Aggressive Cows', 'https://www.spoj.com/problems/AGGRCOW/'),
    ('Book Allocation Problem', 'https://www.geeksforgeeks.org/problems/allocate-minimum-number-of-pages0937/1'),
    ('Median of 2 Sorted Arrays', 'https://leetcode.com/problems/median-of-two-sorted-arrays/'),
    ('Minimize Max Distance to Gas Station', 'https://leetcode.com/problems/minimize-max-distance-to-gas-station/'),
    ('Middle of a LinkedList [TortoiseHare Method]', 'https://leetcode.com/problems/middle-of-the-linked-list/'),
    ('Detect a Loop in LL', 'https://leetcode.com/problems/linked-list-cycle/'),
    ('Remove Nth Node From the Back of the LL', 'https://leetcode.com/problems/remove-nth-node-from-end-of-list/'),
    ('Find the Intersection Point of Y LL', 'https://leetcode.com/problems/intersection-of-two-linked-lists/'),
    ('Sort LL (Merge Sort on Linked List)', 'https://leetcode.com/problems/sort-list/'),
    ('Segregate Odd and Even Nodes in Linked List', 'https://leetcode.com/problems/odd-even-linked-list/'),
    ('Power Set / All Subsets', 'https://leetcode.com/problems/subsets/'),
    ('Combination Sum', 'https://leetcode.com/problems/combination-sum/'),
    ('N Queen', 'https://leetcode.com/problems/n-queens/'),
    ('Sudoku Solver', 'https://leetcode.com/problems/sudoku-solver/'),
    ('M Coloring Problem', 'https://www.geeksforgeeks.org/problems/m-coloring-problem-1587115620/1'),
    ('Word Search', 'https://leetcode.com/problems/word-search/'),
    ('Next Greater Element', 'https://leetcode.com/problems/next-greater-element-i/'),
    ('Trapping Rainwater', 'https://leetcode.com/problems/trapping-rain-water/'),
    ('Largest Rectangle in a Histogram', 'https://leetcode.com/problems/largest-rectangle-in-histogram/'),
    ('Asteroid Collision', 'https://leetcode.com/problems/asteroid-collision/'),
    ('Sliding Window Maximum', 'https://leetcode.com/problems/sliding-window-maximum/'),
    ('LRU Cache', 'https://leetcode.com/problems/lru-cache/'),
    ('K-th Largest Element in an Array', 'https://leetcode.com/problems/kth-largest-element-in-an-array/'),
    ('Task Scheduler', 'https://leetcode.com/problems/task-scheduler/'),
    ('Implement Min Heap', 'https://www.geeksforgeeks.org/problems/operations-on-binary-min-heap/1'),
    ('Diameter of Binary Tree', 'https://leetcode.com/problems/diameter-of-binary-tree/'),
    ('Maximum Path Sum in Binary Tree', 'https://leetcode.com/problems/binary-tree-maximum-path-sum/'),
    ('Bottom View of Binary Tree', 'https://www.geeksforgeeks.org/problems/bottom-view-of-binary-tree/1'),
    ('Lowest Common Ancestor in Binary Tree', 'https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/'),
    ('Minimum Time Taken to Burn the Binary Tree from a Given Node', 'https://www.geeksforgeeks.org/problems/burning-tree/1'),
    ('Construct a Binary Tree from Preorder and Inorder', 'https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/'),
    ('Morris Preorder Traversal of a Binary Tree', 'https://leetcode.com/problems/binary-tree-preorder-traversal/'),
    ('Delete a Node in BST', 'https://leetcode.com/problems/delete-node-in-a-bst/'),
    ('Lowest Common Ancestor in BST', 'https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/'),
    ('Two Sum In BST | Pair with Sum K', 'https://leetcode.com/problems/two-sum-iv-input-is-a-bst/'),
    ('Largest BST in Binary Tree', 'https://leetcode.com/problems/maximum-sum-bst-in-binary-tree/'),
    ('Rotten Oranges', 'https://leetcode.com/problems/rotting-oranges/'),
    ('Word Ladder I', 'https://leetcode.com/problems/word-ladder/'),
    ('Number of Islands', 'https://leetcode.com/problems/number-of-islands/'),
    ('Course Schedule II', 'https://leetcode.com/problems/course-schedule-ii/'),
    ('Alien Dictionary', 'https://leetcode.com/problems/alien-dictionary/'),
    ('Dijkstra\'s Algorithm (Single Source Shortest Path)', 'https://www.geeksforgeeks.org/problems/implementing-dijkstra-set-1-adjacency-matrix/1'),
    ('Cheapest Flights Within K Stops', 'https://leetcode.com/problems/cheapest-flights-within-k-stops/'),
    ('Bellman Ford Algorithm', 'https://www.geeksforgeeks.org/problems/distance-from-the-source-bellman-ford-algorithm/1'),
    ('Floyd Warshall Algorithm', 'https://www.geeksforgeeks.org/problems/implementing-floyd-warshall2042/1'),
    ('Find MST Weight (Prim\'s & Kruskal\'s Algorithm)', 'https://www.geeksforgeeks.org/problems/minimum-spanning-tree/1'),
    ('Accounts Merge', 'https://leetcode.com/problems/accounts-merge/'),
    ('Bridges in Graph / Critical Connections', 'https://leetcode.com/problems/critical-connections-in-a-network/'),
    ('Maximum Sum of Non-Adjacent Elements / House Robber', 'https://leetcode.com/problems/house-robber/'),
    ('Ninja\'s Training (2D DP)', 'https://www.geeksforgeeks.org/problems/geeks-training/1'),
    ('Minimum Path Sum in Grid', 'https://leetcode.com/problems/minimum-path-sum/'),
    ('Subset Sum Equal to Target', 'https://www.geeksforgeeks.org/problems/subset-sum-problem-1611555638/1'),
    ('Assign Cookies', 'https://leetcode.com/problems/assign-cookies/'),
    ('Rod Cutting Problem', 'https://www.geeksforgeeks.org/problems/rod-cutting0840/1'),
    ('Longest Common Subsequence', 'https://leetcode.com/problems/longest-common-subsequence/'),
    ('Longest Palindromic Subsequence', 'https://leetcode.com/problems/longest-palindromic-subsequence/'),
    ('Edit Distance', 'https://leetcode.com/problems/edit-distance/'),
    ('Best Time to Buy and Sell Stock IV', 'https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/'),
    ('Longest Increasing Subsequence', 'https://leetcode.com/problems/longest-increasing-subsequence/'),
    ('Burst Balloons', 'https://leetcode.com/problems/burst-balloons/'),
    ('Trie Implementation and Advanced Operations (Trie II)', 'https://leetcode.com/problems/implement-trie-prefix-tree/'),
    ('Maximum XOR with an Element from Array', 'https://leetcode.com/problems/maximum-xor-with-an-element-from-array/'),
    ('Number of Distinct Substrings in a String', 'https://www.geeksforgeeks.org/problems/count-of-distinct-substrings/1'),
    ('Minimum Bracket Reversals to Balance Expression', 'https://www.geeksforgeeks.org/problems/count-the-reversals0401/1'),
    ('Rabin Karp Algorithm (Rolling Hash Pattern Matching)', 'https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/'),
    ('Z-Function / Z-Algorithm', 'https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/'),
    ('KMP Algorithm / LPS Array (Knuth-Morris-Pratt)', 'https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/')
]

# 3. Blind 75
blind_75 = [
    ('Two Sum', 'https://leetcode.com/problems/two-sum/'),
    ('Best Time to Buy and Sell Stock', 'https://leetcode.com/problems/best-time-to-buy-and-sell-stock/'),
    ('Contains Duplicate', 'https://leetcode.com/problems/contains-duplicate/'),
    ('Product of Array Except Self', 'https://leetcode.com/problems/product-of-array-except-self/'),
    ('Maximum Subarray (Kadane\'s)', 'https://leetcode.com/problems/maximum-subarray/'),
    ('Maximum Product Subarray', 'https://leetcode.com/problems/maximum-product-subarray/'),
    ('Find Minimum in Rotated Sorted Array', 'https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/'),
    ('Search in Rotated Sorted Array', 'https://leetcode.com/problems/search-in-rotated-sorted-array/'),
    ('3Sum', 'https://leetcode.com/problems/3sum/'),
    ('Container With Most Water', 'https://leetcode.com/problems/container-with-most-water/'),
    ('Sum of Two Integers', 'https://leetcode.com/problems/sum-of-two-integers/'),
    ('Number of 1 Bits (Hamming Weight)', 'https://leetcode.com/problems/number-of-1-bits/'),
    ('Counting Bits', 'https://leetcode.com/problems/counting-bits/'),
    ('Missing Number', 'https://leetcode.com/problems/missing-number/'),
    ('Reverse Bits', 'https://leetcode.com/problems/reverse-bits/'),
    ('Climbing Stairs', 'https://leetcode.com/problems/climbing-stairs/'),
    ('Coin Change', 'https://leetcode.com/problems/coin-change/'),
    ('Longest Increasing Subsequence', 'https://leetcode.com/problems/longest-increasing-subsequence/'),
    ('Longest Common Subsequence', 'https://leetcode.com/problems/longest-common-subsequence/'),
    ('Word Break', 'https://leetcode.com/problems/word-break/'),
    ('Combination Sum', 'https://leetcode.com/problems/combination-sum/'),
    ('House Robber', 'https://leetcode.com/problems/house-robber/'),
    ('House Robber II', 'https://leetcode.com/problems/house-robber-ii/'),
    ('Decode Ways', 'https://leetcode.com/problems/decode-ways/'),
    ('Unique Paths', 'https://leetcode.com/problems/unique-paths/'),
    ('Jump Game', 'https://leetcode.com/problems/jump-game/'),
    ('Clone Graph', 'https://leetcode.com/problems/clone-graph/'),
    ('Course Schedule', 'https://leetcode.com/problems/course-schedule/'),
    ('Pacific Atlantic Water Flow', 'https://leetcode.com/problems/pacific-atlantic-water-flow/'),
    ('Number of Islands', 'https://leetcode.com/problems/number-of-islands/'),
    ('Longest Consecutive Sequence', 'https://leetcode.com/problems/longest-consecutive-sequence/'),
    ('Alien Dictionary', 'https://leetcode.com/problems/alien-dictionary/'),
    ('Graph Valid Tree', 'https://leetcode.com/problems/graph-valid-tree/'),
    ('Number of Connected Components in an Undirected Graph', 'https://leetcode.com/problems/number-of-connected-components-in-an-undirected-graph/'),
    ('Insert Interval', 'https://leetcode.com/problems/insert-interval/'),
    ('Merge Intervals', 'https://leetcode.com/problems/merge-intervals/'),
    ('Non-overlapping Intervals', 'https://leetcode.com/problems/non-overlapping-intervals/'),
    ('Meeting Rooms', 'https://leetcode.com/problems/meeting-rooms/'),
    ('Meeting Rooms II', 'https://leetcode.com/problems/meeting-rooms-ii/'),
    ('Reverse Linked List', 'https://leetcode.com/problems/reverse-linked-list/'),
    ('Linked List Cycle', 'https://leetcode.com/problems/linked-list-cycle/'),
    ('Merge Two Sorted Lists', 'https://leetcode.com/problems/merge-two-sorted-lists/'),
    ('Merge k Sorted Lists', 'https://leetcode.com/problems/merge-k-sorted-lists/'),
    ('Remove Nth Node From End of List', 'https://leetcode.com/problems/remove-nth-node-from-end-of-list/'),
    ('Reorder List', 'https://leetcode.com/problems/reorder-list/'),
    ('Set Matrix Zeroes', 'https://leetcode.com/problems/set-matrix-zeroes/'),
    ('Spiral Matrix', 'https://leetcode.com/problems/spiral-matrix/'),
    ('Rotate Image', 'https://leetcode.com/problems/rotate-image/'),
    ('Word Search', 'https://leetcode.com/problems/word-search/'),
    ('Longest Substring Without Repeating Characters', 'https://leetcode.com/problems/longest-substring-without-repeating-characters/'),
    ('Longest Repeating Character Replacement', 'https://leetcode.com/problems/longest-repeating-character-replacement/'),
    ('Minimum Window Substring', 'https://leetcode.com/problems/minimum-window-substring/'),
    ('Valid Anagram', 'https://leetcode.com/problems/valid-anagram/'),
    ('Group Anagrams', 'https://leetcode.com/problems/group-anagrams/'),
    ('Valid Parentheses', 'https://leetcode.com/problems/valid-parentheses/'),
    ('Valid Palindrome', 'https://leetcode.com/problems/valid-palindrome/'),
    ('Longest Palindromic Substring', 'https://leetcode.com/problems/longest-palindromic-substring/'),
    ('Palindromic Substrings', 'https://leetcode.com/problems/palindromic-substrings/'),
    ('Encode and Decode Strings', 'https://leetcode.com/problems/encode-and-decode-strings/'),
    ('Maximum Depth of Binary Tree', 'https://leetcode.com/problems/maximum-depth-of-binary-tree/'),
    ('Same Tree', 'https://leetcode.com/problems/same-tree/'),
    ('Invert Binary Tree', 'https://leetcode.com/problems/invert-binary-tree/'),
    ('Binary Tree Maximum Path Sum', 'https://leetcode.com/problems/binary-tree-maximum-path-sum/'),
    ('Binary Tree Level Order Traversal', 'https://leetcode.com/problems/binary-tree-level-order-traversal/'),
    ('Serialize and Deserialize Binary Tree', 'https://leetcode.com/problems/serialize-and-deserialize-binary-tree/'),
    ('Subtree of Another Tree', 'https://leetcode.com/problems/subtree-of-another-tree/'),
    ('Construct Binary Tree from Preorder and Inorder Traversal', 'https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/'),
    ('Validate Binary Search Tree', 'https://leetcode.com/problems/validate-binary-search-tree/'),
    ('Kth Smallest Element in a BST', 'https://leetcode.com/problems/kth-smallest-element-in-a-bst/'),
    ('Lowest Common Ancestor of a Binary Search Tree', 'https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/'),
    ('Implement Trie (Prefix Tree)', 'https://leetcode.com/problems/implement-trie-prefix-tree/'),
    ('Design Add and Search Words Data Structure', 'https://leetcode.com/problems/design-add-and-search-words-data-structure/'),
    ('Word Search II', 'https://leetcode.com/problems/word-search-ii/'),
    ('Top K Frequent Elements', 'https://leetcode.com/problems/top-k-frequent-elements/'),
    ('Find Median from Data Stream', 'https://leetcode.com/problems/find-median-from-data-stream/')
]

# Manual canonical aliases map
aliases = {
    '3sum': '3sum',
    '3 sum': '3sum',
    '4sum': '4sum',
    '4 sum': '4sum',
    'kadane s algorithm maximum subarray': 'maximum-subarray',
    'maximum subarray kadane s': 'maximum-subarray',
    'maximum subarray': 'maximum-subarray',
    'pascals triangle': 'pascals-triangle',
    'pascals triangle i': 'pascals-triangle',
    'majority element i': 'majority-element',
    'majority element': 'majority-element',
    'majority element ii': 'majority-element-ii',
    'majority element ii n 3': 'majority-element-ii',
    'find the duplicate number': 'find-the-duplicate-number',
    'sort an array of 0 s 1 s and 2 s': 'sort-colors',
    'sort colors': 'sort-colors',
    'merge two sorted arrays without extra space': 'merge-sorted-array',
    'two sum in bst pair with sum k': 'two-sum-iv-input-is-a-bst',
    'two sum iv input is a bst': 'two-sum-iv-input-is-a-bst',
    'lowest common ancestor in bst': 'lowest-common-ancestor-of-a-binary-search-tree',
    'lowest common ancestor of a binary search tree': 'lowest-common-ancestor-of-a-binary-search-tree',
    'lowest common ancestor in binary tree': 'lowest-common-ancestor-of-a-binary-tree',
    'lowest common ancestor of a binary tree': 'lowest-common-ancestor-of-a-binary-tree',
    'mst using prim s algo': 'prims-mst',
    'mst using kruskal s algo': 'kruskals-mst',
    'find mst weight prim s kruskal s algorithm': 'minimum-spanning-tree',
    'power set all subsets': 'subsets',
    'power set this is very important': 'subsets',
    'power set': 'subsets',
    'longest consecutive sequence': 'longest-consecutive-sequence',
    'longest consecutive sequence in an array': 'longest-consecutive-sequence',
    'maximum sum of non adjacent elements house robber': 'house-robber',
    'house robber': 'house-robber',
    'rotting oranges': 'rotting-oranges',
    'rotten oranges': 'rotting-oranges',
    'word ladder i': 'word-ladder',
    'word ladder': 'word-ladder',
    'alien dictionary': 'alien-dictionary',
    'course schedule ii': 'course-schedule-ii',
    'course schedule': 'course-schedule',
    'bridges in graph critical connections': 'critical-connections-in-a-network',
    'critical connections in a network': 'critical-connections-in-a-network',
    'trie implementation and advanced operations trie ii': 'implement-trie-ii',
    'implement trie prefix tree': 'implement-trie-prefix-tree',
    'rabin karp algorithm rolling hash pattern matching': 'rabin-karp',
    'kmp algorithm lps array knuth morris pratt': 'kmp-search',
    'z function z algorithm': 'z-algorithm',
    'z function': 'z-algorithm',
    'z algorithm': 'z-algorithm'
}

def clean_text(s):
    s = s.replace('Î“Ã‡Ã–', "'").replace('â€™', "'").replace('Â', '')
    s = re.sub(r'\(.*?\)', '', s)
    s = re.sub(r'\[.*?\]', '', s)
    s = re.sub(r'[^a-zA-Z0-9 ]', ' ', s)
    return ' '.join(s.lower().split())

def get_key(name, url):
    if url:
        m = re.search(r'leetcode\.com/problems/([^/#?]+)', url)
        if m:
            slug = m.group(1).lower().rstrip('/')
            if slug in ['3sum', '3-sum']: return '3sum'
            if slug in ['4sum', '4-sum']: return '4sum'
            if slug in ['pascals-triangle', 'pascals-triangle-i']: return 'pascals-triangle'
            if slug in ['reverse-words-in-a-string', 'reverse-words']: return 'reverse-words-in-a-string'
            return slug
    ct = clean_text(name)
    if ct in aliases:
        return aliases[ct]
    return ct.replace(' ', '-')

# Build master map: key -> {'name': ..., 'url': ..., 'sources': set()}
master = {}

def insert_problem(name, url, src):
    key = get_key(name, url)
    if key not in master:
        master[key] = {
            'name': name,
            'url': url,
            'sources': {src}
        }
    else:
        master[key]['sources'].add(src)
        # Update URL if current is empty or less specific
        if not master[key]['url'] or 'takeuforward' in master[key]['url']:
            if url: master[key]['url'] = url
        # Keep shorter/cleaner display name if available
        if len(name) < len(master[key]['name']) and not name.startswith('LeetCode') and len(name) > 3:
            master[key]['name'] = name

for n, u, _ in sde_problems: insert_problem(n, u, 'SDE Sheet')
for n, u in striver_79: insert_problem(n, u, 'Striver 79')
for n, u in blind_75: insert_problem(n, u, 'Blind 75')

print(f'Total Unified Unique Problems: {len(master)}')

# Strict validation: verify that EVERY problem in all 3 input lists is mapped to an entry in master
for src_name, src_list in [('SDE Sheet', [(n, u) for n, u, _ in sde_problems]), ('Striver 79', striver_79), ('Blind 75', blind_75)]:
    missing = []
    for n, u in src_list:
        k = get_key(n, u)
        if k not in master:
            missing.append(n)
    print(f'Loss verification for {src_name}: {len(missing)} missing (Target: 0)')
    if missing:
        print('  Missing items:', missing)


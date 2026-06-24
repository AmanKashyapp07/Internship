/**
 * CSES Problem Set / LeetCode / Striver A2Z DSA Sheet
 *
 * Topic: Binary Search - 1D Arrays
 * Description: Contains optimized solutions for all 13 problems listed in the sheet.
 */

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define ff first
#define ss second

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

// 1. Search X in sorted array
bool searchXinSortedArray(const vector<int>& arr, int x) {
    return binary_search(all(arr), x);
}

int lb_idx = lower_bound(all(arr), x) - arr.begin();
// Smallest index where arr[idx] >= x
// If not exists, lb_idx == arr.size()

int ub_idx = upper_bound(all(arr), x) - arr.begin();
// Smallest index where arr[idx] > x
// If not exists, ub_idx == arr.size()

int idx = upper_bound(all(arr), x) - arr.begin() - 1;
// Largest index where arr[idx] <= x
// If not exists, idx == -1

int idx2 = lower_bound(all(arr), x) - arr.begin() - 1;
// Largest index where arr[idx] < x
// If not exists, idx2 == -1
// 4. Search insert position (Same logic as lower bound)


int searchInsertPosition(const vector<int>& arr, int x) {
    return (int)(lower_bound(all(arr), x) - arr.begin());
}



// 6. First and last occurrence
pair<int, int> firstAndLastOccurrence(const vector<int>& arr, int x) {
    auto first_it = lower_bound(all(arr), x);
    if (first_it == arr.end() || *first_it != x) {
        return {-1, -1}; // element not present
    }
    auto last_it = upper_bound(all(arr), x);
    int first_idx = (int)(first_it - arr.begin());
    int last_idx = (int)(last_it - arr.begin()) - 1;
    return {first_idx, last_idx};
}

// 7. Count Occurrences in a Sorted Array
int countOccurrences(const vector<int>& arr, int x) {
    auto p = firstAndLastOccurrence(arr, x);
    if (p.first == -1) return 0;
    return p.second - p.first + 1;
}

// 8. Search in rotated sorted array-I (Unique elements)
int searchInRotatedSortedArrayI(const vector<int>& arr, int target) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        
        // Left half is normally ordered
        if (arr[low] <= arr[mid]) {
            // Target lies strictly within the sorted left half
            if (target >= arr[low] && target < arr[mid]) {
                high = mid - 1;
            } else { // Target is in the right un-sorted/rotated half
                low = mid + 1;
            }
        } 
        // Right half is normally ordered
        else {
            // Target lies strictly within the sorted right half
            if (target > arr[mid] && target <= arr[high]) {
                low = mid + 1;
            } else { // Target is in the left un-sorted/rotated half
                high = mid - 1;
            }
        }
    }
    return -1;
}

// 9. Search in rotated sorted array-II (Contains duplicates)
bool searchInRotatedSortedArrayII(const vector<int>& arr, int target) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return true;
        
        // Edge case: [3, 1, 2, 3, 3, 3, 3]. Cannot determine which half is sorted.
        if (arr[low] == arr[mid] && arr[mid] == arr[high]) {
            low++;  // Shrink search bounds safely
            high--; 
            continue;
        }
        
        // Identical binary search logic to Problem I from this point onwards
        if (arr[low] <= arr[mid]) {
            if (target >= arr[low] && target < arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } 
        else {
            if (target > arr[mid] && target <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return false;
}

// 10. Find minimum in Rotated Sorted Array
int findMinInRotatedSortedArray(const vector<int>& arr) {
    int low = 0, high = arr.size() - 1;
    int ans = INF;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Entire current subarray is already sorted; left-most is the local minimum
        if (arr[low] <= arr[high]) {
            ans = min(ans, arr[low]);
            break;
        }
        
        // Left half is sorted; minimum must be either arr[low] or located in right half
        if (arr[low] <= arr[mid]) {
            ans = min(ans, arr[low]);
            low = mid + 1;
        } 
        // Right half is sorted; minimum must be either arr[mid] or located in left half
        else {
            ans = min(ans, arr[mid]);
            high = mid - 1;
        }
    }
    return ans;
}

// 11. Find out how many times the array is rotated
int findRotationCount(const vector<int>& arr) {
    int low = 0, high = arr.size() - 1;
    int min_val = INF;
    int min_idx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Same logic as finding the minimum, but tracking the index instead of value
        if (arr[low] <= arr[high]) {
            if (arr[low] < min_val) {
                min_val = arr[low];
                min_idx = low; // Rotation count equals the index of smallest element
            }
            break;
        }
        
        if (arr[low] <= arr[mid]) {
            if (arr[low] < min_val) {
                min_val = arr[low];
                min_idx = low;
            }
            low = mid + 1;
        } else {
            if (arr[mid] < min_val) {
                min_val = arr[mid];
                min_idx = mid;
            }
            high = mid - 1;
        }
    }
    return min_idx;
}

// 12. Single element in a Sorted Array (Every other element appears twice)
int singleNonDuplicate(const vector<int>& arr) {
    int n = arr.size();
    if (n == 1) return arr[0];
    if (arr[0] != arr[1]) return arr[0];        // Base case: first element is unique
    if (arr[n - 1] != arr[n - 2]) return arr[n - 1]; // Base case: last element is unique
    
    int low = 1, high = n - 2; // Trim bounds to safely check mid-1 and mid+1
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Element distinct from both its neighbors is our target
        if (arr[mid] != arr[mid - 1] && arr[mid] != arr[mid + 1]) {
            return arr[mid];
        }
        
        // Before unique element, pairs start at even indices: (even, odd) -> (0,1), (2,3)
        // If mid is odd and matches left, or mid is even and matches right, we are on the left side
        if ((mid % 2 == 1 && arr[mid] == arr[mid - 1]) || 
            (mid % 2 == 0 && arr[mid] == arr[mid + 1])) {
            low = mid + 1; // Unique element lies further to the right
        } else {
            high = mid - 1; // Unique element lies to the left
        }
    }
    return -1;
}

// 13. Find peak element (Local maxima where arr[i] > arr[i-1] and arr[i] > arr[i+1])
int findPeakElement(const vector<int>& arr) {
    int n = arr.size();
    if (n == 1) return 0;
    if (arr[0] > arr[1]) return 0;        // Base case: left edge is a peak
    if (arr[n - 1] > arr[n - 2]) return n - 1; // Base case: right edge is a peak
    
    int low = 1, high = n - 2; // Trim bounds to safely check mid-1 and mid+1
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Peak condition satisfied
        if (arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) {
            return mid;
        }
        
        // If array is increasing at mid, at least one peak must exist on the right slope
        if (arr[mid] > arr[mid - 1]) {
            low = mid + 1;
        } else { // Array is decreasing at mid; a peak must exist on the left slope
            high = mid - 1;
        }
    }
    return -1;
}

// 14. kth missing positive number
int findKthPositive(vector<int>& arr, int k) {
        int n = arr.size();
        int low = 0, high = n - 1;
        int idx = -1; 

        while (low <= high) {
            int mid = low + (high - low) / 2;

            int missing = arr[mid] - (mid + 1); // Number of missing elements before arr[mid]

            if (missing < k) {
                idx = mid; 
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (idx == -1) { // All missing numbers are before the first element of arr, if array will be [5,6,7,8] and k=3, so idx will be always -1 in our binary search, so we will return k as the answer.
            return k;
        }
        return k+idx+1; // The kth missing number is after arr[idx], so we add idx + 1 to k to account for the numbers present in arr before arr[idx].
    }


bool searchIn2DMatrix(const vector<vector<int>>& matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    int n = matrix.size();
    int m = matrix[0].size();
    
    int low = 0, high = n * m - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        // Virtual row and column mapping
        int row = mid / m;
        int col = mid % m;
        
        if (matrix[row][col] == target) return true;
        if (matrix[row][col] < target) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

// 3. Search in 2D matrix - II (Rows and Columns are individually sorted)
// Time Complexity: O(N + M) - Starts from top-right corner
bool searchIn2DMatrixII(const vector<vector<int>>& matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    int n = matrix.size();
    int m = matrix[0].size();
    
    int row = 0, col = m - 1; // Top-right corner pointer
    while (row < n && col >= 0) {
        if (matrix[row][col] == target) return true;
        // If current element is larger, target can't be in this entire column
        if (matrix[row][col] > target) col--;
        // If current element is smaller, target can't be in this entire row
        else row++;
    }
    return false;
}

// Helper function for Finding Peak Element - II
// Finds the row index containing the maximum element in a specific column
int findMaxRowInColumn(const vector<vector<int>>& matrix, int col, int n) {
    int max_val = -1;
    int max_row_idx = -1;
    for (int i = 0; i < n; i++) {
        if (matrix[i][col] > max_val) {
            max_val = matrix[i][col];
            max_row_idx = i;
        }
    }
    return max_row_idx;
}

// 4. Find Peak Element - II (A 2D peak is strictly greater than top, bottom, left, right neighbors)
// Time Complexity: O(N * log(M))
vector<int> findPeakElementII(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    
    int low = 0, high = m - 1;
    while (low <= high) {
        int mid_col = low + (high - low) / 2;
        
        // Find the absolute global maximum of this column to isolate vertical checks
        int max_row = findMaxRowInColumn(matrix, mid_col, n);
        
        // Safely check left and right neighbors (boundary checks handled via -1 default)
        int left = (mid_col - 1 >= 0) ? matrix[max_row][mid_col - 1] : -1;
        int right = (mid_col + 1 < m) ? matrix[max_row][mid_col + 1] : -1;
        
        // If it is greater than both left and right, it's a 2D peak
        if (matrix[max_row][mid_col] > left && matrix[max_row][mid_col] > right) {
            return {max_row, mid_col};
        }
        // If left neighbor is larger, peak must exist in the left half
        else if (matrix[max_row][mid_col] < left) {
            high = mid_col - 1;
        } 
        // If right neighbor is larger, peak must exist in the right half
        else {
            low = mid_col + 1;
        }
    }
    return {-1, -1};
}

// Helper function for Matrix Median
// Counts elements <= x in a sorted row using standard upper_bound
int countBlackbox(const vector<int>& row, int x) {
    return upper_bound(row.begin(), row.end(), x) - row.begin();
}

// 5. Matrix Median (Given row-wise sorted matrix with odd dimensions)
// Time Complexity: O(32 * N * log(M))
int matrixMedian(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    
    // Total count of elements smaller than or equal to median must be exactly >= (N * M + 1) / 2
    int target_count = (n * m + 1) / 2;
    
    // Establish binary search range based on matrix absolute boundaries
    int low = INT_MAX, high = INT_MIN;
    for (int i = 0; i < n; i++) {
        low = min(low, matrix[i][0]);
        high = max(high, matrix[i][m - 1]);
    }
    
    int ans = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Count how many total elements in the matrix are <= mid
        int current_count = 0;
        for (int i = 0; i < n; i++) {
            current_count += countBlackbox(matrix[i], mid);
        }
        
        // If count satisfies the target median condition, record and look for a smaller valid element
        if (current_count >= target_count) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

    int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    return 0;
}
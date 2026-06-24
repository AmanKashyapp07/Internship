#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    char id;      // Job ID
    int deadline; // Deadline of job
    int profit;   // Profit if job is over before or on deadline
};

// Comparator function to sort jobs in descending order of profit
bool comparison(Job a, Job b) {
    return (a.profit > b.profit);
}

// Function to find the maximum profit and number of jobs done
void findMaxProfit(vector<Job>& jobs) {
    // 1. Sort all jobs according to decreasing order of profit
    sort(jobs.begin(), jobs.end(), comparison);

    // 2. Find the maximum deadline to decide the size of our time slots
    int maxDeadline = 0;
    for (const auto& job : jobs) {
        maxDeadline = max(maxDeadline, job.deadline);
    }

    // 3. Initialize a slot array to track free time slots (1-indexed)
    // slot[i] will store true if time slot i is occupied
    vector<bool> slots(maxDeadline + 1, false);
    
    int countJobs = 0;
    int maxProfit = 0;

    // 4. Iterate through all given jobs
    for (const auto& job : jobs) {
        // Find a free slot for this job, starting from its last possible slot
        for (int j = job.deadline; j > 0; j--) {
            // Free slot found
            if (!slots[j]) {
                slots[j] = true; // Assign this slot to the current job
                countJobs++;
                maxProfit += job.profit;
                break; // Move to the next job
            }
        }
    }

    cout << "Number of jobs done: " << countJobs << endl;
    cout << "Maximum Profit: " << maxProfit << endl;
}

int main() {
    vector<Job> jobs = { {'A', 2, 100}, {'B', 1, 19}, {'C', 2, 27}, {'D', 1, 25}, {'E', 3, 15} };
    
    findMaxProfit(jobs);
    return 0;
}
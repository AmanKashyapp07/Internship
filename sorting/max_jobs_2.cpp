#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

// DSU Structure to manage available time slots efficiently
struct DisjointSet {
    vector<int> parent;

    DisjointSet(int n) {
        parent.resize(n + 1);
        // Initially, every slot is its own parent (pointing to itself as free)
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    // Find function with Path Compression
    int find(int i) {
        if (i == parent[i])
            return i;
        return parent[i] = find(parent[i]); // Path compression
    }

    // Union function to merge a filled slot 'v' with its left neighbor 'u'
    void merge(int u, int v) {
        parent[v] = u;
    }
};

bool comparison(Job a, Job b) {
    return (a.profit > b.profit);
}

void findMaxProfitWithDSU(vector<Job>& jobs) {
    // 1. Sort jobs by descending profit
    sort(jobs.begin(), jobs.end(), comparison);

    // 2. Find max deadline to size the DSU
    int maxDeadline = 0;
    for (const auto& job : jobs) {
        maxDeadline = max(maxDeadline, job.deadline);
    }

    // 3. Initialize DSU. Slots go from 0 to maxDeadline
    DisjointSet dsu(maxDeadline);

    int countJobs = 0;
    int maxProfit = 0;

    // 4. Process each job
    for (const auto& job : jobs) {
        // Find the latest available free slot for this job's deadline
        int availableSlot = dsu.find(job.deadline); // find function returns the latest available slot <= job.deadline by 

        // If available slot is greater than 0, it means an empty slot exists
        if (availableSlot > 0) {
            // Claim this slot by pointing its parent to the next available left slot (availableSlot - 1)
            dsu.merge(dsu.find(availableSlot - 1), availableSlot);
            
            countJobs++;
            maxProfit += job.profit;
        }
    }

    cout << "Number of jobs done: " << countJobs << endl;
    cout << "Maximum Profit: " << maxProfit << endl;
}

int main() {
    vector<Job> jobs = { {'A', 2, 100}, {'B', 1, 19}, {'C', 2, 27}, {'D', 1, 25}, {'E', 3, 15} };
    
    findMaxProfitWithDSU(jobs);
    return 0;
}
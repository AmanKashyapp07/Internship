class Solution {
public:
    int kthSmallest(vector<vector<int>>& mat, int k) {
        using T = pair<int, vector<int>>; // pair of sum and the indices of the elements taken from each row
        priority_queue<T, vector<T>, greater<T>> pq; // min heap to keep track of the k smallest sums
        int m = mat.size();
        int n = mat[0].size();
        int sum=0;
        for(int i=0;i<m;i++){
            sum+=mat[i][0]; 
        }
        map<vector<int>, bool> visited;
        visited[vector<int>(m, 0)] = true;
        pq.push({sum, vector<int>(m, 0)}); 
        while(!pq.empty() && k!=0){
            auto [currSum, indices] = pq.top();
            pq.pop();
            k--;
            if(k==0) return currSum; 
            for(int i=0;i<m;i++){
                if(indices[i]+1<n){ 
                    vector<int> newIndices = indices; 
                    newIndices[i]++; 
                    int newSum = currSum - mat[i][indices[i]] + mat[i][newIndices[i]]; 
                    if(!visited[newIndices]){ // using visited is important to avoid pushing the same combination of indices into the priority queue multiple times, which would lead to incorrect results and increased time complexity. By marking combinations as visited, we ensure that each unique combination is only processed once. if duplicate combinations are pushed into the priority queue, it can lead to incorrect results because the same sum could be counted multiple times, and it can also increase the time complexity of the algorithm, making it less efficient.
                        visited[newIndices] = true;
                        pq.push({newSum, newIndices}); 
                    }
                    
                }
            }
        }
        return -1; // if we cannot find the kth smallest sum, return -1
    }
};
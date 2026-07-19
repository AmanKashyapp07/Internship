// Link: https://cses.fi/problemset/task/2163

#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree (Binary Indexed Tree)
struct F{
    int n;
    vector<int> b;

    F(int n):n(n),b(n+1){}

    // Add value v at index i
    void u(int i,int v){
        for(++i;i<=n;i+=i&-i) b[i]+=v;
    }

    // Returns prefix sum [0...i]
    int q(int i){
        int s=0;
        for(++i;i;i-=i&-i) s+=b[i];
        return s;
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n,k;
    cin>>n>>k;

    F f(n);

    // Initially every child is present
    for(int i=0;i<n;i++) f.u(i,1);

    int cur=0;   // Current position among remaining children

    // Repeat until all children are removed
    for(int rem=n;rem;rem--){
        // Find next child to remove
        cur=(cur+k)%rem;
        int need=cur+1;

        // Binary search on Fenwick tree to find the actual index
        int l=0,h=n-1,ans=n-1;
        while(l<=h){
            int m=(l+h)>>1;
            if(f.q(m)>=need)
                ans=m,h=m-1;
            else
                l=m+1;
        }

        // Output removed child (1-based indexing)
        cout<<ans+1<<" \n"[rem==1];

        // Mark child as removed
        f.u(ans,-1);
    }
}

// Fenwick Tree stores PREFIX SUMS instead of individual values.
// Prefix sums allow us to quickly know how many active elements
// exist from index 0 to index i in O(log n).

// Use the prefix sum concept when:
// 1. You need frequent point updates.
// 2. You need cumulative/range sum queries.
// 3. You need to find the k-th active element by binary searching
//    on prefix sums (as done in the Josephus problem).
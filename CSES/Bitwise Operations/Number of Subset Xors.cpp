// Link: https://cses.fi/problemset/task/3211

#include <bits/stdc++.h>
using namespace std;

struct XorBasis {
    static const int LOG = 31;
    vector<int> basis;

    XorBasis() : basis(LOG, 0) {}

    void add(int x) {
        for (int i = LOG - 1; i >= 0; i--) {
            if ((x & (1 << i)) == 0) continue;

            if (basis[i] == 0) {
                basis[i] = x;
                return;
            }

            x ^= basis[i];
        }
    }

    int rank() {
        int cnt = 0;
        for (int x : basis)
            if (x) cnt++;
        return cnt;
    }

    int maxXor() {
        int res = 0;
        for (int i = LOG - 1; i >= 0; i--) {
            if ((res ^ basis[i]) > res) {
                res ^= basis[i];
            }
        }
        return res;
    }

    long long countDistinctXors() {
        return 1LL << rank();
    }
};


int main(){
    int n;
    cin>>n;
    vector<int> nums(n);
    for(int i=0; i<n; i++) cin>>nums[i];
    XorBasis xb;
    for (int x : nums) {
        xb.add(x);
    }
    cout<<xb.countDistinctXors()<<endl;
}
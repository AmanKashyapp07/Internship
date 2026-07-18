#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MOD = 1e9 + 7;

int main(){
    int t;cin>>t;
    while(t--){
        int n;
        cin>>n;
        vector<int> a(n);
        for(int i=0;i<n;i++) cin>>a[i];
        int count=0;
        for(int i=0;i<n;i++) if(a[i]&1) count++;
        if(count&1) cout<<"FIRST\n";
        else cout<<"SECOND\n";
    }
}
#include <bits/stdc++.h>
using namespace std;

using vvi = vector<vector<int>>;
using vi = vector<int>;


vvi mexGridConstruction(int n){
    vvi grid(n, vi(n));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = (i ^ j);
        }
    }
    return grid;
}

int main(){
    int n;
    cin >> n;
    vvi grid = mexGridConstruction(n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
    }
}
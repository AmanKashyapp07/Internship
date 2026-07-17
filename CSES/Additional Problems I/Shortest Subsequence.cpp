#include <bits/stdc++.h>
using namespace std;

string s;
int n;

vector<int> dp;
vector<char> choice;
vector<int> parentPos;
vector<int> pos[4]; // pos[0] = positions of 'A', pos[1] = positions of 'C', pos[2] = positions of 'G', pos[3] = positions of 'T'

int id(char c){
    if(c=='A') return 0;
    if(c=='C') return 1;
    if(c=='G') return 2;
    return 3;
}

int getNext(int cur, char ch){
    auto &v = pos[id(ch)]; // Get the vector of positions for the character ch
    auto it = lower_bound(v.begin(), v.end(), cur);
    if(it == v.end()) return -1;
    return *it;
} 

int solve(int cur){

    if(dp[cur] != -1)
        return dp[cur];

    int ans = INT_MAX;
    string dna = "ACGT";

    for(char ch : dna){

        int nxt = getNext(cur, ch);

        int len;
        if(nxt == -1)
            len = 1;
        else
            len = 1 + solve(nxt + 1);

        if(len < ans){
            ans = len;
            choice[cur] = ch;
            parentPos[cur] = (nxt == -1 ? -1 : nxt + 1);
        }
    }

    return dp[cur] = ans;
}

int main(){

    cin >> s;
    n = s.size();

    for(int i = 0; i < n; i++)
        pos[id(s[i])].push_back(i);

    dp.assign(n + 1, -1);
    choice.assign(n + 1, ' ');
    parentPos.assign(n + 1, -1);

    solve(0);

    string ans;
    int cur = 0;

    while(true){
        ans += choice[cur];
        if(parentPos[cur] == -1)
            break;
        cur = parentPos[cur];
    }

    cout << ans;
}
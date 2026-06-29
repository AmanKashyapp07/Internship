
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

class FreqStack {
public:
    stack<int> s;
    vector<int> freq;
    int maxFreq;
    int element;
    FreqStack() {
        s = stack<int>();
        freq.assign(20001, 0); // Assuming the range of values is [0, 100000]
        maxFreq = 0;
        element = -1; // Initialize with an invalid value
    }
    
    void push(int val) {
        s.push(val);
        freq[val]++;
        if(freq[val] > maxFreq) {
            maxFreq = freq[val];
            element = val; // new max frequency element
        }
        else if(freq[val] == maxFreq) {
            element = val; // latest element with the same max frequency
        }
    }
    
    int pop() {
        int val = s.top();
        s.pop();
        freq[val]--;
        return val;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */
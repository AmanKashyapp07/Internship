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


class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        int maxCount=0;
        sort(courses.begin(),courses.end(),[](const vector<int>& a,const vector<int>& b){
            return a[1]<b[1]; // sorting by deadline because we want to take courses with earlier deadlines first to maximise the number of courses we can take
        });
        priority_queue<int> pq; // max heap to keep track of the durations of the courses we have taken
        int currentTime=0;
        for(auto& course:courses){
            int duration=course[0];
            int deadline=course[1];
            if(currentTime+duration<=deadline){ // if we can take this course without exceeding the deadline, just take it and push it into the max heap
                // max heap represents the courses we have taken, and the top of the max heap is the course with the longest duration. If we can take a new course without exceeding the deadline, we just add it to our schedule and update the current time.
                currentTime+=duration;
                pq.push(duration);
                maxCount++;
            }else if(!pq.empty() && pq.top()>duration){ // if we cannot take this course without exceeding the deadline, we check if we can replace the longest duration course we have taken with this one. If the longest duration course is longer than the current course, we can replace it to free up time for the current course.
                // why pq.top()>duration? because we want to replace the longest duration course with the current course to free up time for the current course. If the longest duration course is shorter than the current course, we cannot replace it because it will not free up enough time for the current course.
                currentTime+=duration-pq.top(); // replace the longest duration course with the current one
                pq.pop();
                pq.push(duration);
            }
        }
        return maxCount;
    }
};
// Link: https://cses.fi/problemset/task/1110

#include <bits/stdc++.h>
using namespace std;

string boothAlgorithm(string s){
    string t = s + s;
    int n = s.size();

    int i = 0, j = 1, k = 0;
    // i is the starting index of the current candidate for the smallest rotation
    // j is the starting index of the next candidate for the smallest rotation
    // k is the offset from i and j for comparison, basically, k is the length of the current matching prefix between the two candidates.
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            k++;
        } // If the characters at the current offset k are equal, we increment k to check the next character in the next iteration.
        else if (t[i + k] < t[j + k]) {
            j += k + 1; // Move j to the next candidate
            if (i == j) j++; // Ensure i and j are not the same
            k = 0; // Reset k for the new comparison
        } else {
            i += k + 1; // Move i to the next candidate
            if (i == j) i++; // Ensure i and j are not the same
            k = 0; // Reset k for the new comparison
        }
    }

    int start = min(i, j);
    return t.substr(start, n);
} // it returns the lexicographically smallest rotation of the string s using Booth's algorithm.

int main(){
    string s; cin>>s;
    cout<<boothAlgorithm(s)<<endl;
}
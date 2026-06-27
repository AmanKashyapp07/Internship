/*
==========================================================
                SORTING WITH CUSTOM COMPARATOR
==========================================================

Syntax
------

bool cmp(T a, T b) {
    // return true if 'a' should come before 'b'
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 1 : Sort in Descending Order
----------------------------------------------------------

bool cmp(int a, int b) {
    return a > b;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 2 : Sort Pair by First (Ascending)
----------------------------------------------------------

bool cmp(pair<int,int> a, pair<int,int> b) {
    return a.first < b.first;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 3 : Sort Pair by Second (Ascending)
----------------------------------------------------------

bool cmp(pair<int,int> a, pair<int,int> b) {
    return a.second < b.second;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 4 : Sort Pair by Second Descending
----------------------------------------------------------

bool cmp(pair<int,int> a, pair<int,int> b) {
    return a.second > b.second;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 5 : Multiple Conditions
----------------------------------------------------------

Sort by first ascending.
If first is equal, sort by second descending.

bool cmp(pair<int,int> a, pair<int,int> b) {

    if (a.first != b.first)
        return a.first < b.first;

    return a.second > b.second;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Example 6 : Sort Custom Struct
----------------------------------------------------------

struct Student {
    string name;
    int marks;
};

bool cmp(Student a, Student b) {
    return a.marks > b.marks;
}

sort(v.begin(), v.end(), cmp);

----------------------------------------------------------
Lambda Comparator
----------------------------------------------------------

sort(v.begin(), v.end(),
    [](auto &a, auto &b) {
        return a.first < b.first;
    });

==========================================================
Rules
==========================================================

Comparator must return:

true  -> a should come before b

false -> b should come before (or stay before) a

Comparator should define a strict ordering.

==========================================================
Most Common Comparators
==========================================================

Ascending
----------
return a < b;

Descending
-----------
return a > b;

Pair by First
-------------
return a.first < b.first;

Pair by Second
--------------
return a.second < b.second;

==========================================================

// Arrays
memset(dp, 0, sizeof(dp));
memset(dp, -1, sizeof(dp)); // memset works only for 0 and -1, for other values use fill() or a loop

// Vectors
vector<vector<int>> dp(n, vector<int>(m, -1));

dp.assign(n, vector<int>(m, -1));

for (auto &row : dp)
    fill(row.begin(), row.end(), -1);

*/
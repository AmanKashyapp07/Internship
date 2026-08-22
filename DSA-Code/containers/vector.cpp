/*
==========================================================
Vector Initialization with Size
==========================================================

// 1D Vector
vector<int> v(n);                 // Size n, initialized to 0
vector<int> v(n, 5);              // Size n, initialized to 5

//--------------------------------------------------------

// 2D Vector
vector<vector<int>> mat(n, vector<int>(m));
// n x m matrix initialized to 0

vector<vector<int>> mat(n, vector<int>(m, -1));
// n x m matrix initialized to -1

//--------------------------------------------------------

// 3D Vector
vector<vector<vector<int>>> dp(
    x,
    vector<vector<int>>(
        y,
        vector<int>(z)
    )
);
// x × y × z initialized to 0

vector<vector<vector<int>>> dp(
    x,
    vector<vector<int>>(
        y,
        vector<int>(z, -1)
    )
);
// x × y × z initialized to -1

==========================================================
Quick Syntax to Remember
==========================================================

// 1D
vector<int> v(n, val);

// 2D
vector<vector<int>> v(n, vector<int>(m, val));

// 3D
vector<vector<vector<int>>> v(
    x,
    vector<vector<int>>(
        y,
        vector<int>(z, val)
    )
);

==========================================================

*/
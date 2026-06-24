/**
 * OA / Interview Template
 */

#include <algorithm>
#include <array>
#include <bitset>
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
#define sz(x) ((int)(x).size())

const int INF = INT_MAX;
const ll LINF = LLONG_MAX;
const ll MOD = 1e9 + 7;

/*-------------------------------------------------------------*/
/* FAST POWER */
/*-------------------------------------------------------------*/

ll power(ll base, ll exponent) {
    ll result = 1;
    base %= MOD;

    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % MOD;

        base = (base * base) % MOD;
        exponent >>= 1;
    }

    return result;
}

void printVector(const vector<int>& v) { // why const reference? to avoid copying the vector, which can be expensive for large vectors. Using a const reference allows us to access the vector without modifying it and without the overhead of copying.
    for (int x : v)
        cout << x << " ";
    cout << "\n";
}
/*-------------------------------------------------------------*/
/* GCD / LCM */
/*-------------------------------------------------------------*/

ll gcdll(ll a, ll b) {
    return __gcd(a, b);
}

ll lcmll(ll a, ll b) {
    return a / gcdll(a, b) * b;
}

/*-------------------------------------------------------------*/
/* MODULAR ARITHMETIC */
/*-------------------------------------------------------------*/

ll modAdd(ll a, ll b) {
    return (a + b) % MOD;
}

ll modSub(ll a, ll b) {
    return (a - b + MOD) % MOD;
}

ll modMul(ll a, ll b) {
    return (a * b) % MOD;
}

/*-------------------------------------------------------------*/
/* VECTOR ROTATION */
/*-------------------------------------------------------------*/

vector<int> rotateVector(const vector<int>& v, int k) {
    int n = v.size();

    vector<int> rotated(n);

    for (int i = 0; i < n; i++)
        rotated[(i + k) % n] = v[i];

    return rotated;
}

vector<int> rotateVectorLeft(const vector<int>& v, int k) {
    int n = v.size();

    vector<int> rotated(n);

    for (int i = 0; i < n; i++)
        rotated[i] = v[(i + k) % n];

    return rotated;
}

int maxInVector = *max_element(all(v));
int minInVector = *min_element(all(v));
int sumOfVector = accumulate(all(v), 0);


/*-------------------------------------------------------------*/
/* PRIME CHECK */
/*-------------------------------------------------------------*/

bool isPrime(int n) {

    if (n < 2)
        return false;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }

    return true;
}

/*-------------------------------------------------------------*/
/* PRIME FACTORIZATION */
/*-------------------------------------------------------------*/

vector<pair<int, int>> primeFactorisation(int n) {

    vector<pair<int, int>> factors;

    for (int i = 2; i * i <= n; i++) {

        if (n % i == 0) {

            int cnt = 0;

            while (n % i == 0) {
                n /= i;
                cnt++;
            }

            factors.push_back({i, cnt});
        }
    }

    if (n > 1)
        factors.push_back({n, 1});

    return factors;
}

/*-------------------------------------------------------------*/
/* SIEVE */
/*-------------------------------------------------------------*/

void primeSieve(int n, vector<bool>& is_prime) {

    is_prime.assign(n + 1, true);

    is_prime[0] = false;
    is_prime[1] = false;

    for (int i = 2; i * i <= n; i++) {

        if (is_prime[i]) {

            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }
}

/*-------------------------------------------------------------*/
/* NCR */
/*-------------------------------------------------------------*/

vector<ll> factorial;
vector<ll> inv_factorial;

void init_factorials(int max_n) {

    factorial.assign(max_n + 1, 1);
    inv_factorial.assign(max_n + 1, 1);

    for (int i = 1; i <= max_n; i++)
        factorial[i] = factorial[i - 1] * i % MOD;

    inv_factorial[max_n] = power(factorial[max_n], MOD - 2);

    for (int i = max_n - 1; i >= 0; i--)
        inv_factorial[i] =
            inv_factorial[i + 1] * (i + 1) % MOD;
}

ll nCr(int n, int r) {

    if (r < 0 || r > n)
        return 0;

    return factorial[n] *
           inv_factorial[r] % MOD *
           inv_factorial[n - r] % MOD;
}

/*-------------------------------------------------------------*/
/* RANGE XOR */
/*-------------------------------------------------------------*/

int xorUpto(int x) {

    if (x % 4 == 0)
        return x;

    if (x % 4 == 1)
        return 1;

    if (x % 4 == 2)
        return x + 1;

    return 0;
}

int rangeXOR(int l, int r) {
    return xorUpto(r) ^ xorUpto(l - 1);
}

/*-------------------------------------------------------------*/
/* COUNT SET BITS */
/*-------------------------------------------------------------*/

int countSetBits(int n) {

    int cnt = 0;

    while (n) {
        n &= (n - 1);
        cnt++;
    }

    return cnt;
}

/*-------------------------------------------------------------*/
/* BIT OPERATIONS */
/*-------------------------------------------------------------*/

int setBit(int n, int pos) {
    return n | (1 << pos);
}

int clearBit(int n, int pos) {
    return n & ~(1 << pos);
}

bool checkBit(int n, int pos) {
    return n & (1 << pos);
}

int toggleBit(int n, int pos) {
    return n ^ (1 << pos);
}

bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int rightMostSetBit(int n) {
    return n & -n;
} // returns a number with only the rightmost set bit of n


vector<int> numToBinary(int n) {

    vector<int> binary;

    while (n > 0) {
        binary.push_back(n % 2);
        n /= 2;
    }

    reverse(all(binary));

    return binary;
}


/*-------------------------------------------------------------*/
/* PREFIX SUM */
/*-------------------------------------------------------------*/

vector<ll> buildPrefix(vector<int>& a) {

    int n = a.size();
    vector<ll> prefix(n,0);
    prefix[0] = a[0];
    for(int i = 1; i < n; i++) prefix[i] = prefix[i-1] + a[i];
    return prefix; /// 0-based indexing
}

vector<ll> buildSuffix(vector<int>& a) {

    int n = a.size();
    vector<ll> suffix(n,0);
    suffix[n-1] = a[n-1];
    for(int i = n-2; i >= 0; i--) suffix[i] = suffix[i+1] + a[i];
    return suffix; /// 0-based indexing
}

/*-------------------------------------------------------------*/
/* DSU */
/*-------------------------------------------------------------*/

class DSU {

public:
    vector<int> parent;
    vector<int> sz;

    DSU(int n) {

        parent.resize(n + 1);
        sz.assign(n + 1, 1);

        iota(all(parent), 0);
    }

    int find(int x) {

        if (x == parent[x])
            return x;

        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {

        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (sz[a] < sz[b])
            swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];

        return true;
    }
};

/*-------------------------------------------------------------*/
/* KADANE */
/*-------------------------------------------------------------*/

ll kadane(vector<int>& nums) {

    ll cur = 0;
    ll best = LLONG_MIN;

    for (int x : nums) {
        cur = max((ll)x, cur + x);
        best = max(best, cur);
    }

    return best;
}

pair<ll, pair<int, int>> kadaneWithIndices(vector<int>& nums) {

    ll cur = 0;
    ll best = LLONG_MIN;
    int start = 0, end = -1, temp_start = 0;

    for (int i = 0; i < nums.size(); i++) {
        cur += nums[i];

        if (cur > best) {
            best = cur;
            start = temp_start;
            end = i;
        }

        if (cur < 0) {
            cur = 0;
            temp_start = i + 1;
        }
    }

    return {best, {start, end}};
}

/*-------------------------------------------------------------*/
/* COORDINATE COMPRESSION */
/*-------------------------------------------------------------*/

vector<int> compress(vector<int> a) {

    vector<int> vals = a;

    sort(all(vals));

    vals.erase(unique(all(vals)), vals.end());

    for (int& x : a)
        x = lower_bound(all(vals), x) - vals.begin();

    return a;
}

/*-------------------------------------------------------------*/
/* BINARY STRING */
/*-------------------------------------------------------------*/

string toBinary(int n) {
    return bitset<32>(n).to_string();
}

/*-------------------------------------------------------------*/
/* GRID DIRECTIONS */
/*-------------------------------------------------------------*/

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

int dx8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

/*-------------------------------------------------------------*/
/* SLIDING WINDOW MAXIMUM */
/*-------------------------------------------------------------*/

vector<int> maxSlidingWindow(vector<int>& nums,
                             int k) {

    deque<int> dq;
    vector<int> ans;

    for (int i = 0; i < nums.size(); i++) {

        while (!dq.empty() &&
               dq.front() <= i - k)
            dq.pop_front();

        while (!dq.empty() &&
               nums[dq.back()] <= nums[i])
            dq.pop_back();

        dq.push_back(i);

        if (i >= k - 1)
            ans.push_back(nums[dq.front()]);
    }

    return ans;
}

/*-------------------------------------------------------------*/
/* MAIN */
/*-------------------------------------------------------------*/

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}
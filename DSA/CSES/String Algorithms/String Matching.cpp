// Link: https://cses.fi/problemset/task/1753

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <random>

using namespace std;

// Function to generate a random odd base to prevent hacking/collisions
int get_random_base(int min_val, int max_val) {
    // Seed using high-resolution steady clock
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(min_val, max_val);
    int base = dist(rng);
    return base % 2 == 0 ? base + 1 : base; 
}

struct PolyHash {
    int n;
    string s;
    
    // Using static bases so that all instances of PolyHash share the exact same bases
    static long long P1, P2;
    const long long M1 = 1e9 + 7;
    const long long M2 = 1e9 + 9;
    
    vector<long long> pow1, pow2;
    vector<long long> h1, h2;

    PolyHash(const string& input_str) {
        // Initialize randomized bases once per program execution
        if (P1 == 0) P1 = get_random_base(150, 300);
        if (P2 == 0) P2 = get_random_base(350, 500);

        s = input_str;
        n = s.length();
        
        pow1.assign(n + 1, 1);
        pow2.assign(n + 1, 1);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        
        // 1. Precompute powers of the randomized bases
        for (int i = 1; i <= n; i++) {
            pow1[i] = (pow1[i - 1] * P1) % M1;
            pow2[i] = (pow2[i - 1] * P2) % M2;
        }
        
        // 2. Precompute prefix hashes
        for (int i = 0; i < n; i++) {
            long long val = s[i] - 'a' + 1; 
            h1[i + 1] = (h1[i] * P1 + val) % M1;
            h2[i + 1] = (h2[i] * P2 + val) % M2;
        }
    }

    // 3. O(1) query function to extract substring hashes safely
    pair<long long, long long> get_hash(int L, int R) {
        if (L > R || L < 0 || R >= n) return {0, 0};
        
        long long res1 = (h1[R + 1] - (h1[L] * pow1[R - L + 1]) % M1 + M1) % M1;
        long long res2 = (h2[R + 1] - (h2[L] * pow2[R - L + 1]) % M2 + M2) % M2;
        
        return {res1, res2};
    }
};

// Allocate memory for the static members
long long PolyHash::P1 = 0;
long long PolyHash::P2 = 0;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s, p;
    if (!(cin >> s >> p)) return 0;

    int n = s.length();
    int m = p.length();

    // Corner case: If pattern is longer than the string, it can't match
    if (m > n) {
        cout << 0 << "\n";
        return 0;
    }

    // Generate prefix hashes for both strings
    PolyHash hasher_s(s);
    PolyHash hasher_p(p);

    // Get the static hash signature of the target pattern
    pair<long long, long long> target_hash = hasher_p.get_hash(0, m - 1);

    int count = 0;
    
    // Slide a window of size 'm' across string 's'
    for (int i = 0; i <= n - m; i++) {
        // Query the hash of the current substring in O(1)
        if (hasher_s.get_hash(i, i + m - 1) == target_hash) {
            count++;
        }
    }

    cout << count << "\n";

    return 0;
}
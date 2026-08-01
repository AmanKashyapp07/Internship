// Link: https://cses.fi/problemset/task/1632

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

// Structure to represent a movie
struct Movie {
    int start;
    int end;

    // Sort primarily by end time
    bool operator<(const Movie& other) const {
        if (end != other.end) {
            return end < other.end;
        }
        return start < other.start;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<Movie> movies(n);
    for (int i = 0; i < n; ++i) {
        cin >> movies[i].start >> movies[i].end;
    }

    // Step 1: Sort movies by their end times
    sort(movies.begin(), movies.end());

    // Step 2: Store the end times of the current movie each member is watching.
    // Initially, all k members are free at time 0.
    multiset<int> member_free_times;
    for (int i = 0; i < k; ++i) {
        member_free_times.insert(0);
    }

    int total_movies_watched = 0;

    // Step 3: Process movies greedily
    for (const auto& movie : movies) {
        // Find the member whose free time is strictly greater than the movie's start time
        auto it = member_free_times.upper_bound(movie.start);

        // If 'it' points to the beginning, it means no member is free <= movie.start
        if (it != member_free_times.begin()) {
            // Decrement iterator to get the member with the LATEST free time <= movie.start
            --it;

            // Assign this movie to that member by updating their free time
            member_free_times.erase(it);
            member_free_times.insert(movie.end);

            total_movies_watched++;
        }
    }

    cout << total_movies_watched << "\n";

    return 0;
}
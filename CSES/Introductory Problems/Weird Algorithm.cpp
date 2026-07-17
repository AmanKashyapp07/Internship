#include <iostream>
using namespace std;

int main() {
    long long n;
    cin >> n;

    while (true) {
        cout << n;

        if (n == 1) {
            cout << '\n';
            break;
        }

        cout << ' ';

        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
    }

    return 0;
}
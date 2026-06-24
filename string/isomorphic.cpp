#include <iostream>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, t;
    cin >> s >> t;

    if (s.size() != t.size()) {
        cout << "false\n";
        return 0;
    }

    unordered_map<char, char> mp1, mp2;

    for (int i = 0; i < s.size(); i++) {
        char a = s[i];
        char b = t[i];

        if (mp1.count(a) && mp1[a] != b) {
            cout << "false\n";
            return 0;
        }

        if (mp2.count(b) && mp2[b] != a) {
            cout << "false\n";
            return 0;
        }

        mp1[a] = b;
        mp2[b] = a;
    }

    cout << "true\n";
    return 0;
}
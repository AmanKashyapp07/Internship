#include <vector>
using namespace std;

class TreeAncestor {
    vector<vector<int>> up;
    int LOG = 20;

public:
    TreeAncestor(int n, vector<int>& parent) {
        up.assign(n, vector<int>(LOG, -1));

        for (int i = 0; i < n; i++) up[i][0] = parent[i];

        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                int prev = up[i][j - 1];
                if(prev != -1) up[i][j] = up[prev][j - 1];
            }
        }
    }

    int getKthAncestor(int node, int k) {
        for(int j=0;j<=LOG-1;j++){
            if((k>>j)&1){
                node = up[node][j];
                if(node == -1) return -1;
            }
        }
        return node;
    }
};
class Solution {
public:
    vector<int> parent, rank;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unionSet(int x, int y) {
        int px = find(x);
        int py = find(y);

        if (px == py) return;

        if (rank[px] < rank[py])
            parent[px] = py;
        else if (rank[px] > rank[py])
            parent[py] = px;
        else {
            parent[py] = px;
            rank[px]++;
        }
    }

    int kruskalsMST(int V, vector<vector<int>> &edges) {
        parent.resize(V);
        rank.resize(V, 0);

        for (int i = 0; i < V; i++)
            parent[i] = i;

        sort(edges.begin(), edges.end(), [](vector<int> &a, vector<int> &b) {
            return a[2] < b[2];
        });
        int sum = 0;

        for (auto &e : edges) {
            int u = e[0];
            int v = e[1];
            int w = e[2];

            if (find(u) != find(v)) {
                sum += w;
                unionSet(u, v);
            }
        }

        return sum;
    }
};
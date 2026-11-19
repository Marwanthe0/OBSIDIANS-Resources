struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> pairU, pairV, dist;
    static constexpr int INF = 1e9;
    HopcroftKarp(int n, int m) : n(n), m(m), adj(n + 1), pairU(n + 1, 0), pairV(m + 1, 0), dist(n + 1) {}
    void addEdge(int u, int v) { adj[u].push_back(v); }
    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (pairU[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        dist[0] = INF;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    int nextU = pairV[v];
                    if (dist[nextU] == INF) {
                        dist[nextU] = dist[u] + 1;
                        if (nextU != 0) q.push(nextU);
                    }
                }
            }
        }
        return dist[0] != INF;
    }

    bool dfs(int u) {
        if (u == 0) return true;
        for (int v : adj[u]) {
            int nextU = pairV[v];
            if (dist[nextU] == dist[u] + 1 && dfs(nextU)) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = INF; // Prune dead-end node
        return false;
    }

    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 1; u <= n; u++) {
                if (pairU[u] == 0 && dfs(u)) {
                    matching++;
                }
            }
        }
        return matching;
    }
};
/* Usage:
HopcroftKarp hk(n, m);
hk.addEdge(u, v);
int ans = hk.maxMatching();
// Matched pair for u is hk.pairU[u] (0 if unmatched)
*/

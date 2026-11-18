// https://vjudge.net/problem/LightOJ-1264
const int INF = 1e18;
void solve(int tc) {
    int n;
    cin >> n;

    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }

    int total_masks = 1 << n;
    vector<int> cost(total_masks, 0);

    for (int mask = 1; mask < total_masks; mask++) {
        int lsb_pos = log2(mask);
        int prev_mask = mask ^ (1 << lsb_pos);

        cost[mask] = cost[prev_mask];
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) {
                cost[mask] += a[lsb_pos][i] + a[i][lsb_pos];
            }
        }
    }

    vector<int> dp(total_masks, INF);
    function<int(int)> f = [&](int mask) -> int {
        if (dp[mask] != INF) return dp[mask];

        int ans = cost[mask];
        int lsb = mask & -mask;

        for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
            if (sub & lsb) {
                ans = min(ans, f(sub) + f(mask ^ sub));
            }
        }
        return dp[mask] = ans;
    };
    cout << f((1 << n) - 1) << endl;
}

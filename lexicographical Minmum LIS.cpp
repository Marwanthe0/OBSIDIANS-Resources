vector<int> lexMinLIS(const vector<int>& a, bool by_index = false) {
  int n = a.size();
  if (n == 0) return {};
  
  vector<int> dp(n), tails;
  for (int i = n - 1; i >= 0; i--) {
    int pos = lower_bound(tails.begin(), tails.end(), -a[i]) - tails.begin();
    if (pos == (int)tails.size()) tails.push_back(-a[i]);
    else tails[pos] = -a[i];
    dp[i] = pos + 1;
  }

  int L = tails.size();
  vector<vector<int>> buckets(L + 1);
  for (int i = 0; i < n; i++) buckets[dp[i]].push_back(i);

  vector<int> res;
  int last_idx = -1;
  for (int need = L; need >= 1; need--) {
    int best_idx = -1;
    for (int i : buckets[need]) {
      if (i > last_idx && (last_idx == -1 || a[i] > a[last_idx])) {
        if (best_idx == -1 || a[i] < a[best_idx]) {
          best_idx = i;
          if (by_index) break; // First valid index is minimal by index
        }
      }
    }
    res.push_back(a[best_idx]); // Push best_idx instead if indices needed
    last_idx = best_idx;
  }
  return res;
}

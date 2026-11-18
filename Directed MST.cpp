struct RollbackUF {
    vector<int> e;
    vector<pair<int, int>> st;
    RollbackUF(int n) : e(n, -1) {}
    int size(int x) { return -e[find(x)]; }
    int find(int x) { return e[x] < 0 ? x : find(e[x]); }
    int time() { return (int)st.size(); }
    void rollback(int t) {
        for (int i = time(); i-- > t;)
            e[st[i].first] = st[i].second;
        st.resize(t);
    }
    bool join(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (e[a] > e[b]) swap(a, b);
        st.push_back({a, e[a]});
        st.push_back({b, e[b]});
        e[a] += e[b], e[b] = a;
        return true;
    }
};
struct Edge {
    int u, v;
    long long w;
};
struct Node {
    Edge key;
    Node *l = nullptr, *r = nullptr;
    long long delta = 0;

    void prop() {
        key.w += delta;
        if (l) l->delta += delta;
        if (r) r->delta += delta;
        delta = 0;
    }
    Edge top() {
        prop();
        return key;
    }
};
Node* merge(Node* a, Node* b) {
    if (!a || !b) return a ? a : b;
    a->prop(), b->prop();
    if (a->key.w > b->key.w) swap(a, b);
    a->r = merge(b, a->r);
    swap(a->l, a->r);
    return a;
}

void pop(Node*& a) {
    a->prop();
    a = merge(a->l, a->r);
}
// If no Directed MST exists, returns {-1, {}}.
pair<long long, vector<int>> dmst(int n, int r, const vector<Edge>& g) {
    RollbackUF uf(n);
    vector<Node*> heap(n, nullptr);
    for (const auto& e : g) {
        if (e.u != e.v) {
            heap[e.v] = merge(heap[e.v], new Node{e});
        }
    }
    long long res = 0;
    vector<int> seen(n, -1), path(n), par(n, -1);
    seen[r] = r;
    vector<Edge> Q(n), in(n, {-1, -1, 0});
    deque<tuple<int, int, vector<Edge>>> cycs;
    for (int s = 0; s < n; s++) {
        int u = s, qi = 0, w;
        while (seen[u] < 0) {
            if (!heap[u]) return {-1, {}};
            Edge e = heap[u]->top();
            heap[u]->delta -= e.w;
            pop(heap[u]);
            Q[qi] = e, path[qi++] = u, seen[u] = s;
            res += e.w, u = uf.find(e.u);
            if (seen[u] == s) {
                Node* cyc = nullptr;
                int end = qi, time = uf.time();
                do {
                    cyc = merge(cyc, heap[w = path[--qi]]);
                } while (uf.join(u, w));
                u = uf.find(u), heap[u] = cyc, seen[u] = -1;
                cycs.push_front({u, time, {&Q[qi], &Q[end]}});
            }
        }
        for (int i = 0; i < qi; i++) {
            in[uf.find(Q[i].v)] = Q[i];
        }
    }
    for (auto& [u, t, comp] : cycs) {
        uf.rollback(t);
        Edge inEdge = in[u];
        for (auto& e : comp) {
            in[uf.find(e.v)] = e;
        }
        in[uf.find(inEdge.v)] = inEdge;
    }
    for (int i = 0; i < n; i++) {
        par[i] = in[i].u;
    }
    return {res, par};
}

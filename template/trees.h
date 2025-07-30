//
// Created by ahmed on 7/30/2025.
//

#ifndef TREES_H
#define TREES_H

struct tree {
    int root = 0;
    vector<vector<int>> g;
    explicit tree(int n) : g(n + 1) { }
    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> &operator[](int u) {
        return g[u];
    }

    int cntDfs = 0;
    vector<int> in, out, lvl, sz, top, par, seq;

    void init(int rt = 0) {
        root = rt;
        in = out = lvl = top = par = seq = vector<int>(g.size());
        sz.resize(g.size(), 1);
        par[root] = top[root] = root;
        dfs(root);
        dfs2(root);
    }

    void dfs(int u) {
        for(int &v : g[u]) {
            lvl[v] = lvl[u] + 1;
            par[v] = u;
            g[v].erase(find(g[v].begin(), g[v].end(), u));
            dfs(v);
            sz[u] += sz[v];
            if(sz[v] > sz[g[u][0]])
                swap(v, g[u][0]);
        }
    }
    void dfs2(int u) {
        in[u] = cntDfs++;
        seq[in[u]] = u;
        for(int v : g[u]) {
            top[v] = v == g[u][0]? top[u]: v;
            dfs2(v);
        }
        out[u] = cntDfs - 1;
    }

    int jump(int u, int k) {
        if(k > lvl[u]) return -1;

        int d = lvl[u] - k;
        while (lvl[top[u]] > d) {
            u = par[top[u]];
        }

        return seq[in[u] - lvl[u] + d];
    }

    bool isAncestor(int u, int v) {
        return in[u] <= in[v] && in[v] <= out[u];
    }

    int lca(int u, int v) {
        if(lvl[u] > lvl[v]) swap(u, v);
        if(isAncestor(u, v)) return u;
        while (top[u] != top[v]) {
            if (lvl[top[u]] > lvl[top[v]]) {
                u = par[top[u]];
            } else {
                v = par[top[v]];
            }
        }
        return lvl[u] < lvl[v]? u: v;
    }

    int dis(int u, int v) {
        return lvl[u] + lvl[v] - 2 * lvl[lca(u, v)];
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class binaryLifting {
    static int constexpr LOG = 30;
    vector<vector<int>> up;
    int n;

    void init(const vector<int> &parent) { // 1-based
        for (int i = 2; i <= n; i++) {
            up[i][0] = parent[i];
        }


        for (int j = 1; j < LOG; j++) {

            for (int i = 2; i <= n; i++) {

                up[i][j] = up[ up[i][j - 1] ][j - 1];

            }

        }
    }

    int get(int x, int k) { // 1-based
        for (int j = LOG - 1; j >= 0; j--) {
            if ( k >> j & 1 ) {
                x = up[x][j];
            }
        }

        return !x ? -1 : x;
    }

public:
    explicit binaryLifting(const vector<int> &parent) { // 1-based
        n = static_cast<int>(parent.size()) - 1;
        up = vector(n + 5, vector(LOG, 0));
        init(parent);
    }

    int getKthAncestor(int x, int k) { // 1-based
        return get(x, k);
    }
};


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template < typename vecType = int >
class LCA {
public:
    LCA(
        int n = 0,
        const vector < vector < int > > &G = {},
        int root = 1
    ) : N(n), LOG(0), ROOT(root), adj(G) {
        while((1 << LOG) <= N) LOG++;
        anc.assign(N + 5, vector < int > (LOG));
        depth.assign(N + 5, 0);
        dfs(ROOT);
    }

    int kth_ancestor(int u, int k) const {
        if(depth[u] < k) return -1;
        for(int bit = 0; bit < LOG; bit++)
            if(k & (1 << bit))
                u = anc[u][bit];
        return u;
    }

    int get_lca(int u, int v) const {
        if(depth[u] < depth[v]) swap(u, v);

        u = kth_ancestor(u, depth[u] - depth[v]);
        if(u == v) return u;

        for(int bit = LOG - 1; bit >= 0; bit--)
            if(anc[u][bit] != anc[v][bit])
                u = anc[u][bit], v = anc[v][bit];

        return anc[u][0];
    }

    int get_dist(int u, int v) const {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

private:
    int N, LOG, ROOT;
    const vector < vector < int > > &adj;
    vector < vector < int > > anc;
    vector < int > depth;

    void dfs(int u, int p = 0){
        for(int v : adj[u]){
            if(v == p) continue;
            depth[v] = depth[u] + 1;
            anc[v][0] = u;
            for(int bit = 1; bit < LOG; bit++){
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            }
            dfs(v, u);
        }
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template < typename treeType = int , typename graphType = int >
class LCA {
public:
    LCA(
        int n = 0,
        const vector < vector < pair < int, graphType > > > &G = {},
        function < treeType(treeType, treeType) > op = [](treeType a, treeType b){ return a + b; },
        treeType _neutral = treeType(),
        int root = 1
    ) : N(n), LOG(0), ROOT(root), adj(G), operation(op), neutral(_neutral) {
        while((1 << LOG) <= N) LOG++;
        anc.assign(N + 5, vector < int > (LOG));
        cost.assign(N + 5, vector < treeType > (LOG, neutral));
        depth.assign(N + 5, 0);
        dfs(ROOT);
    }

    int kth_ancestor(int u, int k) const {
        if(depth[u] < k) return -1;
        for(int bit = 0; bit < LOG; bit++)
            if(k & (1 << bit))
                u = anc[u][bit];
        return u;
    }

    int get_lca(int u, int v) const {
        if(depth[u] < depth[v]) swap(u, v);

        u = kth_ancestor(u, depth[u] - depth[v]);
        if(u == v) return u;

        for(int bit = LOG - 1; bit >= 0; bit--)
            if(anc[u][bit] != anc[v][bit])
                u = anc[u][bit], v = anc[v][bit];

        return anc[u][0];
    }

    treeType query(int u, int v) const {
        int lca = get_lca(u, v);
        return operation(get_cost(u, depth[u] - depth[lca]), get_cost(v, depth[v] - depth[lca]));
    }

private:
    int N, LOG, ROOT;
    const vector < vector < pair < int, graphType > > > &adj;
    vector < vector < int > > anc;
    vector < vector < treeType > > cost;
    vector < int > depth;
    function < treeType(treeType, treeType) > operation;
    treeType neutral;

    void dfs(int u, int p = 0){
        for(auto& [v, w] : adj[u]){
            if(v == p) continue;
            depth[v] = depth[u] + 1;
            anc[v][0] = u, cost[v][0] = treeType(w);
            for(int bit = 1; bit < LOG; bit++){
                anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
                cost[v][bit] = operation(cost[v][bit - 1], cost[anc[v][bit - 1]][bit - 1]);
            }
            dfs(v, u);
        }
    }

    treeType get_cost(int u, int dist) const {
        if(depth[u] < dist) return neutral;
        treeType ret = neutral;
        for(int bit = 0; bit < LOG; bit++){
            if(dist & (1 << bit)){
                ret = operation(ret, cost[u][bit]);
                u = anc[u][bit];
            }
        }
        return ret;
    }
};

struct Edge {
    int mn, mx;
    Edge(int x){
        mn = mx = x;
    }
    Edge(int _mn, int _mx) : mn(_mn), mx(_mx) {}
};

Edge merge(const Edge &a, const Edge &b) {
    int mn = min(a.mn, b.mn);
    int mx = max(a.mx, b.mx);
    return Edge(mn, mx);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



#endif //TREES_H

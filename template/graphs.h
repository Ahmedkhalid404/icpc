//
// Created by ahmed on 7/30/2025.
//

#ifndef GRAPHS_H
#define GRAPHS_H


#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

// Depth-First Search (DFS)
// Time: O(N + M) where N is number of nodes, M is number of edges
void dfs(int u, const vector<vector<int>>& g, vector<bool>& vis) {
    vis[u] = true;
    for (int v : g[u]) {
        if (!vis[v])
            dfs(v, g, vis);
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Breadth-First Search (BFS)
// Time: O(N + M)
void bfs(int start, const vector<vector<int>>& g, vector<int>& dist) {
    int n = g.size();
    dist.assign(n, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Dijkstra's Algorithm for Single Source Shortest Path
// Time: O((N + M) * log N)
void dijkstra(int s, const vector<vector<pair<int, int>>>& g, vector<ll>& dist) {
    int n = g.size();
    dist.assign(n, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Floyd-Warshall Algorithm for All-Pairs Shortest Path
// Time: O(N^3)
void floydWarshall(vector<vector<ll>>& dist) {
    int n = dist.size();
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Disjoint Set Union (DSU) / Union-Find
// Time: O(α(N)) per operation with path compression and union by rank
struct DSU {
    vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;
        if (rank[u] < rank[v]) swap(u, v);
        parent[v] = u;
        if (rank[u] == rank[v]) rank[u]++;
        return true;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class DSU{
private:
    int size{};
    vector< int > parent,sz;

    int getRoot(int x){
        return parent[ x ] = ( parent[ x ] == x ? x : getRoot( parent[ x ] ) );
    }

    bool isUnion(int a,int b){
        return getRoot( a ) == getRoot( b );
    }

    void Union(int a,int b){
        int rootA = getRoot( a );
        int rootB = getRoot( b );

        if( rootA == rootB )
            return;

        if( sz[rootA] < sz[rootB] )
            swap(rootA, rootB);

        parent[ rootB ] = rootA;

        sz[ rootA ] += sz[ rootB ];
    }

    void init(int n){
        size = n;
        parent.resize( n );
        sz.resize( n );

        for (int i = 0; i < n; ++i) {
            parent[ i ]  = i;
            sz[ i ]      = 1;
        }

    }
public:
    explicit DSU(int n){
        init( n + 5 );
    }

    void connect(int a,int b){
        Union( a , b );
    }

    bool isConnected(int a,int b){
        return isUnion(a,b);
    }

    int getComponent(int a){
        return getRoot(a);
    }

    int getSize(int a){
        return sz[ getRoot(a) ];
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class DSU{
private:
    vector< int > parents;
    int size;

    bool isRoot(int x){
        return parents[ x ] == x;
    }

    int getRoot(int x){
        if( x <= 0 )
            return 0;

        if( parents[ x ] <= 0 )
            return 0;

        if( isRoot( x ) ){
            return x;
        }

        return parents[ x ] = getRoot( parents[ x ] );
    }

    void init(int n){
        parents.resize( n );
        for (int i = 1; i < n; ++i) {
            parents[ i ] = i;
        }
    }

    void remove(int x){
        parents[ x ] = getRoot( x - 1 );
    }


public:
    explicit DSU(int n){
        init( n + 5 );
        size = n;
    }
    int get(int x){
        return getRoot( x );
    }
    void set(int x){
        remove( x );
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Topological Sort (for DAGs)
// Time: O(N + M)
bool topoSort(const vector<vector<int>>& g, vector<int>& order) {
    int n = g.size();
    vector<int> in_deg(n, 0);
    for (const auto& u : g)
        for (int v : u)
            in_deg[v]++;
    queue<int> q;
    for (int i = 0; i < n; ++i)
        if (in_deg[i] == 0)
            q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--in_deg[v] == 0)
                q.push(v);
        }
    }
    return order.size() == n; // true if graph has no cycle
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Bellman-Ford Algorithm (can detect negative cycles)
// Time: O(N * M)
bool bellmanFord(int s, const vector<tuple<int, int, int>>& edges, int n, vector<ll>& dist) {
    dist.assign(n, INF);
    dist[s] = 0;
    for (int i = 1; i < n; ++i) {
        for (auto [u, v, w] : edges) {
            if (dist[u] < INF)
                dist[v] = min(dist[v], dist[u] + w);
        }
    }
    // Check for negative cycles
    for (auto [u, v, w] : edges)
        if (dist[u] < INF && dist[u] + w < dist[v])
            return false;
    return true;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Returns a topological ordering of a directed acyclic graph (DAG)
// If the graph has a cycle, returns empty vector
// Time Complexity: O(V + E)
vector<int> topological_sort_kahn(int n, const vector<vector<int>>& adj) {
    vector<int> indeg(n), order;
    for (int u = 0; u < n; ++u)
        for (int v : adj[u]) indeg[v]++;

    queue<int> q;
    for (int u = 0; u < n; ++u)
        if (indeg[u] == 0) q.push(u);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) q.push(v);
    }

    if (order.size() != n) return {}; // graph has cycle
    return order;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Computes single-source shortest paths in a weighted graph (supports negative weights)
// Returns distance array, or detects negative cycle if needed
// Time Complexity: O(V * E)
vector<int> bellman_ford(int n, int src, const vector<tuple<int, int, int>>& edges) {
    const int INF = 1e9;
    vector<int> dist(n, INF);
    dist[src] = 0;

    for (int i = 1; i < n; ++i) {
        for (auto [u, v, w] : edges) {
            if (dist[u] < INF)
                dist[v] = min(dist[v], dist[u] + w);
        }
    }

    // Optional: Negative cycle detection
    // for (auto [u, v, w] : edges)
    //     if (dist[u] < INF && dist[v] > dist[u] + w)
    //         throw runtime_error("Negative cycle detected");

    return dist;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Computes shortest paths with edge weights 0 or 1 using a deque
// Time Complexity: O(V + E)
vector<int> zero_one_bfs(int n, int src, const vector<vector<pair<int, int>>>& adj) {
    const int INF = 1e9;
    vector<int> dist(n, INF);
    dist[src] = 0;
    deque<int> dq;
    dq.push_back(src);

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w == 1) dq.push_back(v);
                else dq.push_front(v);
            }
        }
    }

    return dist;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Finds all strongly connected components in a directed graph
// Time Complexity: O(V + E)
class TarjanSCC {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> low, tin, onStack, compId;
    stack<int> st;
    vector<vector<int>> sccs;

    void dfs(int u) {
        tin[u] = low[u] = timer++;
        st.push(u); onStack[u] = true;

        for (int v : adj[u]) {
            if (tin[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (onStack[v]) {
                low[u] = min(low[u], tin[v]);
            }
        }

        if (low[u] == tin[u]) {
            vector<int> scc;
            int v;
            do {
                v = st.top(); st.pop();
                onStack[v] = false;
                compId[v] = sccs.size();
                scc.push_back(v);
            } while (v != u);
            sccs.push_back(scc);
        }
    }

public:
    explicit TarjanSCC(int _n) : n(_n), adj(_n), low(_n, -1), tin(_n, -1), onStack(_n, 0), compId(_n, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    vector<vector<int>> getSCCs() {
        for (int i = 0; i < n; ++i)
            if (tin[i] == -1) dfs(i);
        return sccs;
    }

    int getComponentId(int u) const {
        return compId[u];
    }
};

#endif //GRAPHS_H

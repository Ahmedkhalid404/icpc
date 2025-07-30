//
// Created by ahmed on 7/30/2025.
//

#ifndef SEGMNETTREE_H
#define SEGMNETTREE_H

#include <bits/stdc++.h>
using namespace std;


// normal segment tree
template <class Info>
class SegmentTree {
private:
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        Info val;

        Node() = default;
        explicit Node(const Info& v) : val(v) {}
    };

    Node* root;
    int n;

    void build(Node* node, int l, int r, const std::vector<Info>& arr) {
        if (l == r) {
            node->val = arr[l - 1];
            return;
        }
        int m = (l + r) / 2;
        node->left = new Node();
        node->right = new Node();
        build(node->left, l, m, arr);
        build(node->right, m + 1, r, arr);
        node->val = node->left->val + node->right->val;
    }

    void update(Node* node, int l, int r, int idx, const Info& newVal) {
        if (idx < l || idx > r)
            return;
        if (l == r) {
            node->val = newVal;
            return;
        }
        int m = (l + r) / 2;
        update(node->left, l, m, idx, newVal);
        update(node->right, m + 1, r, idx, newVal);
        node->val = node->left->val + node->right->val;
    }

    Info query(Node* node, int l, int r, int ql, int qr) {
        if (qr < l || ql > r)
            return Info(); // neutral element
        if (ql <= l && r <= qr)
            return node->val;

        int m = (l + r) / 2;
        return query(node->left, l, m, ql, qr) + query(node->right, m + 1, r, ql, qr);
    }

public:
    explicit SegmentTree(const std::vector<Info>& arr) {
        n = arr.size(); // 0-based indexing
        root = new Node();
        build(root, 1, n, arr);
    }

    void set(int idx, const Info& val) {
        update(root, 1, n, idx, val);
    }

    Info get(int l, int r) {
        return query(root, 1, n, l, r);
    }
};

struct Info {
    int sum = 0;
    int mn = INT_MAX;
    int mx = INT_MIN;
    int gcd = 0;
    int lcm = 1;
    int bw_and = -1; // All bits set
    int bw_or = 0;

    Info() = default;
    Info(int x) {
        sum = x;
        mn = mx = x;
        gcd = x;
        lcm = x;
        bw_and = x;
        bw_or = x;
    }

    static int get_gcd(int a, int b) {
        return std::gcd(a, b);
    }

    static int get_lcm(int a, int b) {
        if (a == 0 || b == 0) return 0;
        return std::lcm(a,b);
    }

    Info operator+(const Info& other) const {
        Info res;
        res.sum = sum + other.sum;
        res.mn = std::min(mn, other.mn);
        res.mx = std::max(mx, other.mx);
        res.gcd = get_gcd(gcd, other.gcd);
        res.lcm = get_lcm(lcm, other.lcm);
        res.bw_and = bw_and & other.bw_and;
        res.bw_or = bw_or | other.bw_or;
        return res;
    }
};


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// create only access nodes can access index up to 2 * 1e9 (but you can't use all range)
template <class Info>
class DynamicSegmentTree {
private:
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        Info val;

        Node() = default;
    };

    Node* root;
    int n;

    void update(Node*& node, int l, int r, int idx, const Info& newVal) {
        if (!node) node = new Node();

        if (l == r) {
            node->val = newVal;
            return;
        }

        int m = (l + r) / 2;
        if (idx <= m) update(node->left, l, m, idx, newVal);
        else update(node->right, m + 1, r, idx, newVal);

        Info leftVal = node->left ? node->left->val : Info();
        Info rightVal = node->right ? node->right->val : Info();
        node->val = leftVal + rightVal;
    }

    Info query(Node* node, int l, int r, int ql, int qr) {
        if (!node || qr < l || ql > r)
            return Info(); // neutral
        if (ql <= l && r <= qr)
            return node->val;

        int m = (l + r) / 2;
        Info leftVal = query(node->left, l, m, ql, qr);
        Info rightVal = query(node->right, m + 1, r, ql, qr);
        return leftVal + rightVal;
    }

public:
    explicit DynamicSegmentTree(int size) {
        n = size;
        root = nullptr;
    }

    void set(int idx, const Info& val) {
        update(root, 0, n - 1, idx, val);
    }

    Info get(int l, int r) {
        return query(root, 0, n - 1, l, r);
    }
};


struct Info {
    int sum = 0;
    int mn = INT_MAX;
    int mx = INT_MIN;
    int gcd = 0;
    int lcm = 1;
    int bw_and = -1; // All bits set
    int bw_or = 0;

    Info() = default;
    Info(int x) {
        sum = x;
        mn = mx = x;
        gcd = x;
        lcm = x;
        bw_and = x;
        bw_or = x;
    }

    static int get_gcd(int a, int b) {
        return std::gcd(a, b);
    }

    static int get_lcm(int a, int b) {
        if (a == 0 || b == 0) return 0;
        return std::lcm(a,b);
    }

    Info operator+(const Info& other) const {
        Info res;
        res.sum = sum + other.sum;
        res.mn = std::min(mn, other.mn);
        res.mx = std::max(mx, other.mx);
        res.gcd = get_gcd(gcd, other.gcd);
        res.lcm = get_lcm(lcm, other.lcm);
        res.bw_and = bw_and & other.bw_and;
        res.bw_or = bw_or | other.bw_or;
        return res;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// lazy plus range and get max value
template<class T>
class LazySegmentTree{
private:
    struct node{
        node *left{},*right{};
        T val = -1;
        int64_t lazy = 0;
    };
    node *root;
    vector<int> nums;
    int size;

    void Union(node *seg,int l,int r){
        seg->val = seg->val + seg->lazy;
        if( l != r ){
            seg->left->lazy += seg->lazy;
            seg->right->lazy += seg->lazy;
        }
        seg->lazy = 0;
    }

    void marge(node *seg){
        seg->val = max(seg->left->val, seg->right->val);
    }

    T merge(T left, T right) {
        return max(left, right);
    }

    void build(node *seg,int l,int r){
        if( l == r ) {
            seg->val = nums[l - 1];
            return;
        }

        int md =  (l + r) >> 1;
        seg->left = new node;
        seg->right = new node;
        build( seg->left , l , md );
        build( seg->right , md + 1 , r );

        marge(seg);
    }
    void update(node *seg,int l,int r,int _l,int _r,int val){
        Union(seg,l,r);

        if( ( l > _r ) or ( r < _l ) )
            return;

        if( ( l >= _l ) && ( r <= _r ) ){
            seg->lazy += val;
            Union(seg,l,r);
            return;
        }

        int md = (l + r) >> 1;
        update( seg->left , l , md , _l , _r , val );
        update( seg->right , md + 1 , r , _l , _r , val );

        marge(seg);
    }

    int get(node *seg,int l,int r,int _l, int _r){

        Union(seg,l,r);

        if( ( l > _r ) or ( r < _l ) )
            return -1;

        if( ( l >= _l ) && ( r <= _r ) ){
            return seg->val;
        }

        int md = ( l + r ) >> 1;
        int left = get( seg->left , l , md , _l, _r );
        int right = get( seg->right , md + 1 , r , _l, _r );

        marge(seg);

        return merge(left, right);
    }
public:
    explicit LazySegmentTree(const vector<T> &nums){
        this->nums = nums;
        this->size = nums.size();
        this->root = new node;
        build( this->root , 1 , this->size );
    }
    void plusRange(const int l,const int r,const int val){
        update( this->root , 1 , size , l , r, val );
    }
    int get(const int l, const int r){
        return get( this->root , 1 , size , l, r );
    }
};


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// lazy segment tree set, plus (range)
struct Info {
    int64_t sum = 0, mn = INT64_MAX, mx = INT64_MIN;
    int64_t gcd = 0, lcm = 1, bw_and = -1, bw_or = 0;
    int size = 1;

    Info() = default;
    Info(int64_t x) {
        sum = x;
        // mn = mx = gcd = lcm = bw_and = bw_or = x;
    }

    friend Info operator+(const Info& a, const Info& b) {
        if (a.size == 0) return b;
        if (b.size == 0) return a;
        Info res;
        res.sum = a.sum + b.sum;
        // res.mn = min(a.mn, b.mn);
        // res.mx = max(a.mx, b.mx);
        // res.gcd = std::gcd(a.gcd, b.gcd);
        // res.lcm = std::lcm(a.lcm, b.lcm);
        // res.bw_and = a.bw_and & b.bw_and;
        // res.bw_or = a.bw_or | b.bw_or;
        res.size = a.size + b.size;
        return res;
    }

    void apply(int64_t val, int type) {
        if (type == 1) { // += val
            sum += val * size;
            // mn += val;
            // mx += val;
            // if (size == 1) {
            //     gcd += val;
            //     lcm += val;
            //     bw_and += val;
            //     bw_or += val;
            // }
        } else if (type == 2) { // = val
            sum = val * size;
            // mn = mx = gcd = lcm = bw_and = bw_or = val;
        }
    }
};

struct Lazy {
    int64_t add = 0, assign = LLONG_MIN;

    Lazy() = default;
    Lazy(int64_t v, bool isAssign) {
        if (isAssign) assign = v;
        else add = v;
    }

    void operator+=(const Lazy& other) {
        if (other.assign != LLONG_MIN) {
            assign = other.assign;
            add = other.add;
        } else {
            add += other.add;
        }
    }

    bool has() const {
        return assign != LLONG_MIN || add != 0;
    }

    void apply(Info& info) const {
        if (assign != LLONG_MIN) info.apply(assign, 2);
        if (add != 0) info.apply(add, 1);
    }

    void reset() {
        add = 0;
        assign = LLONG_MIN;
    }
};

class LazySegmentTree {
private:
    struct Node {
        Node *left{}, *right{};
        Info val;
        Lazy lazy;
        bool hasLazy = false;
    };

    Node* root;
    int size;

    void push(Node*& node, int l, int r) {
        if (!node) node = new Node;
        if (!node->hasLazy) return;

        node->lazy.apply(node->val);

        if (l != r) {
            if (!node->left) node->left = new Node;
            if (!node->right) node->right = new Node;

            node->left->lazy += node->lazy;
            node->right->lazy += node->lazy;
            node->left->hasLazy = node->right->hasLazy = true;
        }

        node->lazy.reset();
        node->hasLazy = false;
    }

    void build(Node* node, int l, int r, const vector<int64_t>& a) {
        if (l == r) {
            node->val = Info(a[l]);
            return;
        }
        int m = (l + r) / 2;
        node->left = new Node;
        node->right = new Node;
        build(node->left, l, m, a);
        build(node->right, m + 1, r, a);
        node->val = node->left->val + node->right->val;
    }

    void update(Node*& node, int l, int r, int ql, int qr, Lazy upd) {
        push(node, l, r);

        if (qr < l || ql > r) return;

        if (ql <= l && r <= qr) {
            node->lazy += upd;
            node->hasLazy = true;
            push(node, l, r);
            return;
        }

        int m = (l + r) / 2;
        update(node->left, l, m, ql, qr, upd);
        update(node->right, m + 1, r, ql, qr, upd);
        node->val = node->left->val + node->right->val;
    }

    Info query(Node* node, int l, int r, int ql, int qr) {
        if (!node || qr < l || ql > r) return Info{};

        push(node, l, r);

        if (ql <= l && r <= qr) return node->val;

        int m = (l + r) / 2;
        return query(node->left, l, m, ql, qr) + query(node->right, m + 1, r, ql, qr);
    }

public:
    explicit LazySegmentTree(const vector<int64_t>& a) {
        size = a.size();
        root = new Node;
        build(root, 0, size - 1, a);
    }

    void addRange(int l, int r, int64_t val) {
        update(root, 0, size - 1, l - 1, r - 1, Lazy(val, false));
    }

    void setRange(int l, int r, int64_t val) {
        update(root, 0, size - 1, l - 1, r - 1, Lazy(val, true));
    }

    Info query(int l, int r) {
        return query(root, 0, size - 1, l - 1, r - 1);
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//PersistentSegmentTree

/*
int main() {
    PersistentSegmentTree seg(5); // [0, 0, 0, 0, 0]

    seg.set(0, 2, 10); // v1 = [0, 0, 10, 0, 0]
    seg.set(1, 4, 20); // v2 = [0, 0, 10, 0, 20]

    cout << seg.rangeQuery(0, 0, 4) << "\n"; // 0
    cout << seg.rangeQuery(1, 0, 4) << "\n"; // 10
    cout << seg.rangeQuery(2, 0, 4) << "\n"; // 30
*/

struct Node {
    Node *left = nullptr, *right = nullptr;
    int64_t val = 0;

    Node() = default;
    Node(int64_t v) : val(v) {}
    Node(Node* l, Node* r) {
        left = l;
        right = r;
        val = 0;
        if (l) val += l->val;
        if (r) val += r->val;
    }
};

class PersistentSegmentTree {
    int n;
    vector<Node*> versions;

    Node* build(int l, int r) {
        if (l == r) return new Node(0);
        int m = (l + r) / 2;
        return new Node(build(l, m), build(m + 1, r));
    }

    Node* update(Node* node, int l, int r, int idx, int64_t val) {
        if (l == r)
            return new Node(val);
        int m = (l + r) / 2;
        if (idx <= m)
            return new Node(update(node->left, l, m, idx, val), node->right);
        else
            return new Node(node->left, update(node->right, m + 1, r, idx, val));
    }

    int64_t query(Node* node, int l, int r, int ql, int qr) {
        if (!node || qr < l || ql > r) return 0;
        if (ql <= l && r <= qr) return node->val;
        int m = (l + r) / 2;
        return query(node->left, l, m, ql, qr) +
               query(node->right, m + 1, r, ql, qr);
    }

public:
    explicit PersistentSegmentTree(int size) {
        n = size;
        versions.push_back(build(0, n - 1)); // version 0
    }

    void set(int version, int idx, int64_t val) {
        versions.push_back(update(versions[version], 0, n - 1, idx, val));
    }

    int64_t rangeQuery(int version, int l, int r) {
        return query(versions[version], 0, n - 1, l, r);
    }

    int numVersions() const {
        return versions.size();
    }
};


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SGTBeats {
    static const ll INF = (ll)1e18;
    int n, n0;
    vector<ll> sum, max_v, smax_v, max_c;
    vector<ll> min_v, smin_v, min_c;
    vector<ll> len, ladd, lval;

    SGTBeats(int _n, const vector<ll>& a) {
        n = _n;
        n0 = 1;
        while (n0 < n) n0 <<= 1;
        int m = 2 * n0;
        sum.assign(m, 0);
        max_v.assign(m, -INF);
        smax_v.assign(m, -INF);
        max_c.assign(m, 0);
        min_v.assign(m, INF);
        smin_v.assign(m, INF);
        min_c.assign(m, 0);
        ladd.assign(m, 0);
        lval.assign(m, INF);
        len.assign(m, 0);

        for (int i = 0; i < n; i++) {
            sum[n0 - 1 + i] = max_v[n0 - 1 + i] = min_v[n0 - 1 + i] = a[i];
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 1;
        }
        for (int i = n; i < n0; i++) {
            sum[n0 - 1 + i] = 0;
            max_v[n0 - 1 + i] = -INF; min_v[n0 - 1 + i] = INF;
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 0;
        }
        for (int i = n0 - 2; i >= 0; i--) {
            len[i] = len[2 * i + 1] + len[2 * i + 2];
            pull(i);
        }
    }

    void pull(int k) {
        sum[k] = sum[2*k+1] + sum[2*k+2];
        if (max_v[2*k+1] > max_v[2*k+2]) {
            max_v[k] = max_v[2*k+1];
            max_c[k] = max_c[2*k+1];
            smax_v[k] = max(smax_v[2*k+1], max_v[2*k+2]);
        } else if (max_v[2*k+1] < max_v[2*k+2]) {
            max_v[k] = max_v[2*k+2];
            max_c[k] = max_c[2*k+2];
            smax_v[k] = max(max_v[2*k+1], smax_v[2*k+2]);
        } else {
            max_v[k] = max_v[2*k+1];
            max_c[k] = max_c[2*k+1] + max_c[2*k+2];
            smax_v[k] = max(smax_v[2*k+1], smax_v[2*k+2]);
        }
        if (min_v[2*k+1] < min_v[2*k+2]) {
            min_v[k] = min_v[2*k+1];
            min_c[k] = min_c[2*k+1];
            smin_v[k] = min(smin_v[2*k+1], min_v[2*k+2]);
        } else if (min_v[2*k+1] > min_v[2*k+2]) {
            min_v[k] = min_v[2*k+2];
            min_c[k] = min_c[2*k+2];
            smin_v[k] = min(min_v[2*k+1], smin_v[2*k+2]);
        } else {
            min_v[k] = min_v[2*k+1];
            min_c[k] = min_c[2*k+1] + min_c[2*k+2];
            smin_v[k] = min(smin_v[2*k+1], smin_v[2*k+2]);
        }
    }

    void update_node_max(int k, ll x) {
        sum[k] += (x - max_v[k]) * max_c[k];
        max_v[k] = x;
        if (min_v[k] == x) {
            min_v[k] = x;
        }
    }

    void update_node_min(int k, ll x) {
        sum[k] += (x - min_v[k]) * min_c[k];
        min_v[k] = x;
        if (max_v[k] == x) {
            max_v[k] = x;
        }
    }

    void push(int k) {
        if (lval[k] != INF) {
            apply_assign(2*k+1, lval[k]);
            apply_assign(2*k+2, lval[k]);
            lval[k] = INF;
            return;
        }
        if (ladd[k] != 0) {
            apply_add(2*k+1, ladd[k]);
            apply_add(2*k+2, ladd[k]);
            ladd[k] = 0;
        }
        if (max_v[k] < max_v[2*k+1]) update_node_max(2*k+1, max_v[k]);
        if (max_v[k] < max_v[2*k+2]) update_node_max(2*k+2, max_v[k]);
        if (min_v[2*k+1] < min_v[k]) update_node_min(2*k+1, min_v[k]);
        if (min_v[2*k+2] < min_v[k]) update_node_min(2*k+2, min_v[k]);
    }

    void apply_add(int k, ll x) {
        sum[k] += len[k] * x;
        max_v[k] += x;
        if (smax_v[k] != -INF) smax_v[k] += x;
        min_v[k] += x;
        if (smin_v[k] != INF) smin_v[k] += x;
        ladd[k] += x;
    }

    void apply_assign(int k, ll x) {
        max_v[k] = min_v[k] = x;
        smax_v[k] = -INF;
        smin_v[k] = INF;
        sum[k] = x * len[k];
        lval[k] = x;
        ladd[k] = 0;
    }

    void _update_min(int a, int b, ll x, int k, int l, int r) {
        if (r <= a || b <= l || max_v[k] <= x) return;
        if (a <= l && r <= b && smax_v[k] < x) {
            update_node_max(k, x);
            return;
        }
        push(k);
        int m = (l + r) / 2;
        _update_min(a,b,x,2*k+1,l,m);
        _update_min(a,b,x,2*k+2,m,r);
        pull(k);
    }

    void _update_max(int a, int b, ll x, int k, int l, int r) {
        if (r <= a || b <= l || x <= min_v[k]) return;
        if (a <= l && r <= b && x < smin_v[k]) {
            update_node_min(k, x);
            return;
        }
        push(k);
        int m = (l + r)/2;
        _update_max(a,b,x,2*k+1,l,m);
        _update_max(a,b,x,2*k+2,m,r);
        pull(k);
    }

    void _add(int a,int b,ll x,int k,int l,int r){
        if (r <= a || b <= l) return;
        if (a <= l && r <= b) {
            apply_add(k, x);
            return;
        }
        push(k);
        int m = (l+r)/2;
        _add(a,b,x,2*k+1,l,m);
        _add(a,b,x,2*k+2,m,r);
        pull(k);
    }

    void _assign(int a,int b,ll x,int k,int l,int r){
        if (r <= a || b <= l) return;
        if (a <= l && r <= b) {
            apply_assign(k, x);
            return;
        }
        push(k);
        int m = (l+r)/2;
        _assign(a,b,x,2*k+1,l,m);
        _assign(a,b,x,2*k+2,m,r);
        pull(k);
    }

    ll _query_sum(int a,int b,int k,int l,int r) {
        if (r <= a || b <= l) return 0;
        if (a <= l && r <= b) return sum[k];
        push(k);
        int m = (l+r)/2;
        return _query_sum(a,b,2*k+1,l,m) + _query_sum(a,b,2*k+2,m,r);
    }
    ll _query_min(int a,int b,int k,int l,int r) {
        if (r <= a || b <= l) return INF;
        if (a <= l && r <= b) return min_v[k];
        push(k);
        int m = (l+r)/2;
        return min(_query_min(a,b,2*k+1,l,m), _query_min(a,b,2*k+2,m,r));
    }
    ll _query_max(int a,int b,int k,int l,int r) {
        if (r <= a || b <= l) return -INF;
        if (a <= l && r <= b) return max_v[k];
        push(k);
        int m = (l+r)/2;
        return max(_query_max(a,b,2*k+1,l,m), _query_max(a,b,2*k+2,m,r));
    }

    void range_chmin(int l,int r,ll x){ _update_min(l,r,x,0,0,n0); }
    void range_chmax(int l,int r,ll x){ _update_max(l,r,x,0,0,n0); }
    void range_add (int l,int r,ll x){ _add(l,r,x,0,0,n0); }
    void range_assign(int l,int r,ll x){ _assign(l,r,x,0,0,n0); }
    ll query_sum(int l,int r){ return _query_sum(l,r,0,0,n0); }
    ll query_min(int l,int r){ return _query_min(l,r,0,0,n0); }
    ll query_max(int l,int r){ return _query_max(l,r,0,0,n0); }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// xor basis + segment tree
const int BITS = __lg(100000) + 1;

class XorBasis {
    int size = 0;
    std::array<int, BITS> basis{};

public:
    void clear() {
        size = 0;
        basis.fill(0);
    }

    void insert(int x) {
        if (size == BITS) return;
        for (int i = __lg(x); x; i = __lg(x)) {
            if (!basis[i]) {
                basis[i] = x;
                size++;
                return;
            }
            x ^= basis[i];
        }
    }

    bool contains(int x) const {
        if (size == BITS) return true;
        for (int i = __lg(x); x; i = __lg(x)) {
            if (!basis[i]) return false;
            x ^= basis[i];
        }
        return x == 0;
    }

    int getMaxXor() const {
        int res = 0;
        for (int i = BITS - 1; i >= 0; --i) {
            if ((res ^ basis[i]) > res)
                res ^= basis[i];
        }
        return res;
    }

    XorBasis& operator+=(const XorBasis& other) {
        if (size == BITS) return *this;
        for (int i = 0; i < BITS; ++i)
            if (other.basis[i]) insert(other.basis[i]);
        return *this;
    }

    std::array<int, BITS> exportBasis() const { return basis; }
};

class SegmentTreeXorBasis {
private:
    int n;
    std::vector<XorBasis> tree;
    std::vector<int> lazy;

    void build(int x, int lx, int rx, const std::vector<int>& arr) {
        if (lx == rx) {
            tree[x].insert(arr[lx]);
            return;
        }
        int mid = (lx + rx) / 2;
        build(x * 2 + 1, lx, mid, arr);
        build(x * 2 + 2, mid + 1, rx, arr);
        tree[x] = tree[x * 2 + 1];
        tree[x] += tree[x * 2 + 2];
    }

    void push(int x, int lx, int rx) {
        if (lazy[x] == -1) return;

        auto b = tree[x].exportBasis();
        tree[x].clear();
        for (int val : b)
            if (val) tree[x].insert(val & lazy[x]);

        if (lx != rx) {
            lazy[x * 2 + 1] = (lazy[x * 2 + 1] == -1 ? lazy[x] : lazy[x * 2 + 1] & lazy[x]);
            lazy[x * 2 + 2] = (lazy[x * 2 + 2] == -1 ? lazy[x] : lazy[x * 2 + 2] & lazy[x]);
        }

        lazy[x] = -1;
    }

    void updateAnd(int x, int lx, int rx, int l, int r, int val) {
        push(x, lx, rx);
        if (lx > r || rx < l) return;
        if (lx >= l && rx <= r) {
            lazy[x] = (lazy[x] == -1 ? val : lazy[x] & val);
            push(x, lx, rx);
            return;
        }
        int mid = (lx + rx) / 2;
        updateAnd(x * 2 + 1, lx, mid, l, r, val);
        updateAnd(x * 2 + 2, mid + 1, rx, l, r, val);
        tree[x] = tree[x * 2 + 1];
        tree[x] += tree[x * 2 + 2];
    }

    void pointSet(int x, int lx, int rx, int idx, int val) {
        push(x, lx, rx);
        if (lx == rx) {
            tree[x].clear();
            tree[x].insert(val);
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx <= mid) pointSet(x * 2 + 1, lx, mid, idx, val);
        else pointSet(x * 2 + 2, mid + 1, rx, idx, val);
        tree[x] = tree[x * 2 + 1];
        tree[x] += tree[x * 2 + 2];
    }

    XorBasis query(int x, int lx, int rx, int l, int r) {
        push(x, lx, rx);
        if (lx > r || rx < l) return {};
        if (lx >= l && rx <= r) return tree[x];
        int mid = (lx + rx) / 2;
        XorBasis left = query(x * 2 + 1, lx, mid, l, r);
        XorBasis right = query(x * 2 + 2, mid + 1, rx, l, r);
        return left += right;
    }

public:
    SegmentTreeXorBasis(const std::vector<int>& arr) {
        n = arr.size();
        tree.resize(n << 2);
        lazy.assign(n << 2, -1);
        build(0, 0, n - 1, arr);
    }

    void applyAnd(int l, int r, int val) {
        updateAnd(0, 0, n - 1, l, r, val);
    }

    void update(int idx, int val) {
        pointSet(0, 0, n - 1, idx, val);
    }

    XorBasis get(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }
};


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct HLD {
    int n, t;
    vector<vector<int>> g;
    vector<int> parent, depth, heavy, head, pos, values, seg;

    HLD(int size) : n(size), g(n), parent(n), depth(n), heavy(n, -1),
                    head(n), pos(n), values(n), seg(4 * n), t(0) {}

    // O(1)
    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // O(n)
    int dfs(int u, int p) {
        int sz = 1, maxSub = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            parent[v] = u, depth[v] = depth[u] + 1;
            int sub = dfs(v, u);
            if (sub > maxSub) maxSub = sub, heavy[u] = v;
            sz += sub;
        }
        return sz;
    }

    // O(n)
    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = t++;
        if (heavy[u] != -1)
            decompose(heavy[u], h);
        for (int v : g[u])
            if (v != parent[u] && v != heavy[u])
                decompose(v, v);
    }

    // O(n)
    void build(vector<int> &init) {
        values = init;
        dfs(0, -1);
        decompose(0, 0);
        buildSeg(0, 0, n - 1);
    }

    // Segment Tree Functions


    /// TODO : update here !!!
    // Combine two values (max here)
    int combine(int a, int b) {
        return max(a, b);
    }
    const int neutral = -INF;



    // O(log n)
    void buildSeg(int x, int lx, int rx) {
        if (lx == rx) {
            seg[x] = values[findNode(lx)];
            return;
        }
        int m = (lx + rx) / 2;
        buildSeg(2 * x + 1, lx, m);
        buildSeg(2 * x + 2, m + 1, rx);
        seg[x] = combine(seg[2 * x + 1], seg[2 * x + 2]);
    }

    // O(log n)
    void setSeg(int x, int lx, int rx, int i, int v) {
        if (lx == rx) {
            seg[x] = v;
            return;
        }
        int m = (lx + rx) / 2;
        if (i <= m) setSeg(2 * x + 1, lx, m, i, v);
        else setSeg(2 * x + 2, m + 1, rx, i, v);
        seg[x] = combine(seg[2 * x + 1], seg[2 * x + 2]);
    }

    // O(log n)
    int getSeg(int x, int lx, int rx, int l, int r) {
        if (r < lx || rx < l) return neutral;
        if (l <= lx && rx <= r) return seg[x];
        int m = (lx + rx) / 2;
        return combine(
            getSeg(2 * x + 1, lx, m, l, r),
            getSeg(2 * x + 2, m + 1, rx, l, r)
        );
    }

    int findNode(int p) {
        // get node from position
        static vector<int> index;
        if (index.empty()) {
            index.resize(n);
            for (int i = 0; i < n; i++)
                index[pos[i]] = i;
        }
        return index[p];
    }

    // O(log^2 n)
    void update(int u, int val) {
        setSeg(0, 0, n - 1, pos[u], val);
    }

    // O(log^2 n)
    int query(int u, int v) {
        int res = neutral;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res = combine(res, getSeg(0, 0, n - 1, pos[head[u]], pos[u]));
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        res = combine(res, getSeg(0, 0, n - 1, pos[u], pos[v]));
        return res;
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    int n, q;
    cin >> n >> q;
    HLD hld(n);

    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        --u, --v;
        hld.addEdge(u, v);
    }

    vector<int> vals(n);
    for (int i = 0; i < n; i++) cin >> vals[i];
    hld.build(vals);

    while (q--) {
        int type; cin >> type;
        if (type == 1) {
            int u, x; cin >> u >> x;
            hld.update(u - 1, x);
        } else {
            int u, v; cin >> u >> v;
            cout << hld.query(u - 1, v - 1) << '\n';
        }
    }

    return 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#endif //SEGMNETTREE_H

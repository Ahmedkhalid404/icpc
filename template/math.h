//
// Created by ahmed on 7/30/2025.
//

#ifndef MATH_H
#define MATH_H

#include <bits/stdc++.h>
using namespace std;

/*
 sum of divisors
 prime^power * prime2^power2 * ...

 ((prime^(power + 1) - 1) / (prime - 1)) * ((prime2^(power2 + 1) - 1) / (prime2 - 1)) * ...
 ===================================================================================================
 a % m == b
 a and m not coprime
 g = gcd(a, m)
 (a / g) % (m / g) = b / g

 a^x % m == b
 a and m not coprime
 g = gcd(a, m)
 (a^(x-1) * (a / g)) % (m / g) = b / g
 ===================================================================================================
 a^(power%phi(m)) % m;
 ===================================================================================================
 count balanced brackets
 r=n/2  ||  or r = number of opened brackets
 nCr(n, r) - nCr(n, r-1)
 ===================================================================================================
 // different n*n grids whose each square have m colors
 // if possible to rotate one of them so that they look the same then they same
 t = n * n;
 total = (fp(m, t)
     + fp(m, (t + 1) / 2)
     + 2 * fp(m, (t / 4) + (n % 2))) % mod;
 total = mul(total, fp(4, mod - 2));
 ===================================================================================================
 biggest divisors
 735134400 1344 => 2^6 3^3 5^2 7 11 13 17
 73513440 768
 ===================================================================================================
 for (int x = mask; x > 0; x = (x - 1) & mask)
 get all x such that mask = mask | x
 ===================================================================================================
 sum from 1 to n: i * nCr(n, i) = n * (1LL << (n - 1))
 ==================================================================================================
  g++ main.cpp -o main "-Wl,--stack,16777216"
 */

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


bool isPrime(ll n) {
    if(n < 4) return n > 1;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (ll i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

array<ll, 3> eGcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    auto [g, x1, y1] = eGcd(b, a % b);
    return {g, y1, x1 - (a / b) * y1};
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


ll modInv(ll a, ll m) {
    ll x = 1, x1 = 0, q, t, b = m;
    while(b) {
        q = a / b;
        a -= q * b, t = a, a = b, b = t;
        x -= q * x1, t = x, x = x1, x1 = t;
    }
    assert(a == 1);
    return (x + m) % m;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int fp(int b, int p) {
    int res = 1;
    while(p) {
        if(p & 1) res = int(res * 1LL * b % mod);
        b = int(b * 1LL * b % mod), p >>= 1;
    }
    return res;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int sumNPowerM(int n, int m) { // 1^m + 2^m ... n^m
    int k = m + 3;
    vector<int> res(k);
    for(int i = 1; i < k; i++) res[i] = (res[i - 1] + fp(i, m)) % mod;
    if(n < k) return res[n];
    int facK = k;
    vector<int> p(k); p[0] = 1;
    for(int i = 1; i < k; i++) {
        p[i] = int(p[i - 1] * 1LL * (n - i) % mod);
        facK = int(facK * 1LL * i % mod);
    }
    vector<int> inv(k + 1), s(k + 1);
    inv[k] = fp(facK, mod - 2), s[k] = 1;
    for(int i = k - 1; i >= 0; i--) {
        s[i] = int(s[i + 1] * 1LL * (n - i) % mod);
        inv[i] = int(inv[i + 1] * 1LL * (i + 1) % mod);
    }
    int ans = 0;
    for(int i = 1; i < k; i++) {
        //        int cur = res[i];
        //        for(int j = 1; j < k; j++) {
        //            if(i == j) continue;
        //            cur = int(cur * 1LL * (n - j) % mod);
        //            cur = int(cur * 1LL * fp(abs(i - j), mod - 2) % mod);
        //        }
        //        if((k - i + 1) & 1) cur = (mod - cur) % mod;
        int cur = int(res[i] * 1LL * p[i - 1] % mod * s[i + 1] % mod * inv[i - 1] % mod * inv[k - i - 1] % mod);
        if((k - i + 1) & 1) cur = (mod - cur) % mod;
        ans = (ans + cur) % mod;
    }
    return ans;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<int Log = 60> // Supports up to 60-bit long long values
class trie_xor {
    struct node {
        int cnt = 0;
        node* mp[2]{};

        ~node() {
            delete mp[0];
            delete mp[1];
        }
    };

    node* root = new node;

public:
    ~trie_xor() { delete root; }

    // Inserts a number `num` into the trie `c` times (default is 1)
    void add(long long num, int c = 1) {
        node* cur = root;
        for (int i = Log; i >= 0; --i) {
            cur->cnt += c;
            int bit = (num >> i) & 1;
            if (!cur->mp[bit]) cur->mp[bit] = new node;
            cur = cur->mp[bit];
        }
        cur->cnt += c;
    }

    // Removes a number `num` from the trie `c` times (default is 1)
    void remove(long long num, int c = 1) {
        add(num, -c);
    }

    // Returns the maximum value of (num XOR x) for x in the trie
    long long getMaxXor(long long num) const {
        return get(num, true);
    }

    // Returns the minimum value of (num XOR x) for x in the trie
    long long getMinXor(long long num) const {
        return get(num, false);
    }

    // Clears the entire trie
    void clear() {
        delete root;
        root = new node;
    }

private:
    // Helper function: returns min/max XOR of num with existing trie numbers
    long long get(long long num, bool findMax) const {
        if (root->cnt <= 0) return findMax ? 0 : LLONG_MAX;
        node* cur = root;
        long long ans = 0;
        for (int i = Log; i >= 0; --i) {
            int bit = ((num >> i) & 1) ^ findMax;
            if (cur->mp[bit] && cur->mp[bit]->cnt > 0) {
                if (bit) ans |= (1LL << i);
                cur = cur->mp[bit];
            } else {
                if (!bit) ans |= (1LL << i);
                cur = cur->mp[!bit];
            }
        }
        return ans ^ num;
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const int bits = __lg(100000) + 1;

struct basis {
    int sz = 0;
    array<int, bits> a{};

    // Adds a number to the basis if it cannot already be represented
    void add(int x) {
        if (sz == bits) return;
        for (int i = __lg(x); x; x ^= a[i], i = __lg(x)) {
            if (!a[i]) {
                a[i] = x;
                sz++;
                return;
            }
        }
    }

    // Returns true if x can be formed using XOR of the current basis
    bool find(int x) {
        if (sz == bits) return true;
        for (int i = __lg(x); x; i = __lg(x)) {
            if (a[i]) x ^= a[i];
            else return false;
        }
        return true;
    }

    // Clears the basis and resets all values
    void clear() {
        if (sz) a.fill(0), sz = 0;
    }

    // Returns the maximum XOR value that can be formed using the basis
    int getMax() {
        int r = 0;
        for (int i = bits - 1; i >= 0; i--)
            r = max(r, r ^ a[i]);
        return r;
    }

    // Returns the k-th smallest number that can be formed using XOR of the basis (0-based)
    int find_k(int k) {
        assert(k >= 0 && k < (1 << sz));
        int curr = 0;
        for (int i = bits - 1, b = sz - 1; i >= 0; i--) {
            if (a[i]) {
                if (((k >> b) & 1) ^ ((curr >> i) & 1))
                    curr ^= a[i];
                b--;
            }
        }
        return curr;
    }

    // Merges another basis into the current one
    basis& operator+=(const basis& o) {
        if (sz == bits) return *this;
        if (o.sz == bits) return *this = o;
        for (int i = 0; i < bits; i++)
            if (o.a[i])
                add(o.a[i]);
        return *this;
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace FFT {
    const int mod = 998244353; // 998244353 754974721 167772161
    const int root = 3; // 3 11 3
    const int invRoot = 332748118; // 332748118 617706590 55924054

    inline int mul(int x, int y) { return int(x * 1LL * y % mod); }
    inline int add(int x, int y) { return x + y < mod? x + y: x + y - mod; }
    inline int sub(int x, int y) { return x - y < 0? x - y + mod: x - y; }
    int fp(int b, int e) {
        int res = 1;
        while(e) {
            if(e & 1) res = mul(res, b);
            b = mul(b, b), e >>= 1;
        }
        return res;
    }
    void primitiveRoot() {
        int phi = mod - 1;
        vector<int> fac;
        for(int i = 2; i * 1LL * i < phi; i++) {
            if(phi % i == 0) {
                fac.push_back(i);
                while(phi % i == 0) phi /= i;
            }
        }
        if(phi > 1) fac.push_back(phi);

        for(int g = 2; g < mod; g++) {
            for(int pr : fac) if(fp(g, (mod - 1) / pr) == 1)
                goto bad;
            cout << "const int root = " << g << ";\n";
            cout << "const int invRoot = " << fp(g, mod - 2) << ";\n";
            return;
            bad:;
        }
        cerr << "404\n";
    }

    using cd = complex<double>;
    double pi = acos(-1);

    void fft(vector<cd> &a, bool invert) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            j ^= ((1 << __lg(n - 1 ^ j)) - 1) ^ (n - 1);
            if(i < j)swap(a[i], a[j]);
        }
        double ang = pi * (invert ? -1 : 1);
        for (int len = 1; len < n; len <<= 1, ang /= 2) {
            cd w1(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len * 2) {
                cd w(1), u, v;
                for(int j = 0; j < len; j++) {
                    u = a[i + j], v = a[i + j + len] * w, w *= w1;
                    a[i + j] = u + v, a[i + j + len] = u - v;
                }
            }
        }
        if(invert) for(cd &x : a) x /= n;
    }
    vector<int64_t> mul(vector<int> const &a, vector<int> const &b) {
        int N = 1;
        while (N < a.size() + b.size() - 1) N <<= 1;
        vector<cd> t(N);
        for(int i = 0; i < a.size(); i++) t[i].real(a[i]);
        for(int i = 0; i < b.size(); i++) t[i].imag(b[i]);
        fft(t, false);
        for(auto &x : t) x *= x;
        fft(t, true);
        vector<int64_t> ans(N);
        for(int i = 0; i < N; i++) ans[i] = (int64_t)round(t[i].imag() / 2.0);
        return ans;
    }

    vector<int> string_matching(string &s, string &t) {
        if (t.size() > s.size()) return {};
        int n = s.size(), m = t.size();
        vector<int> s1(n), s2(n), s3(n);
        for(int i = 0; i < n; i++) {
            s1[i] = s[i] == '?' ? 0 : s[i] - 'a' + 1; // assign any non-zero number for non '?'s
            s2[i] = s1[i] * s1[i];
            s3[i] = s1[i] * s2[i];
        }
        vector<int> t1(m), t2(m), t3(m);
        for(int i = 0; i < m; i++) {
            t1[i] = t[m - i - 1] == '?' ? 0 : t[m - i - 1] - 'a' + 1;
            t2[i] = t1[i] * t1[i];
            t3[i] = t1[i] * t2[i];
        }
        auto s1t3 = mul(s1, t3);
        auto s2t2 = mul(s2, t2);
        auto s3t1 = mul(s3, t1);
        vector<int> oc;
        for(int i = m - 1; i < n; i++) if(s1t3[i] - s2t2[i] * 2 + s3t1[i] == 0) oc.push_back(i - m + 1);
        return oc;
    }

    void ntt(vector<int> &a, bool invert) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            j ^= ((1 << __lg(n - 1 ^ j)) - 1) ^ (n - 1);
            if(i < j)swap(a[i], a[j]);
        }
        for(int len = 2, l2 = 1, w1, w, u, v; len <= n; len <<= 1, l2 <<= 1) {
            w1 = fp(invert? invRoot: root, (mod - 1) / len);
            for(int i = 0; i < n; i += len) {
                w = 1;
                for(int j = 0; j < l2; j++) {
                    u = a[i + j], v = mul(a[i + j + l2], w), w = mul(w, w1);
                    a[i + j] = add(u, v), a[i + j + l2] = sub(u, v);
                }
            }
        }
        if (invert) {
            int n_1 = fp(n, mod - 2);
            for(int & x : a) x = mul(x, n_1);
        }
    }
    vector<int> mulMod(vector<int> a, vector<int> b) {
        int N = 1;
        while (N < a.size() + b.size() - 1) N <<= 1;
        a.resize(N);
        b.resize(N);
        ntt(a, false), ntt(b, false);
        for(int i = 0; i < N; i++)
            a[i] = int(a[i] * 1LL * b[i] % mod);
        ntt(a, true);
        return a;
    }

    void fwht_and(vector<ll>& a, bool invert) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    a[i + j] = (a[i + j] + (invert? -1: 1) * a[i + j + len] + mod) % mod;
                }
            }
        }
    }
    void fwht_or(vector<ll>& a, bool invert) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    a[i + j + len] = (a[i + j + len] + (invert? -1: 1) * a[i + j] + mod) % mod;
                }
            }
        }
    }
    void fwht_xor(vector<ll>& a, bool invert) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    ll u = a[i + j], v = a[i + j + len];
                    a[i + j] = (u + v) % mod;
                    a[i + j + len] = (u - v + mod) % mod;
                }
            }
        }
        if (invert) {
            ll inv2 = (mod + 1) / 2;
            ll inv_n = 1;
            for(int i = 1; i < n; i <<= 1)
                inv_n = inv_n * inv2 % mod;
            for (ll &x : a) x = x * inv_n % mod;
        }
    }
    template<typename F>
    vector<ll> convolution(vector<ll> a, vector<ll> b, F const &fun) {
        int n = 1;
        while (n < max(a.size(), b.size())) n <<= 1;
        a.resize(n), b.resize(n);
        fun(a, false);
        fun(b, false);
        for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
        fun(a, true);
        return a;
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





#endif //MATH_H

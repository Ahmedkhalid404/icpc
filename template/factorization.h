//
// Created by ahmed on 7/29/2025.
//

#ifndef FACTORIZATION_H
#define FACTORIZATION_H

#include <bits/stdc++.h>
using namespace std;



class factorization {
public:
    static const int MAX = 1e6 + 5; // max sieve limit
    vector<int> primes;
    vector<bool> is_prime_small;
    vector<int> spf; // smallest prime factor for small numbers
    mt19937_64 rng{static_cast<mersenne_twister_engine<unsigned long long, 64, 312, 156, 31, 13043109905998158313, 29, 6148914691236517205, 17, 8202884508482404352, 37, 18444473444759240704, 43, 6364136223846793005>::result_type>(chrono::steady_clock::now().time_since_epoch().count())};


    factorization() {
        is_prime_small.assign(MAX, true);
        spf.resize(MAX);
        sieve();
    }

    // ----------------------------
    // Sieve of Eratosthenes
    // Inputs: n <= 1e6
    // Output: fills is_prime_small[], primes[], and spf[]
    // Time: O(n log log n)
    // ----------------------------
    void sieve() {
        is_prime_small[0] = is_prime_small[1] = false;
        for (int i = 2; i < MAX; ++i) {
            if (is_prime_small[i]) {
                primes.push_back(i);
                spf[i] = i;
                for (int j = i * 2; j < MAX; j += i) {
                    is_prime_small[j] = false;
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    // ----------------------------
    // Get all divisors of n
    // Input: n <= 1e12
    // Output: vector of all divisors sorted
    // Time: O(sqrt(n))
    // ----------------------------
    vector<int64_t> get_divisors(int64_t n) {
        vector<int64_t> res;
        for (int64_t i = 1; i * i <= n; ++i)
            if (n % i == 0) {
                res.push_back(i);
                if (i != n / i) res.push_back(n / i);
            }
        sort(res.begin(), res.end());
        return res;
    }

    // ----------------------------
    // Prime factorization for small n using SPF
    // Input: n < MAX (≈ 1e6)
    // Output: vector of (prime, count)
    // Time: O(log n)
    // ----------------------------
    vector<pair<int, int>> factorize_small(int n) {
        vector<pair<int, int>> res;
        while (n > 1) {
            int p = spf[n], cnt = 0;
            while (n % p == 0) n /= p, cnt++;
            res.emplace_back(p, cnt);
        }
        return res;
    }

    // ----------------------------
    // Prime factorization for large n using trial division
    // Input: n <= 1e12 (recommended)
    // Output: vector of (prime, count)
    // Time: O(sqrt(n)/ln(n))
    // ----------------------------
    vector<pair<int64_t, int>> factorize_trial(int64_t n) {
        vector<pair<int64_t, int>> res;
        for (int p : primes) {
            if (1LL * p * p > n) break;
            if (n % p == 0) {
                int cnt = 0;
                while (n % p == 0) n /= p, cnt++;
                res.emplace_back(p, cnt);
            }
        }
        if (n > 1) res.emplace_back(n, 1);
        return res;
    }

    // ----------------------------
    // Modular multiplication without overflow
    // Input: a, b < 1e18, m <= 1e18
    // Output: (a * b) % m
    // Time: O(1)
    // ----------------------------
    int64_t modmul(int64_t a, int64_t b, int64_t m) {
        return (__int128)a * b % m;
    }

    // ----------------------------
    // Modular exponentiation
    // Input: a < m, b >= 0
    // Output: (a^b) % m
    // Time: O(log b)
    // ----------------------------
    int64_t modpow(int64_t a, int64_t b, int64_t m) {
        int64_t res = 1;
        a %= m;
        while (b) {
            if (b & 1) res = modmul(res, a, m);
            a = modmul(a, a, m);
            b >>= 1;
        }
        return res;
    }

    // ----------------------------
    // Miller-Rabin primality test (deterministic for n < 1e18)
    // Input: n <= 1e18
    // Output: true if n is prime
    // Time: O(log n)
    // ----------------------------
    bool is_prime(int64_t n) {
        if (n < 2) return false;
        for (int64_t p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31})
            if (n % p == 0) return n == p;
        int64_t d = n - 1, r = 0;
        while ((d & 1) == 0) d >>= 1, r++;
        for (int64_t a : {2, 3, 5, 7, 11}) {
            int64_t x = modpow(a, d, n);
            if (x == 1 || x == n - 1) continue;
            bool ok = false;
            for (int i = 1; i < r; ++i) {
                x = modmul(x, x, n);
                if (x == n - 1) {
                    ok = true;
                    break;
                }
            }
            if (!ok) return false;
        }
        return true;
    }

    // ----------------------------
    // Pollard's Rho algorithm to find non-trivial factor
    // Input: n is composite, n <= 1e18
    // Output: non-trivial divisor of n
    // Time: ~O(n^1/4)
    // ----------------------------
    int64_t pollard(int64_t n) {
        if (n % 2 == 0) return 2;
        int64_t c = rng() % n;
        auto f = [&](int64_t x) { return (modmul(x, x, n) + c) % n; };
        int64_t x = rng() % n, y = x, d = 1;
        while (d == 1) {
            x = f(x), y = f(f(y));
            d = gcd(abs(x - y), n);
        }
        return d;
    }

    // ----------------------------
    // Full prime factorization using Pollard's Rho
    // Input: n <= 1e18
    // Output: fills vector with all prime factors (may include duplicates)
    // Time: O(n^1/4) practically
    // ----------------------------
    void factorize_pollard(int64_t n, vector<int64_t>& f) {
        if (n == 1) return;
        if (is_prime(n)) {
            f.push_back(n);
            return;
        }
        int64_t d = pollard(n);
        factorize_pollard(d, f);
        factorize_pollard(n / d, f);
    }

    // ----------------------------
    // Wrapper: prime factorization (prime → count map)
    // Input: n <= 1e18
    // Output: map of prime → count
    // Time: ~O(n^1/4)
    // ----------------------------
    map<int64_t, int> get_prime_factors(int64_t n) {
        vector<int64_t> f;
        factorize_pollard(n, f);
        map<int64_t, int> freq;
        for (auto p : f) freq[p]++;
        return freq;
    }
};



#endif //FACTORIZATION_H

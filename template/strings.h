//
// Created by ahmed on 7/30/2025.
//

#ifndef STRINGS_H
#define STRINGS_H


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// TODO : don't forget to invoke pre !!!!!!!!!!
/// TODO : get wrong ? try to change b1 and b2 (with value greater than 26)


int b1 = 31, b2 = 69, MODHASH = 1e9 + 7;
vector<int> Pb1, Pb2;



void pre(int maxSize) {
    Pb1 = Pb2 = vector<int>(++maxSize);
    Pb1[0] = 1, Pb2[0] = 1;
    for (int i = 1; i < maxSize; i++) {
        Pb1[i] = int(1LL * Pb1[i - 1] * b1 % MODHASH);
        Pb2[i] = int(1LL * Pb2[i - 1] * b2 % MODHASH);
    }
}

struct Hash {
    pair<int, int> code;
    int size;
    explicit Hash(pair<int, int> x = {}, int sz = {}) : code(std::move(x)), size(sz) { }

    Hash(int x) : code({x % MODHASH, x % MODHASH}), size(1) { }

    Hash(const string &x) : code(), size(0) {
        for(char c : x) *this = *(this) + c;
    }

    friend Hash operator+(const Hash &f, const Hash &o) {
        return Hash({int((1LL * f.code.first * Pb1[o.size] + o.code.first) % MODHASH),
                     int((1LL * f.code.second * Pb2[o.size] + o.code.second) % MODHASH)}
                , f.size + o.size);
    }

    bool operator==(const Hash &o) const {
        return size == o.size && code == o.code;
    }
    bool operator<(const Hash &o) const {
        return std::tie(code, size) < std::tie(o.code, o.size);
    }
};

struct HashRange {
    vector<Hash> p, s;
    HashRange(const string &t) : p(t.size()), s(t.size()) {
        p.front() = t.front();
        for(int i = 1; i < t.size(); i++) {
            p[i] = p[i - 1] + t[i];
        }
        s.back() = t.back();
        for(int i = int(t.size()) - 2; i >= 0; i--) {
            s[i] = s[i + 1] + t[i];
        }
    }
    Hash get(int l, int r) { // zero-based
        if(l > r) return Hash();
        if(!l) return p[r];
        return Hash({(p[r].code.first - p[l - 1].code.first * 1LL * Pb1[r - l + 1] % MODHASH + MODHASH) % MODHASH,
                     (p[r].code.second - p[l - 1].code.second * 1LL * Pb2[r - l + 1] % MODHASH + MODHASH) % MODHASH}
                , r - l + 1);
    }
    Hash inv(int l, int r) { // zero-based
        if(l > r) return Hash();
        if(r + 1 == s.size()) return s[l];
        return Hash({(s[l].code.first - s[r + 1].code.first * 1LL * Pb1[r - l + 1] % MODHASH + MODHASH) % MODHASH,
                     (s[l].code.second - s[r + 1].code.second * 1LL * Pb2[r - l + 1] % MODHASH + MODHASH) % MODHASH}
                , r - l + 1);
    }
};


namespace std {
    template <>
    struct hash<Hash> {
        size_t operator()(const Hash& h) const {
            return (1LL * h.code.first * 31 + h.code.second) ^ h.size;
        }
    };
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct Hash : public array<ll, 3> {
    Hash(char c) : array({c, c, 1}) {}
    Hash() { }
    friend Hash operator+(Hash a, Hash b) {
        a[0] = (a[0] * p1[b[2]] + b[0]) % mod;
        a[1] = (a[1] * p2[b[2]] + b[1]) % mod;
        a[2] += b[2];
        return a;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct Hash : public array<int64_t, 3> {
    Hash(char c) : array({c, c, 1}) {}
    Hash() { }
    friend Hash operator+(Hash a, Hash b) {
        a[0] = (a[0] * p1[b[2]] + b[0]) % mod;
        a[1] = (a[1] * p2[b[2]] + b[1]) % mod;
        a[2] += b[2];
        return a;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct SetHasher {
    static constexpr int64_t MOD1 = 1e9 + 7;
    static constexpr int64_t MOD2 = 1e9 + 9;
    static constexpr int64_t BASE1 = 31;
    static constexpr int64_t BASE2 = 53;

    int64_t h1 = 0, h2 = 0;

    void add(const int x) {
        h1 = (h1 + power(x, BASE1, MOD1)) % MOD1;
        h2 = (h2 + power(x, BASE2, MOD2)) % MOD2;
    }

    [[nodiscard]] pair<int64_t, int64_t> get() const {
        return {h1, h2};
    }

    bool operator==(const SetHasher& other) const {
        return h1 == other.h1 && h2 == other.h2;
    }

private:
    [[nodiscard]] static int64_t power(int x, int64_t base, int64_t mod) {
        int64_t res = 1;
        while (x) {
            if (x & 1) res = res * base % mod;
            base = base * base % mod;
            x >>= 1;
        }
        return res;
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// hash vector
template <typename T>
class VectorHasher {
private:
    static constexpr size_t PRIME = 5381;

    static size_t addToHash(size_t currentHash, const T& value) {
        size_t seed = currentHash;
        seed ^= static_cast<size_t>(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

public:
    pair<size_t, int64_t> operator()(const std::vector<T>& vec) const {
        size_t hashValue = PRIME;
        int64_t sum = 0;
        for (const T& num : vec) {
            hashValue = addToHash(hashValue, num);
            sum = sum + num;
        }
        return {hashValue, sum};
    }
};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
vector<int> z_algo(string s) {
    vector<int> z(s.size());
    for(int i = 1, l = 0, r = 0; i < s.size(); i++) {
        if(i < r) z[i] = min(r - i, z[i - l]);
        while(i + z[i] < s.size() && s[z[i]] == s[z[i] + i]) z[i]++;
        if(i + z[i] > r) r = i + z[i], l = i;
    }
    return z;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
auto manacher(const string &t) {
    string s = "%#";
    s.reserve(t.size() * 2 + 3);
    for(char c : t) s += c + "#"s;
    s += '$';
    // t = aabaacaabaa -> s = %#a#a#b#a#a#c#a#a#b#a#a#$

    vector<int> res(s.size());
    for(int i = 1, l = 1, r = 1; i < s.size(); i++) {
        res[i] = max(0, min(r - i, res[l + r - i]));
        while(s[i + res[i]] == s[i - res[i]]) res[i]++;
        if(i + res[i] > r) {
            l = i - res[i];
            r = i + res[i];
        }
    }
    for(auto &i : res) i--;
    return vector(res.begin() + 2, res.end() - 2); // a#a#b#a#a#c#a#a#b#a#a
    // get max odd len = res[2 * i]; aba -> i = b
    // get max even len = res[2 * i + 1]; abba -> i = first b
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Trie{
private:
    struct node{
        node *nxt[ int( 'z' - 'a' + 1 ) ]{};
        int cnt , end;

        explicit node(){
            memset( nxt , 0 , sizeof nxt);
            cnt = end = 0;
        }
    }*root;

    static void insert(node *trie ,string &s){
        for(auto &ch : s){
            int idx = int(ch - 'a');
            if( trie->nxt[ idx ] == nullptr ){
                trie->nxt[ idx ] = new node;
            }
            trie = trie->nxt[ idx ];
            trie->cnt++;
        }
        trie->end++;
    }

    static bool isFound(node *trie,string &s){
        for(auto &ch : s){
            int idx = int(ch - 'a');
            if( trie->nxt[ idx ] == nullptr ){
                return false;
            }
            trie = trie->nxt[ idx ];
        }
        return ( trie->end > 0 );
    }
    static int getPrefix(node *trie,string &s){
        for(auto &ch : s){
            int idx = int(ch - 'a');
            if( trie->nxt[ idx ] == nullptr ){
                return 0;
            }
            trie = trie->nxt[ idx ];
        }
        return trie->cnt;
    }

public:
    explicit Trie(){
        root = new node;
    }
    void insert(string &s){
        insert( root , s );
    }
    bool isFound(string &s){
        return isFound(root , s);
    }
    int getPrefix(string &s){
        return getPrefix(root,s);
    }

};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#endif //STRINGS_H

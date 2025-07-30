//
// Created by ahmed on 7/30/2025.
//

#ifndef DS_H
#define DS_H

#include <bits/stdc++.h>
using namespace std;


template<typename T>
struct sparse{
    int Log, n;
    vector<vector<T>> table;
    function<T(T, T)> merge;
    template<class U>
    explicit sparse(vector<T> arr, U merge) : merge(merge), n((int)arr.size()), Log(__lg(arr.size()) + 1), table(Log, vector<T>(n)) {
        table[0] = arr;
        for(int l = 1; l < Log; l++) {
            for(int i = 0; i + (1 << (l - 1)) < n; i++) {
                table[l][i] = merge(table[l - 1][i], table[l - 1][i + (1 << (l - 1))]);
            }
        }
    }
    T query(int l, int r) {
        if(l > r) return {};
        int len = __lg(r - l + 1);
        return merge(table[len][l], table[len][r - (1 << len) + 1]);
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


template<class T>
struct BIT { // 0-based
    int n;
    vector<T> tree;
    explicit BIT(int size) : n(size + 5), tree(n + 1) { }

    void add(int i, T val) {
        for (i++; i <= n; i += i & -i)
            tree[i] += val;
    }

    T query(int i) {
        T sum = 0;
        for (i++; i > 0; i -= i & -i)
            sum += tree[i];
        return sum;
    }

    T range(int l, int r) {
        if(l > r) return T();
        return query(r) - query(l - 1);
    }

    int lower_bound(T target) {
        if(target <= 0) return 0;
        int pos = 0;
        T sum = 0;
        for (int i = 1 << __lg(n); i > 0; i >>= 1) {
            if(pos + i <= n && sum + tree[pos + i] < target) {
                sum += tree[pos + i];
                pos += i;
            }
        }
        return pos;
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


template<typename T>
class BITRange { // 0-based
    int n;
    vector<T> B1, B2;

    void add(vector<T>& bit, int i, T x) {
        for (++i; i <= n; i += i & -i)
            bit[i] += x;
    }

    T query(vector<T>& bit, int i) {
        T res = 0;
        for (++i; i > 0; i -= i & -i)
            res += bit[i];
        return res;
    }

public:
    explicit BITRange(int size) : n(size + 5), B1(n + 2), B2(n + 2) {}

    void add(int l, int r, T x) {
        add(B1, l, x);
        add(B1, r + 1, -x);
        add(B2, l, x * (l - 1));
        add(B2, r + 1, -x * r);
    }
    void add(int i, T x) { add(B2, i, -x); }

    T query(int i) {
        return query(B1, i) * i - query(B2, i);
    }

    T range(int l, int r) {
        if (l > r) return T();
        return query(r) - query(l - 1);
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class sqrtDecomposition {
public:
    size_t n, z;
    vector<vector<int>> arr;

    explicit sqrtDecomposition(const vector<int> &a) : n(a.size()), z(int(sqrt(a.size())) + 1), arr(z) {
        for(int i = 0; i < n; i++) arr[i / z].push_back(a[i]);
    }

    void rebuild() {
        int tot{};
        vector<int> a(n);
        for(auto &B : arr) {
            for(auto i : B) a[tot++] = i;
            B.clear();
        }
        z = int(sqrt(a.size()) + 1);
        arr.resize(z);
        for(tot = 0; tot < n; tot++) arr[tot / z].push_back(a[tot]);
    }

    void insert(int j, int x) {
        for(int i = 0; i < z; i++) {
            if(j <= arr[i].size()) {
                arr[i].insert(arr[i].begin() + j, x), n++;
                if(arr[i].size() > z * 10) rebuild();
                return;
            }
            j -= int(arr[i].size());
        }
    }

    int erase(int j) {
        for(int i = 0; i < z; i++) {
            if(j < arr[i].size()) {
                int x = arr[i][j];
                for(j++; j < arr[i].size(); j++)
                    arr[i][j - 1] = arr[i][j];
                arr[i].pop_back();
                return n--, x;
            }
            j -= int(arr[i].size());
        }
        return -1;
    }

    int element(int j) {
        for(int i = 0; i < z; j -= int(arr[i++].size()))
            if(j < arr[i].size()) return arr[i][j];
        return -1;
    }

    int count(int l, int r) {
        int j = 0;
        int res = 0;
        for(int b = 0; b < z; b++) {
            if(j > r) break;
            if(j >= l) {
                if(j + arr[b].size() - 1 <= r) { // take all block

                    if (arr[b][i] % 2 == 0) res++; // replace with your condition

                    j += int(arr[b].size());
                    continue;
                }
                for(int i = 0; i < arr[b].size() && j <= r; j++, i++ ) if (arr[b][i] % 2 == 0) res++; // replace with your condition
            }
            else if(j + arr[b].size() - 1 >= l) {
                int i = l - j;
                for(j = l; i < arr[b].size() && j <= r; j++, i++) if (arr[b][i] % 2 == 0) res++; // replace with your condition
            }
            else j += int(arr[b].size());
        }
        return res; // return something
    }
};

/*

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> initial = {1, 2, 3, 4, 5};
    sqrtDecomposition sd(initial);

    cout << "Element at index 2: " << sd.element(2) << "\n";  // 3

    sd.insert(2, 100);
    cout << "After inserting 100 at index 2:\n";
    for (int i = 0; i < 6; ++i)
        cout << sd.element(i) << " ";
    cout << "\n";  // 1 2 100 3 4 5

    sd.erase(4);
    cout << "After erasing index 4:\n";
    for (int i = 0; i < 5; ++i)
        cout << sd.element(i) << " ";
    cout << "\n";  // 1 2 100 3 5

    cout << "Manual count of elements > 50 in [1,4]: ";
    int cnt = 0;
    for (int i = 1; i <= 4; ++i)
        cnt += sd.element(i) > 50;
    cout << cnt << "\n"; // هيحسب فقط >= 100

    return 0;
}

 */

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void moAlgo() {
    int n, q;
    cin >> n >> q;
    int z = int(sqrt(n)) + 1;

    vector<int> arr(n);
    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> ans(q);
    vector<array<int, 2>> qu(q);
    vector<vector<int>> _mo(z);
    for(int i = 0; i < q; i++) {
        cin >> qu[i][0] >> qu[i][1];
        _mo[qu[i][0] / z].push_back(i);
    }

    int curr = 0;
    vector<int> freq(100001), f_f(100001);
    auto add = [&](int i) {
        f_f[++freq[arr[i]]]++;
        curr = max(curr, freq[arr[i]]);
    };
    auto del = [&](int i) {
        if(!--f_f[freq[arr[i]]] && curr == freq[arr[i]])
            curr--;
        --freq[arr[i]];
    };

    int c = 0, l = 0, r = -1;
    for(auto &mo : _mo) {
        if(mo.empty()) continue;
        c? std::sort(mo.begin(), mo.end(), [&](int i, int j){return qu[i][1] < qu[j][1];}):
        std::sort(mo.begin(), mo.end(), [&](int i, int j){return qu[i][1] > qu[j][1];});
        c ^= 1;

        for(int i : mo) {
            while(r < qu[i][1]) add(++r);
            while(l > qu[i][0]) add(--l);
            while(r > qu[i][1]) del(r--);
            while(l < qu[i][0]) del(l++);
            ans[i] = curr;
        }
    }
    for(int i : ans)
        cout << i << '\n';
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




#endif //DS_H

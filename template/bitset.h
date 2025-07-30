//
// Created by ahmed on 7/30/2025.
//

#ifndef BITSET_H
#define BITSET_H

#include <bits/stdc++>
using namespace std;

void fun() {
    bitset<50> b;

    b.set(5);
    b.set(10);

    cout << b._Find_first() << ' ' << b._Find_next(5);
}

#endif //BITSET_H

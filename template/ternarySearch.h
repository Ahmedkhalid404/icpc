//
// Created by ahmed on 7/30/2025.
//

#ifndef TERNARYSEARCH_H
#define TERNARYSEARCH_H

#include<bits/stdc++.h>
using namespace std;

void fun() {
    int st = 1;
    int ed = 1e9;
    int md1, md2, ans = -1;
    // ternary search
    while( st <= ed ){
        md1 = st + (ed - st) / 3;
        md2 = ed - (ed - st) / 3;

        ll fn1 = isValid( md1 );
        ll fn2 = isValid( md2 );

        if( fn1 > fn2 ){
            ed = md2 - 1;
        }
        else
            st = md1 + 1;
    }
    cout << md1 << ' ';
}


#endif //TERNARYSEARCH_H

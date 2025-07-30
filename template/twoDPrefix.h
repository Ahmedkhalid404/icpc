//
// Created by ahmed on 7/30/2025.
//

#ifndef TWODPREFIX_H
#define TWODPREFIX_H

#include <bits/stdc++>
using namespace std;

void prefix2d(){ // youtube.com/watch?v=CcBg1yWXTXY&t=4351s&ab_channel=ICPCAssiutCommunity
    int n , m , q; cin >> n >> m >> q;
    vector< vector< int > > grid( n + 1 , vector< int >( m + 1 ) ) , prefix( n + 1 , vector< int >( m + 1 ) );
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m ; ++j) {
            char ch;cin >> ch;
            grid[i][j] = ch == '*';
        }
    }

    // get prefix row
    for (int i = 1; i <= n ; ++i) {

        for (int j = 1; j <= m ; ++j) {

            prefix[ i ][ j ] = prefix[ i ][ j - 1 ] + grid[ i ][ j ];

        }

    }

    // get column prefix from row prefix
    for (int i = 1; i <= m; ++i) {

        for (int j = 1; j <= n; ++j) {

            prefix[ j ][ i ] = prefix[ j - 1 ][ i ] + prefix[ j ][ i ];

        }

    }

    auto getQuery = [&](int x1,int y1,int x2,int y2){
        //               A                    B                            C                        D
        int sum = prefix[ x2 ][ y2 ] - prefix[ x2 ][ y1 - 1 ] - prefix[ x1 - 1 ][ y2 ] + prefix[ x1 - 1 ][ y1 - 1 ];
        return sum;
    };

    while( q-- ){

        int x1,y1,x2,y2; cin >> x1 >> y1 >> x2 >> y2;

        cout<< getQuery( x1 , y1 , x2 , y2 );

    }
}

#endif //TWODPREFIX_H

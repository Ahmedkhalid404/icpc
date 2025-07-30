//
// Created by ahmed on 7/30/2025.
//

#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include <bits/stdc++>
using namespace std;

int dx[] = { 1 , -1 , 0 , 0 },
        dy[] = { 0 , 0 , 1 , -1 };

int dx_8[] = { 1, -1,  0, 0,  1,  1, -1, -1 };
int dy_8[] = { 0,  0,  1, -1, 1, -1,  1, -1 };

bool isValid(int i,int j,int k){
    int new_i = i + dy[ k ];
    int new_j = j + dx[ k ];

    return (
            ( ( new_i < n ) && ( new_i > -1) ) &&
            ( ( new_j < m ) && ( new_j > -1 ) ) &&
            ( !vis[ new_i ][ new_j ] ) &&
            ( grid[ new_i ][ new_j ] == '#' )
    );
}

#endif //DIRECTIONS_H

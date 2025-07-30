//
// Created by ahmed on 7/29/2025.
//

#ifndef ORDEREDSET_H
#define ORDEREDSET_H

#include <bits/stdc++.h>
using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>


using namespace __gnu_pbds;


template<typename K, typename V, typename Comp = std::less<K>>
using ordered_map = tree<K, V, Comp, rb_tree_tag, tree_order_statistics_node_update>;
template<typename K, typename Comp = std::less<K>>
using ordered_set = ordered_map<K, null_type, Comp>;

template<typename K, typename V, typename Comp = std::less_equal<K>>
using ordered_multimap = tree<K, V, Comp, rb_tree_tag, tree_order_statistics_node_update>;
template<typename K, typename Comp = std::less_equal<K>>
using ordered_multiset = ordered_multimap<K, null_type, Comp>;

//order_of_key(key): Returns the number of elements in the set strictly less than key.
//find_by_order(k): Returns an iterator to the k-th element in the set (0-based index).

//-----------------------------------------------------------------------------------------------------------------------------------------------------------


// ex -> indexed_ds<ordered_set<int>, int> ost;
// ex -> indexed_ds<ordered_set<long long>, long long> ost;

template<typename ds ,typename T>
struct indexed_ds{
    ds os;

    indexed_ds()= default;

    template<class container>
    explicit indexed_ds(container v){
        for(T &x : v)
            os.insert(x);
    }

    void insert(T x){
        os.insert(x);
    }

    void erase_all(T x){
        os.erase(x);
    }

    void erase_one(T x){
        int i = idx(x);
        os.erase(os.find_by_order(i));
    }

    int idx(T x){
        int index = os.order_of_key(x);
        if( ( index == os.size() ) || ( (*os.find_by_order( index )) != x ) )
            return -1;
        return index;
    }

    int countRange(int a,int b){
        if( a > b )
            swap( a , b );

        int countB = os.order_of_key(b + 1);
        int countA = os.order_of_key(a);
        return countB - countA;
    }

    auto lower_bound(int num){
        return os.lower_bound( num );
    }

    auto upper_bound(int num){
        return os.upper_bound( num );
    }

    T operator[](int i){
        return *os.find_by_order(i);
    }

    int size(){
        return os.size();
    }
    auto begin(){
        return os.begin();
    }
    auto end(){
        return os.end();
    }
    auto rbegin(){
        return os.rbegin();
    }
    auto rend(){
        return os.rend();
    }

};

#endif //ORDEREDSET_H

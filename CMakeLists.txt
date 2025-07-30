cmake_minimum_required(VERSION 3.30)
project(icpc)

set(CMAKE_CXX_STANDARD 23)

add_executable(icpc main.cpp
        template/factorization.h
        template/orderedSet.h
        template/segmnetTree.h
        template/ds.h
        template/math.h
        template/trees.h
        template/graphs.h
        template/strings.h
        template/randomValue.h
        template/bitset.h
        template/twoDPrefix.h
        template/monoStackQueue.h
        template/directions.h
        template/ternarySearch.h)

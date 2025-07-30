//
// Created by ahmed on 7/30/2025.
//

#ifndef RANDOMVALUE_H
#define RANDOMVALUE_H

#define random(min, max) ([ & ]() { \
static std::random_device rd; \
static std::mt19937 gen(rd()); \
std::uniform_int_distribution<> dis((min), (max)); \
return dis(gen); \
})()

#endif //RANDOMVALUE_H

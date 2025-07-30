//
// Created by ahmed on 7/30/2025.
//

#ifndef MONOSTACKQUEUE_H
#define MONOSTACKQUEUE_H

#include <bits/stdc++>
using namespace std;

template<typename T>
class MonoStack {
private:
    struct node {
        T el, mx, mn;
    };
    stack<node> monoStack;
public:
    void push(const T &x) {
        T mx = monoStack.empty() ? x : max(this->getMax(), x);
        T mn = monoStack.empty() ? x : min(this->getMin(), x);
        monoStack.push({x, mx, mn});
    }
    T pop() {
        T el = monoStack.top();
        monoStack.pop();
        return el;
    }
    size_t size() {
        return monoStack.size();
    }
    bool empty() {
        return monoStack.empty();
    }
    T top() {
        return monoStack.top().el;
    }
    T getMax() {
        return monoStack.top().mx;
    }
    T getMin() {
        return monoStack.top().mn;
    }
};


template<typename T>
class MonoQueue {
private:
    MonoStack<T> in, out;
    void moveInToOut() {
        while ( !in.empty() )
            out.push(in.pop());
    }
public:
    void push(const T &x) {
        in.push(x);
    }
    T pop() {
        if ( out.empty() )
            moveInToOut();
        T el = out.top();
        out.pop();
        return el;
    }
    size_t size() {
        return in.size() + out.size();
    }
    bool empty() {
        return in.empty() and out.empty();
    }
    T top() {
        if (out.empty())
            moveInToOut();

        return out.top();
    }
    T getMax() {
        if ( in.empty() )
            return out.getMax();
        if ( out.empty() )
            return in.getMax();
        return max(in.getMax(), out.getMax());
    }
    T getMin() {
        if ( in.empty() )
            return out.getMin();
        if ( out.empty() )
            return in.getMin();
        return min(in.getMin(), out.getMin());
    }
};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
struct AggQueue {
    // Stores the actual values and their indices
    deque<pair<T, int>> minq, maxq;
    deque<T> window;
    int l = 0, r = 0;
    T currSum = 0, currGcd = 0, currLcm = 0, currAnd = -1, currOr = 0;

    // GCD helper
    T gcd(T a, T b) {
        return b ? gcd(b, a % b) : a;
    }

    // LCM helper
    T lcm(T a, T b) {
        if (a == 0 || b == 0) return 0;
        return a / gcd(a, b) * b;
    }

    // Push element to the window
    // Time Complexity: O(1) amortized
    void push(T val) {
        // min queue
        while (!minq.empty() && minq.back().first >= val)
            minq.pop_back();
        minq.emplace_back(val, r);

        // max queue
        while (!maxq.empty() && maxq.back().first <= val)
            maxq.pop_back();
        maxq.emplace_back(val, r);

        // maintain window
        window.push_back(val);
        currSum += val;
        currGcd = r == l ? val : gcd(currGcd, val);
        currLcm = r == l ? val : lcm(currLcm, val);
        currAnd = r == l ? val : (currAnd & val);
        currOr = r == l ? val : (currOr | val);

        r++;
    }

    // Pop element from the front of the window
    // Time Complexity: O(1)
    void pop() {
        if (window.empty()) return;
        T val = window.front();
        window.pop_front();

        if (!minq.empty() && minq.front().second == l)
            minq.pop_front();
        if (!maxq.empty() && maxq.front().second == l)
            maxq.pop_front();

        currSum -= val;

        // For others, we need to recompute
        currGcd = currLcm = currAnd = currOr = window.empty() ? 0 : window.front();
        for (int i = 1; i < window.size(); ++i) {
            currGcd = gcd(currGcd, window[i]);
            currLcm = lcm(currLcm, window[i]);
            currAnd &= window[i];
            currOr |= window[i];
        }

        l++;
    }

    // Get current min in O(1)
    T getMin() const {
        return minq.front().first;
    }

    // Get current max in O(1)
    T getMax() const {
        return maxq.front().first;
    }

    // Get sum in O(1)
    T getSum() const {
        return currSum;
    }

    // Get GCD in O(1) for small window, O(n) worst case after pop
    T getGcd() const {
        return currGcd;
    }

    // Get LCM in O(1) for small window, O(n) worst case after pop
    T getLcm() const {
        return currLcm;
    }

    // Get AND in O(1) for small window, O(n) worst case after pop
    T getAnd() const {
        return currAnd;
    }

    // Get OR in O(1) for small window, O(n) worst case after pop
    T getOr() const {
        return currOr;
    }

    int size() const {
        return r - l;
    }

    bool empty() const {
        return size() == 0;
    }

    void clear() {
        minq.clear(); maxq.clear(); window.clear();
        l = r = 0;
        currSum = currGcd = currLcm = currOr = 0;
        currAnd = -1;
    }
};


#endif //MONOSTACKQUEUE_H

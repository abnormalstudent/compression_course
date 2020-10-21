#pragma once
#include <unordered_map>
#include <iostream>
#include <bitset>

template<typename A, typename B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B> p) {
    return out << "{" << p.first << ", " << p.second << "}"; 
}
std::ostream& operator<<(std::ostream& out, const std::bitset<33> bs) {
    for (int i = 32; i >= 0; i--) {
        out << bs[i];
        if (i % 4 == 0 && i != 0) {
            out << "'";
        }
    }
    return out;
}
#define debug(x) std::cout << #x << " " << x << std::endl;
#define debug_bin(x) debug(std::bitset<33>(x));

template<typename T, typename D>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, D> m) {
    if (m.empty()) {
        return out << "{}\n";
    } else {
        out << "{";
        for (auto it = m.begin(); it != m.end(); it++) {
            out << *it;
            if (next(it) != m.end()) {
                out << ",\n";
            }
        }
        return out << "}\n";
    }
}

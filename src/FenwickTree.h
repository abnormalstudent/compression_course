#pragma once
#include <vector>
#include <stdint.h>
#include <stdexcept>


// Actual Fenwick Tree article. Implementation described in 1-indexation
// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.14.8917&rep=rep1&type=pdf

class FenwickTree {
public:
    FenwickTree();

    explicit FenwickTree(uint32_t _size);

    // Initializes the tree,
    // so that tree.sum(i) = sum of all frequencies in range [0, i]
    FenwickTree(const std::vector<int64_t>& freq);

    // Returns sum on segment [0, r]
    // Complexity : O(log(size))
    uint64_t sum(int64_t r);

    // Returns tree[index] (for debug purposes)
    uint64_t get(size_t index) const;

    // Updates the tree
    // Complexity : O(log(size)), where "size" is the number of nodes
    // In this implementation "size" is a constant, and "size" = 257 (from 0 to 256 inclusive)
    void add(int32_t idx, int64_t delta);

    uint32_t getSize() const;
private: 
    std::vector<uint64_t> tree;
    uint32_t size;
};
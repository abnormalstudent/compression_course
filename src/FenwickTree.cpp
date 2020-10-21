#include "FenwickTree.h"

FenwickTree::FenwickTree() {}

FenwickTree::FenwickTree(uint32_t _size) : size(_size) {
        tree.assign(size, 0);
}

// We can use initialization in O(n) instead of O(n * log(n)),
// still has to be tested
// FenwickTree::FenwickTree(const std::vector<uint64_t>& freq) : FenwickTree(freq.size()) {
//     for (size_t i = 0; i < freq.size(); i++)
//         add(i, freq[i]);
// }

// This should work in O(n)
FenwickTree::FenwickTree(const std::vector<int64_t>& freq) : FenwickTree(freq.size()) {
    for (size_t i = 1; i <= freq.size(); i++) {
        tree[i - 1] += freq[i - 1];
        if (i + (i & -i) < size + 1) {
            tree[i + (i & -i) - 1] += tree[i - 1];
        }
    }
}

uint64_t FenwickTree::get(size_t index) const {
        return tree.at(index);
}

uint64_t FenwickTree::sum(int64_t r) {
    uint64_t ret = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1)
        ret += tree[r];
    return ret;
}
uint32_t FenwickTree::getSize() const {
    return size;
}

void FenwickTree::add(int32_t idx, int64_t delta) {
    if (idx >= size || idx < 0) 
        throw std::logic_error("Index out of range");
    for (; idx < size; idx = idx | (idx + 1))
        tree[idx] += delta;
}
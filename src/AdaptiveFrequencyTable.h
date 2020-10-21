#pragma once

#include <stdint.h>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>

#include "globals.h"
#include "FenwickTree.h"

// Frequency Table implemented using FenwickTree
class FrequencyTable {
public:
    FrequencyTable(uint32_t size, uint32_t aggr, bool fillOnes);

    FrequencyTable(uint32_t size, uint32_t aggr);
    
    FrequencyTable(const FrequencyTable& freq, uint32_t size, uint32_t aggr);
    // Returns the number of symbols in FT
    uint32_t getSymbolLimit() const;

    // Return the frequency of given element, need for debug
    uint32_t get(uint32_t symbol) const;

    // Basically does what it said.
    void normalizeProbabilities();

    // Depricated~
    bool conditionMet(const FrequencyTable& fast_freqs, bool write_logs) const;

    // Update frequency table according to fast frequencies. 
    void update(const FrequencyTable& relevant_freqs);
    
    // Increments the frequency of the given symbol 
    void increment(uint32_t symbol); 

    void increment(uint32_t symbol, uint64_t time);
    
    void increment(uint32_t symbol, uint64_t time, const FrequencyTable& relevant_freqs); 

    // Returns sum of all frequencies
    uint32_t getTotal() const;

    void set(uint32_t symbol, uint32_t value);
    // Returns the cumulative frequency in range [0, symbol)
    uint32_t getLow(uint32_t symbol) const;
    
    // Returns the cumulative frequency in range [0, symbol]
    uint32_t getHigh(uint32_t symbol) const;

    // Save the table 
    void write_info() const;
    friend std::ostream& operator<<(std::ostream& out, const FrequencyTable& freqs);
private:
    // Since this function is only used when we 
    // completely reconstruct the cumulative frequencies, hence
    // we will init FenwickTree lazily.
    void sub(uint32_t symbol, uint32_t value);
    void add(uint32_t symbol, uint32_t value);
    uint32_t aggressivity;
    uint64_t total;
    uint32_t num_symbols;
    uint32_t previous_update;
    mutable FenwickTree cumulative;

    // New feature
    // Probably this will allow us to detect changes in context;
    // We will need 2 parameters :
    // 1) AMOUNT of some symbol that changes now
    // 2) TIME_THRSESHOLD which will tell us, when this symbol changed last time
    // 3) And we'll hold TIME variable, so we don't care about O(n) updates of this table
    // HOLY JESUS.
    std::vector<uint64_t> last_updated;
    std::vector<int64_t> frequencies;
};

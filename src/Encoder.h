#pragma once

#include "AbstractCoder.h"

class Encoder : private AbstractCoder {
public:
    explicit Encoder(uint32_t num_bits, BitOutputStream& out);
    // Write encoded representation of the current symbol to the output
    void write(const FrequencyTable& freqs, uint32_t symbol);
    
    // Output some EOF symbol that will help Decoder to work
    void finish();
protected:
    // Writes bits to the output and makes some corrections on low/high bounds.
    void shift() override;
    // Counts number of bits that we have to write when we figure out the first bit
    void bits_plus_follow() override;
private:
    BitOutputStream& output;
    uint64_t num_bits_to_follow;
};
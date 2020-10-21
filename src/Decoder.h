#pragma once

#include "AbstractCoder.h"

class Decoder : private AbstractCoder {
public:
    // Fills in the code variable with 31 encoded bits
    explicit Decoder(uint32_t, BitInputStream& in);

    uint32_t read(const FrequencyTable& freq);
protected:
    // We have to update frequency table according 
    // to updates in Encoder, so.

    // Get new bit from the output stream
    void shift() override;

    // Deletes seconds to MSB and after replaces LSB with
    // the next bit from encoded stream
    void bits_plus_follow() override;
private:
    // Encoded sequence
    uint64_t code;
    BitInputStream& input;

    // Returns the next bit from the input stream. 
    // Symbols since EOF treated as zeros.
    int readbit();
};
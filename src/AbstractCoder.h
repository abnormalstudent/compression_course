#pragma once

#include <stdint.h>
#include <algorithm>
#include <stdexcept>
#include <limits>

#include "AdaptiveFrequencyTable.h"
#include "BitIo.h"

// Let's create AbstractCoder class which shares features of 
// both Encoder and Decoder subclasses.
// With OOP we can see, how beautifull out code can look.
class AbstractCoder {
public:
    explicit AbstractCoder(uint32_t bound);
    virtual ~AbstractCoder() = 0;
protected:
    // The number of bits used to perform computations 
    uint32_t precision;

    // The maximum value of high
    uint64_t whole_range;

    uint64_t half;
    uint64_t quater;

    // Mask to control number of active bits in number
    uint64_t state_mask;

    // Bounds to update
    uint64_t low;
    uint64_t high;

    virtual void update(const FrequencyTable& freq, uint32_t symbol);
    virtual void normalize();
    
    virtual void shift() = 0;
    virtual void bits_plus_follow() = 0;
};

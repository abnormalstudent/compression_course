#include "AbstractCoder.h"

AbstractCoder::AbstractCoder(uint32_t num_bits) {
    precision = num_bits;
    
    // 1 00....00 - we use 32-bit representation 
    whole_range = static_cast<uint64_t>(1) << precision;
    
    // 0 100...00
    half = whole_range >> 1;
    
    // 0 010...00
    quater = half >> 1;
    
    // We want to use it to prevent high or low from going outside of desired range
    // of representation (i.e. presicion)
    // 0 1111..11
    state_mask = whole_range - 1;
    
    low = 0;
    high = state_mask;
}

AbstractCoder::~AbstractCoder() {}

void AbstractCoder::update(const FrequencyTable& freq, uint32_t symbol) {
    uint64_t range = high - low + 1;

    uint64_t total = freq.getTotal();
    uint64_t lower_bound = freq.getLow(symbol);
    uint64_t upper_bound = freq.getHigh(symbol);

    uint64_t newLow = low + lower_bound * range * static_cast<double>(1.) / total;
    uint64_t newHigh = low + upper_bound * range * static_cast<double>(1.) / total - 1;
        
    low = newLow;
    high = newHigh;
    
    normalize();
}

void AbstractCoder::normalize() {
    // One can prove, that these two while loops are equivalent to 
    // the given "for (;;)" loop, described on lecture

    // While low and high have the same top bit value, shift them out
	// (high < half) || (low >= half) ~
	while (((low ^ high) & half) == 0) {
		shift();
		low  = ((low  << 1) & state_mask);
		high = ((high << 1) & state_mask) | 1;
	}
	// While low's top two bits are 01 and high's are 10, delete the second highest bit of both.
	// (low >= quarterRange) && (high < halfRange + quarterRange) != false ~
	// to the version below, and it's definitely faster because of bit operations

    // ToDo : maybe I should measure it?
    // Remember that MSB(high) == 1 && MSB(low) == 0 holds
    // after every iteration of this loop
    while ((low & ~high & quater) != 0) {
        bits_plus_follow();
        // Delete second to the MSB and shift everything to the left
        low = (low << 1) ^ half; 
        // Same, but don't forget about trailing one
        high = ((high ^ half) << 1) | half | 1;
	}
}

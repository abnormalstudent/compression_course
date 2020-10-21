#include "Encoder.h"

Encoder::Encoder(uint32_t num_bits, BitOutputStream& output) :
    AbstractCoder(num_bits),
    output(output),
    num_bits_to_follow(static_cast<uint32_t>(0)) {}

void Encoder::write(const FrequencyTable& freq, uint32_t symbol) {
    update(freq, symbol);
}

void Encoder::finish() {
    output.write(1);
}

void Encoder::shift() {
    // Write the MSB of low/high (they are the same) into output 
    int bit = static_cast<int>(low >> (precision - 1));
	output.write(bit);
	
	// Write out saved underflow bits
	for (; num_bits_to_follow > 0; num_bits_to_follow--)   
		output.write(bit ^ 1);
}

void Encoder::bits_plus_follow() {
    num_bits_to_follow++;
}
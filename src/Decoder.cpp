#include "Decoder.h" 

Decoder::Decoder(uint32_t num_bits, BitInputStream &in) :
		AbstractCoder(num_bits),
		input(in),
		code(0) {
	for (int i = 0; i < precision; i++)
		code = code << 1 | readbit();
}


uint32_t Decoder::read(const FrequencyTable &freqs) {
	uint32_t total = freqs.getTotal();
	
    uint64_t range = high - low + 1;

    // Approximated frequency of the next decoded symbol
	// Which lies in range [low, high) * total
	uint64_t freq = ((code - low + 1) * total - 1) / range;

    // Probably we can exatract symbol even more efficiently, but it'll require
    // building Segment Tree on frequencies, and can be 
    // memory expensive on alphabet-fancy dictionaries.
    // ToDo : maybe I should try it out.

	// Perform binary search. Find highest symbol such that freqs.getLow(symbol) <= freq.
    // One can prove that, also, "symbol" is the first element such that
    // freqs.getHigh(symbol) > freq, so it's the one that we output as decoded,
    // It implies, that freqs.getLow(symbol) <= freq < freqs.getHigh(symbol)
    // and means, that this symbol should be output as decoded!

	uint32_t start = 0;
	uint32_t end = freqs.getSymbolLimit();

    // Since we use Fenwick Tree to compute .getLow(),
    // complexity of the following part is O(log^2(n))
	while (end > start + 1) {
		uint32_t middle = (start + end) >> 1;
		if (freqs.getLow(middle) > freq)
			end = middle;
		else
			start = middle;
	} 
    // Symbol found, now perform update stage, which actually 
    // has couple of differences from Encoder::update, - we
    // have to override two methods to maintain the 
    // "code" variable 
	uint32_t symbol = start;
	update(freqs, symbol);
	return symbol;
}
// To get proper decoding, we need to perform exact same
// operations on "code" variable, which we did when we were
// writing to the encoded stream. 

void Decoder::shift() {
    // ~ to code += code; code += readCodeBit();, and don't forget about state_mask
    code = ((code << 1) & state_mask) | readbit();
}


void Decoder::bits_plus_follow() {
    // Deletes second to MSB, shifts everything to the left,
    // (starting from 3rd bit) and replaces LSB with next bit from the output stream
	code = (code & half) | ((code << 1) & (state_mask >> 1)) | readbit();
}


int Decoder::readbit() {
	int temp = input.read();
	if (temp == -1)
		temp = 0;
	return temp;
}
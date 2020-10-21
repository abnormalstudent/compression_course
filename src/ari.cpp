//
// Created by eduard on 30/09/2020.
//
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <stdexcept>

#include "globals.h"
#include "ari.h"
#include "BitIo.h"
#include "Encoder.h"
#include "Decoder.h"

using namespace globals;

void compress_ari(const char *ifile, char *ofile) {
    std::ifstream in(ifile, std::ifstream::binary);
    std::ofstream out(ofile, std::ofstream::binary);
    BitOutputStream output(out);
    
	FrequencyTable freqs(SIZE, AGGRESIVITY);
    FrequencyTable fast_freqs(SIZE, MULTIPLIER * AGGRESIVITY, false);
    
    Encoder encoder(PRECISION, output);

    int64_t reinit_count = 0;
    int64_t normilize_count = 0;
    uint64_t time = 0;

    while (true) {  
        uint32_t symbol = in.get();
        if (symbol == EOF) { 
            break;
        }
        encoder.write(freqs, symbol);
    
        fast_freqs.increment(symbol, time);
        freqs.increment(symbol, time, fast_freqs);
        if (freqs.getTotal() > NORMALIZE_THRESHOLD) {
            freqs.normalizeProbabilities();
        }
        // The key idea of this part is :
        // sometimes we have reinitialize frequency table
        // because previous information may become useless
        // so we reinit the table and keep the most important 
        // information that we recently collected.
        // if (reinit_count == REINIT_THRESHOLD) {
        //     freqs = FrequencyTable(SIZE, AGGRESIVITY);
        //     // freqs.normalizeProbabilities();
        //     freqs.update(fast_freqs);
        //     fast_freqs = FrequencyTable(SIZE, AGGRESIVITY * MULTIPLIER);
        //     reinit_count = 0;
        // }
        reinit_count++;
        normilize_count++;
        time++;
    }
    // Write the least frequent symbol as EOF
    encoder.write(freqs, 256);   
    encoder.finish();
    output.finish();
}

void decompress_ari(const char *ifile, char *ofile) {
    std::ifstream in(ifile, std::ios::binary);
	std::ofstream out(ofile, std::ios::binary);
	BitInputStream input(in);

    FrequencyTable freqs(SIZE, AGGRESIVITY);
    FrequencyTable fast_freqs(SIZE, MULTIPLIER * AGGRESIVITY, false);

	Decoder decoder(PRECISION, input);

    int64_t reinit_count = 0;
    int64_t normilize_count = 0;
    uint64_t time = 0;

	while (true) {
		uint32_t symbol = decoder.read(freqs);
		if (symbol == 256)
			break;
		out.put(symbol);

        // Probably a bad codestyle, but I don't care (lol I do)
        fast_freqs.increment(symbol, time);    
        freqs.increment(symbol, time, fast_freqs);
        if (freqs.getTotal() > NORMALIZE_THRESHOLD) {
            freqs.normalizeProbabilities();
        }
        // if (reinit_count == REINIT_THRESHOLD) {
        //     freqs = FrequencyTable(SIZE, AGGRESIVITY);
        //     //freqs.normalizeProbabilities();
        //     freqs.update(fast_freqs);
        //     fast_freqs = FrequencyTable(SIZE, AGGRESIVITY * MULTIPLIER);
        //     reinit_count = 0;
        // }
        reinit_count++;
        normilize_count++;
        time++;
	} 
} 


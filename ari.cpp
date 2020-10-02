//
// Created by eduard on 30/09/2020.
//
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cassert>

#include "ari.h"
#include "BitIo.h"

#ifdef LOCAL_DEBUG
#include "debug.h"
#endif

namespace globals {
    const uint64_t TOP_BOUND = 0xFFFF'FFFF;
} 

std::pair<std::unordered_map<char, Segment>, uint64_t> get_info(const char* ifile) {
    std::ifstream input;
    input.open(ifile);

    std::unordered_map<char, int> weights;
    char symbol;
    uint64_t file_length = 0;
    while (input.get(symbol)) {
        // Since constructor for char is almost free
        weights[symbol]++;
        file_length++;
    }   

    uint64_t unique_symbols = weights.size();

    std::vector<uint32_t> cumulative_weight;
    std::unordered_map<char, Segment> letter_to_segment;

    // Get cumulative weight
    cumulative_weight.reserve(unique_symbols + 1);
    cumulative_weight.emplace_back(0); // The most left boundary is always equal to zero
    size_t previous_symbol = 0;
    for (const auto& [symbol, weight] : weights) {
        cumulative_weight.emplace_back(cumulative_weight[previous_symbol++] + weight);
    } 

    // Get a mapping from indices to (Segments???)
    // Probably should refactor it after a bit.
    letter_to_segment.reserve(unique_symbols);
    size_t current_symbol = 0;
    for (const auto& [symbol, weight] : weights) {
        letter_to_segment[symbol] = Segment{cumulative_weight[current_symbol],
                                            cumulative_weight[current_symbol + 1]};
        current_symbol++;
    }
    try {
        assert(cumulative_weight[unique_symbols] == file_length);
    } catch(std::exception& e) {
        std::cout << "Difference between variables : \n" << "cumulative_weight[unique_symbols] == " << cumulative_weight[unique_symbols] << '\n' << 
                                                       "file_length == " << file_length << '\n';
    }
    input.close();
    return std::make_pair(letter_to_segment, file_length);
}

void output_bit_and_pending(uint64_t bitwise_mult, int& pending_bits, std::ofstream& out) {
    bool bit = bitwise_mult != 0;
    out << bit;
    while (pending_bits > 0) {
        out << !bit;
        pending_bits--;
    }
}

void compress_ari(const char *ifile, char *ofile) {
    auto cache = get_info(ifile);
    std::unordered_map<char, Segment> letter_to_segment = cache.first;
    uint64_t file_length = cache.second;

    #ifdef LOCAL_DEBUG
        std::cout << "Letter to segment map : " << std::endl << letter_to_segment;
        std::cout << "File length : " << file_length << std::endl;
    #endif
    std::ifstream input(ifile, std::ifstream::binary);
    std::ofstream output(ofile, std::ofstream::binary);

    uint64_t low = 0, high      = globals::TOP_BOUND;
    
    const uint64_t quater       = (high + 1ULL) / 4;
    const uint64_t half         = 2 * quater;
    const uint64_t three_quater = 3 * quater;
    int pending_bits = 0;

    char symbol;
    int iter = 0;
    while (input.get(symbol)) {
        #ifdef LOCAL_DEBUG
            std::cout << "Low : " << std::bitset<64>(low) << std::endl << "High : " << std::bitset<64>(high) << "\n\n";
        #endif
        uint64_t range = high - low;
        Segment current = letter_to_segment[symbol];

        // Still don't quite understand how it is loss-less comprsession, if algorithm isn't determined 
        // e.g., (range + 1) * current.upper % unique_symbols != 0, so we have some rounding errors and stuff.    
        // UPD : get it, these rounds doesn't hurt performans cause we apply same technique to the decompression as well

        // Simple hack to prevent overflow, may cause execution time loss
        low = low + (range + 1) * current.lower / file_length;
        high = low + (range + 1) * current.upper / file_length;
        for(;;) {
            if ((low & half) == (high & half)) {
                output_bit_and_pending(high & half, pending_bits, output);   
                #ifdef LOCAL_DEBUG
                    std::cout << "(low & half) == (high & half)" << "\n";
                    std::cout << "Low : " <<  std::bitset<64>(low) << std::endl << "High : " << std::bitset<64>(high) << "\n";
                    std::cout << "Iter : " << iter << "\n\n";
                #endif 
            } else if ((low & half) && !(high & half)) {
                pending_bits++;
                // Delete second to the most significant bit, so after the shift MSB will be 0
                low &= (half - 1);
                // Make sure that MSB after shift is 1
                high |= half;
                #ifdef LOCAL_DEBUG
                    std::cout << "(low & half) == (high & half)" << std::endl;
                    std::cout << "Low : "  << std::bitset<64>(low) << std::endl << "High : " << std::bitset<64>(high) << "\n";
                    std::cout << "Iter : " << iter << "\n\n";
                #endif
            } else {
                break;
            }
            low <<= 1;
            high <<= 1;
            high |= 1;
            iter++;
        }
    } 
}

void decompress_ari(const char *ifile, char *ofile) {
    FILE *ifp = (FILE *)fopen(ifile, "rb");
    FILE *ofp = (FILE *)fopen(ofile, "wb");

    /** PUT YOUR CODE HERE
      * implement an arithmetic encoding algorithm for decompression
      * don't forget to change header file `ari.h`
    */

    // This is an implementation of simple copying
    size_t n, m;
    unsigned char buff[8192];

    do {
        n = fread(buff, 1, sizeof buff, ifp);
        if (n)
            m = fwrite(buff, 1, n, ofp);
        else
            m = 0;
    } while ((n > 0) && (n == m));

    fclose(ifp);
    fclose(ofp);
}


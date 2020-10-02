//
// Created by eduard on 30/09/2020.
//

#ifndef CPP_COMPRESSOR_ARI_H
#define CPP_COMPRESSOR_ARI_H

#pragma once
#include <unordered_map>

struct Segment {
    uint64_t lower;
    uint64_t upper; 
};
void compress_ari(const char* input_file, char* output_file);
void decompress_ari(const char* input_file, char* output_file);
#endif //CPP_COMPRESSOR_ARI_H

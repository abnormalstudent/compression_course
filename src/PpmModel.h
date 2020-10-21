#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <deque>
#include <fstream>
#include <cmath>

#include "Context.h"
#include "AdaptiveFrequencyTable.h"
#include "globals.h"

class PpmModel {
public:
    explicit PpmModel(uint32_t order, uint32_t symbol_limit, uint32_t escape_symbol); 
   
    void init_worst_table(const char* input_file);

    // int getBestOrder() const;

    void update(std::deque<uint32_t>& context, int symbol, int order);
    Context* getRoot() const;

    std::unique_ptr<Context> context;
    FrequencyTable worst_table;
private:
    uint32_t esc;
    uint32_t total;
    uint32_t sym_lim;
    uint32_t ord;
};
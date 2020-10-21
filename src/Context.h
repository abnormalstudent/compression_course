#pragma once

#include <memory>

#include "globals.h"
#include "AdaptiveFrequencyTable.h"

struct Context {
    explicit Context(uint32_t symbol_limit, bool has_childs, uint32_t depth);

    uint32_t update_rule_C() const;
    uint32_t update_rule_D() const;
    

    uint32_t time;
    uint32_t last_update;
    uint32_t different_symbols;
    uint32_t total_calls;
    uint32_t depth;

    std::vector<std::unique_ptr<Context>> subcontexts;
    FrequencyTable frequencies;
};
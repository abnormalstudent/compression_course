#include "Context.h"

Context::Context(uint32_t symbol_limit, bool has_childs, uint32_t d) :
    frequencies(FrequencyTable(symbol_limit, globals::deep_aggressivity.at(d), 0)),
    total_calls(1),
    different_symbols(0),
    depth(d),
    last_update(0), 
    time(0) {
    if (has_childs) {
        subcontexts.reserve(symbol_limit);
        for (size_t i = 0; i < symbol_limit; i++) {
            subcontexts.push_back(std::unique_ptr<Context>(nullptr));
        }
    }
}

uint32_t Context::update_rule_C() const {
    return frequencies.getTotal() * different_symbols / (different_symbols + total_calls);
}

uint32_t Context::update_rule_D() const {
    return frequencies.getTotal() * (different_symbols) / (total_calls);
}
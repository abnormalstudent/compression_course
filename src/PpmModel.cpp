#include "PpmModel.h"

PpmModel::PpmModel(uint32_t order, uint32_t symbol_limit, uint32_t escape_symbol) :
    worst_table(FrequencyTable(symbol_limit, 0, true)),
    ord(order),
    esc(escape_symbol),
    sym_lim(symbol_limit),
    total(0) {
    context = std::unique_ptr<Context>(nullptr);
    if (order >= 0) {
        context.reset(new Context(sym_lim, ord >= 1, 0));
        context -> frequencies.increment(esc);
    }
}

void PpmModel::init_worst_table(const char* input_file) {
    std::ifstream input(input_file, std::ios_base::binary);
    char symbol;
    FrequencyTable initialized_table(globals::SIZE, 1, true);
    while (true) {
        input.get(symbol);
        if (symbol == EOF) {
            break;
        }
        initialized_table.increment(symbol);
    }
    worst_table = initialized_table;
    input.close();
}

// void PpmModel::getBestOrder() const {
//     double score = globals::SIZE * log(globals::SIZE);

//     // order 0 model
//     Context* context = getRoot();

//     for (int i = 1; i <= order; i++) {
//         context = context -> subcontexts
//     }

// }

Context* PpmModel::getRoot() const {
    return context.get();
}

void PpmModel::update(std::deque<uint32_t>& history, int symbol, int order) {
    // Increment frequency in order-0 table
    Context* p_context = getRoot();
    int i = 0;
    
    if (i >= order)
        p_context -> frequencies.increment(symbol);
    
    if (p_context-> frequencies.getTotal() > globals::deep_normilize_threshold.at(i)) {
        p_context -> frequencies.normalizeProbabilities();
    }

    
    // Update every other table and if it doesn't exist - create it.
    if (history.size() > ord) {
        throw "PpmModel::update, history.size() must be <= ord.";
    }
    for (uint32_t sym : history) {
        std::unique_ptr<Context>& subcontext = p_context -> subcontexts.at(sym); 
        if (subcontext.get() == nullptr) {
            subcontext.reset(new Context(sym_lim, i + 1 < ord, i + 1));
        }
        p_context = subcontext.get();
        bool first_time = false;
        if (p_context -> frequencies.get(symbol) == 0) {
            p_context -> different_symbols++;
            first_time = true;
        }
        // if (p_context -> frequencies.getTotal() > globals::deep_normilize_threshold.at(i + 1) && 
        //     p_context -> time > p_context -> last_update + globals::UPDATE_THRESHOLD &&
        //     !first_time && i < 1) {
        //     p_context -> frequencies.normalizeProbabilities();
        //     p_context -> last_update = p_context -> time;
        // }
        p_context -> time++;
        p_context -> total_calls++;
        
        // Update only if depth of the context GE than order 
        if (i + 1 >= order)
            p_context -> frequencies.increment(symbol);
        p_context -> frequencies.set(esc, p_context -> update_rule_D());
        i++;
    }
}

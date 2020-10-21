#include <cmath>
#include "AdaptiveFrequencyTable.h"


FrequencyTable::FrequencyTable(uint32_t size, uint32_t aggr, bool fillOnes) :
    num_symbols(size),
    aggressivity(aggr),
    last_updated(std::vector<uint64_t>(size, 0)),
    previous_update(0) {
    // For simplicity assume, that each symbol will appear atleast once
    // ToDo: for more fancy tables we won't make such assumption
    frequencies.reserve(size);
    for (size_t i = 0; i < size; i++) {
        frequencies.push_back(fillOnes);
    }
    // Build prefix sum of that array and maintain 
    // the Fenwick Tree data structure
    cumulative = FenwickTree(frequencies);
    total = getHigh(size - 1);

}

// Probably redundant constructor
FrequencyTable::FrequencyTable(uint32_t size, uint32_t aggr) : 
        num_symbols(size),
        aggressivity(aggr),
        last_updated(std::vector<uint64_t>(size, 0)),
        previous_update(0) {
    // For simplicity assume, that each symbol will appear atleast once
    // ToDo: for more fancy tables we won't make such assumption
    frequencies.reserve(size);
    for (size_t i = 0; i < size; i++) {
        frequencies.push_back(1);
    }
    // Build prefix sum of that array and maintain 
    // the Fenwick Tree data structure
    cumulative = FenwickTree(frequencies);
    total = getHigh(size - 1);
}

FrequencyTable::FrequencyTable(const FrequencyTable& freq, uint32_t size, uint32_t aggr) :
    aggressivity(aggr),
    num_symbols(size),
    last_updated(std::vector<uint64_t>(size, 0)),
    previous_update(0) {
    frequencies.reserve(size);
    for (size_t i = 0; i < getSymbolLimit(); i++) {
        frequencies.at(i) = freq.get(i);
    }
    cumulative = FenwickTree(frequencies);
    total = getHigh(size - 1);
}

uint32_t FrequencyTable::getSymbolLimit() const {
    return num_symbols;
}

uint32_t FrequencyTable::get(uint32_t symbol) const {
    if (symbol < 0 || symbol >= getSymbolLimit())
        throw std::logic_error("Frequencies index out of range");
    return frequencies[symbol];
}

void FrequencyTable::update(const FrequencyTable& relevant_freqs) {
    std::vector<std::pair<int, double>> differences;
    differences.reserve(getSymbolLimit());

    for (size_t i = 0; i < getSymbolLimit(); i++) {
        double multiplier = globals::DIFFERENCE_MULTIPLIER;   
        double difference = multiplier * (std::round(relevant_freqs.get(i) - this->get(i)));
        differences.emplace_back(std::make_pair(i, difference));
        add(i, difference);
        cumulative.add(i, difference);
    }
    std::sort(differences.begin(), differences.end(), 
    [=] (const std::pair<int, double>& a, const std::pair<int, double>& b) {
        return a.second > b.second;
    });
    // Let's give even more weight to the some symbols which present in the top-, 
    // say 10 symbols.
    if (globals::HIGHEST_UPDATE_THRESHOLD > getSymbolLimit()) {
        throw std::logic_error("globals::HIGHEST_UPDATE_THRESHOLD > frequecies.size()");
    }
    for (size_t i = 0; i < globals::HIGHEST_UPDATE_THRESHOLD; i++) {
        add(differences[i].first, differences[i].second * globals::UPDATE_MULTIPLIER);
        cumulative.add(differences[i].first, differences[i].second * globals::UPDATE_MULTIPLIER);
    }
    
}

void FrequencyTable::normalizeProbabilities() {
    for (size_t i = 0; i < getSymbolLimit(); i++) {
        if (get(i) > 1) {
            sub(i, (frequencies.at(i) >> 1));
        }
    }
    cumulative = FenwickTree(frequencies);
}

void FrequencyTable::increment(uint32_t symbol) {
    if (symbol < 0 || symbol >= getSymbolLimit()) {
        std::logic_error("Exception throwed in FrequencyTable::increment");
    }
    total += aggressivity;
    frequencies.at(symbol) += aggressivity;
    cumulative.add(symbol, aggressivity);
}

void FrequencyTable::increment(uint32_t symbol, uint64_t time) {
    if (time < last_updated.at(symbol))
        throw std::logic_error("time < last_updated[symbol]");
    if (time > last_updated.at(symbol) + globals::TIME_THRESHOLD && 
        time > previous_update + globals::UPDATE_THRESHOLD) {
        frequencies = std::vector<int64_t>(num_symbols, 0);
        cumulative = FenwickTree(frequencies);
    } 
    last_updated.at(symbol) = time;
    previous_update = time;
    increment(symbol);
}
void FrequencyTable::increment(uint32_t symbol, uint64_t time, const FrequencyTable& relevant_freqs) {
    increment(symbol);
    if (time < last_updated.at(symbol))
        throw std::logic_error("time < last_updated[symbol]");
    if (time > last_updated.at(symbol) + globals::TIME_THRESHOLD && 
        time > previous_update + globals::UPDATE_THRESHOLD) {
        update(relevant_freqs);
    } 
    last_updated.at(symbol) = time;
    previous_update = time;
}

uint32_t FrequencyTable::getTotal() const {
    return total;
}

void FrequencyTable::set(uint32_t symbol, uint32_t value) {
    int64_t old_value = get(symbol);

    total -= old_value;
    total += value;
    frequencies.at(symbol) = value;
    cumulative.add(symbol, value - old_value);
}
// We have to use this tricky hack, because 
// FenwickTree::sum(i) returns sum of segment
// of size [0, i], hence to get sum(0) 
// use elvis operator
uint32_t FrequencyTable::getLow(uint32_t symbol) const {
    return (symbol == 0) ? 0 : cumulative.sum(symbol - 1);
}

uint32_t FrequencyTable::getHigh(uint32_t symbol) const {
    return cumulative.sum(symbol);
}

void FrequencyTable::write_info() const {
    std::ofstream table_stream(globals::TABLE_PATH, std::ios_base::app);
    for (size_t i = 0; i < getSymbolLimit(); i++) {
        table_stream << frequencies.at(i) << " ";
    }   table_stream << std::endl;
}


// We don't have to check if index is correct here
// because we would've thrown the error before. 
void FrequencyTable::add(uint32_t symbol, uint32_t value) {
    frequencies.at(symbol) += value;
    total += value;
}

void FrequencyTable::sub(uint32_t symbol, uint32_t value) {
    frequencies.at(symbol) -= value;
    total -= value;
}

std::ostream& operator<<(std::ostream& out, const FrequencyTable& freqs) {
    for (size_t i = 0; i < freqs.getSymbolLimit(); i++) {
        out << i << " " << freqs.get(i) << std::endl;
    }
    return out;
}
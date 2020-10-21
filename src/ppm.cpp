#include <deque>
#include <stdexcept>

#include "ppm.h"
#include "PpmModel.h"


// Let's store context in reversed order 
// to make coding easier.
void update_context(std::deque<uint32_t>& deq, int symbol) {
    deq.push_front(symbol);
    if (!deq.empty() && 
        deq.size() > globals::ORDER) {
        deq.pop_back();
    }
}

int encodeSymbol(PpmModel& model, std::deque<uint32_t>& hist, int symbol, Encoder& encoder) {
    if (hist.size() > globals::ORDER) 
        throw std::logic_error("hist.size() must be <= globals::ORDER");
    for (int order = hist.size(); order >= 0; order--) {
        Context* context = model.getRoot();
        for (int i = 0; i < order && context != nullptr; i++) {
            context = context -> subcontexts.at(hist.at(i)).get();
        }
        if (context == nullptr) continue;

        if (symbol != 256 && context -> frequencies.get(symbol) > 0) {
            encoder.write(context -> frequencies, symbol);
            return order;
        } else {
            encoder.write(context -> frequencies, 256); 
        }
    } 
    encoder.write(model.worst_table, symbol);
    return -1;
}

std::pair<int, uint32_t> decodeSymbol(PpmModel &model, std::deque<uint32_t>& hist, Decoder& decoder) {
    if (hist.size() > globals::ORDER) 
        throw std::logic_error("hist.size() must be <= globals::ORDER");
	for (int order = hist.size(); order >= 0; order--) {
		Context* context = model.getRoot();
		for (int i = 0; i < order && context != nullptr; i++) {
			context = context -> subcontexts.at(hist.at(i)).get();;
		}
        if (context == nullptr) continue;
		uint32_t symbol = decoder.read(context -> frequencies);
		if (symbol < 256)
			return std::make_pair(order, symbol);
	}
	return std::make_pair(-1, decoder.read(model.worst_table));
}

void compress_ppm(const char *ifile, const char *ofile) {
    PpmModel model(globals::ORDER, 257, 256);
    //model.init_worst_table(ifile);

    std::ifstream in(ifile, std::ios_base::binary);
    std::ofstream out(ofile, std::ios_base::binary);
    BitOutputStream output(out);

    Encoder encoder(globals::PRECISION, output);
    
    std::deque<uint32_t> context;
    while (true) {
        int symbol = in.get();
        if (symbol == EOF) break;
        int order = encodeSymbol(model, context, symbol, encoder);
        model.update(context, symbol, order);
        update_context(context, symbol);
    }
    auto IDontNeedThis = encodeSymbol(model, context, 256, encoder);
    encoder.finish();
    output.finish();
}   

void decompress_ppm(const char *ifile, const char *ofile) {
    PpmModel model(globals::ORDER, 257, 256);
    //model.init_worst_table(ifile);
   
    std::ifstream in(ifile, std::ios_base::binary);
    std::ofstream out(ofile, std::ios_base::binary);

    BitInputStream input(in);

    Decoder decoder(globals::PRECISION, input);

    std::deque<uint32_t> context;
    while (true) {
        auto cache = decodeSymbol(model, context, decoder);
        auto order = cache.first;
        auto symbol = cache.second;

        if (symbol == 256) break;
        out.put(symbol);
        model.update(context, symbol, order);
        update_context(context, symbol);
    }
}
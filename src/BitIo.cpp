#include "BitIo.h"

BitInputStream::BitInputStream(std::istream& in):
        input(in),
        current_byte(0),
        bits_remaining(0) {}


int BitInputStream::BitInputStream::read() {
    if (current_byte == -1) {
        return -1;
    }
    if (bits_remaining == 0) {
        current_byte = input.get();
        if (current_byte == EOF) {
            return -1;
        }
        bits_remaining = 8;
    }
    bits_remaining--;
    return (current_byte >> bits_remaining) & 1;
}

BitOutputStream::BitOutputStream(std::ostream& out):
        output(out),
        current_byte(0),
        bits_filled(0) {}


void BitOutputStream::write(int b) {
    if (b != 0 && b != 1) {
        throw std::runtime_error("Argument isn't a bit");
    }
    current_byte = (current_byte << 1) | b;
    bits_filled++;
    // Whole byte is done
    if (bits_filled == 8) {
        output.put(static_cast<char>(current_byte));
        current_byte = 0;
        bits_filled = 0;              
    }
}


void BitOutputStream::finish() {
    while (bits_filled != 0) {
        write(0);
    }
}
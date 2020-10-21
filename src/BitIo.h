#pragma once

#include <istream>
#include <ostream>


class BitInputStream {
public:
    explicit BitInputStream(std::istream& in);

    // Get one bit from input stream
    int read();
private:
    std::istream& input;
    int current_byte; // Current byte value 
    int bits_remaining; // Bits remaining, to complete "current_byte"     
};



class BitOutputStream {
public:
    explicit BitOutputStream(std::ostream& out);

    // Write one bit to the byte
    void write(int b);

    // Complete byte with zeros in case of EOF before bits_filled == 0
    void finish();
private:
    std::ostream& output;
    int current_byte;
    int bits_filled;
};
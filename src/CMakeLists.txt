cmake_minimum_required(VERSION 3.8)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(compress
        globals.h
        ari.cpp
        ari.h
        debug.h
        main.cpp
        FenwickTree.cpp
        FenwickTree.h
        BitIo.cpp
        BitIo.h
        AdaptiveFrequencyTable.cpp
        AdaptiveFrequencyTable.h
        AbstractCoder.cpp
        AbstractCoder.h
        Decoder.cpp
        Decoder.h
        Encoder.cpp
        Encoder.h
        ppm.cpp
        ppm.h
        Context.cpp
        Context.h
        PpmModel.cpp
        PpmModel.h
        utils.cpp
        utils.h)

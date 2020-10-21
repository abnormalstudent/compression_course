#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// All variables are sorted in the 
// chronological order of their appearing in the program.

namespace globals {
    // True constants
    const uint32_t SIZE = 257;
    const uint32_t PRECISION = 32;
    // ---------------------------

    // Increasing aggresivity is a good way to maintain 
    // fast updates to the tables and spot bursts
    // in the byte's distributions.
    
    // More than that, it can be used to update 
    // main frequecny table according to the 
    // changes in fast frequencies.

    // So, by increasing aggresivity we probably will get a better
    // compression compared to usual AGGRESIVITY = 1;

    // Neutral element : 1
    const uint32_t AGGRESIVITY = 9;
    // Threshold is probably not a nice thing to work with
    // but we can use it to not to update frequency tables too
    // often.

    // Warning : can be overfitted option, so that, on one file
    // it will work fine, and on other's, it will do poorly 
    const uint32_t REINIT_THRESHOLD = 500;

    // Some constants for debugging
    const std::string LOG_PATH = "../logs.txt";
    const std::string TIME_PATH = "../time_logs.txt";
    const std::string TABLE_PATH = "../table.txt";
    const std::string CONFIG_PATH = "../configs.txt";

    // Multiplier is a parameter of the algorithm which is responsible 
    // for an aggresivity of the fast frequencies.
    // I think, by varying it we can probably discover 
    // the threshold which is the best to spot bursts
    // of changing distributions. 

    // UPD : probably it was a bad idea to add this parameter 
    // cause now it's so painful to handle update cases
    // Solution : don't check update conditions at all 
    // and just update freq. tables like the weights in Machine Learning
    // (By saying that, I mean update em every time)
    
    // Neutral element = 1
    const uint32_t MULTIPLIER = 6;

    // Neutral element : < 0
    // Normilize probabilities THRESHOLD (so that normalization doesn't happen too often)
    const uint64_t NORMALIZE_THRESHOLD = 5000;
    
    // Depricated
    // Might be useful in FrequencyTable::update function 
    const double DIFFERENCE_THRESHOLD = 0.5;

    // The key part of the algorithm. It's provides an opportunity to do
    // scaled update to frequency tables.

    // Neutral element : 0
    const double DIFFERENCE_MULTIPLIER = 0.10;

    // Neutral elemnt : anything < 0
    const uint32_t HIGHEST_UPDATE_THRESHOLD = 5;
    
    // Neutral element : 0
    const double UPDATE_MULTIPLIER = 0.1;

    // Order of the context model.
    const uint32_t ORDER = 3;

    // Let's change aggresivity according to length of the context;
    const std::vector<uint32_t> deep_aggressivity = {5, 15, 20, 75, 150, 400, 700};

    // Normilize table every CALLS_THRESHOLD calls.
    const uint32_t CALLS_THRESHOLD = 10;

    // Let's try TIME == 100
    // So if symbol was updated 100 symbols ago - the table get's reconstructed.
    const uint64_t TIME_THRESHOLD = 50;

    const uint32_t UPDATE_THRESHOLD = 100;

    // Change normalize threshold according to depth of the context
    const std::vector<uint32_t> deep_normilize_threshold = {300, 1500, 2000, 8000, 15000, 100000};
}

inline void save_globals() {
    std::ofstream config_stream(globals::CONFIG_PATH, std::ios_base::app);

    using namespace globals;
    config_stream << AGGRESIVITY << " " << REINIT_THRESHOLD << " " 
                  << MULTIPLIER << " " << NORMALIZE_THRESHOLD << " "
                  << DIFFERENCE_THRESHOLD << " " << DIFFERENCE_MULTIPLIER << " "
                  << HIGHEST_UPDATE_THRESHOLD << " " << UPDATE_MULTIPLIER << std::endl;
}
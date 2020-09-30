//
// Created by eduard on 30/09/2020.
//

#ifndef CPP_COMPRESSOR_UTILS_H
#define CPP_COMPRESSOR_UTILS_H

#pragma once

enum {
    ARI,
    PPM,
    BWT
};

struct CompressOptions {
    char *ifile;
    char *ofile;
    char mode;
    int  method;
};

CompressOptions *parse_args(int argc, char **argv);

CompressOptions *default_config();

void print_config(CompressOptions *opts);

void free_compress_opts(CompressOptions *opts);

int can_open_file(const char *filename);

#endif //CPP_COMPRESSOR_UTILS_H

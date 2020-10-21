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
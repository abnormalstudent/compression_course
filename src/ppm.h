#pragma once

#include "Encoder.h"
#include "Decoder.h"
#include "globals.h"

void compress_ppm(const char *ifile, const char *ofile);
void decompress_ppm(const char *ifile, const char *ofile);
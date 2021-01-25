# Arithmetic coding

Arithmetic coding is the loss-less compression algorithm which works the best by a theoretical bound. When encrypting the current symbol, we have to predict how distribution of the real data will change with time, the better we do that - the better the compression will be.

### Template

[MSU Videogroup](https://github.com/msu-video-group)

### Algorithm

Me

## Execution options
```
--input  <file>      = input file, default = `input.txt`.
--output <file>      = output file, default = `output.txt`.
--mode   {c | d}     = 'c' ~ compression, 'd' ~ decompression, default = 'd', .
--method {ari | ppm} = 'ari' - arithmetic coding, 'ppm' - prediction by partial matching, default = 'ari'.
```


### Examples  
```sh
# Unix
archiver --input the_financier.txt --output the_financier.cmp --mode c --method ppm
```

```sh
# Windows
archiver --input the_financier.txt --output the_financier.cmp --mode c --method ppm
```

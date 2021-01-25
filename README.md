# Arithmetic coding

### Template

[MSU Videogroup](https://github.com/msu-video-group)

### Algorithm

Me

## Execution options
```
--input  <file>      = input file, default = `input.txt`.
--output <file>      = output file, default = `output.txt`.
--mode   {c | d}     = 'c' ~ encryption, 'd' ~ decryption, default = 'd', .
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

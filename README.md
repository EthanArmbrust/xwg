# xwg
xor encryptor with garbage inserter

# Usage

`./xwg [mode] {parameters}`  

### Parameters

`--input` - the input file  
`--output` - the output file.  Defaults to value of `--input` + "-out" if no output specified.  
`--password` - the xor password  
`--bytes-per-block` - the average number of bytes between blocks of garbage that are inserted.  Default is 10.  
`--max-block-size` - the maximum size of a garbage block. Garbage blocks are a random size between 0 and `--max-block-size`.  Default is 256.  
`--garbage-signifier` - the char symbol used to signify the begin and end of a garbage block. Should not appear in the original file anywhere. Default is `~`.  


### Modes

`encrypt` and `decrypt` - does a simple xor function on the input file using the given password.  
Parameters `--input` and `--password` are required. Can take `--output`.  

`defile` - inserts random garbage into a file.  Requires `input`.  Can take `--output`, `--bytes-per-block`, `--max-block-size`, and `--garbage-signifier`.  

`clean` - removes garbage from a file. Requires `input`. Can take `--output` and `--garbage-signifier`.  

`encrypt-garbage` - inserts random garbage into a file and then performs a xor encryption.  Requires `--input` and `--password`.  Can take `--output`, `--bytes-per-block`, `--max-block-size`, and `--garbage-signifier`.  

`decrypt-garbage` - performs an xor encryption on a file and then removes the garbage from it.  Requires `--input` and `--password`.  Can take `--output` and `--garbage-signifier`.

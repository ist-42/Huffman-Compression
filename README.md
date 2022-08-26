# Huffman-Compression
A text file compression tool using huffman code.


## Build
### macOs/Linux:
```
mkdir build && cd build

cmake ..

make
```

## Usage


### Encode:
E.g. Encode a txt file: "test.txt"
```
./HuffEncode test.txt
```
The command above will generate ```test.txt.hufffile``` (Encoded file) and ```test.txt.hufftree``` (Huff tree).

### Decode:

```
./HuffDecode test.txt
```

The decoded file is ```test.txt.fromhuff```

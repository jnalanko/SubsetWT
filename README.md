# SubsetWT
This is the code for the paper "Subset Wavelet Trees", SEA 2023. 

A SubsetWT supports two basic operations on degenerate strings: subset-rank(*i*,*c*), which returns the number of subsets up to the i<sup>th</sup> subset in the degenerate string that contain the symbol *c*; and subset-select(*i*,*c*), which returns the index in the degenerate string of the i<sup>th</sup> subset that contains symbol *c*. These queries are analogs of rank and select queries that have been widely studied for ordinary strings.
This implementation currently supports only subset-rank queries.

The repository includes implementations of the 4 different approaches for solving rank queries using this new data structure described in the paper:
- Wavelet Trees;
  * compressed bitvectors (RRR);
  * uncompressed bitvectors (plain);  
- Scanning Rank;
- Sequence Splitting;
- Generalized RRR.
  
Experiments in the paper can be reproduced with the code stored in the [SubsetWT-Experiments repository](https://github.com/jnalanko/SubsetWT-Experiments).

```
git submodule init
git submodule update
cd sdsl-lite/build
cmake .. -DCMAKE_CXX_COMPILER=$(which g++) -DCMAKE_C_COMPILER=$(which gcc)
make
cd ../..
make example
```

This compiles `example.cpp` which shows how to use the subset wavelet tree class.

Tests can be compiled with `make tests` and run with `./tests`.

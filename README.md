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

# More on Subset Wavelet Trees
A Subset Wavelet Tree is a tree with $\log \sigma$ levels. The root node corresponds to the full alphabet. The alphabets of the rest of the nodes are defined recursively such that the left child of a node corresponds to the first half of the alphabet, and the right child corresponds to the second half of the alphabet. Empty sets are present only at the root.
Conceptually there are 2 bitvectors at each node of the tree: $L_v$ and $R_v$.
These two bivectors can be combined into a base-4 sequence at the root and into a base-3 sequence at other nodes.

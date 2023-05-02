# SubsetWT

```
git submodule init
git submodule update
cd sdsl-lite/build
cmake .. -DCMAKE_CXX_COMPILER=$(which g++) -DCMAKE_C_COMPILER=$(which gcc)
make
cd ../..
make example
```

This creates an executable called `example` that shows an example for how to use the subset wavelet tree class..

Tests can be compile with `make tests` and run with `./tests`.

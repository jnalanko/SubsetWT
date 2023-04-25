# SubsetWT

```
git submodule init
git submodule update
cd sdsl-lite/build
cmake .. -DCMAKE_CXX_COMPILER=$(which g++) -DCMAKE_C_COMPILER=$(which gcc)
make
cd ../..
make
```

This creates an executable called `main` that runs some tests.


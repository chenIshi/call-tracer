# call-tracer
LLVM based Call graph generator

1. Run
  1. `cmake ./`
  2. `make`
  3. `clang -O0 -S -emit-llvm data/test.c -o test.ll`
  4. `opt -load MyFunctionPass/libMyFunctionPass.so -CGPass test.ll`

(CMake out-of-tree build will be done after I figure out what LLVM is doing)

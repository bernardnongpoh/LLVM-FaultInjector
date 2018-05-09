clang  -emit-llvm -c ../runtime/Runtime.c -o runtime.bc
clang -g -emit-llvm -c sobel/sobel.c -o sobel/sobel.bc
llvm-link  runtime.bc sobel/sobel.bc -S -o sobel.bc
opt -load ../cmake-build-debug/dycount/libdycount.so -DynCount <sobel.bc> sobelfinal.bc
lli sobelfinal.bc sobel/i2.pgm
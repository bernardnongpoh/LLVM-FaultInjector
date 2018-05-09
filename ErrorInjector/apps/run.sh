clang++ -std=c++11 -fno-use-cxa-atexit -emit-llvm -c ../runtime/Runtime.cpp -o runtime.bc
clang++  -std=c++11 -g -emit-llvm -c hello.c -o hello.bc
llvm-link  runtime.bc hello.bc -S -o insthello.bc
opt -load  ../cmake-build-debug/errorinjector/liberrorinjector.so -FaultInjector   <insthello.bc> instprog.bc

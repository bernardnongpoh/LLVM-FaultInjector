clang++ -emit-llvm -c ../runtime/Runtime.c -o runtime.bc
clang++ -g -emit-llvm -c hello.c -o hello.bc
llvm-link  hello.bc -S -o insthello.bc
opt -load  ../cmake-build-debug/varinfo/libvarinfo.so -DynCount -stats  <insthello.bc> instprog.bc

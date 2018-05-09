#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int global;
int hello(){
	int a=1+2;
    int b=10;
    int c=20;
    a=c+b;
   	return a;
}

int passParam(int aParam,int *aPointer){

     aParam=aParam+1;
    *aPointer=*aPointer+1;
    return aParam;

}

int dependency(){
    int a; // declare varible a alloca in llvm
    a=10; // load 10 in %a
    int b=20;
    if(b==20){ // This will eliminate by compiler, but for testing only assume 20 is not constant
        a=a+1; // loading and storing re-  assigned a
    }
    else{
        a=a+2;
    }

    passParam(a,&a);
return a;
}


int main(int argc, char const *argv[])
{
    srand(time(NULL));
	int res=hello();
    dependency();
printf("\nResult =%d",res);
    return 0;
}

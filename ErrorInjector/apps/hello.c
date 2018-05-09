#include <stdio.h>
char hello(){
    char c='a';
    short int a=10;
    int b=20;

    return c;
}
int main(){
    char res=hello();
    printf("Res=%c",res);
}
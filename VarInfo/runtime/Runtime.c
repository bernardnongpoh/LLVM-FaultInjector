//
// Created by bernard on 30/11/17.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <set>


unsigned int
randr(unsigned int min, unsigned int max)
{

    double scaled = (double)rand()/RAND_MAX;

    return (max - min +1)*scaled + min;
}



int flipBits(int number,int numberOfBits){

    // Need to generate random number from 1 to 32 bits for int variable.
    // Get index from 0 to 32
// Flip bits make it is in unique position
    // This code flip randomly one bit , extension for more than one bit also need TODO
    int randomBit=randr(0,32);
    number^=1<<randomBit;
    return number;

}


int loadCounter=0;
int storeCounter=0;
void LOAD(){

    loadCounter++;
}


void STORE(){

    storeCounter++;
}

int injectError32IntData(int *a){

    *a=flipBits(*a,2);

    //*a=1234;
    return *a;
}

uint32_t injectErrorStore(int a){
    return 4321;
}

void PRINT(){

   printf("----------Statistics----------\n");
    printf(" # Load = %d \n",loadCounter);
    printf(" # STORE = %d \n",storeCounter);
}


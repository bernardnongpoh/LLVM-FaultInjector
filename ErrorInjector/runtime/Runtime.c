//
// Created by bernard on 30/11/17.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <random>
#include <limits>


unsigned int
randr(unsigned int min, unsigned int max)
{

    double scaled = (double)rand()/RAND_MAX;

    return (max - min +1)*scaled + min;
}



int flipBits(int number,int numberOfBits,int bitwidth){

    // Need to generate random number from 1 to 32 bits for int variable.
    // Get index from 0 to 32
// Flip bits make it is in unique position
    // This code flip randomly one bit , extension for more than one bit also need TODO
    int randomBit=randr(0,bitwidth);
    number^=1<<randomBit;
    printf("random = %d",randomBit);
    return number;

}

float flipFloatBits(float number,int numberOfBits,int bitwidth){

    // Need to generate random number from 1 to 32 bits for int variable.
    // Get index from 0 to 32
// Flip bits make it is in unique position
    // This code flip randomly one bit , extension for more than one bit also need TODO
    int randomBit=randr(0,bitwidth);
    int *temp=(int *) &number;
    *temp^=1<<randomBit;
    return number;

}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double flipDoubleBits(double number,int numberOfBits,int bitwidth){


    // Need to generate random number from 1 to 32 bits for int variable.
    // Get index from 0 to 32
// Flip bits make it is in unique position
    // This code flip randomly one bit , extension for more than one bit also need TODO
    int randomBit=randr(0,52);
    int64_t *temp=(int64_t *) &number;
    long N=1L;
    *temp^=N<<randomBit;
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

    *a=flipBits(*a,2,32);

    //*a=1234;
    return *a;
}

// this is for 32 bit integer
int32_t flipBitOn32IntegerValue(int32_t a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    int32_t res= flipBits(a,1,32);
    return res;
}

int64_t flipBitOn64IntegerValue(int64_t a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    int64_t res= flipBits(a,1,64);
    return res;
}

float flipBitOnFloatValue(float a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    float res= flipFloatBits(a,1,32);
    return res;
}


double flipBitOnDoubleValue(double a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    double res= flipDoubleBits(a,1,64);

    // from min to max double
    res= fRand(std::numeric_limits<double>::min(),std::numeric_limits<double>::max());
    // Limit from -1 to 1
    res= fRand(-1,1);
    return res;
}

void initializeIntegerArray(int array[],int size){
    int i=0;
    for(i=0;i<size;i++)
    {
        array[i]=10;
    }
}


void initializeDoubleArray(double *array,int size){
    int i=0;
    for(i=0;i<size;i++)
    {

        array[i]=10.0;

    }
}

void PRINT(){

   printf("----------Statistics----------\n");
    printf(" # Load = %d \n",loadCounter);
    printf(" # STORE = %d \n",storeCounter);
}

void SRAND(){
    srand(time(NULL));
}


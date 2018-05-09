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

/** @globalDimensionArray
 *
 * This global array is acting as a scrach pad for array error injection.
 * When allocaInst is call for an array,
 * extract all dimension and call insertGlobalDimensionArray for each dimension.
 *
 * During error injection @FaultEngine.cpp we only need to pass the size of the dimension and inject error will inject random value.
 *
 *
 * */


std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> dis(-100000,100000); // std::numeric_limits<int >::min(), std::numeric_limits<int >::max() for MAX and MIN
std::uniform_real_distribution<> disRealFloat(-100000.000, 100000.000);
std::uniform_real_distribution<> disRealDouble(-100000.000, 100000.000);// Range of values
int globalDimensionArray[4]; // Maximum dimension for this tool = 4 TODO: handling n-dimensional array This is scratch pad for dimension of an array globalDimensionArray[0]-1 st dimension etc

void clearGlobalDimensionArray(){

    int i;
    for(i=0;i<4;i++){
        globalDimensionArray[i]=-1;
    }
}

void insertGlobalDimensionArray(int data,int index){

    printf("Data: %d, Index: %d\n",data,index);

    if(index>4)
        printf("Tool can handle only up to 4th Dimension");
    globalDimensionArray[index]=data;

}

int getGlobalDimensionalArray(int index){
    if(index>4) {
        printf( "Tool can handle only up to 4th Dimension");
        return 0;
    }
    return globalDimensionArray[index];
}


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

double doubleRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

float floatRand(float fMin, float fMax)
{
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


int intRand(int fMin, int fMax)
{
    int f = (int)rand() / RAND_MAX;
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
int16_t flipBitOn16IntegerValue(int16_t a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    int16_t res= flipBits(a,1,16);

    return res;
}

// this is for 32 bit integer
int32_t flipBitOn32IntegerValue(int32_t a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    int32_t res= flipBits(a,1,32);

    return res;
}



// this is for 32 bit integer
int flipBitOnIntegerValue(int a){
    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    int res= flipBits(a,1,32);
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
    //float res= flipFloatBits(a,1,32);
   float res=floatRand(std::numeric_limits<float >::min(),std::numeric_limits<float >::max());
    return res;
}


double flipBitOnDoubleValue(double a){

    // Generate a random number here
    // Random number or Bit flip, which one is better, I think bitflip is much more practical as compared to random number.
    double res= flipDoubleBits(a,1,64);

    // from min to max double
    res= doubleRand(std::numeric_limits<double>::min(),std::numeric_limits<double>::max());
    // Limit from -1 to 1
    //res= fRand(-1,1);
    return res;
}

// need to modify this one, how to get the type of array
void initializeIntegerArray(int *array,int noOfDimension){
    int dim=0;
    // Create inner for loop according to noOfDimension
    printf("Dimension = %d\n",noOfDimension);
    int i,j,k;
    switch(noOfDimension){
        case 1:
        {
            for(i=0;i<globalDimensionArray[0];i++){
                *(array+i)=dis(gen);
            }
        }
            break;


        case 2:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    *((array+i*globalDimensionArray[1]) + j)= dis(gen);
// *((arr+i*n) + j)
                }

            }
        }
            break;

        case 3:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    for(k=0;k<globalDimensionArray[2];k++){


                        *(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k)= dis(gen);

                        //printf("%p\n",(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k));
                        // data = *(base + a * (b x h) + b * (h) + c);
                    }
                }

            }
        }
            break;
    }
}

void initializeFloatArray(float *array,int noOfDimension){
    int dim=0;
    // Create inner for loop according to noOfDimension
    printf("Dimension = %d\n",noOfDimension);
    int i,j,k;
    switch(noOfDimension){
        case 1:
        {
            for(i=0;i<globalDimensionArray[0];i++){
                *(array+i)=disRealFloat(gen);
            }
        }
            break;


        case 2:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    *((array+i*globalDimensionArray[1]) + j)= disRealFloat(gen);
// *((arr+i*n) + j)
                }

            }
        }
            break;

        case 3:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    for(k=0;k<globalDimensionArray[2];k++){


                        *(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k)= disRealFloat(gen);

                        //printf("%p\n",(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k));
                        // data = *(base + a * (b x h) + b * (h) + c);
                    }
                }

            }
        }
            break;
    }
}


void initializeDoubleArray(double *array,int noOfDimension){

    int dim=0;
    // Create inner for loop according to noOfDimension
    printf("Dimension = %d\n",noOfDimension);
    int i,j,k;
    switch(noOfDimension){
        case 1:
        {
            for(i=0;i<globalDimensionArray[0];i++){
                *(array+i)=disRealDouble(gen);
            }
        }
            break;


        case 2:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    *((array+i*globalDimensionArray[1]) + j)= disRealDouble(gen);
// *((arr+i*n) + j)
                }

            }
        }
            break;

        case 3:
        {
            for(i=0;i<globalDimensionArray[0];i++){

                for(j=0;j<globalDimensionArray[1];j++){

                    for(k=0;k<globalDimensionArray[2];k++){


                        *(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k)= disRealDouble(gen);

                        //printf("%p\n",(array+i*(globalDimensionArray[1]*globalDimensionArray[2]) + j * globalDimensionArray[2] + k));
                        // data = *(base + a * (b x h) + b * (h) + c);
                    }
                }

            }
        }
            break;
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


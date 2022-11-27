/*The code archive of he musicdsp mailing list (www.musicdsp.org) includes and example 
 * under the Synthesis category of an exponential decay generator ("Fast Exponential 
 * Envelop Generator"). Make a program to implement and test this code, using Gnuplot
 * to display the output*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Tried multiple values and checked the formula in different ways but didn't get how it worked
void checkInput(float levelBegin, float levelEnd, float releaseTime, int samplePoints){
  
  if (levelEnd==1||levelEnd==1) {
    printf("Levels can't be equal to 1");
    exit(0);
  }

  if (releaseTime<=0) {
    printf("Release time can't be less than or equal to 0");
    exit(0);
  }
}

void init(char* fileName, float levelBegin, float levelEnd, float releaseTime, int samplePoints){

  checkInput(levelBegin, levelEnd, releaseTime, samplePoints);

  FILE* output;
  output = fopen(fileName,"w");
  const float sampleRate = 44100;
  float coeff, currentLevel, time=0;

  currentLevel = levelBegin;
  coeff = (log(levelEnd)-log(levelBegin)) / (releaseTime * sampleRate);
  printf("%f\n",log(levelBegin));
  for (int i = 0; i<samplePoints; ++i) {
    currentLevel += (coeff * currentLevel);
    time += releaseTime/samplePoints;
    fprintf(output,"%f\t%f\n",time,currentLevel);
  }
}

int main(int argc, char* argv[]){
  init("output.txt",0.3,0.2,5,8);
  return 0;
}



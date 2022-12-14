/* The simplest possible envelope generator for sound synthesis comprises a two-segment
 * attack-decay shape, where the peak value is user-controllable, as is the duration of each
 * segment. Write a program to generate such an envelope as standard breakpoints. Use two
 * duration arguments and a single-level argument, together  with and argument to set the 
 * overall number of points. Use Gnuplot to verify correctness*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct breakpoint{
  float time;
  float value;
}BREAKPOINT;


BREAKPOINT* generateEnvelope(int npoints, double startval, double endval,double duration);

void joinEnvelopesIntoFile (char* path, int npoints, double attackdur, double decaydur, double peak){
  FILE* fp = fopen(path, "w");
  BREAKPOINT* attack = generateEnvelope(npoints/2, 0, peak, attackdur); 
  BREAKPOINT* decay = generateEnvelope(npoints/2, peak, 0, decaydur); 
 
  BREAKPOINT* envelope = (BREAKPOINT*)malloc(sizeof(BREAKPOINT)*npoints);

  for (int i = 0; i<npoints/2; ++i) {
    envelope[i].time = attack[i].time;
    envelope[i].value = attack[i].value;
    envelope[i+(npoints/2)].time = decay[i].time+attack[npoints/2].time;
    envelope[i+(npoints/2)].value = decay[i].value;
  }

  for (int p=0; p<npoints; ++p) {
    fprintf(fp,"%4lf\t%.8lf\n", envelope[p].time, envelope[p].value);
  }  
}

BREAKPOINT* generateEnvelope(int npoints, double startval, double endval,double duration){

  BREAKPOINT* breakpoints = (BREAKPOINT*) malloc(sizeof(BREAKPOINT)*npoints);
  int i;
  double step, valrange, offset, end, start, thisstep, fac;
  const double verysmall = 1.0e-4;

  step = duration/npoints;
  valrange = endval - startval;
  if (valrange == 0.0) {
    fprintf(stderr, "warning: start and end values are the same!\n"); 
  }
  if (startval>endval) {
    start = 1.0;
    end = verysmall;
    valrange = -valrange;
    offset = endval;
  }else {
    start = verysmall;
    end = 1.0;
    offset = startval;
  }
  thisstep = 0.0;
  fac = pow(end/start, 1.0/npoints);
  for (i = 0; i<npoints; ++i) {
    breakpoints[i].time = thisstep;
    breakpoints[i].value = offset + (start*valrange);
    start *=fac;
    thisstep+=step;
  }
    breakpoints[npoints].time = thisstep;
    breakpoints[npoints].value = offset + (start*valrange);

    fprintf(stderr, "done\n");
    return breakpoints;
}

int main(int argc, char** argv)
{
  if (argc != 6) {
    fprintf(stderr, "Usage: file npoints attackduration decaydur peak\n");
    return 1;
  }

  double npoints = atoi(argv[2]);
  if (npoints <= 0) {
    fprintf(stderr,"Error: npoints must be positive");
    return 1;
  }

  double attackdur = atof(argv[3]);
  if (attackdur <= 0.0) {
    fprintf(stderr, "Error: duration must be positive \n");
    return 1;
  }
  double decaydur = atof(argv[4]);
  if (decaydur <= 0.0) {
    fprintf(stderr, "Error: duration must be positive \n");
    return 1;
  }
  double peak = atof(argv[5]);
  if (peak <= 0.0) {
    fprintf(stderr, "Error: duration must be positive \n");
    return 1;
  }
  joinEnvelopesIntoFile("test.txt",npoints, attackdur, decaydur, peak);
    return 0;
}

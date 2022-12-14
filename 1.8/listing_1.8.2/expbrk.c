#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv)
{
  FILE* fp;
  int i, npoints;
  double startval,endval;
  double dur, step, start, end,thisstep;
  double fac, valrange, offset;
  const double verysmall = 1.0e-4;
  fp = fopen("text.txt", "w"); 

  if (argc != 5) {
    fprintf(stderr, "Usage: expbrk duration npoints startval endval\n");
    return 1;
  }
  dur = atof(argv[1]);
  if (dur <= 0.0) {
    fprintf(stderr, "Error: duration must be positive \n");
    return 1;
  }
  npoints = atoi(argv[2]);
  if (npoints <= 0) {
    fprintf(stderr,"Error: npoints must be positive");
    return 1;
  }
  step = dur/npoints;
  startval = atof(argv[3]);
  endval = atof(argv[4]);
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
    fprintf(fp, "%4lf\t%.8lf\n", thisstep,offset + (start*valrange));
    start *=fac;
    thisstep+=step;
  }
    fprintf(fp, "%4lf\t%.8lf\n", thisstep,offset + (start*valrange));
    fprintf(stderr, "done\n");
    return 0;
}

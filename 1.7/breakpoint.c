#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct breakpoint{
  double time;
  double value;
} BREAKPOINT;

BREAKPOINT get_maxpoint (const BREAKPOINT* points, unsigned long npoints);
BREAKPOINT* get_breakpoints(FILE* fp, long*psize); 

BREAKPOINT stretch_time(BREAKPOINT point, long duration);
BREAKPOINT scale_values(BREAKPOINT point, float rate);
BREAKPOINT* normalize_to_value(BREAKPOINT* points, float top);
BREAKPOINT* shift_values(BREAKPOINT* points, float shift);
void insert_delete_point(char* option, BREAKPOINT* points, long value, long duration);
BREAKPOINT* truncate_to_duration();

BREAKPOINT get_maxpoint (const BREAKPOINT* points, unsigned long npoints)
{
  int i;
  BREAKPOINT point;

  point.time = points[0].time;
  point.value = points[0].value;

  for (i=1; i<npoints; i++) {
    if (point.value < points[i].value) {
      point.value = points[i].value;
      point.time = points[i].time; 
    }
  }
  return point; 
}

BREAKPOINT* get_breakpoints(FILE* fp, long*psize){
  int got;
  unsigned long npoints = 0, size = 64;
  double lasttime = 0.0;
  BREAKPOINT* points = NULL;
  char line[80];

  if (fp==NULL) 
    return NULL;
  points = (BREAKPOINT*) malloc(sizeof(BREAKPOINT)*size);
    if (points==NULL) 
      return NULL;
    
  while (fgets(line,80,fp)) {
    got = sscanf(line, "%lf%lf", &points[npoints].time, &points[npoints].value);
    if (got<0) {
      continue;
    }    
    if (got==0) {
      printf("Line %lu has non-numeric data\n", npoints+1);
      break;
    }
    if (got==1) {
      printf("Incomplete breakpoint found at %lu\n", npoints+1);
      break;
    }
    if (points[npoints].time<lasttime) {
      printf("Data error at point %lu: time not increasing\n", npoints+1);
      break;
    }

  lasttime = points[npoints].time;
  if(++npoints==size){
    BREAKPOINT* tmp;
    size+= npoints;
    tmp=(BREAKPOINT*)realloc(points, sizeof(BREAKPOINT)*size);
    if(tmp==NULL)
    {
      npoints=0;
      free(points);
      points=NULL;
      break;
    }
    points=tmp;
  }
}
  if(npoints)
  *psize = npoints;
return points;
}

int main(int argc, char* argv[]){
  long size;
  double dur;
  BREAKPOINT point, *points;
  FILE* fp;

  printf("breakdur: find duration of breakpoint file \n");
  if (argc<2) {
    printf("usage; breakdur infile.txt \n");
    return 0;
  }
  fp = fopen(argv[1],"r");
  if (fp == NULL) {
    return 0;
  }
  size = 0;
  points = get_breakpoints(fp, &size);
  if (points==NULL) {
    printf("No breakpoints read.\n");
    fclose(fp);
  
  }
  if(size<2)
  {
    printf("Error: at least two breakpoints required \n");
    free(points);
    fclose(fp);
    return 1;
  }

  /*we require breakpoints to start from 0 */
  if (points[0].time != 0.0) {
    printf("Error in breakpoint data: first time must be 0.0");
    free(points);
    fclose(fp);
    return 1;
  }

  printf("read %lu breakpoints \n", size);
  dur = points[size-1].time;
  printf("duration: %f seconds \n", dur);
  point = get_maxpoint(points,size);
  printf("maximum value: %f at %f secs \n", point.value, point.time);
  free(points);
  fclose(fp);
  return 0;
}

BREAKPOINT stretch_time (BREAKPOINT point, long duration){
  point.time += duration;
  return point;
}

BREAKPOINT scale_values(BREAKPOINT point, float rate){
  point.time *= rate;
  point.value *= rate;
  return point;
}

BREAKPOINT* normalize_to_value(BREAKPOINT* points, float top){
  int i; 
  BREAKPOINT maxpoint = get_maxpoint(points, sizeof(points)/sizeof(points[0])); 
  double difference = top - maxpoint.value;

  for (i=0; i<(sizeof(points)/sizeof(points[0])); ++i) {
    points[i].value+=difference;
  }
  return points;
}

BREAKPOINT * shift_values(BREAKPOINT* points, float shift){
  for (size_t s = 0; s<sizeof(points)/sizeof(points[0]); ++s) {
   points[s].time += shift; 
  }
  return points;
}

/*void insert_delete_point(char* option, BREAKPOINT* points, long value, long time){
  size_t size = sizeof(points)/sizeof(points[0]);

  if (strcmp(option, " ")) {
    printf("please choose an option");
    return;
  }else if (strcmp(option, "insert")) {
    for (size_t p = 0; p<size; ++p) {
      if (time < points[p+1].time){
        //Push points ahead one place forward
      points=tmp;
        for (size_t t = 0; t<size; ++t) {
          
        }
        points[p].value = value;
        points[p].time = time;
      }else if (points[p+1].time == time) {
        //overwrites 
        points[p+1].value = value;
      }else if (p==(size-1)) {
        //insertion comes after last point
      }
    }
  }else if (strcmp(option, "delete")) {
    for (size_t p = time; p<size; ++p) {
     points[p]=points[p+1]; 
    }   
    //find value, overwrite the array and delete last value
  }
}*/

BREAKPOINT* truncate_to_duration(BREAKPOINT* input, long time){
  BREAKPOINT* temp;
  for (size_t i; i<(sizeof(input)/sizeof(input[0])); ++i) {
    if (input[i].time == time) {
      for (size_t c; c<i; ++c) {
        temp[c] = input[c];
      }
      return temp;
    }
  }
}

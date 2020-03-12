#include <stdio.h>
#include <math.h>
#include "pav_analysis.h"
#include "fic_wave.h"

float compute_power (const float *x, unsigned int N){
  float res = 0;
  int i = 0;
  for (i = 0; i < N; i++){
    res += x[i]*x[i];
  }
  res = 10*log10(res/N);
  return res;
}

float compute_am (const float *x, unsigned int N){
  float res = 0;
  int i = 0;
  for (i = 0; i < N; i++){
    res += fabs(x[i]);
  }
  res = res/N;
  return res;
}

float compute_zcr (const float *x, unsigned int N, float fm){
  float res = 0;
  int i = 0;
  for (i = 1; i < N; i++){
    if (x[i] * x[i-1] < 0) res += 1.;
  }
  res = (fm/(2*(N-1)))*res;
  return res;
}

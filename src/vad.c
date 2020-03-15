#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"
#include "vad.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
    "UNDEF", "MS", "MV", "S", "V", "INIT"};

const char *state2str(VAD_STATE st)
{
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct
{
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N)
{
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  float fm = 16000;
  feat.zcr = compute_zcr(x, N, fm);
  feat.p = compute_power(x, N);
  feat.am = compute_am(x, N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA *vad_open(float rate)
{
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data)
{
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data)
{
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x)
{
  // float k0;
  float sum;
  // float k1;
  //const float minsilence = 0.23;
  //const float minvoice = 0.00275;
  int count, count1, count2, count3, count4;
  clock_t time;

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

  switch (vad_data->state)
  {
  case ST_INIT:

    if (count < 10)
    {
      //sum += pow(10, compute_features(x, vad_data->frame_length).p / 10);
      sum += compute_features(x, vad_data->frame_length).p;
      count++;
    }
    else
    {
      vad_data->state = ST_SILENCE;
      count = 0;
      //vad_data->k0 = 10 * log10(sum /10);
      vad_data->k0 = sum/10 + 1;
      vad_data->k1 = vad_data->k0 + 27;
      //vad_data->k2 = vad_data->k0 + 43;
      sum = 0;
    }
    break;

  case ST_SILENCE:
    //float k1=k0+10; // tenint en compte que la pot sera aprox 90, la menor potencia de fricatives es 76, agafem 80 que es una mica per sota d'aquestes

    if (f.p > vad_data->k1)
    {
      vad_data->state = ST_MAYBE_VOICE;
      
    } //  time = clock(); // start timer

    break;

  case ST_MAYBE_VOICE:

    if (f.p > vad_data->k1)
    {
      //time = clock() - time;
      if (count1 > 42)
      {
        vad_data->state = ST_VOICE;
        count1 = 0; 
      
      }else{
        vad_data->state = ST_MAYBE_VOICE;
      }
    }
    
    if (f.p < vad_data->k1)
    {
      count1 = 0;
      vad_data->state = ST_SILENCE;
     
    }
    count1++;
    break;
    

  case ST_VOICE:
    if (f.p < vad_data->k1)
    {
      vad_data->state = ST_MAYBE_SILENCE;
    }
    // time = clock(); // start timer

  break;

  case ST_MAYBE_SILENCE:

    if (f.p < vad_data->k1)
    {
      // time = clock() - time;
      if (count2 >= 13)
      {
        vad_data->state = ST_SILENCE;
        count2 = 0;
      }else{
        vad_data->state = ST_MAYBE_SILENCE;
      }
      
    }
   if (f.p > vad_data->k1)
    {
      vad_data->state = ST_VOICE;
      count2 = 0;
    }
    count2++;

    break;    

  case ST_UNDEF:
    break;
  }
  
  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE || vad_data->state == ST_MAYBE_VOICE || vad_data->state == ST_MAYBE_SILENCE)
    return vad_data->state;
  else if (vad_data->state == ST_INIT)
    return ST_SILENCE;
  else
    return ST_UNDEF;

}

void vad_show_state(const VAD_DATA *vad_data, FILE *out)
{
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
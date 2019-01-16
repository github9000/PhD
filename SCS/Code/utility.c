/*----------------------------------------------------------*/
/* utility.c - contains random number generator and related */
/* utilities, including advance_random, warmup_random,      */
/* random, randomize, flip, and rnd                         */
/*----------------------------------------------------------*/

#include <math.h>
#include "scs.h"



float poweri(float x, int i)
{
 float powertemp;

 powertemp = 1.0;
 if (i==0) powertemp = 1.0;
 else
 if (i>0) 
   do {
     powertemp *= x;
     i -= 1;
   } while (i != 0);
 else
 if (i<0) 
   do {
     powertemp /= x;
     i += 1;
   } while (i != 0);
 
 return powertemp;
}




float max(float x, float y)
{
  /* return max of two values */ 
  if (x > y) return x;
  return y;
}

float min(float x, float y)
{
  /* return min of two values */ 
  if (x < y) return x;
  return y;
}

float avg(float x, float y)
{
  /* return avg of two values */ 
  return 0.5 * (x + y);
}

boolean halt()
{
  /* Test for keypress and query halt flag */
  const int times = 000;   /* 1000 */

  boolean temp;
  char ch;
  static int j = 300;

  for (; j < times; ) {
	 j++;
         if ((j % 100) == 0) printf("Generation %d\n", j);
	 return 0;
  }

  j = 0;

  printf("Halt (y/n) ? >> ");

  do {
    ch = getchar();
  } while (ch == '\n');

  temp = ((ch == 'y') || (ch == 'Y')) ? 1 : 0;

  return temp;
}
  


/*
 Variables are declared static so that they cannot conflict 
 with names of other global variables in other files.  See K&R, 
 p 80, for scope of static */
static double oldrand[55];    /* Array of 55 random numbers */
static int jrand;             /* current random number */
static double rndx2;          /* used with random normal deviate */
static int rndcalcflag;       /* used with random normal deviate */

void advance_random()
/* Create next batch of 55 random numbers */
{
    int j1;
    double new_random;

    for(j1 = 0; j1 < 24; j1++)
    {
        new_random = oldrand[j1] - oldrand[j1+31];
        if(new_random < 0.0) new_random = new_random + 1.0;
        oldrand[j1] = new_random;
    }
    for(j1 = 24; j1 < 55; j1++)
    {
        new_random = oldrand [j1] - oldrand [j1-24];
        if(new_random < 0.0) new_random = new_random + 1.0;
        oldrand[j1] = new_random;
    }
}


int flip(float prob)
/* Flip a biased coin - true if heads */
{
    float randomperc();

    if(randomperc() <= prob)
        return(1);
    else
        return(0);
}


void initrandomnormaldeviate()
/* initialization routine for randomnormaldeviate */
{
    rndcalcflag = 1;
}


double noise(double mu, double sigma)
/* normal noise with specified mean & std dev: mu & sigma */
{
    double randomnormaldeviate();

    return((randomnormaldeviate()*sigma) + mu);
}


void randomize()
/* Get seed number for random and start it up */
{
    float randomseed;
    int j1;


    for(j1=0; j1<=54; j1++) 
      oldrand[j1] = 0.0;

    
    jrand=0;

    do {
        fprintf(stdout," Enter random number seed, 0.0 to 1.0 -> ");
        fscanf(stdin,"%f", &randomseed);
    } while((randomseed < 0.0) || (randomseed > 1.0));
/*
    }
    else
    {
        fscanf(stdin,"%f", &randomseed);
    }
*/

    warmup_random(randomseed);
}


double randomnormaldeviate()
/* random normal deviate after ACM algorithm 267 / Box-Muller Method */
{
    double sqrt(), log(), sin(), cos();
    float randomperc(); 
    double t, rndx1;

    if(rndcalcflag)
    {
        rndx1 = sqrt(- 2.0*log((double) randomperc()));
        t = 6.2831853072 * (double) randomperc();
        rndx2 = rndx1 * sin(t);
        rndcalcflag = 0;
        return(rndx1 * cos(t));
    }
    else
    {
        rndcalcflag = 1;
        return(rndx2);
    }
}


float randomperc()
/* Fetch a single random number between 0.0 and 1.0 - Subtractive Method */
/* See Knuth, D. (1969), v. 2 for details */
/* name changed from random() to avoid library conflicts on some machines*/
{
    jrand++;
    if(jrand >= 55)
    {
        jrand = 1;
        advance_random();
    }
    return((float) oldrand[jrand]);
}


int rnd(int low, int high)
/* Pick a random integer between low and high */
{
    int i;
    float randomperc();

    if(low >= high)
        i = low;
    else
    {
        i = (randomperc() * (high - low + 1)) + low;
        if(i > high) i = high;
    }
    return(i);
}


float rndreal(float lo , float hi)
/* real random number between specified limits */
{
    return((randomperc() * (hi - lo)) + lo);
}


void warmup_random(float random_seed)
/* Get random off and running */
{
    int j1, ii;
    double new_random, prev_random;

    oldrand[54] = random_seed;
    new_random = 0.000000001;
    prev_random = random_seed;
    for(j1 = 1 ; j1 <= 54; j1++)
    {
        ii = (21*j1)%54;
        oldrand[ii] = new_random;
        new_random = prev_random-new_random;
        if(new_random<0.0) new_random = new_random + 1.0;
        prev_random = oldrand[ii];
    }

    advance_random();
    advance_random();
    advance_random();

    jrand = 0;
}

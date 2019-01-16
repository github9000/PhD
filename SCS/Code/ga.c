/**************************************************************/
/* FILENAME : ga.c                                            */
/**************************************************************/

#include "scs.h"


/* Genetic algorithm code for SCS */



void initga(FILE *gfile, grecord *garec, poptype *population)
{
  /* initialize GA parameters */

  fscanf(gfile, "%f", &(garec->proportionselect));
  fscanf(gfile, "%f", &(garec->pmutation));
  fscanf(gfile, "%f", &(garec->pcrossover));
  fscanf(gfile, "%d", &(garec->crowdingfactor));
  fscanf(gfile, "%d", &(garec->crowdingsubpop));

  garec->nselect = (int) ( garec->proportionselect *
          population->nclassifier * 0.5);
        /* Number of mate pairs to select */

 garec->nmutation = 0;
 garec->ncrossover = 0;
}



void initrepga( FILE *rep, grecord *garec )
{
  /* initial report */
  fprintf(rep, "\n\n");
  fprintf(rep, "Genetic Algorithm Parameters\n");
  fprintf(rep, "----------------------------\n");
  fprintf(rep, "Proportion to select/gen  = %f\n", garec->proportionselect);
  fprintf(rep, "Number to select  = %d\n", garec->nselect);
  fprintf(rep, "Mutation Probability = %f\n", garec->pmutation);
  fprintf(rep, "Crossover Probability = %f\n", garec->pcrossover);
  fprintf(rep, "Crowding Factor = %d\n", garec->crowdingfactor);
  fprintf(rep, "Crowding Subpopulation = %d\n", garec->crowdingsubpop);
}


int select(poptype *population)
{
  /* Select a single individual according to strength */
  float rand, partsum;
  int j;

  partsum = 0.0;
  j = 0;
  rand = randomperc() * population->sumstrength;

  do {
  j += 1;
  partsum += population->classifier[j].strength;
  } while ((partsum < rand) && (j != population->nclassifier));

  return j;
}



trit mutation(trit positionvalue, float pmutation, int *nmutation)
{
  /* mutate a single position with specified probability */

  int tempmutation;

  if (flip(pmutation))  {
  tempmutation = (positionvalue + rnd(1,2)+1) % 3 - 1;
  *nmutation += 1;
  }
  else
  tempmutation = positionvalue;

  return tempmutation;
}


bit bmutation(bit positionvalue, float pmutation, int *nmutation)
{
  /* mutate a single bit with specified probability */

  int tempmutation;

  if (flip(pmutation))  {
  tempmutation = (int) ((positionvalue + 1) % 2);
  *nmutation += 1;
  }
  else
  tempmutation = (int) positionvalue;

  return tempmutation;
}

void crossover( classtype parent1, classtype parent2, classtype *child1,
      classtype *child2, float pcrossover, float pmutation,
      int *sitecross, int *nposition, 
                                         int *ncrossover, int *nmutation)
{
  /* Cross a pair at given site with mutation on the trit transfer */

  float inheritence;
  int j;

  if (flip(pcrossover))  {
  *sitecross = rnd(1, *nposition);
  *ncrossover += 1;
  }
  else
  *sitecross = *nposition + 1;   /* transfer but no cross */

  /* transfer action part regardless of sitecross */
  child1->act = bmutation (parent1.act, pmutation, nmutation);
  child2->act = bmutation (parent2.act, pmutation, nmutation);

  /* transfer and cross above cross-site */
  j = *sitecross;

  while (j <= *nposition) {
  child2->c[j] = mutation (parent1.c[j], pmutation, nmutation);
  child1->c[j] = mutation (parent2.c[j], pmutation, nmutation);
  j += 1;
  }

  j = 1;

  /* transfer only below cross site */
  while (j < *sitecross) {
  child1->c[j] = mutation (parent1.c[j], pmutation, nmutation);
  child2->c[j] = mutation (parent2.c[j], pmutation, nmutation);
  j += 1;
  }

  /* children inherit average of parental strength values */
  inheritence = avg(parent1.strength, parent2.strength);

  child1->strength = inheritence;
  child1->matchflag = FALSE;
  child1->ebid = 0.0;
  child1->bid = 0.0;
  child1->specificity = countspecificity(child1->c, *nposition);

  child2->strength = inheritence;
  child2->matchflag = FALSE;
  child2->ebid = 0.0;
  child2->bid = 0.0;
  child2->specificity = countspecificity(child2->c, *nposition);
}

int worstofn(poptype *population, int n)
{
  /* select worst individual from random subpopulation of size n */
  int j, worst, candidate;
  float worststrength;

  /* initialize population randomly */
  worst = rnd(1, population->nclassifier);
  worststrength = population->classifier[worst].strength;

  /* select and compare from remaining population */
  if (n > 1)
  for (j = 2; j <= n; j++) {
  candidate = rnd(1, population->nclassifier);

  if (worststrength > population->classifier[candidate].strength) {
    worst = candidate;
    worststrength = population->classifier[worst].strength;
  }
  }

  /* return worst */
  return worst;
}


int matchcount(classtype classifier1, classtype classifier2, int nposition)
{
  /* count number of positions of similarity */

  int tempcount, j;

  if (classifier1.act == classifier2.act)
  tempcount = 1;
  else
  tempcount = 0;

  for (j=1; j <= nposition; j++)
  if (classifier1.c[j] == classifier2.c[j])
  tempcount += 1;

  return tempcount;
}

int crowding (classtype *child, poptype *population, int crowdingfactor,
      int crowdingsubpop)
{
  /* replacement using modified De Jong crowding */
  int popmember, j, match, matchmax, mostsimilar;

  matchmax = -1;
  mostsimilar = 0;

  if (crowdingfactor < 1)
   crowdingfactor = 1;

  for (j=1; j <= crowdingfactor; j++) {
  popmember = worstofn(population, crowdingsubpop);
  match = matchcount(*child, population->classifier[popmember],
          population->nposition);

  if (match > matchmax) {
  matchmax = match;
  mostsimilar = popmember;
  }
  }

  return mostsimilar;
}


void statistics(poptype *population)
{
 /* population stats: max, avg, min, sum of strength */

  int j = 1;

  population->maxstrength = population->classifier[j].strength;
  population->minstrength = population->classifier[j].strength;
  population->sumstrength = population->classifier[j].strength;

  j++;

  while  (j <= population->nclassifier) {
  population->maxstrength = max(population->maxstrength,
           population->classifier[j].strength);
  population->minstrength = min(population->minstrength,
           population->classifier[j].strength);
  population->sumstrength += population->classifier[j].strength;
  j += 1;
  }

  population->avgstrength = population->sumstrength / population->nclassifier;
}

/* Copy one details from one classtype variable into another */
void classifier_copy( classtype *source, classtype *target)
{ int i;

  for (i=1; i < MAXPOSITION; i++)
  target->c[i] = source->c[i];

  target->act = source->act;
  target->strength = source->strength;
  target->bid = source->bid;
  target->ebid = source->ebid;
  target->matchflag = source->matchflag;
  target->specificity = source->specificity;

}


void ga(grecord *garec, poptype *population)
{
  /* coordinate selection, mating, xover, mutation, & replacement */
  int j;
  classtype *child1, *child2;

  child1 = (classtype *) safe_malloc(sizeof(classtype));
  child2 = (classtype *) safe_malloc(sizeof(classtype));

  statistics(population);

  for (j=1; j <= garec->nselect; j++) {
  garec->mating[j].mate1 = select(population);
  garec->mating[j].mate2 = select(population);

  crossover(population->classifier[garec->mating[j].mate1],
      population->classifier[garec->mating[j].mate2],
      child1, child2, garec->pcrossover, garec->pmutation,
      &(garec->mating[j].sitecross), 
                                  &(population->nposition),
      &(garec->ncrossover), 
                                  &(garec->nmutation));

  garec->mating[j].mort1 = crowding(child1, population,
           garec->crowdingfactor, garec->crowdingsubpop);

  population->sumstrength += - population->classifier
            [garec->mating[j].mort1].strength
            + child1->strength;

  classifier_copy(child1,
      &(population->classifier[garec->mating[j].mort1]));

  garec->mating[j].mort2 = crowding(child2, population,
           garec->crowdingfactor, garec->crowdingsubpop);

  population->sumstrength += - population->classifier
            [garec->mating[j].mort2].strength
            + child2->strength;


  classifier_copy(child2,
      &(population->classifier[garec->mating[j].mort2]));

  free(child1);
  free(child2);
  }
}

void reportga(FILE *rep, grecord *garec, poptype *population)
{
  /* report on mating, xover, and replacement */
  int j;

  page(rep);
  fprintf(rep,"Genetic Algorithm Report\n");
  fprintf(rep,"------------------------\n\n");
  fprintf(rep,"Pair  Mate1  Mate2  SiteCross  Mort1  Mort2\n");
  fprintf(rep,"-------------------------------------------\n");

  for (j=1; j <= garec->nselect; j++) {
  fprintf(rep, "%4d   %4d   %4d   %4d   %4d   %4d\n", j,
           garec->mating[j].mate1,
           garec->mating[j].mate2,
           garec->mating[j].sitecross,
           garec->mating[j].mort1,
           garec->mating[j].mort2);
  fprintf(rep, "Statistics Report\n");
  fprintf(rep, "-----------------\n");
  fprintf(rep, "Average Strength     = %f\n", population->avgstrength);
  fprintf(rep, "Maximum Strength     = %f\n", population->maxstrength);
  fprintf(rep, "Minimum Strength     = %f\n", population->minstrength);
  fprintf(rep, "Sum of Strength      = %f\n", population->sumstrength);
  fprintf(rep, "No. of Crossings     = %d\n", garec->ncrossover);
  fprintf(rep, "No. of Mutations     = %d\n", garec->nmutation);
  }
}









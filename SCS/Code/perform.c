/**************************************************************/
/* FILENAME : perform.c                                       */
/**************************************************************/

#include "scs.h"


int randomchar(float pgeneral)
{
 /* Set position at random with specified generality probability */

  if (flip(pgeneral))
  return WILDCARD;
  else
  if (flip(0.5))
  return 1;
  else
  return 0;
}

void readcondition(FILE *cfile, trit *c, float pgeneral, int nposition)
{
  /* read a single condition */
  char ch;
  int j;

  for (j = nposition; j >= 1; j--) {

         /* Ignore extraneous characters */
         do {
           ch = fgetc(cfile);
         } while ((ch != '0') && (ch != '1') && 
                  (ch != '#') && (ch != 'R'));

  if (ch == '0') c[j] = 0;
  else
  if (ch == '1') c[j] = 1;
  else
  if (ch == '#') c[j] = WILDCARD;
  else
  if (ch == 'R') c[j] = randomchar(pgeneral);
  }
}


void readclassifier(FILE* cfile, classtype *class1, float pgeneral, int nposition)
{
  /* Read a single classifier */

  char ch;

  readcondition(cfile, class1->c, pgeneral, nposition);
  ch = fgetc(cfile);       /* read ':' and ignore it */
  class1->act = ((int) fgetc(cfile)) - 48;    /* read action */
  fscanf(cfile, "%f", &(class1->strength));
  class1->bid = 0.0;
  class1->ebid = 0.0;
  class1->matchflag = 0;

}



int countspecificity(trit *c, int nposition)
{
  /* count condition specificity */

  int temp = 0;

  while (nposition >= 1) {
  if (c[nposition] != WILDCARD)
  temp += 1;
  nposition -= 1;
  }

  return temp;
}



void initclassifiers( FILE *cfile, poptype *population)
{
  /* initialize classifiers */
  int j;

  fscanf(cfile, "%d\n", &(population->nposition));
  fscanf(cfile, "%d\n", &(population->nclassifier));
  fscanf(cfile, "%f\n", &(population->pgeneral));
  fscanf(cfile, "%f\n", &(population->cbid));
  fscanf(cfile, "%f\n", &(population->bidsigma));
  fscanf(cfile, "%f\n", &(population->bidtax));
  fscanf(cfile, "%f\n", &(population->lifetax));
  fscanf(cfile, "%f\n", &(population->bid1));
  fscanf(cfile, "%f\n", &(population->bid2));
  fscanf(cfile, "%f\n", &(population->ebid1));
  fscanf(cfile, "%f\n", &(population->ebid2));

  for (j=1; j <= population->nclassifier; j++) {

  readclassifier(cfile, &(population->classifier[j]),
   population->pgeneral, population->nposition);

  population->classifier[j].specificity =
   countspecificity( population->classifier[j].c,
                                          population->nposition);
  }



}



void initrepclassifiers(FILE *rep, poptype *population)
{
  /* Initial report on population parameters */

  fprintf(rep, "\n\n");
  fprintf(rep, "Population Parameters\n");
  fprintf(rep, "---------------------\n");
  fprintf(rep, "Number of classifiers   = %d\n", population->nclassifier);
  fprintf(rep, "Number of positions   = %d\n", population->nposition);
  fprintf(rep, "Bid coefficient = %f\n", population->cbid);
  fprintf(rep, "Bid spread = %f\n", population->bidsigma);
  fprintf(rep, "Bidding tax = %f\n", population->bidtax);
  fprintf(rep, "Existence tax = %f\n", population->lifetax);
  fprintf(rep, "Generality probability = %f\n", population->pgeneral);
  fprintf(rep, "Bid specificity base = %f\n", population->bid1);
  fprintf(rep, "Bid specificity mult. = %f\n", population->bid2);
  fprintf(rep, "Ebid specificity base = %f\n", population->ebid1);
  fprintf(rep, "Ebid specificity mult. = %f\n", population->ebid2);
}




void writecondition(FILE *rep, trit *c, int nposition)
{
  /* Convert internal condition format to external format
   and write to file/device */

  int j;

  for (j = nposition; j >= 1; j--) {

  if (c[j] ==  1) fprintf(rep, "1");
  else
  if (c[j] ==  0) fprintf(rep, "0");
  else
  if (c[j] ==  WILDCARD) fprintf(rep, "#");
  }
}


void writeclassifier(FILE *rep, classtype class1, int number, int nposition)
{
  /* write a single classifier */

  fprintf(rep," %5d  %4.5f  %4.5f  %4.5f ", number, class1.strength,
               class1.bid, class1.ebid);

  if (class1.matchflag)
  fprintf(rep, " X ");
  else
  fprintf(rep, "   ");

  writecondition(rep, class1.c, nposition);

  fprintf(rep, ": [ %d ]\n", class1.act);
}


void reportclassifiers(FILE *rep, poptype *population)
{
  /* Generate classifiers report */

  int j;

  fprintf(rep, "\n");
  fprintf(rep, "  No.   Strength       bid   ebid    M   Classifier\n");
  fprintf(rep, "--------------------------------------------------\n\n");

  for (j=1; j <= population->nclassifier; j++)
  writeclassifier(rep, population->classifier[j], j, population->nposition);
}


boolean match(trit *c, bit *m, int nposition)
{
  /* match a single condition to a single message */

  boolean matchtemp;

  matchtemp = TRUE;

  while ((matchtemp == (int) TRUE) && (nposition > 0)) {
  matchtemp = ((c[nposition] == WILDCARD) || (c[nposition] == m[nposition]));
  nposition -= 1;
  }

  return matchtemp;
}



void matchclassifiers(poptype *population, bit *emess, classlist *matchlist)
{
  /* match all classlist against environmental message and create match list */

  int j;

  matchlist->nactive = 0;
  for (j=1; j <= population->nclassifier; j++) {

  population->classifier[j].matchflag = 
                             match(population->classifier[j].c,
                     emess, population->nposition);

  if (population->classifier[j].matchflag) {
  matchlist->nactive = matchlist->nactive + 1;
  matchlist->clist[matchlist->nactive] = j;
  }
  }
}














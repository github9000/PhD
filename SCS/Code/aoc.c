/**************************************************************/
/* FILENAME : aoc.c                                           */
/**************************************************************/

#include "scs.h"


extern FILE *cfile;


void initaoc( crecord *clearingrec )
{
  /* initialize clearinghouse record */
  char ch;

  ch = fgetc(cfile);
  clearingrec->bucketbrigadeflag = ((ch == 'Y') || (ch == 'y')) ? 1 : 0;

  clearingrec->winner = 1;
  clearingrec->oldwinner = 1;
}


void initrepaoc(FILE *rep, crecord *clearingrec )
{
  /* Initial report of clearinghouuse parameters */

  fprintf(rep, "\n\n");
  fprintf(rep, "Apportionment of Credit Parameters\n");
  fprintf(rep, "----------------------------------\n");
  fprintf(rep, "Bucketbrigadeflag   = ");
  if (clearingrec->bucketbrigadeflag)
	 fprintf(rep, " True\n");
  else
    fprintf(rep, " False\n");
}


int auction(poptype *population, classlist *matchlist, int oldwinner)
{
  /* Auction among currently matched classifiers - return winner */

  int j, k, winner;
  float bidmaximum = 0.0;

  winner = oldwinner;    /* if no match, oldwinner wins again */

  if (matchlist->nactive > 0) 
    for (j = 1; j <= matchlist->nactive; j++) {
      k = matchlist->clist[j];
      population->classifier[k].bid = population->cbid * 
                   (population->bid1 + population->bid2 *  
                    population->classifier[k].specificity) *
                    population->classifier[k].strength;

  
      population->classifier[k].ebid = population->cbid * 
                   (population->ebid1 + population->ebid2 *  
                    population->classifier[k].specificity) *
                    population->classifier[k].strength +
                    noise(0.0, population->bidsigma);


      if (population->classifier[k].ebid > bidmaximum) {
         winner = k;
         bidmaximum = population->classifier[k].ebid;
      }
    }

  return winner;
}


void clearinghouse( poptype *population, crecord *clearingrec)
{
  /* Distribute payment from recent winner to old winner */

  float payment;

  payment = population->classifier[clearingrec->winner].bid;
  population->classifier[clearingrec->winner].strength -= payment;
 
  if (clearingrec->bucketbrigadeflag) 
    population->classifier[clearingrec->oldwinner].strength += payment;
}

void taxcollector( poptype *population )
{
  /* Collect existence and bidding taxes from population members */

  int j;
  float bidtaxswitch;

  /* Take life tax from everyone and bidtax from actives */
  if ((population->lifetax != 0) || (population->bidtax != 0))
    for (j=1; j <= population->nclassifier; j++) {
      if (population->classifier[j].matchflag) 
        bidtaxswitch = 1.0;
      else
        bidtaxswitch = 0.0;
      
      population->classifier[j].strength = 
          population->classifier[j].strength - population->lifetax * 
          population->classifier[j].strength - population->bidtax  *
          bidtaxswitch * population->classifier[j].strength; 
    }
}


void reportaoc( FILE *rep, crecord *clearingrec)
{
  /* Report who pays to whom */
  fprintf(rep, "\n\n");
  fprintf(rep, "New winner [%d] : Old winner [%d]\n", 
                           clearingrec->winner, clearingrec->oldwinner); 
}


void aoc (poptype *population, classlist *matchlist, crecord *clearingrec)
{
  /* apportionment of credit coordinator */
  clearingrec->winner = auction(population, matchlist, clearingrec->oldwinner);
  taxcollector(population);
  clearinghouse(population, clearingrec);
}






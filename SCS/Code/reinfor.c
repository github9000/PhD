/**************************************************************/
/* FILENAME : reinfor.c                                       */
/**************************************************************/

#include "scs.h"


void initreinforcement(FILE *rfile, rrecord *reinforcementrec)
{
  /* initialize reinforcement parameters */

  fscanf(rfile, "%f\n", &(reinforcementrec->reward));
  reinforcementrec->rewardcount = 0.0;
  reinforcementrec->rewardcount50 = 0.0;
  reinforcementrec->totalcount = 0.0;
  reinforcementrec->count50 = 0.0;
  reinforcementrec->proportionreward = 0.0;
  reinforcementrec->proportionreward50 = 0.0;
  reinforcementrec->lastwinner = 0;
}


void initrepreinforcement(FILE *rep, rrecord *reinforcementrec)
{
  /* initial reinforcement report */

  fprintf(rep, "\n\n");
  fprintf(rep, "Reinforcement Parameters\n");
  fprintf(rep, "------------------------\n");
  fprintf(rep, "Reinforcement reward  =  %lf\n", reinforcementrec->reward);
}


boolean criterion(rrecord *rrec, erecord *environrec)
{
  /* return TRUE if criterion is achieved */
  boolean tempflag;

  tempflag = (environrec->output == environrec->classifieroutput) ? 1: 0;
  rrec->totalcount += 1;
  rrec->count50 += 1;

  /* increment reward counters */
  if (tempflag) {
	 rrec->rewardcount += 1;
	 rrec->rewardcount50 += 1;
  }

  /* calculate reward proportions: runnng & last 50 */
  rrec->proportionreward = rrec->rewardcount / rrec->totalcount;

  if ((int) (rrec->count50 - 50.0) == 0) {
	 rrec->proportionreward50 = rrec->rewardcount50 / 50.0;
	 rrec->rewardcount50 = 0;
	 rrec->count50 = 0;
  }

  return tempflag;
}


void payreward(poptype *population, rrecord *rrec, crecord *clearingrec)
{
  /* pay reward to appropriate individual */
  population->classifier[clearingrec->winner].strength += rrec->reward;
  rrec->lastwinner = clearingrec->winner;
}


void reportreinforcement(FILE *rep, rrecord *reinforcementrec)
{
  /* report award */

  fprintf(rep, "\n\n");
  fprintf(rep, "Reinforcement Report\n");
  fprintf(rep, "--------------------\n");
  fprintf(rep, "Proportion correct (from start) = %lf\n",
						 reinforcementrec->proportionreward);
  fprintf(rep, "Proportion correct (last 50) = %lf\n",
						 reinforcementrec->proportionreward50);
  fprintf(rep, "Last winning classifier number  = %d\n",
						 reinforcementrec->lastwinner);
}


void reinforcement(rrecord *reinforcementrec, poptype *population,
						 crecord *clearingrec, erecord *environrec)
{
  /* make payment if criterion is satisfied */
  if (criterion(reinforcementrec, environrec))
	 payreward(population, reinforcementrec, clearingrec);
}









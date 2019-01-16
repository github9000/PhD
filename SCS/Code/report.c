/**************************************************************/
/* FILENAME : report.c                                        */
/**************************************************************/

#include "scs.h"


extern trecord *timekeeprec;
extern erecord *environrec;
extern crecord *clearingrec;
extern rrecord *reinforcementrec;
extern drecord *detectrec;
extern grecord *garec;

extern poptype *population;      /* population of classifiers */
extern classlist *matchlist;     /* who matched */
extern bit *envmessage;          /* environmental message */



void reportheader( FILE *rep )
{
  /* send report header to specified file/device */

  page(rep);
  fprintf(rep, "Snapshot report\n");
  fprintf(rep, "---------------\n\n");
}


void report( FILE *rep )
{
  /* report coordination routine */

  reportheader(rep);
  reporttime(rep, timekeeprec);
  reportenvironment(rep, environrec);
  reportdetectors(rep, envmessage, population->nposition);
  reportclassifiers( rep, population );
  reportaoc(rep, clearingrec);
  reportreinforcement(rep, reinforcementrec);
}


void consolereport(rrecord *reinforcementrec)
{
  /* write console report */

  printf("\n\n\n");
  printf("|------------------------|\n");
  printf("     Iteration = %d\n", (int) reinforcementrec->totalcount);
  printf("     P correct = %f\n", reinforcementrec->proportionreward);
  printf("   P50 correct = %f\n", reinforcementrec->proportionreward50);
  printf("|------------------------|\n");
}


void plotreport(FILE *pfile, rrecord *reinforcementrec)
{
  /* write plot report to pfile */
  fprintf(pfile, "%d %f %f\n", (int) reinforcementrec->totalcount,
		 reinforcementrec->proportionreward,
       reinforcementrec->proportionreward50);
}



/**************************************************************/
/* FILENAME : timekeep.c                                      */
/**************************************************************/

#include "scs.h"


const int iterationsperblock = 10000;  /* 10000 iterations per block */



int addtime(int t, int dt, boolean *carryflag)
{
 /* increment iterations counter and set carry flag if necc.. */

 int tempadd;

 tempadd = t + dt;

 *carryflag = (tempadd >= iterationsperblock) ? 1 : 0;

 if (*carryflag == TRUE)
	tempadd %= iterationsperblock;

 return tempadd;
}


void inittimekeeper(FILE *tfile, trecord *timekeeprec)
{
	/* initialize timekeeper */

  boolean dummyflag = 0;

  timekeeprec->iteration = 0;
  timekeeprec->block = 0;
  fscanf(tfile, "%d\n", &(timekeeprec)->initialiteration);
  fscanf(tfile, "%d\n", &(timekeeprec->initialblock));
  fscanf(tfile, "%d\n", &(timekeeprec->reportperiod));
  fscanf(tfile, "%d\n", &(timekeeprec->consolereportperiod));
  fscanf(tfile, "%d\n", &(timekeeprec->plotreportperiod));
  fscanf(tfile, "%d\n", &(timekeeprec->gaperiod));
  timekeeprec->iteration = timekeeprec->initialiteration;
  timekeeprec->block = timekeeprec->initialblock;
  timekeeprec->nextga = addtime(timekeeprec->iteration,
	timekeeprec->gaperiod, &dummyflag);
  timekeeprec->nextreport = addtime(timekeeprec->iteration,
	                            timekeeprec->reportperiod, &dummyflag);
  timekeeprec->nextconsolereport = addtime(timekeeprec->iteration,
				 timekeeprec->consolereportperiod, &dummyflag);
  timekeeprec->nextplotreport = addtime(timekeeprec->iteration,
					  timekeeprec->plotreportperiod, &dummyflag);
}


void initreptimekeeper(FILE *tfile, trecord *timekeeprec)
{
  /* initial timekeeper report */

  fprintf(tfile, "\n\n");
  fprintf(tfile, "Timekeeper Parameters\n");
  fprintf(tfile, "---------------------\n");
  fprintf(tfile, "Initial iteration         = %d\n", timekeeprec->initialiteration);
  fprintf(tfile, "Initial block  = %d\n", timekeeprec->initialblock);
  fprintf(tfile, "Report period = %d\n", timekeeprec->reportperiod);
  fprintf(tfile, "Console report period = %d\n", timekeeprec->consolereportperiod);
  fprintf(tfile, "Plot report period = %d\n", timekeeprec->plotreportperiod);
  fprintf(tfile, "GA period = %d\n", timekeeprec->gaperiod);
}




void timekeeper(trecord *timekeeprec)
{
  /* keep time and set flags for time-driven events */

  boolean carryflag = FALSE, dummyflag = FALSE;

  timekeeprec->iteration = addtime(timekeeprec->iteration, 1, &carryflag);

  if (carryflag == TRUE) timekeeprec->block += 1;

  timekeeprec->reportflag = (timekeeprec->nextreport == timekeeprec->iteration) ? 1 : 0;

  if (timekeeprec->reportflag) 
    timekeeprec->nextreport = addtime(timekeeprec->iteration,
                                     timekeeprec->reportperiod,
                                     &dummyflag);

  timekeeprec->consolereportflag = (timekeeprec->nextconsolereport == 
                                   timekeeprec->iteration) ? 1 : 0;


  if (timekeeprec->consolereportflag) 
    timekeeprec->nextconsolereport = addtime(timekeeprec->iteration,
                                    timekeeprec->consolereportperiod,
                                    &dummyflag);



  timekeeprec->plotreportflag = (timekeeprec->nextplotreport == 
                                   timekeeprec->iteration) ? 1 : 0;


  if (timekeeprec->plotreportflag) 
    timekeeprec->nextplotreport = addtime(timekeeprec->iteration,
                                 timekeeprec->plotreportperiod,
                                 &dummyflag);


  timekeeprec->gaflag = (timekeeprec->nextga == timekeeprec->iteration) ? 1 : 0;


  if (timekeeprec->gaflag) 
    timekeeprec->nextga = addtime(timekeeprec->iteration,
                                 timekeeprec->gaperiod,
                                 &dummyflag);
}



void reporttime(FILE *rep, trecord *timekeeprec)
{
  /* print out block and iteration number */

  fprintf( rep, "[ Block : Iteration] = [ %d : %d ]\n", 
                  timekeeprec->block, timekeeprec->iteration);
}






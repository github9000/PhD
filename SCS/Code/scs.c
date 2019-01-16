/**************************************************************/
/* FILENAME : scs.c                                           */
/**************************************************************/


#include "scs.h"


poptype *population;          /* population of classifiers */
classlist *matchlist;         /* who matched */
bit *envmessage;              /* environmental message */

char fn[80];

FILE *rep;
FILE *cfile;
FILE *efile;
FILE *rfile;
FILE *tfile;
FILE *gfile;
FILE *pfile;


trecord *timekeeprec;
erecord *environrec;
crecord *clearingrec;
rrecord *reinforcementrec;
drecord *detectrec;
grecord *garec;


int main()
{
        int count = 0, gens = 0;

        while (gens < 1) {
          printf("How many generations ? : ");
          scanf("%d", &gens);
        }

        initialization();

        detectors(environrec, detectrec, envmessage);
        report(rep);

        do { 
           do {
     
              while (gens < 1) {
                printf("How many generations ? : ");
                scanf("%d", &gens);
              }
   
              timekeeper(timekeeprec);
              environment(environrec);
              detectors(environrec, detectrec, envmessage);
              matchclassifiers(population, envmessage, matchlist);
              aoc(population, matchlist, clearingrec);
              effector(population, clearingrec, environrec);
              reinforcement(reinforcementrec, population, clearingrec, environrec);
              if (timekeeprec->reportflag)
                report(rep);
   
              if (timekeeprec->consolereportflag)
                consolereport(reinforcementrec);
   
              if (timekeeprec->plotreportflag)
                plotreport(pfile, reinforcementrec);

              advance(clearingrec);
 
              if (timekeeprec->gaflag) {
                ga(garec, population);
                if (timekeeprec->reportflag)  {
                  reportga(rep, garec, population);
                }
              }
              count += 1;

           } while (count < gens);

           count = gens = 0;

        } while (!halt());


        report(rep);   /* final report */


        /* close files and free memory */
        fclose(rep); fclose(pfile);
        fclose(cfile); fclose(efile);
        fclose(rfile); fclose(tfile);
        fclose(gfile);

        free(population);  free(matchlist);    
        free(envmessage);  free(timekeeprec); 
        free(environrec); free(clearingrec);
        free(reinforcementrec); free(detectrec); 
        free(garec);

        return 0;
}

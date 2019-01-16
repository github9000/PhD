/**************************************************************/
/* FILENAME : effector.c                                      */
/**************************************************************/

#include "scs.h"


void effector(poptype *population, crecord *clearingrec, 
                                  erecord *environrec)
{
  /* Set action in object as dictated by auction winner */
  environrec->classifieroutput = 
     population->classifier[clearingrec->winner].act ; 
}

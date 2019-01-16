/**************************************************************/
/* FILENAME : advance.c                                       */
/**************************************************************/


#include "scs.h"

void advance(crecord *clearingrec)
{
  /* advance winner */

  clearingrec->oldwinner = clearingrec->winner ; 
}


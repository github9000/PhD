/**************************************************************/
/* FILENAME : detector.c                                      */
/**************************************************************/


#include "scs.h"


void detectors( erecord *environrec, drecord *detectrec,
                bit *envmessage)
{
/* convert environmental state to env. message */
  int j;
  /* place signal message in env. message */
 

  for (j = 1; j <= environrec->lsignal; j++)  
    envmessage[j] = environrec->signal[j];

  
/*   envmessage = environrec->signal;  */
}



void writemessage(FILE *outfp, bit *mess, int lmessage)
{
  /** write a message in bit reverse order */
  int j;

  for (j = lmessage; j >= 1; j--)
	 fprintf(outfp, "%d", mess[j]);

}


void reportdetectors(FILE *outfp, bit *envmessage, int nposition)
{

  /* write out environmental message */

  fprintf(outfp, "\n");
  fprintf(outfp, "Environmental message:   ");
  writemessage(outfp, envmessage, nposition);
  fprintf(outfp, "\n");

}



void initdetectors( FILE *efile, drecord *detectrec)
{
  /* dummy detector initialization */
}


void initrepdetectors( FILE *rep, drecord *detectrec)
{
  /* dummy initial detectors report */
}









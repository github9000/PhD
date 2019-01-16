/**************************************************************/
/* FILENAME : environ.c                                       */
/**************************************************************/

#include "scs.h"


void generatesignal( erecord *environrec )
{
  /* generate random signal */
  int j;

  for (j=1 ; j <= environrec->lsignal; j++) 
    if (flip(0.5))
      environrec->signal[j] = 1;
    else
      environrec->signal[j] = 0;
}


int decode(bit *mess, int start, int length)
{
  /* decode substring as unsigned binary integer */

  int j, accum = 0, powerof2 = 1;

  for (j=start; j <= start+length-1; j++) {
    accum += powerof2 * mess[j];
    powerof2 *= 2;
  }

  return accum;
}

 
void multiplexoroutput(erecord *environrec)
{
  /* calculate correct multiplexor output */

  /* decode the address */
  environrec->address = decode(environrec->signal, 1, environrec->laddress);
  
  /* set the output */
  environrec->output = environrec->signal[environrec->laddress +
                                        environrec->address + 1]; 
}



void environment(erecord *environrec)
{
  /* coordinate multiplexor calculations */

  generatesignal(environrec);
  multiplexoroutput(environrec);
}


void initenvironment( FILE *efile, erecord *environrec)
{
  /* Initialize multiplexor environment */
  int j;

  /* Read number of address lines */
  fscanf(efile, "%d", &(environrec->laddress));

  /* Calculate number of data lines */
  environrec->ldata = (int) poweri(2.0, environrec->laddress);

  /* Calculate length of signal */
  environrec->lsignal = environrec->laddress + environrec->ldata;

  environrec->address = 0;
  environrec->output = 0;
  environrec->classifieroutput = 0;

  for (j = 1; j <= environrec->lsignal; j++) 
    environrec->signal[j] = 0;

}


void initrepenvironment( FILE *rep, erecord *environrec)
{
  /* Write initial environmental report */

  fprintf(rep, "\n\n");
  fprintf(rep, "Environmental Parameters (Multiplexor)\n");
  fprintf(rep, "--------------------------------------\n");
  fprintf(rep, "Number of Address Lines = %d\n", environrec->laddress);
  fprintf(rep, "Number of Data Lines = %d\n", environrec->ldata);
  fprintf(rep, "Total Number of Lines = %d\n", environrec->lsignal);
}



void writesignal(FILE* rep, bit *signal, int lsignal)
{
  /* write a signal in bit-reverse order */
  int j;

  for (j = lsignal; j >= 1; j--) 
    fprintf(rep, " %d ", signal[j]);
}


void reportenvironment(FILE *rep, erecord *environrec)
{
  /* write current multiplexor info */
  
  fprintf(rep, "\n\n"); 
  fprintf(rep, "Current Multiplexor Status\n");
  fprintf(rep, "--------------------------\n");
  fprintf(rep, "Signal                = ");
  writesignal(rep, environrec->signal, environrec->lsignal);
  fprintf(rep, "\n"); 
  fprintf(rep, "Decoded address       = %d\n", environrec->address);
  fprintf(rep, "Multiplexor output    = %d\n", environrec->output);
  fprintf(rep, "Classifier output     = %d\n", environrec->classifieroutput);
}







/**************************************************************/
/* FILENAME : initial.c                                       */
/**************************************************************/

#include "scs.h"


char ch;

extern trecord *timekeeprec;
extern erecord *environrec;
extern crecord *clearingrec;
extern rrecord *reinforcementrec;
extern drecord *detectrec;
extern grecord *garec;



extern FILE *rep;
extern FILE *cfile;
extern FILE *efile;
extern FILE *rfile;
extern FILE *tfile;
extern FILE *gfile;
extern FILE *pfile;


extern poptype *population;      /* population of classifiers */
extern classlist *matchlist;     /* who matched */
extern bit *envmessage;          /* environmental message */




void initrepheader(FILE *outfp)
{
 /* write a header to a specified file/dev */

  void repchar(), skip();
  int iskip;
  int ll = 59;

  iskip = (LINELENGTH - ll)/2;
  skip(outfp,1);
  repchar(outfp," ",iskip); repchar(outfp,"-",ll); skip(outfp,1);
  repchar(outfp," ",iskip);
  fprintf(outfp,"|       SCS-C (v1.0) - A Simple Classifier System         |\n");
  repchar(outfp," ",iskip);
  fprintf(outfp,"|    (c) David E. Goldberg 1987, All Rights Reserved      |\n");
  repchar(outfp," ",iskip);
  fprintf(outfp,"|    C version by Kenneth P. Williams, U. of Reading      |\n");
  repchar(outfp," ",iskip); repchar(outfp,"-",ll); skip(outfp,2);
}

void interactiveheader()
/** clear screen and print interactive header **/
{
  printf("\n\n\n");
  initrepheader(stdout);
}



/* Safe Memory allocation function */
/* Rather than have a test for a null pointer each time we call "malloc"
	we write our own safe version, "safe_malloc()". If memory runs out
	there is no more we can do, and so the routine aborts the whole
	program.  */
void *safe_malloc( int n )
{
	void *t = malloc( n );

	/* Check for success or failure */

	if (t == NULL)
	{
		printf("\n malloc() failed\n");
		exit( 0 );
	}

	return t;
}


void initialization()
{  /* coordinate input and initialization */
  int i;

  /* random number and normal init */
/*   interactiveheader(); */


  population = (poptype *) safe_malloc (sizeof(poptype));
  timekeeprec = (trecord *) safe_malloc (sizeof(trecord));
  environrec = (erecord *) safe_malloc (sizeof(erecord));
  clearingrec = (crecord *) safe_malloc (sizeof(crecord));
  reinforcementrec = (rrecord *) safe_malloc (sizeof(rrecord));
  detectrec = (drecord *) safe_malloc (sizeof(drecord));
  garec = (grecord *) safe_malloc (sizeof(grecord));
  matchlist = (classlist *) safe_malloc (sizeof(classlist));
  i = sizeof(bit);
  envmessage = (bit *) safe_malloc (i * MAXPOSITION); 

  randomize();
  initrandomnormaldeviate();

  /* file/dev init */
/*
  open_input(cfile, interactive, "   classifier    ", fn);
  open_input(efile, interactive, "   environment   ", fn);
  open_input(rfile, interactive, "   reinforcement ", fn);
  open_input(tfile, interactive, "   timekeeper    ", fn);
  open_input(gfile, interactive, " gen. algorithm  ", fn);
  open_output(rep, interactive,  "     report      ", fn);
  open_output(pfile, interactive,"     plot file   ", fn);
*/
  cfile = (FILE *) open_input(batch, "   classifier    ", "perfect.dta");
  efile = (FILE *) open_input( batch, "   environment   ", "environ.dta");
  rfile = (FILE *) open_input( batch, "   reinforcement ", "reinf.dta");
  tfile = (FILE *) open_input( batch, "   timekeeper    ", "time.dta");
  gfile = (FILE *) open_input( batch, " gen. algorithm  ", "ga.dta");
  rep = (FILE *) open_output( batch,  "     report      ", "rep.out");
  pfile = (FILE *) open_output( batch,"     plot file   ", "plot.out");

  initrepheader(rep);
  initclassifiers(cfile, population);
  initrepclassifiers(rep, population);
  initenvironment(efile, environrec);
  initrepenvironment(rep, environrec);
  initdetectors(efile, detectrec);
  initrepdetectors(rep, detectrec);
  initaoc(clearingrec);
  initrepaoc(rep, clearingrec);
  initreinforcement(rfile, reinforcementrec);
  initrepreinforcement(rep, reinforcementrec);
  inittimekeeper(tfile, timekeeprec);
  initreptimekeeper(rep, timekeeprec);
  initga(gfile, garec, population);
  initrepga(rep, garec);
}



void repchar (FILE *outfp, char *ch, int repcount)
/* Repeatedly write a character to stdout */
{
	 int j;

	 for (j = 1; j <= repcount; j++) fprintf(outfp,"%s", ch);
}


void skip(FILE *outfp, int skipcount)
/* Skip skipcount lines */
{
    int j;
    for (j = 1; j <= skipcount; j++) fprintf(outfp,"\n");
}







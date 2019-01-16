/*----------------------------------------------------------------------------*/
/* sccs.h - global declarations for main().                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <malloc.h>

#define MAXPOSITION      51
#define MAXCLASS        101
#define WILDCARD         -1
#define TRUE              1
#define FALSE             0
#define LINELENGTH       80                       /* width of printout */

#define interactive 0
#define batch 1


typedef short boolean;
typedef boolean bit;
typedef short trit;
typedef bit action;
typedef int query_type;



typedef struct CLASSTYPE {
	trit c [MAXPOSITION];
	action act;
	float strength, bid, ebid;
	boolean matchflag;
	int specificity;
} classtype;


typedef struct CLASSLIST {
  int clist [MAXCLASS];
  int nactive;
} classlist;

typedef struct POPTYPE {
  classtype classifier[MAXCLASS];
  int nclassifier, nposition;
  float pgeneral, cbid, bidsigma, bidtax, lifetax,
		  bid1, bid2, ebid1, ebid2,
		  sumstrength, maxstrength, avgstrength, minstrength;
} poptype;





/* Detector Record structure */
typedef struct DRECORD {
  int dummy_val;
} drecord;
		/* For this problem no detector record is
		 required. Normally, the detector record
		 contains information for mapping
		 environmental state variables to the
		 environmental bit-string.
								 */


/* Environment Record structure */
typedef struct ERECORD {
  int laddress, ldata, lsignal, address, output, classifieroutput;
  bit signal[MAXPOSITION];
} erecord;


/* Timekeeper Record structure */

typedef struct TRECORD {
  int initialiteration, initialblock, iteration, block,
		reportperiod, gaperiod, consolereportperiod,
		plotreportperiod, nextplotreport, nextconsolereport,
		nextreport, nextga;
  boolean reportflag, gaflag, consolereportflag, plotreportflag;
} trecord;



/* Credit apportionment record structure */

typedef struct CRECORD {
  int winner, oldwinner;
  boolean bucketbrigadeflag;
} crecord;



/* Reinforcement record structure */

typedef struct RRECORD {
  float reward, rewardcount, totalcount, count50,
		  rewardcount50, proportionreward, proportionreward50;
  int lastwinner;
} rrecord;



#define maxmating  10


/* GA Code structure */

typedef struct MRECORD {
  int mate1, mate2, mort1, mort2, sitecross;
} mrecord;


typedef struct GRECORD {
  float proportionselect, pmutation, pcrossover;
  int ncrossover, nmutation, crowdingfactor, crowdingsubpop, nselect;
  mrecord mating[maxmating];      /* Mating records for GA report */
} grecord;


/* Function Prototypes */
 
/* advance.c */
void advance(crecord *clearingrec);
 
/* aoc.c */
void initaoc( crecord *clearingrec );
void initrepaoc(FILE *rep, crecord *clearingrec );
int auction(poptype *population, classlist *matchlist, int oldwinner);
void clearinghouse( poptype *population, crecord *clearingrec);
void taxcollector( poptype *population );
void reportaoc( FILE *rep, crecord *clearingrec);
void aoc (poptype *population, classlist *matchlist, crecord *clearingrec);
 
/* detector.c */
void detectors( erecord *environrec, drecord *detectrec, bit *envmessage);
void writemessage(FILE *outfp, bit *mess, int lmessage);
void reportdetectors(FILE *outfp, bit *envmessage, int nposition);
void initdetectors( FILE *efile, drecord *detectrec);
void initrepdetectors( FILE *rep, drecord *detectrec);

/* effector.c */
void effector(poptype *population, crecord *clearingrec,
                                  erecord *environrec);
 
/* environ.c */
void generatesignal( erecord *environrec );
int decode(bit *mess, int start, int length);
void multiplexoroutput(erecord *environrec);
void environment(erecord *environrec);
void initenvironment( FILE *efile, erecord *environrec);
void initrepenvironment( FILE *rep, erecord *environrec);
void writesignal(FILE* rep, bit *signal, int lsignal);
void reportenvironment(FILE *rep, erecord *environrec);
 
/* ga.c */
void initga(FILE *gfile, grecord *garec, poptype *population);
void initrepga( FILE *rep, grecord *garec );
int select(poptype *population);
trit mutation(trit positionvalue, float pmutation, int *nmutation);
bit bmutation(bit positionvalue, float pmutation, int *nmutation);
void crossover( classtype parent1, classtype parent2,
					 classtype *child1, classtype *child2,
					 float pcrossover, float pmutation,
					 int *sitecross, int *nposition,
					 int *ncrossover, int *nmutation);
int worstofn(poptype *population, int n);
int matchcount(classtype classifier1, classtype classifier2,
					int nposition);
int crowding (classtype *child, poptype *population,
				  int crowdingfactor, int crowdingsubpop);
void statistics(poptype *population);
void ga(grecord *garec, poptype *population);
void reportga(FILE *rep, grecord *garec, poptype *population);

/* initial.c */
void initrepheader(FILE *outfp);
void interactiveheader();
void *safe_malloc( int n );
void initialization(void);
void repchar (FILE *outfp,char *ch, int repcount);
void skip(FILE *outfp, int skipcount);

/* io.c */
void page(FILE* out);
FILE* open_input(query_type query_flag, char* message,
					 char* filename);
FILE* open_output(query_type query_flag, char* message,
					 char*  filename);


/* perform.c */
int randomchar(float pgeneral);
void readcondition(FILE *cfile, trit *c, float pgeneral,
						 int nposition);
void readclassifier(FILE* cfile, classtype *class1,
			  float pgeneral, int nposition);
int countspecificity(trit *c, int nposition);
void initclassifiers( FILE *cfile, poptype *population);
void initrepclassifiers(FILE *rep, poptype *population);
void writecondition(FILE *rep, trit *c, int nposition);
void writeclassifier(FILE *rep, classtype class1,
                     int number, int nposition);
void reportclassifiers(FILE *rep, poptype *population);
boolean match(trit *c, bit *m, int nposition);
void matchclassifiers(poptype *population, bit *emess,
                      classlist *matchlist);
 
/* reinfor.c */
void initreinforcement(FILE *rfile, rrecord *reinforcementrec);
void initrepreinforcement(FILE *rep, rrecord *reinforcementrec);
boolean criterion(rrecord *rrec, erecord *environrec);
void payreward(poptype *population, rrecord *rrec,
					crecord *clearingrec);
void reportreinforcement(FILE *rep, rrecord *reinforcementrec);
void reinforcement(rrecord *reinforcementrec,
                   poptype *population,
                   crecord *clearingrec, erecord *environrec);
 
/* report.c */
void reportheader( FILE *rep );
void report( FILE *rep );
void consolereport(rrecord *reinforcementrec);
void plotreport(FILE *pfile, rrecord *reinforcementrec);
 
/* timekeep.c */
int addtime(int t, int dt, boolean *carryflag);
void inittimekeeper(FILE *tfile, trecord *timekeeprec);
void initreptimekeeper(FILE *tfile, trecord *timekeeprec);
void timekeeper(trecord *timekeeprec);
void reporttime(FILE *rep, trecord *timekeeprec);
 
 
/* utility.c */
float poweri(float x, int i);
float max(float x, float y);
float min(float x, float y);
float avg(float x, float y);
boolean halt();
void advance_random();
int flip(float prob);
void initrandomnormaldeviate();
double noise(double mu, double sigma);
void randomize();
double randomnormaldeviate();
float randomperc();
int rnd(int low, int high);
float rndreal(float lo , float hi);
void warmup_random(float random_seed);

 
 




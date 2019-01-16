/**************************************************************/
/* FILENAME : io.c                                            */
/**************************************************************/

#include "scs.h"


query_type qflag;


void page(FILE* out)
{
  fprintf(out, "\n");
}


FILE* open_input(query_type query_flag, char* message,
                char* filename)
{
  FILE *input;

  if (query_flag == (int) batch) {
     if((input = fopen(filename,"r")) == NULL) {
         fprintf(stderr,"Input file %s not found\n",filename);
         exit(-1);
     }
  }
  else {
    printf("Enter %s filename: ", message);
    scanf("%s", &filename[0]);
    if((input = fopen(filename,"r")) == NULL) {
        fprintf(stderr,"Input file %s not found\n",filename);
        exit(-1);
    }
  }
  return input;
}


FILE* open_output(query_type query_flag, char* message,
                char*  filename)
{
  FILE *output;

  if (query_flag == (int) batch) {
     if((output = fopen(filename,"w")) == NULL) {
         fprintf(stderr,"Output file %s not found\n",filename);
         exit(-1);
     }
  }
  else {
    printf("Enter %s filename: ", message);
    scanf("%s", &filename[0]);
    if((output = fopen(filename,"w")) == NULL) {
        fprintf(stderr,"Output file %s not found\n",filename);
        exit(-1);
    }
  }
  return output;
}





 

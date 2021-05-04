#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "mymalloc.h"

float repeat = 0.675;
char cmds[] = "aaaaaffff";
char tokens []= "abcdef";
//char tokens []= "ab";

int verbose = 0;

#define RAND_BYTES (((rand()%15)+1) * ((rand()%15)+1))


int main(int argc, char **argv){

  char flag_tag = 'A';

  int c;
  while ((c = getopt (argc, argv, "v")) != -1){
    switch (c){
      case 'v':
	verbose = 1;
        break;
    }
  }

  srand( getpid());
  void *BlockListp = init();

  int words = atoi(argv[optind]);
  int req_level = atoi(argv[optind+1]);

  if ( req_level < 2){
    req_level = (int)(strchr(cmds, 'f') - cmds);
  }else {
    req_level = strlen(cmds);
  }

  if (verbose){
    printf("Input:  words:%d, req_level:%d\n",words, req_level);
  }
     
  char prev_cmd[25];

  int i=0;
  int bytes;
  void *bp;

  
  while ( i < words ){

    if (i < 2 ){
      i += 1;
      bytes = RAND_BYTES;
      printf ("a%d\n",bytes );
      bp = find(BlockListp, bytes);
      if ( bp ){
        place(bp, bytes);
      }else{
        printf("Unable to find %d free bytes in block\n", bytes);
      }
    }else{
      char cmd = cmds[rand() % req_level];
      char *map ;
      int c, count;
      int ith, loc;
      switch (cmd) {
        case 'a':
          i += 1;
          bytes = RAND_BYTES;
          printf ("a%d\n",bytes);
	  bp = find(BlockListp, bytes);
          if ( bp ){
              place(bp, bytes);
          }else{
            printf("Unable to find %d free bytes in block\n", bytes);
          }
 	  break;
		
        case 'f':

	  map = block_map(BlockListp);
          count = 0;
          c = 0;
          while (map[c] != '\0') {
            if ( map[c] == '1' ) count++; // count number of allocated blocks.
            c++;
          }
          if (count == 0) break; // Nothing to do here.

	  ith = (rand() % count)+1; // pick one of those at random

          //printf ("count %d -%s- \n",count, map);
          for ( loc = c = 0; c < ith; loc++ ){
            if (map[loc] == '1') c++;
          }

          printf("f%d-(%s-count=%d, ith=%d loc%d)\n",loc, map, count, ith, loc);
          free(BlockListp,loc);
 	  break;
      }
    }
  }
  print_block(BlockListp);
  
}


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "mymalloc.h"

using namespace std;

int verbose = 0;

int main(){
   
  void *BlockListp = init(); 
  
  char action;
  int arg;
  while ( scanf("%c", &action) > 0 ){
    void *bp;
    switch(action){
      case 'a':
        scanf("%d",  &arg);
        bp = find(BlockListp, arg);
        place(bp, arg);
        break;
      case 'f':
        scanf("%d",  &arg);
        free(BlockListp, arg);
	break;
      case 'i':
        print_block(BlockListp);
    }
  }

  return 0;
}



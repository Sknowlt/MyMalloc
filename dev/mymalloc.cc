#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

/* each block will have a size  */

#define GET(p)       (*(int *)(p))
#define PUT(p, val)  (*(int *)(p) = (val)) 

#define get_flag(hdr) (GET(hdr) & 1)
#define get_size(hdr) (GET(hdr) & ~07)
#define next_bp(p) (void *)((long)p + get_size(p))
#define pack(size,flag) (size|flag)



// Spaces need to be multiples of 8 (0x7) 
// We also need to store a four char heaDER
// REQ(1) = 8 (4+1+3U)
// REQ(4) = 8( 4+4)
// REQ(5) = 16 (4+5+7U)
// REQ(8) = 16 (4+8+4U)
// REQ(12) = 16 (4+12)
// REQ(13) = 24 (4+13+7U)

extern int verbose;


void *init(){
  
  int  blocks = 2;
  int blocksize = 1024 * blocks;
  
  void *bp = (void*)malloc((sizeof(char) * blocksize) + 4); // size + prologue.
  if (verbose) printf("Ptr to Row Super Block  = %p (calc size: %d)\n", bp, blocksize);

  // Wrirting Header block.
  int *header;
  header = (int *)bp;
  *header = (blocksize | 0) ;
 
  // Need to write explog block.

  int *epilog = (int *)next_bp(bp);
  *epilog = 0 ;
  
  return bp;
}


void *find(void *bp, int size){
 
  int asize; // allocated size. 
  asize = size + 4; // Add the record size.
  asize  += (asize % 8) ? 8-(asize%8) : 0; // Round up to next multiple of 8.

  while ( get_size(bp) != 0 ){
     if ( get_flag(bp) == 0 ){
       int csize = get_size(bp);
        if (verbose) printf("Found a free block size %d need %d\n", csize, asize);
       if (csize >= asize){
	 if (verbose) printf("Found a free block of size %d to house new block of size %d\n", get_size(bp), asize);
         return bp;
       }
     }
     bp = next_bp(bp);
  }
  return NULL;

}


void free(void *bp, int item){
   int loc = 0;
   while (item > ++loc ){
    bp = next_bp(bp);
   }
   int csize = get_size(bp);
   PUT(bp, pack(csize, 0));
}


 
void place(void *bp, int size){

   int csize;
   csize = get_size(bp);

   int asize; // allocated size. 
   asize = size + 4; // Add the record size.
   asize  += (asize % 8) ? 8-(asize%8) : 0; // Round up to next multiple of 8.

   if (verbose) printf("Old Block %p, old size %d - setting new size to %d\n", bp, csize, asize);
   PUT(bp, pack(asize,1));
   if (csize > asize){
     bp = next_bp(bp);
     csize -= asize;
     if (verbose) printf("New Block %p, New size %d\n", bp, csize);
     PUT(bp, pack(csize,0));
   }
}


void print_block(void *bp){  
  int i = 0;
  while ( get_size(bp) != 0 ){
    printf("Block %d: Address:%p, Size:%d(0x%x), Flag:%c\n", ++i, bp, get_size(bp), get_size(bp), (get_flag(bp) ? 'a' : 'f')  );
    bp = next_bp(bp);
  }
}



char *block_map(void *bp){
  int bcount = 0;
  void *tbp = bp;

  while ( get_size(tbp) != 0 ){
    tbp = next_bp(tbp);
    ++bcount;
  } 
  //printf( "Past block 1 ================= count = %d\n", bcount);

  char *map = (char *)malloc(sizeof(char) * (bcount + 1));

  bcount = 0;
  tbp = bp;
  while ( get_size(tbp) != 0 ){
    map[bcount] = ( get_flag(tbp) ? '1' : '0');
    tbp = next_bp(tbp);
    ++bcount;
  }
  //printf("Past block 2 ================= count=%d\n", bcount);
  map[bcount] = '\0';
  return map;
}





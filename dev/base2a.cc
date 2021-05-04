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

void initialize_block(){
/* going to setup a  */
}



// Spaces need to be multiples of 8 (0x7) 
// We also need to store a four char heaDER
// REQ(1) = 8 (4+1+3U)
// REQ(4) = 8( 4+4)
// REQ(5) = 16 (4+5+7U)
// REQ(8) = 16 (4+8+4U)
// REQ(12) = 16 (4+12)
// REQ(13) = 24 (4+13+7U)


void *find(void *bp, int size){
 
  int asize; // allocated size. 
  asize = size + 4; // Add the record size.
  asize  += (asize % 8) ? 8-(asize%8) : 0; // Round up to next multiple of 8.

  while ( get_size(bp) != 0 ){
     if ( get_flag(bp) == 0 ){
       if (get_size(bp) > asize){
	 printf("Found a free block of size %d to house new block of size %d\n", get_size(bp), asize);
         return bp;
       }
     }
     bp = next_bp(bp);
  }
  return NULL;
}


 
void place(void *bp, int size){

   int csize;
   csize = get_size(bp);

   int asize; // allocated size. 
   asize = size + 4; // Add the record size.
   asize  += (asize % 8) ? 8-(asize%8) : 0; // Round up to next multiple of 8.

   printf("Old Block %p, old size %d - setting new size to %d\n", bp, csize, asize);
   PUT(bp, pack(asize,1));
   bp = next_bp(bp);
   csize -= asize;
   printf("New Block %p, New size %d\n", bp, csize);
   PUT(bp, pack(csize,0));
    
}


int main(){
   
  int  blocks = 2;
  int blocksize = 1024 * blocks;
   
  // Getting an extra four blocks for the epilog
  void *BlockListp; 
  BlockListp = (void*)malloc((sizeof(char) * blocksize)+4);
 
  printf("Ptr to Row Super Block  = %p (calc size: %d)\n", BlockListp, blocksize);

  // Wrirting Header block.
  int *header;
  header = (int *)BlockListp;
  *header = (blocksize | 0) ;
  
  // Need to write explog block.

  //int *nextbp = (int *)(long(BlockListp) + *(int *)BlockListp); 
  //int *nextbp = (int *)(long(BlockListp) + get_size(BlockListp)); 
  int *epilog = (int *)next_bp(BlockListp); 
  *epilog = 0 ;
  

  int size;
  while ( scanf("%d", &size) > 0 ){
      void *bp = find(BlockListp, size);
      place(bp, size);
  }

  
  void *p = BlockListp;
  while ( get_size(p) != 0 ){
    //int y = (*(int*)BlockListp);;
    printf("Ptr to Next Block: Curr:%p, size:%d(0x%x), next:%p \n", p, get_size(p), get_size(p), next_bp(p) );
    p = next_bp(p);
    
  }
   
  return 0;

}


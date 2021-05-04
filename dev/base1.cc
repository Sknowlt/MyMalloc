#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;



void initialize_block(){
}


int main(){
   
  int  blocks = 1;
  int blocksize = 1026 * blocks;
   
  void *ptr ;
  int *intp ;
  char *charp ;
  ptr = malloc(blocksize * blocks);

 
  printf ("sizeof char %li \n",sizeof(char));
  printf ("sizeof int %li \n",sizeof(int));
  printf ("sizeof ptr %li \n",sizeof(void *));

  charp = (char *)ptr;
  printf("charp: %p\n", charp);;
  charp++;
  printf("charp: %p\n", charp);;

  intp = (int *)charp;
  printf("intp: %p\n", intp);;
  intp++;
  *intp = 10;
  
  printf("intp: %p %d \n", intp, *intp);;

  //ptr += sizeof(char);

   
  //*intp = 10;

  return 0;




}

   

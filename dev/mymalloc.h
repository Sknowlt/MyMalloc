void *init();
void *find(void *bp, int size);
void free(void *bp, int item);
void place(void *bp, int size);
void print_block(void *bp);
char* block_map(void *bp);


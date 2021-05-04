all: mymalloc genwords

mymalloc: mymalloc.cc mymalloc.h base2b.cc
	g++ -o mymalloc mymalloc.cc base2b.cc

genwords: genwords.cc mymalloc.cc mymalloc.h
	g++ -g -o genwords genwords.cc mymalloc.cc

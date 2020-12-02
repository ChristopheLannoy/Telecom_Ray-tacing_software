LIB = -lm -lblas -llapack -lSDL2 -lSDL2main #-lSDL_mixer -lgfortran
COPT = -O3

default: main

clean: 
	rm *.o 
	rm *.txt 
	rm main



main: main.c powerCalculations.o map.o geometry.o plot.o 
	gcc $(COPT) -std=c99 $^ -o $@ $(LIB)

%.o: %.c
	gcc $(COPT) -std=c99 -c $< -o $@ $(LIB)

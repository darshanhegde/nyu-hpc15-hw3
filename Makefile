EXECS=jacobi-omp.o gs-omp.o omp_solved*.o
OMPCC=gcc -fopenmp

all: ${EXECS}

jacobi-omp.o: jacobi-omp.c
	${OMPCC} -o jacobi-omp.o jacobi-omp.c
	
gs-omp.o: gs-omp.c
	${OMPCC} -o gs-omp.o gs-omp.c

omp_solved*.o: omp_solved*.c
	${OMPCC} -o omp_solved2.o omp_solved2.c
	${OMPCC} -o omp_solved3.o omp_solved3.c
	${OMPCC} -o omp_solved4.o omp_solved4.c
	${OMPCC} -o omp_solved5.o omp_solved5.c
	${OMPCC} -o omp_solved6.o omp_solved6.c

clean:
	rm -f ${EXECS}

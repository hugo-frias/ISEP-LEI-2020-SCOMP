# A Makefile with variables and suffix rules
# variaveis

SOURCES = ex08.c
OBJFILES = ex08.o
EXEC = prog_m04_ex08

# Suffix rules
.SUFFIXES : .c .o
.SUFFIXES : .o .s

# How to build an object .o from a code file .c ; $< -- file name
.c.o:

	gcc -Wall -g -c $<
	

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt -lpthread

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
# A Makefile with variables and suffix rules
# variaveis

#INCLUDES = count_even.h
SOURCES = ex04.c
OBJFILES = ex04.o
EXEC = prog_m04_ex04

# Suffix rules
.SUFFIXES : .c .o
.SUFFIXES : .o .s

# How to build an object .o from a code file .c ; $< -- file name
.c.o:

	gcc -Wall -g -c $<
	

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt -lpthread

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}

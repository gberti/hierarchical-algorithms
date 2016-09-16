

export LANG=C
DEBUG=debug
CCFLAGS = -Wall -fdiagnostics-color=never
OPTFLAGS_BASE = -O3 --fast-math
OPTFLAGS_VEC  = $(OPTFLAGS_BASE) -ftree-vectorize -fopt-info-vec-missed
#OPTFLAGS_VEC  = $(OPTFLAGS_BASE) -ftree-vectorize -fopt-info-optimized
OPTFLAGS_sse= -mtune=core2 -msse2  #-std=c99
OPTFLAGS_avx= -march=corei7-avx -mtune=corei7-avx
OPTFLAGS = $(OPTFLAGS_VEC) $(OPTFLAGS_$(VEC))

ASFLAGS = -Wa,-adhln=
GENFLAGS = -fverbose-asm -masm=intel  -ftree-vectorizer-verbose=3
CC  = gcc
CCC = g++
CCCFLAGS = -std=c++11 

HIER_ROOT = ..
IFLAGS = -I$(HIER_ROOT) -I$(HIER_ROOT)/hier-algorithms -I$(HIER_ROOT)/iterators
CCCFLAGS += $(IFLAGS)

ifeq ($(DEBUG), debug)
GENFLAGS = -g
OPTFLAGS = 
endif


# SRC =  test-segmented-iterator.cpp  
SRC = $(shell ls *.cpp)

EXEC = $(SRC:%.cpp=%.exe)

first: all

all: exec

exec: $(EXEC)

.SUFFIXES: .c .cpp .s .exe .o


.cpp.exe:
	$(CCC) $(CCFLAGS) $(CCCFLAGS)  $(OPTFLAGS) $<  -o $@


.c.s:
	$(CC)  -c  $(CCFLAGS) $(OPTFLAGS) $(ASFLAGS)$@.$(VEC) $(GENFLAGS) $<

.cpp.s:
	$(CCC) -c  $(CCFLAGS) $(CCCFLAGS) $(OPTFLAGS) $(ASFLAGS)$@.$(VEC) $(GENFLAGS) $<

.cpp.o:
	$(CCC) -c -v $(CCFLAGS) $(CCCFLAGS) $(OPTFLAGS) $<

#	$(CC) -S -c $< -o $@ 



clean:
	rm -f *.o *~ *.exe *.s.sse *.s.avx *.stackdump

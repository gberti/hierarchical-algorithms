

DEBUG=debug
VEC=sse# sse, avx
CCFLAGS = -Wall 
OPTFLAGS_BASE = -O3 --fast-math
OPTFLAGS_VEC  = $(OPTFLAGS_BASE) -ftree-vectorize -fopt-info-vec
#OPTFLAGS_VEC  = $(OPTFLAGS_BASE) -ftree-vectorize -fopt-info-optimized
OPTFLAGS_sse= -mtune=core2 -msse2  #-std=c99
OPTFLAGS_avx= -march=corei7-avx -mtune=corei7-avx
OPTFLAGS = $(OPTFLAGS_VEC) $(OPTFLAGS_$(VEC))

ASFLAGS = -Wa,-adhln=
GENFLAGS = -fverbose-asm -masm=intel  -ftree-vectorizer-verbose=3
CC  = gcc
CCC = g++
CCCFLAGS = -std=c++11 

ifeq ($(DEBUG), debug)
GENFLAGS += -g
endif

ifeq ($(CC), gcc)
CFLAGS += -std=c99
endif

HIER_ROOT = ..
IFLAGS = -I$(HIER_ROOT) -I$(HIER_ROOT)/hier-algorithms -I$(HIER_ROOT)/iterators
CCCFLAGS += $(IFLAGS)

all: bench-accumulate.exe bench-binarysearch.exe

.SUFFIXES: .c .cpp .s .exe .o

bench-accumulate.exe: bench-accumulate.o dummy.o sum-cstyle.o sumv.o sumd.o sumvv-flat.o sumvv-hier.o sumvv-chunked-hier.o prodvv-hier.o prodvv-chunked-hier.o prod-cstyle.o prodvv-flat.o
	$(CCC) $(CCFLAGS) $(CCCFLAGS)  $(OPTFLAGS_BASE) $^  -o $@


bench-accumulate-vector-vs-deque.o: bench-accumulate-vector-vs-deque.cpp
	$(CCC) -c -v $(CCFLAGS) $(CCCFLAGS) $(OPTFLAGS_BASE) $<

bench-binarysearch.exe: ../iterators/segmented-iterator.h

.cpp.exe:
	$(CCC) $(CCFLAGS) $(CCCFLAGS)  $(OPTFLAGS) $<  -o $@


.c.s:
	$(CC)  -c  $(CCFLAGS) $(OPTFLAGS) $(ASFLAGS)$@.$(VEC) $(GENFLAGS) $<

.cpp.s:
	$(CCC) -c  $(CCFLAGS) $(CCCFLAGS) $(OPTFLAGS) $(ASFLAGS)$@.$(VEC) $(GENFLAGS) $<

.cpp.o:
	$(CCC) -c -v $(CCFLAGS) $(CCCFLAGS) $(OPTFLAGS) $<




clean:
	rm -f *.o *~ *.exe *.s.sse *.s.avx *.stackdump

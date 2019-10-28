# Makefile for intro to C++ with ROOT

CPP=g++
LD=g++

ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)
ROOTLIBS+= -lRooFit -lRooFitCore -lMinuit
ROOTLDFLAGS=$(shell root-config --ldflags)

CFLAGS =
LFLAGS =

RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC1= CreateRooWorkspace.o create_roo.o
SL_HEADERS= BkgPdf4p.h
SL_SOURCES= BkgPdf4p.C

default: all

all: libRooVLQ.so create_roo

RooVLQDict.C: $(SL_HEADERS) Linkdef.h
	rootcint -f $@ -c $(ROOTCFLAGS) -p $^

libRooVLQ.so: RooVLQDict.C $(SL_SOURCES)
	$(LD) -shared -o $@ $(ROOTLDFLAGS) $(ROOTCFLAGS) $(RLXX) $^

%.o: %.C
	$(CPP) $(ROOTCFLAGS) $(RCXX) -c $<

create_roo: $(SRC1)
	$(LD) $(SRC1) $(RLXX) -L. -lRooVLQ  -o create_roo
	@echo '-> create_roo created!'

clean:
		@rm -f *~
		@rm -f *.o 
		@rm -f *.x
		@rm -f *Dict.C
		@rm -f *_rdict.pcm
		@rm -f libRooVLQ.so
		@rm -f create_roo
		
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
SRC2= CreateRooWorkspace.o create_roo_2017.o
SL_HEADERS= BkgPdfExp2.h BkgPdf3p.h BkgPdf4p.h RooMultiPdf.h
SL_SOURCES= BkgPdfExp2.C BkgPdf3p.C BkgPdf4p.C RooMultiPdf.C

default: all

all: libRooVLQ.so create_roo create_roo_2017

RooVLQDict.C: $(SL_HEADERS) Linkdef.h
	rootcint -f $@ -c $(ROOTCFLAGS) -p $^

libRooVLQ.so: RooVLQDict.C $(SL_SOURCES)
	$(LD) -shared -fPIC -o $@ $(ROOTLDFLAGS) $(ROOTCFLAGS) $(RLXX) $^

%.o: %.C
	$(CPP) $(ROOTCFLAGS) $(RCXX) -c $<

create_roo: $(SRC1)
	$(LD) $(SRC1) $(RLXX) -L. -lRooVLQ  -o create_roo
	@echo '-> create_roo created!'

create_roo_2017: $(SRC2)
	$(LD) $(SRC2) $(RLXX) -L. -lRooVLQ  -o create_roo_2017
	@echo '-> create_roo_2017 created!'


clean:
		@rm -f *~
		@rm -f *.o 
		@rm -f *.x
		@rm -f *Dict.C
		@rm -f *_rdict.pcm
		@rm -f libRooVLQ.so
		@rm -f create_roo
		@rm -f create_roo_2017

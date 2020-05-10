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
SRC3= CreateRooWorkspace.o create_roo_2018.o
SRC4= CreateRooWorkspace.o create_roo_CR.o
SRC5= CreateRooWorkspace.o create_roo_2017_CR.o
SRC6= CreateRooWorkspace.o create_roo_2018_CR.o
SL_HEADERS= BkgPdfExp2.h BkgPdf3p.h BkgPdf4p.h RooMultiPdf.h BkgPdf2p.h SignalDoubleGauss.h
SL_SOURCES= BkgPdfExp2.C BkgPdf3p.C BkgPdf4p.C RooMultiPdf.C BkgPdf2p.C SignalDoubleGauss.C

default: all

all: libRooVLQ.so create_roo create_roo_2017 create_roo_2018 create_roo_CR create_roo_2017_CR create_roo_2018_CR

RooVLQDict.C: $(SL_HEADERS) Linkdef.h
	rootcint -f $@ -c $(ROOTCFLAGS) -p $^

libRooVLQ.so: RooVLQDict.C $(SL_SOURCES)
	$(LD) -shared -fPIC -o $@ $(ROOTLDFLAGS) $(ROOTCFLAGS) $(RLXX) $^

%.o: %.C
	$(CPP) $(ROOTCFLAGS) $(RCXX) -c $<

create_roo: $(SRC1)
	$(LD) $(SRC1) $(RLXX) -L. -lRooVLQ  -o create_roo
	@echo '-> create_roo created!'

create_roo_CR: $(SRC4)
	$(LD) $(SRC4) $(RLXX) -L. -lRooVLQ  -o create_roo_CR
	@echo '-> create_roo_CR created!'

create_roo_2017: $(SRC2)
	$(LD) $(SRC2) $(RLXX) -L. -lRooVLQ  -o create_roo_2017
	@echo '-> create_roo_2017 created!'

create_roo_2017_CR: $(SRC5)
	$(LD) $(SRC5) $(RLXX) -L. -lRooVLQ  -o create_roo_2017_CR
	@echo '-> create_roo_2017_CR created!'


create_roo_2018: $(SRC3)
	$(LD) $(SRC3) $(RLXX) -L. -lRooVLQ  -o create_roo_2018
	@echo '-> create_roo_2018 created!'


create_roo_2018_CR: $(SRC6)
	$(LD) $(SRC6) $(RLXX) -L. -lRooVLQ  -o create_roo_2018_CR
	@echo '-> create_roo_2018_CR created!'


clean:
		@rm -f *~
		@rm -f *.o 
		@rm -f *.x
		@rm -f *Dict.C
		@rm -f *_rdict.pcm
		@rm -f libRooVLQ.so
		@rm -f create_roo
		@rm -f create_roo_2017
		@rm -f create_roo_2018

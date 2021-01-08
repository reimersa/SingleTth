
OBJDIR=obj
INCLUDES = -IObjects -I$(ROOTSYS)/include/root
DICTFILE=DictObjs.C
DICTOBJ=DictObjs.o
SHLIBFILE=libLHE_NtupleWriter.so

LINKLIBSIN = $(shell root-config --libs)
CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)
LDFLAGS = $(shell root-config --ldflags)
CXXFLAGS = $(shell root-config --cflags)
INAME = -install_name libLHE_NtupleWriter.so 

DICTHLIST = Objects/Particle.h Objects/GenParticle.h Objects/FlavorParticle.h Objects/LinkDef.h

default: create_ntuple

# Implicit rule to compile all classes
$(OBJDIR)/%.o : %.C
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -g -c $< -o $(OBJDIR)/$(notdir $@) $(INCLUDES)

# Rule to make the dictionary
$(DICTFILE):  $(DICTHLIST) $(DICTLDEF)
	@echo "Generating dictionary $@" 
#	@echo rootcint -f $(DICTFILE) $^
	rootcint -f $(DICTFILE) -c $(CXXFLAGS) $(INCLUDES) -p $^

# compile the dictionary
$(OBJDIR)/$(DICTOBJ): $(DICTFILE)
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -g -c $(INCLUDES) -o $@ $<

# Rule to combine objects into a unix shared library
$(SHLIBFILE): $(OBJDIR)/LHE_NtupleWriter.o $(OBJDIR)/$(DICTOBJ)
	@echo "Making shared library: $(SHLIBFILE)"
	@rm -f $(SHLIBFILE)
	@$(LD) -shared $(LDFLAGS) $(CXXFLAGS)  $^ -o $(SHLIBFILE) $(LINKLIBSIN) $(INAME) 

create_ntuple: obj/create_ntuple.o $(SHLIBFILE)
	@echo "Making executable create_ntuple"
	@echo @$(CXX) $< -L./ -lLHE_NtupleWriter -o create_ntuple
	@$(CXX) $< -L./ -lLHE_NtupleWriter $(LINKLIBSIN) -o create_ntuple

shlib: $(SHLIBFILE)

clean:
	rm -f $(DICTFILE)
	rm -f $(SHLIBFILE)
	rm -f $(OBJDIR)/*.o
	rm -f DictObjs.cxx
	rm -f DictObjs_rdict.pcm


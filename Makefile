CXX = g++
CXXFLAGS = -std=c++11 -Wall -W -pthread 
INC_DIRS = -I../include

LIBNAME = libtest

ifdef DEBUG_MYLIB
	CXFLAGS += -g
	LIBNAME := $(LIBNAME)_g
else
	CXXFLAGS += -O2
endif

SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPDIR = .deps

all: build_lib

build_lib: mkdepdir $(OBJS)
	ar rcs ../lib/$(LIBNAME).a $(OBJS) 

mkdepdir:
	@rm -f $*.d.tmp
	mkdir -p $(DEPDIR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD $< -o $@
	@mv -f $*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

clean:
	rm -rf $(DEPDIR) *.o *~ ../lib/*.a a.out core core.*

-include $(SRCS:%.cpp=$(DEPDIR)/%.d)


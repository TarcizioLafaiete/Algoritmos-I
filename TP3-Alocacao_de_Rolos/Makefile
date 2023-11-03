CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall
LDFLAGS=-g
LDLIBS=
EXECUTABLE=tp03

SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) $(EXECUTABLE)

distclean: clean
	$(RM) *~ .depend $(EXECUTABLE)

test: $(EXECUTABLE)
	bash run_tests.sh

include .depend
# GNU make手册：http://www.gnu.org/software/make/manual/make.html

CC = g++

CFILES = $(shell find ./ -name "*.cpp")
OBJS = $(CFILES:.cpp=.o)

all: $(OBJS)
	$(CC) $(OBJS) -o turing

debug:
	$(CC) -g $(CFILES) -o turing

.PHONY: clean test
test: all
	./turing -v gcd.tm 1011 > tmp.gh
clean:
	rm -f turing
	rm -f $(OBJS)

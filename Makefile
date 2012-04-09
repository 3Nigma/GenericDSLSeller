PRJ_NAME = sale

CPP = g++
CXFLAGS = -Wall -g -std=c++0x
LDXFLAGS = -llua5.1 -lboost_regex
OBJS = genericproperty.o \
numericalproperty.o \
stringproperty.o \
genericclass.o \
genericinstance.o \
seller.o \
interpres.o

%.o : %.cpp %.hpp
	$(CPP) -c $(CXFLAGS) $< -o $@

all: $(OBJS)
	$(CPP) $(CXFLAGS) $(OBJS) main.cpp -o $(PRJ_NAME) $(LDXFLAGS)
	./$(PRJ_NAME)

clean:
	rm -f $(PRJ_NAME) $(OBJS)
CC=gcc
#CPPFLAGS= -Wall  -Werror  -O3 -g -d 
CPPFLAGS= -Wall  -Werror -g 
LDFLAGS = -lm

SRC = allocate.o processor.o MMU.o queue.o process.o paged_management.o contiguous_management.o
TARGET=allocate


debug ?= 1 
ifeq ($(debug),1) 
	CPPFLAGS= -Wall -Werror -g -d
endif


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CPPFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
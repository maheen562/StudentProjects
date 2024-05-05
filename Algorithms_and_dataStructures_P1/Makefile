# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g

# Define libraries to be linked (for example -lm)
LDLIBS = 

# Define sets of source files and object files
SRC = driver.c footpath_data.c linklist.c 
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
 
SRC2 = driver2.c footpath_data.c linklist.c 
# OBJ is the same as SRC, just replace .c with .o
OBJ2 = $(SRC2:.c=.o) 
all : dict1 dict2

# Define the executable name(s)
EXE = dict1
EXE2 = dict2

# The first target:
$(EXE): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LDLIBS)

# The secound target:
$(EXE2): $(OBJ2) 
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ2) $(LDLIBS)

clean:
	rm -f $(OBJ) $(EXE) $(EXE2)

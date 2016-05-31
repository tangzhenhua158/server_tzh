include ../Makefile.inc


SVC_COMN_LIB =  -L../lib 
SVC_COMN_LIB_R = -L../lib 

INCLUDE := $(INCLUDE)  -I./h/ 

SRC_COMM = src/main.cpp src/testServer.cpp
SRC_CLIENT = src/testClient.cpp
           
OBJ_COMM = $(patsubst %.c, %.o, $(SRC_COMM:.cpp=.o))
OBJ_COMM := $(OBJ_COMM:.c=.o)

OBJ_CLIENT = $(patsubst %.c, %.o, $(SRC_CLIENT:.cpp=.o))
OBJ_CLIENT := $(SRC_CLIENT:.c=.o)

.SUFFIXES: .o .cpp .c
.cpp.o:
	$(CXX32) $(DBCXXFLAG) $(INCLUDE) -c -o $@ $<
.c.o:
	$(CXX32) $(DBCXXFLAG) $(INCLUDE) -c -o $@ $<


PROGRAM = test_d  
all: test_d test_client

test_d: $(OBJ_COMM)  
	$(CXX32) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_COMM) $(COMM_LIB) 

test_client: $(OBJ_CLIENT)  
	$(CXX32) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_CLIENT) $(COMM_LIB) 

depend:
	mkdep $(INCLUDE) $(SRC_COMM) $(CXXFLAG)

install:
	install $(PROGRAM) ../bin
clean: 
	rm -f $(OBJ_COMM)
	rm -f src/*.o
	rm -f $(PROGRAM)
	rm -f test_client



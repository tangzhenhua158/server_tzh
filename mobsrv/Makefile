include ../Makefile.inc


SVC_COMN_LIB =  -L../lib 
SVC_COMN_LIB_R = -L../lib 

COMM_LIB += ../lib/libmuduo_protorpc.a  -lprotobuf ../lib/libmuduo_protobuf_codec_cpp11.a  ../lib/libmuduo_protorpc_wire.a

INCLUDE := $(INCLUDE)  -I./h/ -I../thrd/tinyxml/

SRC_COMM = src/app.cpp src/mobServer.cpp src/main.cpp ../protocol_src/daemon.pb.cc  ../protocol_src/rpc.pb.cc
           
OBJ_COMM = $(patsubst %.c, %.o, $(SRC_COMM:.cpp=.o))
OBJ_COMM := $(OBJ_COMM:.c=.o)
OBJ_COMM := $(OBJ_COMM:.cc=.o)


.SUFFIXES: .o .cpp .c
.cpp.o:
	$(CXX32) $(DBCXXFLAG) $(INCLUDE) -c -o $@ $<
.c.o:
	$(CXX32) $(DBCXXFLAG) $(INCLUDE) -c -o $@ $<


PROGRAM = mobsrv  
all: mobsrv 

mobsrv: $(OBJ_COMM)  
	$(CXX32) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_COMM) $(COMM_LIB) 

depend:
	mkdep $(INCLUDE) $(SRC_COMM) $(CXXFLAG)

install:
	install $(PROGRAM) ../bin
clean: 
	rm -f $(OBJ_COMM)
	rm -f src/*.o
	rm -f $(PROGRAM)



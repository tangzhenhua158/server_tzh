CC = gcc
CXX32 = g++ 
CC32 = gcc

ARRU = ar -ru

DEFINE_SEL=-DHAVE_EPOLL
TRFLAG=-DTRACE_EAGLE_EYE

#DEF_STATUSPROTOCOL=-DSTATUS_OLDPROTOCOL
DEF_STATUSPROTOCOL=-DSTATUS_NEWPROTOCOL

DEF_DBTHREAD=-DDB_MULTITHREAD

# 修改 make_basedir 可以方便在其他目录编译，比如ent/test/Makefile 
ifeq ($(make_basedir),)
	make_basedir := ..
endif

LIBDIR := $(make_basedir)/lib

COMM_LIB := $(LIBDIR)/libmuduo_net.a  $(LIBDIR)/libmuduo_base.a ../thrd/lib/libtinyxml.a -lpthread


# DEF_SELECTOR_THREAD 如果需要每个线程使用自己的selector
# DEF_SELECTOR_THREAD=-DONE_SELECTOR_PER_THREAD make
#

VERSION=
CXXFLAG =  -m32 -Wall -ggdb -D_REENTRANT $(DEFINE_SEL) -DXML_NULL  $(DEF_STATUSPROTOCOL) $(TRFLAG) $(DEF_SELECTOR_THREAD)
CXXFLAG_R = -Wall -O2 -O3 -D_REENTRANT $(DEFINE_SEL) $(DEF_STATUSPROTOCOL) $(TRFLAG) $(DEF_SELECTOR_THREAD)
LINK_CXXFLAG = $(CXXFLAG) -Wl,-rpath,../lib -Wl,-rpath,$(make_basedir)/bin
LINK_CXXFLAG_R = $(CXXFLAG_R) -Wl,-rpath,$(make_basedir)/bin

INCLUDE :=
INCLUDE += -I$(make_basedir)/common
INCLUDE += -I./
INCLUDE += -I$(make_basedir)/common/muduo/net/
INCLUDE += -I$(make_basedir)/common/muduo/base/
INCLUDE += -I../
INCLUDE += -I../protocol_include/
INCLUDE += -I../thrd/tinyxml/
INCLUDE += -I$(BOOST_ROOT)/boost_1_60_0/

#	vim: ft=make


.ver:
	export LC_ALL=C && sed -i 's/[0-9][0-9]*/'`svn info|grep Revision|awk '{print $$2}'`'/' $(make_basedir)/helper/version.h


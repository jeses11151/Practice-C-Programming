#--------------------------------------------------------------
#               netprisma OpenLinux
#--------------------------------------------------------------
QL_SDK_PATH   ?= $(shell pwd)/../..

# Output targets
QL_EXP_TARGETS = q8_multithread_app
QL_EXP_LDLIBS  = -lcomms

#--------------------------------------------------------
# COMPILER SWITCH: Use 'make ARCH=arm' for ARM build, or default (x86_64)
#--------------------------------------------------------
ARCH ?= native

ifeq ($(ARCH),arm)
CC = arm-oe-linux-gnueabi-gcc
CPPFLAGS = -I./ -I$(QL_TOOLS_PATH)/mdm9607/usr/include -I$(QL_SDK_PATH)/include
CFLAGS   += -march=armv7-a -mfloat-abi=softfp -mfpu=neon -fPIC
LDFLAGS  = -L./ -L$(QL_SDK_PATH)/lib -lrt ${QL_EXP_LDLIBS} -lpthread
else
CC = gcc
CPPFLAGS = -I./ -I$(QL_SDK_PATH)/include
CFLAGS   += -fPIC
LDFLAGS  = -L./ -lrt ${QL_EXP_LDLIBS} -lpthread
endif

#--------------------------------------------------------
# Build Rules
#--------------------------------------------------------
all: libcomms.so $(QL_EXP_TARGETS)

libcomms.so: q8_pipe_transfer.o q8_socket_transfer.o
	$(CC) -shared -o libcomms.so q8_pipe_transfer.o q8_socket_transfer.o

q8_multithread_app: q8_multithread.o libcomms.so
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o q8_multithread_app q8_multithread.o -Wl,-rpath,.

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.so $(QL_EXP_TARGETS)

.PHONY: checkmake
checkmake:
	@echo -e "CURDIR = ${CURDIR}"
	@echo -e "MAKE_VERSION = ${MAKE_VERSION}"
	@echo -e "MAKEFILE_LIST = ${MAKEFILE_LIST}"
	@echo -e "COMPILE.c = ${COMPILE.c}"
	@echo -e "COMPILE.cc = ${COMPILE.cc}"
	@echo -e "COMPILE.cpp = ${COMPILE.cpp}"
	@echo -e "LINK.cc = ${LINK.cc}"
	@echo -e "LINK.o = ${LINK.o}"
	@echo -e "CPPFLAGS = ${CPPFLAGS}"
	@echo -e "CFLAGS = ${CFLAGS}"
	@echo -e "LDFLAGS = ${LDFLAGS}"
	@echo -e "LDLIBS = ${LDLIBS}"

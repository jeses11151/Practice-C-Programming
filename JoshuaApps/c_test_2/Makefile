# Output targets
QL_EXP_TARGETS = q2_reverse_string_app

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
CPPFLAGS = -I./ -I$(QL_TOOLS_PATH)/mdm9607/usr/include -I$(QL_SDK_PATH)/include
CFLAGS   += -fPIC
LDFLAGS = -L./ -L$(QL_SDK_PATH)/lib -lrt ${QL_EXP_LDLIBS} -lpthread

CC ?= arm-oe-linux-gnueabi-gcc

q2_reverse_string_app: q2_reverse_string.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o q2_reverse_string_app q2_reverse_string.o -Wl,-rpath,.

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
	@echo -e "CXXFLAGS = ${CXXFLAGS}"
	@echo -e "LDFLAGS = ${LDFLAGS}"
	@echo -e "LDLIBS = ${LDLIBS}"

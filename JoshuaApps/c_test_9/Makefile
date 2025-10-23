# Output target
TARGET = q9_server_socket_2
SRC = q9_server_socket_2.c

# Paths
C_TEST_8_PATH = ../c_test_8
LIBCOMMS = $(C_TEST_8_PATH)/libcomms.so

# Compiler
CC = gcc
QCC = gcc
# Flags
CFLAGS = -Wall -Wextra -I$(C_TEST_8_PATH)
LDFLAGS = -L$(C_TEST_8_PATH) -lcomms -lpthread
RPATH = -Wl,-rpath=$(C_TEST_8_PATH)

# Default target
all: $(TARGET)

# Ensure libcomms.so is built before linking this target
$(TARGET): $(SRC) $(LIBCOMMS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(RPATH)

# This rule tells make how to build libcomms.so if it's missing
$(LIBCOMMS):
	$(MAKE) -C $(C_TEST_8_PATH) libcomms.so

clean:
	rm -f $(TARGET)

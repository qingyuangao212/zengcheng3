# Makefile to build IRE3_20260105 shared library (.so) on Linux

CC = gcc
CFLAGS = -O2 -fPIC -Wall
LDFLAGS = -shared
TARGET = libIRE3_20260105.so

# Source files
SRC = \
    IRE3_20260105.c \
    IRE3_20260105_data.c \
    rtGetInf.c \
    rtGetNaN.c \
    rt_nonfinite.c

# Headers
HDR = \
    IRE3_20260105.h \
    IRE3_20260105_types.h \
    rtGetInf.h \
    rtGetNaN.h \
    rt_nonfinite.h \
    rt_defines.h \
    rtwtypes.h

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET) *.o

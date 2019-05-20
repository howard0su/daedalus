TARGET = getmodel
OBJS = main.o

INCDIR =
CFLAGS = -Os -G0 -Wall


CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = kstuff.exp 
 
USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1 
 
LIBDIR =
LDFLAGS = -nostartfiles
LIBS = -lpspctrl_driver

PSP_FW_VERSION=500

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

EXTRA_TARGETS += getmodel.S
EXTRA_CLEAN += getmodel.o

getmodel.S : $(PRX_EXPORTS)
	psp-build-exports --build-stubs $(PRX_EXPORTS)

all: $(FINAL_TARGET) getmodel.S
	cp $(FINAL_TARGET) "../../../../DaedalusX64/"

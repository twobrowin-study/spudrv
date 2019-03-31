# SPU Leonhard program interface main Makefile
# Contains var definitions and runs of other makefiles
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

ARCH           = mips
BAIKAL_HOME    = ~/.baikal
CROSS_COMPILE  = ${BAIKAL_HOME}/usr/x-tools/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-
GPP            = ${CROSS_COMPILE}g++
COMPILER_FLAGS = -Wall -ggdb

ifeq (${DEBUG}, true)
	COMPILER_FLAGS += -DDEBUG
endif

COMPILER_FLAGS += -DSPU64

default: spudrv.ko libspu.so

spudrv.ko:
	@echo "Cleaning Driver Kernel Module"
	${MAKE} -C spudrv BAIKAL_HOME="${BAIKAL_HOME}" clean
	@echo "Building Driver Kernel Module"
	${MAKE} -C spudrv BAIKAL_HOME="${BAIKAL_HOME}" ARCH="${ARCH}" CROSS_COMPILE="${CROSS_COMPILE}" COMPILER_FLAGS="${COMPILER_FLAGS}"

libspu.so:
	@echo "Building Program Interface Library"
	${MAKE} -C libspu GPP="${GPP}" COMPILER_FLAGS="${COMPILER_FLAGS}"

# Copy objects to Leonhard server after compile
srv-cp: default
	@./cp_images_to_srv.sh

clean:
	@echo "Cleaning Driver Kernel Module"
	${MAKE} -C spudrv BAIKAL_HOME="${BAIKAL_HOME}" clean
	@echo "Cleaning Program Interface Library"
	${MAKE} -C libspu clean
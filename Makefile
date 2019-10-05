# SPU Leonhard driver top Makefile
# Contains var definitions and runs of other makefiles
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

# Targets
DRIVER     = spudrv
DRIVER_DIR = source

# Current arch
ARCH     = mips
SPU_ARCH = 64

# Configuration pathes
BAIKAL_HOME   = ~/.baikal
CROSS_COMPILE = ${BAIKAL_HOME}/usr/x-tools/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-
KERNEL_SOURCE = ${BAIKAL_HOME}/src/kernel
GPP           = ${CROSS_COMPILE}g++

# Compiler flags config
COMPILER_FLAGS += -Wall -ggdb
COMPILER_FLAGS += -DSPU${SPU_ARCH}
ifeq (${DEBUG}, true)
	COMPILER_FLAGS += -DDEBUG
endif

# Default targets
default: clean $(DRIVER).ko
all: default

# Building SPU driver
$(DRIVER).ko:
	@echo "Building driver $(DRIVER)"
	${MAKE} -C $(DRIVER_DIR) KERNEL_SOURCE="${KERNEL_SOURCE}" ARCH="${ARCH}" CROSS_COMPILE="${CROSS_COMPILE}" COMPILER_FLAGS="${COMPILER_FLAGS}"

clean:
	@echo "Cleaning Driver Kernel Module"
	${MAKE} -C $(DRIVER_DIR) KERNEL_SOURCE="${KERNEL_SOURCE}" clean

# Compile and copy to Leonhard server all object files
srv-cp: default
	@./cp_images_to_srv.sh -d $(DRIVER)
	@./help_srv.sh $(DRIVER)
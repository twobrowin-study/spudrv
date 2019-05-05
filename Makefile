# SPU Leonhard program interface main Makefile
# Contains var definitions and runs of other makefiles
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

# Targets
TARGET  ?= dijkstra
LIBRARY  = libspu
DRIVER   = spudrv

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
default: clean $(DRIVER).ko $(TARGET).out
all: default

# Building Target
$(TARGET).out:
	@echo "Building target $(TARGET)"
	${MAKE} -C $(TARGET) GPP="${GPP}" COMPILER_FLAGS="${COMPILER_FLAGS}" LIBRARY="../${LIBRARY}"

# Building SPU driver
$(DRIVER).ko:
	@echo "Building driver $(DRIVER)"
	${MAKE} -C $(DRIVER) KERNEL_SOURCE="${KERNEL_SOURCE}" ARCH="${ARCH}" CROSS_COMPILE="${CROSS_COMPILE}" COMPILER_FLAGS="${COMPILER_FLAGS}"

clean:
	@echo "Cleaning Driver Kernel Module"
	${MAKE} -C $(DRIVER) KERNEL_SOURCE="${KERNEL_SOURCE}" clean
	@echo "Cleaning $(TARGET)"
	${MAKE} -C $(TARGET) clean

# Compile and copy to Leonhard server all object files
srv-cp: default
	@./cp_images_to_srv.sh -d $(DRIVER) -t $(TARGET)
	@./help_srv.sh $(DRIVER) $(TARGET)

# Compile and copy to Leonhard server only driver
srv-cp-$(DRIVER): clean $(DRIVER).out
	@./cp_images_to_srv.sh -t $(DRIVER)
	@./help_srv.sh $(DRIVER) $(TARGET)

# Compile and copy to Leonhard server only target
srv-cp-$(TARGET): clean $(TARGET).out
	@./cp_images_to_srv.sh -t $(TARGET)
	@./help_srv.sh $(DRIVER) $(TARGET)
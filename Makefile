# SPU Leonhard program interface main Makefile
# Contains var definitions and runs of other makefiles
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

# Targets
DRIVER  = spudrv
LIBRARY = libspu
TARGET  = test04
OBJS    = test04.o

# Current arch
ARCH = mips

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
default: clean $(DRIVER).ko $(TARGET)
all: default

# Building SPU driver
$(DRIVER).ko:
	@echo "Building driver $(DRIVER)"
	${MAKE} -C $(DRIVER) KERNEL_SOURCE="${KERNEL_SOURCE}" ARCH="${ARCH}" CROSS_COMPILE="${CROSS_COMPILE}" COMPILER_FLAGS="${COMPILER_FLAGS}"

# Building target with SPU library support
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	${GPP} ${COMPILER_FLAGS} -I$(LIBRARY) -o "$@" $(OBJS)

# Building obj files
%.o: %.cpp
	@echo 'Building file: $<'
	${GPP} ${COMPILER_FLAGS} -I$(LIBRARY) -c -o "$@" "$<"

clean:
	@echo "Cleaning Driver Kernel Module"
	${MAKE} -C $(DRIVER) KERNEL_SOURCE="${KERNEL_SOURCE}" clean
	@echo "Cleaning $(TARGET)"
	$(RM) $(OBJS) $(TARGET)

# Copy objects to Leonhard server after compile
srv-cp: default
	@./cp_images_to_srv.sh
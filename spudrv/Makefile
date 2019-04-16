# SPU Leonhard PCI Driver Linux Kernel Module
# Has to be run from ../ Makefile
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

BINARY  = spudrv
TARGETS = modules
OBJECTS = \
					module.o \
					pcidrv.o \
					chardev.o \
					cmdexec.o \
					gsidresolver.o \

obj-m       += $(BINARY).o
$(BINARY)-y := $(OBJECTS)
ccflags-y   += ${COMPILER_FLAGS}

PWD := ${shell pwd}

$(BINARY).ko:
	${MAKE} -C "${KERNEL_SOURCE}" SUBDIRS="${PWD}" ARCH="${ARCH}" CROSS_COMPILE="${CROSS_COMPILE}" $(TARGETS)

clean:
	${MAKE} -C "${KERNEL_SOURCE}" SUBDIRS="${PWD}" clean
# SPU Interface object Library
# Has to be run from ../ Makefile
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

TARGET = test04
OBJS   = main.o

RM = rm -rf

# Default invoke
default: clean $(TARGET)

# Building target
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	${GPP} ${COMPILER_FLAGS} -o "$@" $(OBJS)

# Building obj files
%.o: %.cpp
	@echo 'Building file: $<'
	${GPP} ${COMPILER_FLAGS} -c -o "$@" "$<"

# Cleaning
clean:
	$(RM) $(OBJS) $(TARGET)
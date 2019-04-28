# Test program with included SPU library
# Has to be run from ../ Makefile
# Made by Dubrovin Egor <dubrovin.en@ya.ru>

# Binary (executable) and object files
BINARY = test04
OBJS   = main.o

# Building binary (executable) with SPU library support
$(BINARY): $(OBJS)
	@echo 'Building binary (executable): $@'
	${GPP} ${COMPILER_FLAGS} -I$(LIBRARY) -o "$@" $(OBJS)

# Building obj files
%.o: %.cpp
	@echo 'Building file: $<'
	${GPP} ${COMPILER_FLAGS} -I$(LIBRARY) -c -o "$@" "$<"

clean:
	@echo "Cleaning binary $(BINARY)"
	$(RM) $(OBJS) $(BINARY)
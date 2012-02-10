CPP=g++
CPP_FLAGS=-ggdb -Wall -std=gnu++0x
LD_FLAGS=-lglut -lGL -lGLU

OBJ_DIR=.objects
SRC_DIR=src

OBJS=$(addprefix $(OBJ_DIR)/, main.o mainloop.o draw.o handlers.o findpaths.o)

executable=cube

.PHONY: clean

# $^ = RHS (dependencies)
# this has to come BEFORE the ld flags, otherwise you get a million errors
# $@ is the target name
$(executable): $(OBJS)
	$(CPP) $^ $(LD_FLAGS) $(CPP_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -c $(firstword $^) -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(executable)

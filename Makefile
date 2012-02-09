CPP=colorgcc
CPP_FLAGS=-Wall
LD_FLAGS=-lglut -lGL -lGLU

OBJ_DIR=.objects
SRC_DIR=src

OBJS=$(addprefix $(OBJ_DIR)/, main.o mainloop.o)

executable=cube

.PHONY: clean

# $^ = RHS (dependencies)
# this has to come BEFORE the ld flags, otherwise you get a million errors
# $@ is the target name
$(executable): $(OBJS)
	$(CPP) $^ $(LD_FLAGS) $(CPP_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -c $(firstword $^) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(executable)

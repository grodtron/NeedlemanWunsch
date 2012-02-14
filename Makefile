CPP=g++
CPP_FLAGS=-ggdb -Wall #-std=gnu++0x
VIS_LD_FLAGS=-lglut -lGL -lGLU
LD_FLAGS=

OBJ_DIR=.objects
SRC_DIR=src

VIS_OBJS=$(addprefix $(OBJ_DIR)/, main_vis.o mainloop.o draw.o handlers.o NeedlemanWunsch.o)
OBJS=$(addprefix $(OBJ_DIR)/, main.o NeedlemanWunsch.o)

vis_executable=nwvis
executable=nw

.PHONY: clean all

all: $(vis_executable) $(executable)

# $^ = RHS (dependencies)
# this has to come BEFORE the ld flags, otherwise you get a million errors
# $@ is the target name
$(vis_executable): $(VIS_OBJS)
	$(CPP) $^ $(VIS_LD_FLAGS) $(CPP_FLAGS) -o $@

$(executable): $(OBJS)
	$(CPP) $^ $(LD_FLAGS) $(CPP_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -c $(firstword $^) -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(executable)
	rm -f $(vis_executable)

# check g++ version for sometime-necessary flags
GCC_MAJOR_VERSION := $(shell g++ -dumpversion | awk -F. '{ print $$1 }')
GCC_MINOR_VERSION := $(shell g++ -dumpversion | awk -F. '{ print $$2 }')
GCC_PATCH_VERSION := $(shell g++ -dumpversion | awk -F. '{ print $$3 }')

GCC_IS_NEW := $(shell expr `expr $(GCC_MAJOR_VERSION) == 4` \& `expr $(GCC_MINOR_VERSION) \>= 4` )

CPP_FLAGS=-ggdb -Wall
LD_FLAGS=

# necessary for newer versions of g++ for <random>
ifeq "$(GCC_IS_NEW)" "1"
CPP_FLAGS += -std=gnu++0x
endif

# Check for colorgcc and use it if it exists
# update LD_FLAGS because it will run as if it is gcc, not g++
COLOR_GCC := $(shell which colorgcc | wc -c)
ifeq "$(COLOR_GCC)" "0"
CPP=g++
else
CPP=colorgcc
VIS_LD_FLAGS += -lstdc++
LD_FLAGS += -lstdc++
endif


OBJ_DIR=.objects
SRC_DIR=src

OBJS=$(addprefix $(OBJ_DIR)/, main.o NeedlemanWunsch.o qwertyDistance.o gapPenaltyFunctions.o)

executable=nw

.PHONY: clean all

all: $(executable)

# $^ = RHS (dependencies)
# this has to come BEFORE the ld flags, otherwise you get a million errors
# $@ is the target name

$(executable): $(OBJS)
	$(CPP) $^ $(LD_FLAGS) $(CPP_FLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -c $< -o $@

$(SRC_DIR)/qwertyDistance.cpp: $(SRC_DIR)/makeQwertyDistance.py
	python $< $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(executable)

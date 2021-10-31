# ---------------------------------------------------------------
#
# Commands:
# 	build 		: build program with debug mode 
# 	release 	: build program with optimizations on 
# 	clear 		: clear all compiled executables
# 	install  	: installs the binary in the specified "INSTALL_" path variable

CC := gcc

EXE := cponto
OBJS = main.o

C_FLAGS =
I_FLAGS =
L_FLAGS = -lcmdf -ldoc

INSTALL_BIN_DIR := /usr/local/bin
# INSTALL_LIB_DIR := /usr/local/lib
# INSTALL_INC_DIR := /usr/local/include

# ---------------------------------------------------------------

build : C_FLAGS += -g
build : $(EXE)

release : C_FLAGS += -O2
release : clear $(EXE)

$(EXE) : $(OBJS)
	$(CC) $^ $(L_FLAGS) -o $@

%.o : %.c
	$(CC) $(C_FLAGS) -c $^ -o $@

clear : 
	@rm -rdf $(OBJS) $(EXE)

install : release
	cp $(EXE) $(INSTALL_BIN_DIR)/
# ---------------------------------------------------------------
#
# Commands:
# 	build 		: build program with debug mode 
# 	release 	: build program with optimizations on 
# 	clear 		: clear all compiled executables
# 	install  	: installs the binary in the specified "INSTALL_" path variable

CC := gcc

EXE := cponto

OUTPUT_CSV_FILENAME_WINDOWS	:= $(HOMEPATH)/Documents/c_ponto.csv
OUTPUT_CSV_FILENAME_LINUX	:= $(HOME)/Documents/c_ponto.csv

INSTALL_BIN_DIR_WINDOWS 	:= $(HOMEDRIVE)/msys64/usr/local/bin
INSTALL_BIN_DIR_LINUX 		:= /usr/local/bin

OBJS = main.o

C_FLAGS =
D_FLAGS =
I_FLAGS =
L_FLAGS = -lcmdf -ldoc

OUTPUT_CSV_FILENAME :=
ifeq ($(OS),Windows_NT)

	D_FLAGS += -DOUTPUT_CSV_FILENAME="\"$(OUTPUT_CSV_FILENAME_WINDOWS)\""
	INSTALL_BIN_DIR = $(INSTALL_BIN_DIR_WINDOWS)

	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
#		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
#		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)

		D_FLAGS += -DOUTPUT_CSV_FILENAME="\"$(OUTPUT_CSV_FILENAME_LINUX)\""
	INSTALL_BIN_DIR = $(INSTALL_BIN_DIR_LINUX)

	endif
	ifeq ($(UNAME_S),Darwin)
#		OSFLAG += -D OSX
	endif
		UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
#		OSFLAG += -D AMD64
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
#		OSFLAG += -D IA32
		endif
	ifneq ($(filter arm%,$(UNAME_P)),)
#		OSFLAG += -D ARM
	endif
endif

# ---------------------------------------------------------------

build : C_FLAGS += -g
build : $(EXE)

release : C_FLAGS += -O2
release : clear $(EXE)

$(EXE) : $(OBJS)
	$(CC) $^ $(L_FLAGS) -o $@

%.o : %.c
	$(CC) $(D_FLAGS) $(C_FLAGS) -c $^ -o $@

clear : 
	@rm -rdf $(OBJS) $(EXE)

install :
	cp $(EXE) $(INSTALL_BIN_DIR)/

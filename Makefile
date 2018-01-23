###################################################
# Define colors
CLR_DEFAULT := $(shell echo '\033[00m')
CLR_RED     := $(shell echo '\033[31m')
CLR_GREEN   := $(shell echo '\033[32m')
CLR_YELLOW  := $(shell echo '\033[33m')
CLR_BLUE    := $(shell echo '\033[34m')
CLR_PURPLE  := $(shell echo '\033[35m')
CLR_CYAN    := $(shell echo '\033[36m')

###################################################
# Display predefine colorful messages
print-simple = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT)"
print-from = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) <= $(CLR_YELLOW)$3$(CLR_DEFAULT)"
print-to = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) => $(CLR_YELLOW)$3$(CLR_DEFAULT) $4"

###################################################
# Executable name
TARGET = NeuNeu

###################################################
# Default path storing compilation files (*.o ...)
ifeq ($(BUILD),)
BUILD = build
endif

###################################################
# Third part: external libraries
ifeq ($(THIRDPART),)
THIRDPART = external
endif

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -Isrc -Isrc/chess -Isrc/players -Isrc/utils -Isrc/GUI -I$(THIRDPART)

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):src:src/chess:src/players:src/utils:src/GUI:src/players:$(THIRDPART)

###################################################
# Store files dependencies in *.d files.  When a file
# is modified compile others which depend on it.
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# List of files to compile. Splited by directories
OBJ = Pieces.o Rules.o Board.o IPC.o Stockfish.o tcsp.o NeuNeu.o Human.o GUI.o main.o

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 -O2 -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

###################################################
# Common defines
DEFINES +=

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS =

###################################################
all: $(TARGET)

###################################################
# Link sources
$(TARGET): CmdParser $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Download external libs
CmdParser: $(THIRDPART)
	cd $(THIRDPART) && rm -fr CmdParser && git clone https://github.com/FlorianRappl/CmdParser.git --depth=1 2> /dev/null

###################################################
# Compress SimTaDyn sources without its .git, build
# folders and doc generated files. If a tarball
# already exists, it'll not be smashed.
.PHONY: targz
targz:
	@./.makefile/targz.sh $(PWD)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) 2> /dev/null

###################################################
# Display the compilator version (g++, clang ...)
.PHONY: which-gcc
which-gcc:
	@$(call print-simple,"Version","$(CXX)")
	@$(CXX) --version

###################################################
# Create a temporary folder to store *.o and *.d files
$(DEPFILES): | $(BUILD)
$(OBJ): | $(BUILD)
version.h: | $(BUILD)
$(THIRDPART): $(BUILD)
$(BUILD): which-gcc
	@mkdir -p $(BUILD)
	@mkdir -p $(THIRDPART)

###################################################
# Auto-Dependency Generation
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJ)))

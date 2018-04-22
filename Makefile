##=====================================================================
## ChessNeuNeu: Non serious chess engine for learning neural networks.
## Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of ChessNeuNeu.
##
## ChessNeuNeu is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

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
TARGET = ChessNeuNeu

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
INCLUDES = -I$(BUILD) -Isrc -Isrc/Players -Isrc/Utils -I$(THIRDPART)

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):src:src/Players:src/Utils:src/Chess:src/GUI:$(THIRDPART)

###################################################
# Store files dependencies in *.d files.  When a file
# is modified compile others which depend on it.
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# List of files to compile. Splited by directories
OBJ = Debug.o FEN.o Rules.o Board.o Promotion.o IPC.o Player.o Stockfish.o TSCP.o NeuNeu.o Human.o GUI.o main.o

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++14 -O2 -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread

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
$(TARGET): CmdParser.done $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp CmdParser.done $(BUILD)/%.d Makefile
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Download external libs
CmdParser.done: CmdParser
CmdParser: $(THIRDPART)
	@$(call print-to,"Downloading","$@","$</")
	@cd $(THIRDPART) && rm -fr CmdParser && git clone https://github.com/FlorianRappl/CmdParser.git --depth=1 2> /dev/null && touch CmdParser.done

###################################################
# Generate the code source documentation with doxygen.
.PHONY: doc
doc:
	@doxygen Doxyfile
	@xdg-open doc/html/index.html >/dev/null

###################################################
# Compress ChessNeuNeu sources without its .git, build
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
.PHONY: veryclean
veryclean: clean
	@rm -fr $(THIRDPART) doc/html 2> /dev/null

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

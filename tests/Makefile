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
# Project definition
#
PROJECT = ChessNeuNeu
TARGET = $(PROJECT)-UnitTest
DESCRIPTION = Unit-tests for $(PROJECT)
STANDARD = --std=c++14
BUILD_TYPE := debug
USE_COVERAGE = 1

###################################################
# Location of the project directory and Makefiles
#
P=..
M=$(P)/.makefile
include $(M)/Makefile.header

###################################################
# Inform Makefile where to find header files
#
INCLUDES += -I$(P)/src -I$(P)/src/Players -I$(P)/src/Utils -I$(THIRDPART)

###################################################
# Inform Makefile where to find *.cpp and *.o files
#
VPATH += $(P)/src $(P)/src/Players $(P)/src/Utils $(P)/src/Chess $(P)/src/GUI $(THIRDPART)

###################################################
# Suppress warnings
#
CXXFLAGS = -W -Wall -Wextra -Wshadow

###################################################
# Project defines
#
DEFINES = -DDATADIR=\"$(DATADIR)\"

###################################################
# Make the list of compiled files
#
OBJS = FEN.o Rules.o Debug.o FENTests.o RulesTests.o DebugTests.o UnitTests.o
#PositionTests.o

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.
#
LINKER_FLAGS += -lcrpcut -ldl

###################################################
# Compile OpenGLCppWrapper unit tests.
.PHONY: all
all: $(TARGET)

###################################################
# Compile and run unit tests.
.PHONY: unit-tests
.PHONY: check
unit-tests check: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@$(SANITIZER) ./$(BUILD)/$(TARGET) || (cat $(TARGET).log; return 1)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer

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
# Executable name
PROJECT = ChessNeuNeu
TARGET = $(PROJECT)-UnitTest
DESCRIPTION = Unit-tests for $(PROJECT)

###################################################
# Debug mode or Release mode
BUILD_TYPE := debug

###################################################
# Location from the project root directory.
P=..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(P)/Makefile.common
include $(M)/Makefile.header

###################################################
# Make the list of files to compile
OBJS = FEN.o Rules.o Debug.o FENTests.o RulesTests.o DebugTests.o UnitTests.o
#PositionTests.o

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.
LDFLAGS += -lcrpcut -ldl

###################################################
# Compile OpenGLCppWrapper unit tests.
all: $(TARGET)

###################################################
# Compile and run unit tests.
.PHONY: check
check: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@$(SANITIZER) ./$(BUILD)/$(TARGET) || (cat $(TARGET).log; return 1)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer

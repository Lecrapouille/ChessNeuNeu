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
# Location of the project directory and Makefiles
#
P := .
M := $(P)/.makefile

###################################################
# Project definition
#
TARGET = $(PROJECT)
DESCRIPTION = Non serious chess engine for learning how to develop neural networks
BUILD_TYPE := debug

include $(P)/Makefile.common

###################################################
# Make the list of compiled files
#
OBJ_UTILS = IPC.o GUI.o main.o
OBJ_CHESS = Debug.o FEN.o Rules.o
OBJ_GUI = Board.o Promotion.o
OBJ_PLAYERS = Player.o Stockfish.o TSCP.o NeuNeu.o Human.o
OBJS += $(OBJ_UTILS) $(OBJ_CHESS) $(OBJ_GUI) $(OBJ_PLAYERS)

###################################################
# Compile the project
all: $(TARGET)

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: check
check:
	@$(call print-simple,"Compiling unit tests")
	@$(MAKE) -C tests coverage

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: unit-tests
unit-tests: check

###################################################
# Install project. You need to be root.
.PHONY: install
install: $(TARGET)
	$(call RULE_INSTALL_BINARIES)
	$(call RULE_INSTALL_DOC)

###################################################
# Uninstall the project. You need to be root. FIXME: to be updated
#.PHONY: uninstall
#uninstall:
#	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
#	@rm $(PROJECT_EXE)
#	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the whole project.
.PHONY: veryclean
veryclean: clean
	@rm -fr cov-int $(PROJECT).tgz *.log foo 2> /dev/null
	@(cd tests && $(MAKE) -s clean)
	@rm -fr $(THIRDPART) doc/html 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer

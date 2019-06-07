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
TARGET = $(PROJECT)
DESCRIPTION = Non serious chess engine for learning neural networks

###################################################
# Debug mode or Release mode
BUILD_TYPE := debug

###################################################
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(P)/Makefile.common
include $(M)/Makefile.header

###################################################
# Make the list of files to compile
OBJS = Debug.o FEN.o Rules.o Board.o Promotion.o IPC.o \
       Player.o Stockfish.o TSCP.o NeuNeu.o Human.o GUI.o main.o

###################################################
# Compile the project
all: $(TARGET)

###################################################
# Download external libs
main.o: .CmdParser.done
.CmdParser.done: $(THIRDPART)
	@$(call print-to,"Downloading","$@","$</")
	@cd $(THIRDPART) && rm -fr CmdParser && git clone https://github.com/FlorianRappl/CmdParser.git --depth=1 2> /dev/null && touch .CmdParser.done

###################################################
# Create the directory storing all third-part libs
$(THIRDPART): $(BUILD)
	@mkdir -p $(THIRDPART)

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: check
check:
	@$(call print-simple,"Compiling unit tests")
	@$(MAKE) -C tests coverage

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
	@rm -fr $(THIRDPART)/*/ $(THIRDPART)/.* doc/html 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer

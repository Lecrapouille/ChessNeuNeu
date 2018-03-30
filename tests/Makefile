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
TARGET = ChessNeuNeu-UnitTest

###################################################
# Default path storing compilation files (*.o ...)
ifeq ($(BUILD),)
BUILD = build
endif

###################################################
# Path where to store the generated coverage docs
DOC = doc/coverage
# Path where to store lcov rapport
RAPPORT = $(BUILD)/rapport.info

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -I../src -I../src/Players -I../src/Utils

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):../src:../src/Players:../src/Utils:../src/Chess:../src/GUI

###################################################
# Store files dependencies in *.d files.  When a file
# is modified compile others which depend on it.
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# List of files to compile. Splited by directories
OBJ = Rules.o Debug.o FEN.o FENTests.o RulesTests.o UnitTests.o

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 -O2 -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lcrpcut -ldl

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
# Common defines
DEFINES +=

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS =

###################################################
# Compile ChessNeuNeu unit tests
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile unit test sources
%.o: %.cpp $(BUILD)/%.d Makefile
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Compile an run unit tests with address sanitizer (if present).
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./build/$(TARGET) $(TU_OPTIONS) || (cat ChessNeuNeu.log; return 1)

###################################################
# Generate the code coverage html rapport.
.PHONY: coverage
coverage: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@./build/$(TARGET) || exit 0;
	@$(call print-to,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' 'external/*' 'tests/*' -o $(RAPPORT)
	@genhtml --quiet -o $(DOC) -t "$(TARGET)" $(RAPPORT)
	@xdg-open $(DOC)/index.html >/dev/null

###################################################
# Clean the local build/ directory.
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) $(DOC) 2> /dev/null

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
$(BUILD): which-gcc
	@mkdir -p $(BUILD)

###################################################
# Auto-Dependency Generation
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJ)))

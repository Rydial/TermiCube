# CPP Compiler
CXX 			:= g++

# Compile-time Flags
CXXFLAGS		:= -std=c++20 -Wall -Wextra -Wvla -Weffc++ -Wsign-conversion -Werror

# Source Directory
SOURCE			:= src

# Include Directory
INCLUDE			:= include

# Build Directory
BUILDDIR		:= build

# Output Directory
OUTPUT			:= output

# Error Directory
ERROR			:= error

# Program command-line arguments
ARGS			:=

# Debugging Flags
DEBUG			:= -g -DDEBUG

# Program Executable
MAIN			:= App


# Windows OS
ifeq ($(OS),Windows_NT)
	FIXPATH = $(subst /,\,$1)
	MAIN		:= $(OUTPUT)/$(MAIN).exe
	LIBS		:= -lglfw3 -lopengl32 -lglew32
  	#LIBS		+= -lgdi32
    #LIBS		+= -lglu32
endif

# Linux OS
ifeq ($(shell uname), Linux)
	SOURCEDIR	:= $(shell find $(SOURCE) -type d)
	INCLUDEDIR	:= $(shell find $(INCLUDE) -type d)
	OUTPUTDIR	:= $(BUILDDIR)/$(OUTPUT)
	OUTPUTMAIN	:= $(OUTPUTDIR)/$(MAIN)
	ERRORLOG	:= $(BUILDDIR)/$(ERROR)/err.log
    # Compiler Flags
	LIBS		:= -lglfw -lGL -lpanelw -lncursesw
	CXXFLAGS	+= -fsanitize=address -fsanitize=undefined
    # Commands
	MKDIR		:= mkdir -p
	RM 			:= rm -f
	DISPLAY		:= echo
	CLEAR		:= clear
	TIME_FORMAT	:= "Real Time:\t\t%e sec\nKernal Time:\t\t%S sec\nUser Time:\t\t%U sec\n"
	TIME		:= time -f $(TIME_FORMAT) ./$(OUTPUTMAIN) > /dev/null
endif

# Mac OS - Darwin
ifeq ($(shell uname), Darwin)
	FIXPATH 	:= $1
	SOURCEDIR	:= $(SOURCE)
	INCLUDEDIR	:= $(INCLUDE)
	OUTPUTDIR	:= $(BUILDDIR)/$(OUTPUT)
	OUTPUTMAIN	:= $(OUTPUTDIR)/$(MAIN)
	ERRORLOG	:= $(BUILDDIR)/$(ERROR)/err.log
    # Compiler Flags
	LIBS		:= -lglfw -lpanel -lncurses
	CXXFLAGS	+= -fsanitize=address -fsanitize=undefined
    # Commands
	MKDIR		:= mkdir -p
	RM 			:= rm -f
	DISPLAY		:= echo
	CLEAR		:= clear
	TIME_FORMAT	:= "Real Time:\t\t%e sec\nKernal Time:\t\t%S sec\nUser Time:\t\t%U sec\n"
	TIME		:= time -f $(TIME_FORMAT) ./$(OUTPUTMAIN) > /dev/null
endif


# Source Files
SOURCES			:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIR)))

# Include Directories
INCLUDES		:= $(patsubst %,-I%, $(INCLUDEDIR:%/=%))

# Object Files
OBJECTS			:= $(addprefix $(BUILDDIR)/, $(SOURCES:.cpp=.o))


compile: $(OUTPUT) $(MAIN)
	@$(DISPLAY) "\n\033[38;5;15m[ \033[38;5;46mCompiled\033[38;5;15m ]\033[0m\n"

$(OUTPUT):
	@$(MKDIR) $(BUILDDIR)/$(SOURCEDIR)
	@$(MKDIR) $(OUTPUTDIR)
	@$(MKDIR) $(BUILDDIR)/$(ERROR)

$(MAIN): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $(OUTPUTMAIN) $(OBJECTS) 2> $(ERRORLOG)

$(BUILDDIR)/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 2> $(ERRORLOG)
	

.PHONY: help build debug clean run time

help:
	@$(CLEAR)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mRules \033[38;5;15m]\n"
	@$(DISPLAY) "\033[0;0;0mmake \033[38;5;15mbuild\033[0;0;0m : Builds program.\n"
	@$(DISPLAY) "\033[0;0;0mmake \033[38;5;15mclean\033[0;0;0m : Cleans executables.\n"
	@$(DISPLAY) "\033[0;0;0mmake [\033[38;5;15mcompile\033[0;0;0m] : Compiles program.\n"
	@$(DISPLAY) "\033[0;0;0mmake \033[38;5;15mdebug\033[0;0;0m : Debugs program.\n"
	@$(DISPLAY) "\033[0;0;0mmake \033[38;5;15mrun\033[0;0;0m : Runs program.\n"
	@$(DISPLAY) "\033[0;0;0mmake \033[38;5;15mtime\033[0;0;0m : Measures program execution time.\n"

build: compile run

remake: clean compile

rebuild: clean compile run

debug: # Need fixing
	@$(CLEAR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUG) $(SRCS) $(LIBS) -o $(OUTPUTS) 2> $(ERRORLOG)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mDebug Compiled\033[38;5;15m ]\033[0m\n"


clean:
	@$(RM) $(OUTPUTMAIN) $(OBJECTS)
	@$(DISPLAY) "\n\033[38;5;15m[ \033[38;5;46mCleaned\033[38;5;15m ]\033[0m\n"

run:
	@$(CLEAR)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mRunning Program\033[38;5;15m ]\033[0m\n"
	@./$(OUTPUTDIR)/$(MAIN) $(ARGS) 2> $(ERRORLOG)
	@$(DISPLAY) "\n\033[38;5;15m[ \033[38;5;46mProgram Finished\033[38;5;15m ]\033[0m\n"

time:
	@$(CLEAR)
	@$(DISPLAY) "\n\033[38;5;15m[ \033[38;5;46mProgram Analysis\033[38;5;15m ]\033[0m\n"
	@$(SH)

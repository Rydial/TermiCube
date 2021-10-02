# CPP Compiler
CXX 		:= g++

# Compile-time Flags
CXXFLAGS	:= -std=c++20 -Wall -Wextra -Wvla -Weffc++ -Wsign-conversion -Werror

# Source Directory
SRC	:= src

# Include Directory
INCLUDE	:= include

# Output Directory
OUTPUT	:= output

# Program command-line arguments
ARGS		:=

# Debugging Flags
DEBUG		:= -g -DDEBUG

# Program Executable
EXEC		:= App


# Windows OS
ifeq ($(OS),Windows_NT)
	MAIN		:= $(OUTPUT)/$(EXEC).exe
	LIBS		:= -lglfw3 -lopengl32 -lglew32
  	#LIBS		+= -lgdi32
    #LIBS		+= -lglu32
endif

# Linux OS
ifeq ($(shell uname), Linux)
	MAIN		:= $(OUTPUT)/$(EXEC)
	SOURCEDIRS	:= $(shell find $(SRC) -type d)
	INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
	LIBS		:= -lglfw -lGL
	CXXFLAGS	+= -fsanitize=address -fsanitize=undefined
    # Commands
	RM 			:= rm -f
	DISPLAY		:= echo
	CLEAR		:= clear
	TIME_FORMAT	:= "Real Time:\t\t%e sec\nKernal Time:\t\t%S sec\nUser Time:\t\t%U sec\n"
	SH			:= time -f $(TIME_FORMAT) ./$(MAIN) > /dev/null
endif

# Mac OS - Darwin
ifeq ($(shell uname), Darwin)
	MAIN		:= $(OUTPUT)/$(EXEC)
	SOURCEDIRS	:= $(SRC)
	INCLUDEDIRS	:= $(INCLUDE)
	LIBS		:= -lglfw
	CXXFLAGS	+= -fsanitize=address -fsanitize=undefined
	# Commands
	RM 			:= rm -f
	DISPLAY		:= echo
	CLEAR		:= clear
	TIME_FORMAT	:= "Real Time:\t\t%e sec\nKernal Time:\t\t%S sec\nUser Time:\t\t%U sec\n"
	SH			:= time -f $(TIME_FORMAT) ./$(MAIN) > /dev/null
endif


# Source Files
SRCS		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# Include Directories
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# Output Files
OUTPUTS		:= $(MAIN)

compile:
	@$(CLEAR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) $(LIBS) -o $(OUTPUTS)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mCompiled\033[38;5;15m ]\033[0m\n"

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

build: clean compile run

debug:
	@$(CLEAR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEBUG) $(SRCS) $(LIBS) -o $(OUTPUTS)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mDebug Compiled\033[38;5;15m ]\033[0m\n"


clean:
	@$(CLEAR)
	@$(RM) $(OUTPUTS)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mCleaned\033[38;5;15m ]\033[0m\n"

run:
	@$(CLEAR)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mRunning Program\033[38;5;15m ]\033[0m\n"
	@$(MAIN) $(ARGS)
	@$(DISPLAY) "\n\033[38;5;15m[ \033[38;5;46mProgram Finished\033[38;5;15m ]\033[0m\n"

time:
	@$(CLEAR)
	@$(DISPLAY) "\033[38;5;15m[ \033[38;5;46mProgram Analysis\033[38;5;15m ]\033[0m\n"
	@$(SH)

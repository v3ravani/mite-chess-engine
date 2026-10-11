# Mite Chess Engine — Makefile
# Author: Viraj Ravani

# Compiler and configurable flags
CC      ?= gcc
CFLAGS  ?= -O3 -fomit-frame-pointer

# Directories and files
SRC_DIR ?= src
BIN_DIR ?= bin
SRC     := $(SRC_DIR)/Mite.c

# Platform-specific paths and commands
ifeq ($(OS),Windows_NT)
    TARGET    := $(BIN_DIR)/Mite.exe
    MKDIR     := if not exist $(BIN_DIR) mkdir $(BIN_DIR)
    RM        := if exist $(subst /,\,$(TARGET)) del /Q /F $(subst /,\,$(TARGET))
else
    TARGET    := $(BIN_DIR)/Mite
    MKDIR     := mkdir -p $(BIN_DIR)
    RM        := rm -f $(TARGET)
endif

# Default target
.PHONY: all clean help

all: $(TARGET)

$(TARGET): $(SRC)
	@$(MKDIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@$(RM)
	@echo Cleaned build artifacts.

help:
	@echo Mite Chess Engine Build System
	@echo ------------------------------
	@echo Targets:
	@echo   all    - Compile Mite (default: $(TARGET))
	@echo   clean  - Remove compiled binary
	@echo   help   - Display this help message
	@echo.
	@echo Configurable variables:
	@echo   CC     - C compiler (default: gcc)
	@echo   CFLAGS - Optimization flags (default: -O3 -fomit-frame-pointer)
	@echo.
	@echo Examples:
	@echo   mingw32-make
	@echo   mingw32-make clean
	@echo   mingw32-make CC=clang CFLAGS="-O3 -flto"
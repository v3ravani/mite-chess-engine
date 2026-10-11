@echo off
rem Build script for Windows to compile Mite chess engine into WebAssembly using Emscripten
cd /d "%~dp0\.."

echo Compiling Mite to WebAssembly...
call emcc -O3 ^
  -s WASM=1 ^
  -s EXPORTED_FUNCTIONS="['_process_command', '_main']" ^
  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" ^
  -s STACK_SIZE=4194304 ^
  -s INITIAL_MEMORY=33554432 ^
  -s ALLOW_MEMORY_GROWTH=1 ^
  -s MODULARIZE=1 ^
  -s "EXPORT_NAME=""MiteModule""" ^
  -s ENVIRONMENT="web,worker,node" ^
  engine/mite.c ^
  -o engine/mite.js

if %ERRORLEVEL% EQU 0 (
    echo Successfully built engine/mite.js and engine/mite.wasm
) else (
    echo Compilation failed with error code %ERRORLEVEL%
)

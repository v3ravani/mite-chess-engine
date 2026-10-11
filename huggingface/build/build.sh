#!/usr/bin/env bash
# Build script to compile Mite chess engine into WebAssembly using Emscripten
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$DIR"

echo "Compiling Mite to WebAssembly..."
emcc -O3 \
  -s WASM=1 \
  -s EXPORTED_FUNCTIONS="['_process_command', '_main']" \
  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
  -s STACK_SIZE=4194304 \
  -s INITIAL_MEMORY=33554432 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MODULARIZE=1 \
  -s 'EXPORT_NAME="MiteModule"' \
  -s ENVIRONMENT='web,worker,node' \
  engine/mite.c \
  -o engine/mite.js

echo "Successfully built engine/mite.js and engine/mite.wasm"

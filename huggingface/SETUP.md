# Mite Chess Engine - Hugging Face Space Setup & Deployment Guide

This guide covers building, testing, locally running, and deploying the **Mite Chess Engine** Hugging Face Static Space.

---

## 1. Prerequisites & Toolchain

To recompile the WebAssembly binary from source, you will need:

1. **Emscripten SDK (emsdk)**: Version 3.x or 4.x (or 6.x).
   - [Download Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
   - Ensure `emcc` is accessible in your `PATH`.
2. **Local HTTP Server**: Any lightweight static server:
   - Python: `python -m http.server`
   - Node.js: `npx serve .` or `npx http-server .`
   - VS Code / Antigravity: "Live Server" extension
3. **Modern Web Browser**: Chrome, Edge, Firefox, Safari (all support WebAssembly, Web Workers, and Web Audio API).

> **Note**: Pre-compiled WebAssembly files (`engine/mite.js` and `engine/mite.wasm`) are already provided inside `engine/`. You do not need Emscripten if you only wish to deploy or run the pre-built Space!

---

## 2. Compiling the Engine to WebAssembly

If you make modifications to `engine/mite.c`, recompile the WebAssembly module using the included build scripts.

### Windows (PowerShell or Command Prompt)

First, activate your Emscripten environment:
```cmd
call C:\path\to\emsdk\emsdk_env.bat
```

Then run the Windows build script:
```cmd
huggingface\build\build.bat
```

### Linux / macOS

```bash
source /path/to/emsdk/emsdk_env.sh
chmod +x huggingface/build/build.sh
./huggingface/build/build.sh
```

### Compilation Flags Explained

```bash
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
```

- `-O3`: Maximum compiler optimizations for fast move search.
- `-s WASM=1`: Generates binary WebAssembly (`.wasm`) alongside JavaScript loader (`.js`).
- `-s EXPORTED_FUNCTIONS`: Exports the UCI command processor `_process_command` and runtime entry point `_main`.
- `-s STACK_SIZE=4194304`: 4 MB stack allocation for deep recursive minimax and quiescence search.
- `-s INITIAL_MEMORY=33554432`: 32 MB initial heap to comfortably fit the 16 MB transposition table and engine state.
- `-s MODULARIZE=1` & `-s EXPORT_NAME="MiteModule"`: Wraps the engine as a clean, self-contained factory function that can be safely instantiated inside Web Workers.
- `-s ENVIRONMENT='web,worker,node'`: Enables execution in browser workers and Node.js testing environments.

---

## 3. Running Locally Through an HTTP Server

WebAssembly files and Web Workers cannot be loaded via `file:///` URLs due to browser Cross-Origin (CORS) security restrictions. You must run a local HTTP server from the `huggingface/` directory.

### Using Python:

```bash
cd huggingface
python -m http.server 8080
```
Open your browser to: [http://localhost:8080](http://localhost:8080)

### Using Node.js:

```bash
cd huggingface
npx serve -l 8080
```

---

## 4. Testing UCI Communication & Engine Responses

### Testing in Node.js (Headless)

You can verify that the WebAssembly binary correctly handles UCI commands without launching a browser:

```bash
cd huggingface
node -e "const MiteModule = require('./engine/mite.js'); (async () => { const engine = await MiteModule({ print: t => console.log(t) }); engine.ccall('process_command', null, ['string'], ['uci']); engine.ccall('process_command', null, ['string'], ['position startpos moves e2e4']); engine.ccall('process_command', null, ['string'], ['go movetime 200']); })();"
```

Expected output:
```text
id name Mite
id author Viraj Ravani
uciok
info depth ...
bestmove ...
```

### Testing in the Browser

1. Open `http://localhost:8080` in Chrome, Firefox, or Edge.
2. Open the Browser Developer Console (`F12` or `Ctrl+Shift+I` -> **Console**).
3. Check the console logs:
   - `Mite WebAssembly Engine ready` indicates successful worker instantiation.
4. Make a move on the board (e.g., `e2` to `e4`):
   - Status bar will display: `Mite is thinking...`
   - Evaluation bar will update with search depth, evaluation score, nodes evaluated, and NPS.
   - Mite will return a valid response (e.g., `c7c5` or `e7e5`).

---

## 5. Troubleshooting & Debugging

| Issue | Cause | Solution |
| :--- | :--- | :--- |
| `Failed to load resource: net::ERR_FILE_NOT_FOUND` on `.wasm` | Opening `index.html` via `file:///` | Run a local HTTP server (`python -m http.server 8080`) |
| `TypeError: Failed to fetch / CORS error` | Missing local HTTP server | Always serve the directory through HTTP/HTTPS |
| Engine does not move / clock frozen | Web Worker script blocked | Ensure modern browser with Web Workers enabled |
| Pieces not appearing | Relative asset paths incorrect | Check that all SVG pieces exist in `assets/pieces/` |

---

## 6. Uploading to Hugging Face Static Space

A Hugging Face Static Space serves files directly from the **root** of the Space repository.

### Step-by-Step Deployment:

1. **Create a Free Space on Hugging Face**:
   - Go to [huggingface.co/spaces](https://huggingface.co/spaces) and click **Create new Space**.
   - Set **Space Name** (e.g., `mite-chess`).
   - Set **Space SDK** to **Static** (HTML/CSS/JS).
   - Set **License** to `MIT`.
   - Set **Visibility** to **Public**.
   - Click **Create Space**.

2. **Clone your Hugging Face Space repository locally**:
   ```bash
   git clone https://huggingface.co/spaces/<YOUR_USERNAME>/<SPACE_NAME> hf-space-repo
   cd hf-space-repo
   ```

3. **Copy the CONTENTS of `huggingface/` into the root of the Space repository**:
   > **Important**: Do not copy the `huggingface/` folder itself. Copy the files and directories inside it so that `index.html` and `README.md` are at the root of the Space!

   **Windows PowerShell:**
   ```powershell
   Copy-Item -Path "c:\Users\ravan\chessengine\huggingface\*" -Destination "." -Recurse -Force
   ```

   **Linux / macOS:**
   ```bash
   cp -r /path/to/chessengine/huggingface/* .
   ```

4. **Verify the Space directory layout**:
   The root of `hf-space-repo` should look like:
   ```text
   ├── README.md        (Contains frontmatter: sdk: static)
   ├── SETUP.md
   ├── index.html
   ├── styles/
   ├── scripts/
   ├── engine/
   ├── assets/
   └── build/
   ```

5. **Commit and Push to Hugging Face**:
   ```bash
   git add .
   git commit -m "Deploy Mite Chess Engine Static Space"
   git push origin main
   ```

6. **Verify Live Deployment**:
   - Navigate to `https://huggingface.co/spaces/<YOUR_USERNAME>/<SPACE_NAME>`.
   - Hugging Face will immediately host your static site.
   - Test on desktop and mobile browsers.
   - Confirm that moves are calculated by WebAssembly Mite, clocks count down, audio triggers, and board flips work smoothly over HTTPS.

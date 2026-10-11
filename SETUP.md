# Mite Chess Engine — Setup, Compilation & Testing Guide

A step-by-step, practical guide for Windows developers to compile the **Mite** chess engine, verify its Universal Chess Interface (UCI) functionality, and reproduce automated benchmark matches against reference engines using Cute Chess CLI and graphical interfaces.

---

## Table of Contents

- [1. Prerequisites](#1-prerequisites)
  - [1.1 C Compiler (MinGW-w64 / GCC)](#11-c-compiler-mingw-w64--gcc)
  - [1.2 Cute Chess CLI](#12-cute-chess-cli)
  - [1.3 Optional Tools](#13-optional-tools)
- [2. Compile the Engine](#2-compile-the-engine)
  - [2.1 Building with the Makefile (Recommended)](#21-building-with-the-makefile-recommended)
  - [2.2 Manual Step-by-Step Compilation](#22-manual-step-by-step-compilation)
  - [2.3 Verifying Build Success and Binary Size](#23-verifying-build-success-and-binary-size)
  - [2.4 Alternative Compilers (Clang, MSVC)](#24-alternative-compilers-clang-msvc)
  - [2.5 Resolving Common Compiler Errors](#25-resolving-common-compiler-errors)
- [3. Run Mite](#3-run-mite)
  - [3.1 Interactive UCI Handshake](#31-interactive-uci-handshake)
  - [3.2 Automated One-Line UCI Verification](#32-automated-one-line-uci-verification)
  - [3.3 Understanding UCI vs. Terminal Play](#33-understanding-uci-vs-terminal-play)
  - [3.4 Loading Mite into a Chess GUI](#34-loading-mite-into-a-chess-gui)
- [4. Test Matches with Cute Chess CLI](#4-test-matches-with-cute-chess-cli)
  - [4.1 Command Structure & Essential Arguments](#41-command-structure--essential-arguments)
  - [4.2 Obtaining and Placing Opponent Engines](#42-obtaining-and-placing-opponent-engines)
  - [4.3 Smoke Test (4-Game Verification)](#43-smoke-test-4-game-verification)
  - [4.4 Mite vs. Stockfish (2000 Elo Benchmark)](#44-mite-vs-stockfish-2000-elo-benchmark)
  - [4.5 Mite vs. TSCP (Benchmark Match)](#45-mite-vs-tscp-benchmark-match)
  - [4.6 Mite vs. Fairy-Max (Benchmark Match)](#46-mite-vs-fairy-max-benchmark-match)
  - [4.7 Matches with Opening Books and Increments](#47-matches-with-opening-books-and-increments)
- [5. Configure Other Testing Platforms](#5-configure-other-testing-platforms)
  - [5.1 Cute Chess GUI](#51-cute-chess-gui)
  - [5.2 Arena Chess GUI](#52-arena-chess-gui)
  - [5.3 Banksia GUI](#53-banksia-gui)
  - [5.4 Online & Competitive Platforms (Code Arena, Lichess)](#54-online--competitive-platforms-code-arena-lichess)
  - [5.5 In-Browser Web Play (Hugging Face Static Space)](#55-in-browser-web-play-hugging-face-static-space)
- [6. Analyze Results](#6-analyze-results)
  - [6.1 Reading Cute Chess CLI Output](#61-reading-cute-chess-cli-output)
  - [6.2 Calculating Scores & Elo Differences](#62-calculating-scores--elo-differences)
  - [6.3 Evaluating White vs. Black Performance](#63-evaluating-white-vs-black-performance)
  - [6.4 Inspecting PGN Game Terminations](#64-inspecting-pgn-game-terminations)
  - [6.5 Updating Results Documentation](#65-updating-results-documentation)
- [7. Troubleshooting](#7-troubleshooting)
- [8. Suggested Testing Workflow](#8-suggested-testing-workflow)

---

## 1. Prerequisites

To build and run matches on Windows 10 or 11, you need a standard C compiler and the Cute Chess command-line tool.

### 1.1 C Compiler (MinGW-w64 / GCC)

Mite is written in standard ANSI C (`src/Mite.c`) and requires no external third-party libraries. A modern MinGW-w64 GCC toolchain is recommended.

#### Option A: Install via Windows Package Manager (winget) — Recommended
Open Command Prompt or PowerShell as Administrator and run:
```cmd
winget install BrechtSanders.WinLibs.POSIX.UCRT
```
Restart your terminal after installation so that `gcc` is available in your system `PATH`.

#### Option B: Install via MSYS2
1. Download and run the installer from [msys2.org](https://www.msys2.org/).
2. Open the MSYS2 UCRT64 terminal and install GCC:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc
   ```
3. Add `C:\msys64\ucrt64\bin` to your Windows user `PATH` environment variable.

#### Verification
In Command Prompt or PowerShell, verify that GCC is accessible:
```cmd
gcc --version
```
*Expected output: `gcc.exe (MinGW-W64 ...) 11.x` or higher.*

---

### 1.2 Cute Chess CLI

[Cute Chess](https://github.com/cutechess/cutechess) is the standard tournament director tool used across computer chess for running automated matches between engines.

1. Download the latest Windows release archive (e.g., `cutechess-1.5.1-win64.zip`) from the official [Cute Chess Releases page](https://github.com/cutechess/cutechess/releases).
2. Extract the archive to a convenient directory on your machine, for example:
   ```
   C:\tools\cutechess\
   ```
3. (Optional) Add `C:\tools\cutechess\` to your system `PATH`, or reference the executable directly using its full path:
   ```cmd
   C:\tools\cutechess\cutechess-cli.exe --version
   ```
*Expected output: `cutechess-cli 1.5.1 ...`*

---

### 1.3 Optional & Build Tools

| Tool | Purpose | Status |
| :--- | :--- | :--- |
| **GNU Make (`mingw32-make`)** | Build automation via the included `Makefile`. | Bundled with WinLibs MinGW / MSYS2 |
| **Arena Chess GUI** | Visual desktop interface for playing against Mite or watching engine matches. | Optional ([playwitharena.de](http://www.playwitharena.de/)) |
| **Cute Chess GUI** | Graphical counterpart to Cute Chess CLI for visual engine-vs-engine play. | Included in the Cute Chess release zip |
| **Banksia GUI** | Modern cross-platform chess GUI for match management and analysis. | Optional ([banksiagui.com](https://banksiagui.com/)) |
| **Git** | Cloning the repository and managing version control. | Recommended |

---

## 2. Compile the Engine

Mite is designed as a single monolithic C source file: `src/Mite.c`. Compilation produces a standalone static binary without DLL dependencies.

### 2.1 Building with the Makefile (Recommended)

Mite includes a clean, portable `Makefile` configured for Windows (MinGW GCC) and POSIX systems. It automatically creates the `bin/` directory if missing and compiles the binary.

From Command Prompt or PowerShell in the repository root:

#### Default Build
Compile `bin/Mite.exe` with GCC `-O3 -fomit-frame-pointer`:
```cmd
mingw32-make
```
*(On POSIX systems or environments with standard `make`, simply run `make`)*

#### Clean Build Artifacts
To remove the compiled binary:
```cmd
mingw32-make clean
```

#### View Build Options & Help
Display all available targets and configurable variables:
```cmd
mingw32-make help
```

#### Custom Compiler & Flags
You can override the default compiler and optimization flags directly:
```cmd
mingw32-make CC=clang CFLAGS="-O3 -flto"
```

---

### 2.2 Manual Step-by-Step Compilation

If you prefer compiling directly without Make:

Open Command Prompt (`cmd.exe`) and navigate to your repository root:
```cmd
cd C:\Users\ravan\chessengine
```

#### Step 1: Create the Output Directory
Ensure the `bin/` directory exists:
```cmd
if not exist bin mkdir bin
```
*(In PowerShell: `if (!(Test-Path bin)) { New-Item -ItemType Directory -Path bin }`)*

#### Step 2: Compile with GCC
Run GCC with level-3 optimization (`-O3`) and frame-pointer omission (`-fomit-frame-pointer`):
```cmd
gcc -O3 -fomit-frame-pointer src/Mite.c -o bin/Mite.exe
```

> **Why these flags?**
> - `-O3`: Activates aggressive loop unrolling, function inlining, and SIMD/instruction scheduling critical for minimax game-tree search speed.
> - `-fomit-frame-pointer`: Frees up an extra general-purpose CPU register for evaluation and move generation.
> - Optional `-static`: If your MinGW installation does not bundle runtime libraries into system directories, add `-static` to produce a fully self-sufficient binary:
>   ```cmd
>   gcc -O3 -fomit-frame-pointer -static src/Mite.c -o bin/Mite.exe
>   ```

---

### 2.3 Verifying Build Success and Binary Size

#### Check Exit Code
In Command Prompt:
```cmd
echo %ERRORLEVEL%
```
*(In PowerShell: `echo $LASTEXITCODE`)*  
A value of `0` indicates successful compilation without fatal errors.

#### Verify the Executable File Size
Mite is designed to fit well under 220 KB. To inspect the exact compiled size:

**In Command Prompt:**
```cmd
dir bin\Mite.exe
```

**In PowerShell:**
```powershell
Get-Item bin\Mite.exe | Select-Object Name, Length, @{Name="Size (KB)";Expression={[math]::Round($_.Length/1KB, 2)}}
```
*Expected result: File size is approximately **~115 KB** (117,987 bytes with MinGW GCC 16.2), well under the 220 KB ceiling.*

---

### 2.4 Alternative Compilers (Clang, MSVC)

If you prefer using LLVM/Clang or Microsoft Visual Studio:

#### Clang (LLVM on Windows)
```cmd
clang -O3 -flto src/Mite.c -o bin/Mite.exe
```

#### Microsoft Visual C++ (MSVC / Developer Command Prompt)
Open `x64 Native Tools Command Prompt for VS` and execute:
```cmd
cl /O2 /Oi /Ot /GS- src\Mite.c /Fe:bin\Mite.exe
```
- `/O2`: Maximize speed.
- `/Oi`: Enable compiler intrinsic functions.
- `/Ot`: Favor fast code over small code.
- `/GS-`: Disable buffer security check overhead in the hot search loop.

---

### 2.5 Resolving Common Compiler Errors

1. **`'gcc' is not recognized as an internal or external command`**
   - **Cause**: The compiler binary directory is not in your system `PATH`.
   - **Fix**: Re-check your MinGW installation path (e.g., `C:\msys64\ucrt64\bin` or the WinLibs path) and add it to `PATH`. Open a new command prompt session.

2. **`Permission denied` / `Cannot open output file bin/Mite.exe`**
   - **Cause**: A previous instance of `Mite.exe` is currently running in background, or is locked by a GUI/testing runner.
   - **Fix**: Terminate running instances via task manager or command line:
     ```cmd
     taskkill /F /IM Mite.exe
     ```
     Then re-run the compilation command.

3. **`fatal error: windows.h: No such file or directory`**
   - **Cause**: An incomplete or non-Windows GCC build is being used.
   - **Fix**: Mite includes `<windows.h>` on Windows systems to call `GetTickCount()`. Ensure you are using MinGW-w64 targeting Windows, not a bare-metal cross compiler.

---

## 3. Run Mite

Mite is a pure **Universal Chess Interface (UCI)** engine. It communicates via standard input (`stdin`) and standard output (`stdout`).

### 3.1 Interactive UCI Handshake

You can launch `bin\Mite.exe` directly in the terminal to inspect its text protocol interface:

```cmd
bin\Mite.exe
```
When running, type the following UCI commands followed by `Enter`:

```
uci
```
**Mite's response:**
```
id name Mite
id author Viraj Ravani
uciok
```

Test engine readiness:
```
isready
```
**Mite's response:**
```
readyok
```

Instruct the engine to evaluate the standard starting position to depth 5:
```
position startpos
go depth 5
```
**Mite's response:**
```
info depth 1 score cp 87 time 0 nodes 42 nps 0 pv d2d4
info depth 2 score cp 2 time 0 nodes 135 nps 0 pv d2d4
info depth 3 score cp 74 time 0 nodes 409 nps 0 pv d2d4
info depth 4 score cp 22 time 0 nodes 1520 nps 0 pv e2e4
info depth 5 score cp 65 time 3 nodes 5110 nps 1703333 pv e2e4
bestmove e2e4
```

To close the engine cleanly:
```
quit
```

---

### 3.2 Automated One-Line UCI Verification

To quickly verify that the compiled executable responds properly without manual typing:

**In Command Prompt:**
```cmd
(echo uci & echo isready & echo quit) | bin\Mite.exe
```

**In PowerShell:**
```powershell
"uci", "isready", "quit" | .\bin\Mite.exe
```
*If both `uciok` and `readyok` are output before the process exits, Mite is operating correctly.*

---

### 3.3 Understanding UCI vs. Terminal Play

> **Important**: Launching `Mite.exe` by double-clicking in Windows Explorer will open a black console window that appears to hang without printing a chessboard.  
> **This is normal and expected.** UCI chess engines do not print graphical boards or interact with keyboard players directly; they are headless background calculation processes designed to be controlled by chess GUIs (like Arena) or tournament runners (like Cute Chess).

---

### 3.4 Loading Mite into a Chess GUI

To play against Mite on a graphical board:

#### Arena Chess GUI (Free)
1. Open Arena and navigate to **Engines** → **Install New Engine...**
2. Browse to `bin\Mite.exe` and click **Open**.
3. Select **UCI** when prompted for the protocol type.
4. Arena will handshake with Mite and register it in your engine list.
5. Click **Engine** → **Manage** to select Mite, or choose **Game** → **New Game** to start playing.

#### Cute Chess GUI
1. Open Cute Chess and go to **Tools** → **Settings** → **Engines**.
2. Click the **+** (Add) button and browse to `bin\Mite.exe`.
3. Cute Chess automatically sends `uci` and auto-populates the engine name as *Mite* and author as *Viraj Ravani*.
4. Click **OK**. You can now select Mite from the player dropdown in **Game** → **New Game**.

---

## 4. Test Matches with Cute Chess CLI

To reproduce the tournament benchmark results documented in [`benchmarks/overview.md`](benchmarks/overview.md), use `cutechess-cli.exe`.

### 4.1 Command Structure & Essential Arguments

A typical Cute Chess match command follows this structure:

```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=path\to\opponent.exe name=Opponent proto=uci ^
  -each tc=10+0.1 ^
  -games 200 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout match_results.pgn
```

#### Argument Explanations:
- `-engine cmd=...`: Sets the path to the engine binary.
  - `name=...`: Descriptive label used in PGN headers and console logs.
  - `proto=...`: Protocol used by the engine. Must be `uci` for Mite, or `xboard` (CECP) for older engines.
  - `dir=...`: Working directory for the engine (required by engines like Fairy-Max that load local `.ini` config files).
  - `option.X=Y`: Configures engine-specific options (e.g., `option.UCI_LimitStrength=true option.UCI_Elo=2000` for Stockfish).
- `-each`: Applies the following settings to both participating engines.
  - `tc=10+0.1`: Time control of 10 seconds base time plus 0.1-second increment per move.
  - `st=0.5`: Alternative fixed time control: exactly 0.5 seconds per move.
- `-games N`: Total number of games to play in the match series.
- `-repeat`: Pairs each opening twice so players alternate sides (Game 1: Engine A is White; Game 2: Engine B is White from identical opening).
- `-concurrency 1`: Runs matches sequentially (one game at a time). **Crucial on Windows** to prevent thread contention and timer inaccuracies.
- `-recover`: Automatically restarts an engine if it crashes or disconnects, allowing the tournament to finish without interruption.
- `-pgnout FILE`: Writes all moves, clocks, and results to a standard PGN file.

---

### 4.2 Obtaining and Placing Opponent Engines

To run the exact benchmark suites, obtain the reference opponents and place them in an accessible directory (e.g., `engines\` in your repo root or inside your Cute Chess folder):

1. **Stockfish**:
   - Download the official Windows build from [stockfishchess.org/download](https://stockfishchess.org/download/).
   - Place as `engines\stockfish.exe`.
2. **TSCP (Tom Kerrigan's Simple Chess Program)**:
   - Download the precompiled binary from [tckerrigan.com/Chess/TSCP](https://www.tckerrigan.com/Chess/TSCP/).
   - Place as `engines\tscp181.exe`. Note: TSCP is a **CECP / XBoard** engine (`proto=xboard`).
3. **Fairy-Max (H.G. Muller)**:
   - Download from the [Fairy-Max project page](https://home.hccnet.nl/h.g.muller/dwnldpage.html).
   - Place `fmax.exe` and its companion configuration file `fmax.ini` in `engines\`. Fairy-Max requires `proto=xboard` and its working directory set to where `fmax.ini` is located.

---

### 4.3 Smoke Test (4-Game Verification)

Before launching a 200-game match, run a 4-game smoke test against Stockfish to ensure paths and configurations are working:

**Command Prompt:**
```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=engines\stockfish.exe name=Stockfish2000 proto=uci option.UCI_LimitStrength=true option.UCI_Elo=2000 ^
  -each tc=1+0.05 ^
  -games 4 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout test_smoke.pgn
```

**PowerShell equivalent:**
```powershell
& cutechess-cli.exe `
  -engine cmd=bin\Mite.exe name=Mite proto=uci `
  -engine cmd=engines\stockfish.exe name=Stockfish2000 proto=uci option.UCI_LimitStrength=true option.UCI_Elo=2000 `
  -each tc=1+0.05 `
  -games 4 `
  -repeat `
  -concurrency 1 `
  -recover `
  -pgnout test_smoke.pgn
```

---

### 4.4 Mite vs. Stockfish (2000 Elo Benchmark)

To replicate the 200-game match against Stockfish calibrated to 2000 Elo at 10 seconds per move (`tc 10`):

```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=engines\stockfish.exe name=Stockfish2000 proto=uci option.UCI_LimitStrength=true option.UCI_Elo=2000 ^
  -each tc=10 ^
  -games 200 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout benchmarks\vs_Stockfish(2000 elo)\mite_vs_stockfish2000_tc10.pgn
```
*Reference Outcome: Mite scores 66.5% (+127 −61 =12) across 200 games.*

---

### 4.5 Mite vs. TSCP (Benchmark Match)

TSCP uses the XBoard / CECP protocol (`proto=xboard`):

```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=engines\tscp181.exe name=TSCP proto=xboard ^
  -each tc=10 ^
  -games 200 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout benchmarks\vs_TSCP\mite_vs_tscp_tc10.pgn
```
*Reference Outcome: Mite scores 86.5% (+165 −19 =16) across 200 games.*

---

### 4.6 Mite vs. Fairy-Max (Benchmark Match)

Fairy-Max uses `proto=xboard` and requires access to `fmax.ini`. Specify its directory using `dir=engines`:

```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=engines\fmax.exe dir=engines name=FairyMax proto=xboard ^
  -each tc=5 ^
  -games 200 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout benchmarks\vs_Fairy-Max\mite_vs_fairymax_tc5.pgn
```
*Reference Outcome: Mite scores 68.0% (+123 −51 =26) across 200 games.*

---

### 4.7 Matches with Opening Books and Increments

To prevent repetitive opening lines and ensure opening diversity, add an opening book file (such as a 2-ply or 8-ply PGN/EPD book):

```cmd
cutechess-cli.exe ^
  -engine cmd=bin\Mite.exe name=Mite proto=uci ^
  -engine cmd=engines\stockfish.exe name=Stockfish2000 proto=uci option.UCI_LimitStrength=true option.UCI_Elo=2000 ^
  -each tc=5+0.1 ^
  -openings file=openings_100.pgn format=pgn order=random ^
  -games 200 ^
  -repeat ^
  -concurrency 1 ^
  -recover ^
  -pgnout match_openings.pgn
```
- `-openings file=openings_100.pgn`: Path to opening file.
- `format=pgn`: Format (`pgn` or `epd`).
- `order=random`: Selects opening positions randomly from the suite.
- `-repeat`: Guarantees each opening is played once with Mite as White and once with Mite as Black.

---

## 5. Configure Other Testing Platforms

### 5.1 Cute Chess GUI

1. Launch `cutechess.exe`.
2. Go to **Tools** → **Settings** → **Engines** and add both `bin\Mite.exe` (UCI) and your opponent engine.
3. Select **Game** → **New Tournament**.
4. Add Mite and your chosen opponent to the tournament list.
5. In the **Tournament** tab, select **Round-Robin** or **Match**, specify the number of rounds, set the time control (e.g. 5 min + 2 sec), and check **Play each opening twice (alternate colors)**.
6. Click **Start**. The GUI will play out all games visually while recording results.

---

### 5.2 Arena Chess GUI

Arena features built-in automated match functionality:
1. Register Mite under **Engines** → **Install New Engine...**
2. Register your opponent engine.
3. Navigate to **Engines** → **Tournament** → **New Tournament**.
4. Select the two engines, set games (e.g., 100 or 200), check **Alternate Colors**, and define the time control.
5. Provide a PGN filename for recording and click **Start**.

---

### 5.3 Banksia GUI

Banksia GUI is another popular tournament manager for Windows:
1. Open Banksia GUI and select **Engines** → **Manage Engines** → **Add Engine**.
2. Select `bin\Mite.exe`. Banksia automatically probes the binary and detects its UCI protocol support.
3. Create a new tournament via **Tournament** → **New Tournament**, specify the engine roster, set rounds and time controls, and launch the match.

---

### 5.4 Online & Competitive Platforms (Code Arena, Lichess)

#### Online Competitive Coding Platforms (e.g., Code Arena, Codingame)
- **Compatibility Limitations**: Platforms that host "Code Arena" coding challenges typically run headless **Linux (x86_64 / ARM)** sandbox containers. They do not execute Windows PE (`.exe`) binaries directly.
- **Source Submission Constraints**: Many platforms require submitting source code within a single file under a strict size limit (often 50 KB or 100 KB). Mite's ANSI C source code (`src/Mite.c`, ~19.7 KB) easily satisfies these code-size limits.
- **Portability**: Because Mite includes POSIX fallback timing using `clock_gettime(CLOCK_MONOTONIC, ...)` when `_WIN32` is not defined, Mite can be compiled on Linux using `gcc -O3 src/Mite.c -o Mite` for environments that support standard C binaries.

#### Playing Online via Lichess Bot API
Mite can be hosted as an online bot on Lichess using the open-source [`lichess-bot`](https://github.com/ShashCode/lichess-bot) bridge:
1. Register a BOT account on [Lichess.org](https://lichess.org/).
2. Clone the `lichess-bot` Python client.
3. In `config.yml`, set the engine path to your local `bin/Mite.exe` with protocol `uci`.
4. Run the Python client to allow human players or other bots to challenge Mite in online rated matches.

---

### 5.5 In-Browser Web Play (Hugging Face Static Space)

Mite is compiled to WebAssembly and deployed as a free, public Hugging Face Static Space:

 **[https://huggingface.co/spaces/virajravani/mite-chess](https://huggingface.co/spaces/virajravani/mite-chess)**

- **No installation or downloads required**: Play live against Mite in any web browser on desktop, laptop, tablet, or smartphone.
- **WebAssembly & Web Worker**: Runs the actual C chess logic in WebAssembly (`engine/mite.wasm`, ~60 KB) inside a background Web Worker (~800,000–1,500,000 nodes/sec).
- **Space Source & Documentation**: Complete WebAssembly source code, build scripts, and deployment instructions are maintained in the [`huggingface/`](huggingface/) directory.

---

## 6. Analyze Results

### 6.1 Reading Cute Chess CLI Output

During and after a match, Cute Chess CLI prints real-time tournament standings:

```
Score of Mite vs Stockfish2000: 127 - 61 - 12  [0.665] 200
Elo difference: 119.1 +/- 49.6, LOS: 100.0 %, Draw ratio: 6.0 %
```

#### Breakdown of Terms:
- `127 - 61 - 12`: 127 Wins for Mite, 61 Losses for Mite (wins for opponent), 12 Draws.
- `[0.665]`: Normalized score (66.5%).
- `200`: Total completed games.
- `Elo difference`: Estimated rating difference based on match outcome.
- `LOS` (Likelihood of Superiority): The probability (100.0%) that Mite is genuinely stronger than the opponent rather than winning by statistical noise.
- `Draw ratio`: 12 draws / 200 games = 6.0%.

---

### 6.2 Calculating Scores & Elo Differences

To calculate Mite's score percentage from standard tournament results:

$$\text{Score \%} = \frac{\text{Wins} + 0.5 \times \text{Draws}}{\text{Total Games}} \times 100$$

#### Worked Example:
- Wins ($W$) = 127
- Losses ($L$) = 61
- Draws ($D$) = 12
- Total Games = 200

$$\text{Score \%} = \frac{127 + 0.5 \times 12}{200} \times 100 = \frac{133}{200} \times 100 = 66.5\%$$

---

### 6.3 Evaluating White vs. Black Performance

To assess whether the engine exhibits color bias, filter the output PGN file:

**In PowerShell:**
```powershell
$pgn = Get-Content match_results.pgn -Raw
$whiteWins = ([regex]::Matches($pgn, '\[White "Mite"\][\s\S]*?\[Result "1-0"\]')).Count
$whiteLosses = ([regex]::Matches($pgn, '\[White "Mite"\][\s\S]*?\[Result "0-1"\]')).Count
$whiteDraws = ([regex]::Matches($pgn, '\[White "Mite"\][\s\S]*?\[Result "1/2-1/2"\]')).Count

$blackWins = ([regex]::Matches($pgn, '\[Black "Mite"\][\s\S]*?\[Result "0-1"\]')).Count
$blackLosses = ([regex]::Matches($pgn, '\[Black "Mite"\][\s\S]*?\[Result "1-0"\]')).Count
$blackDraws = ([regex]::Matches($pgn, '\[Black "Mite"\][\s\S]*?\[Result "1/2-1/2"\]')).Count

Write-Host "Mite as White: +$whiteWins -$whiteLosses =$whiteDraws"
Write-Host "Mite as Black: +$blackWins -$blackLosses =$blackDraws"
```

---

### 6.4 Inspecting PGN Game Terminations

Open the resulting `.pgn` file in any text editor to verify game terminations:
- **Checkmate**: `1-0 {White mates}` or `0-1 {Black mates}`
- **Repetition**: `1/2-1/2 {3-fold repetition}`
- **Fifty-Move Rule**: `1/2-1/2 {50 moves without progress}`
- **Insufficient Material**: `1/2-1/2 {Draw by insufficient material}`
- **Time Forfeit**: `1-0 {Black loses on time}` or `0-1 {White loses on time}`
- **Crash / Disconnect**: `0-1 {White disconnects}`

*Note: Across all 2,600 verified tournament games in this repository, Mite experienced 0 crashes and 0 time losses.*

---

### 6.5 Updating Results Documentation

When contributing new benchmark matches:
1. Verify that the match completed all intended games without manual interruptions.
2. Record the PGN file in the appropriate directory under `benchmarks/`.
3. Update [`benchmarks/overview.md`](benchmarks/overview.md) with exact win, loss, draw counts and score percentages. Do not round or estimate statistics.

---

## 7. Troubleshooting

| Issue | Likely Cause | Solution |
| :--- | :--- | :--- |
| **`gcc: command not found`** | Compiler not installed or missing from Windows `PATH`. | Install MinGW-w64 via WinGet and restart terminal. Verify with `gcc --version`. |
| **`bin\Mite.exe: No such file`** | The `bin/` directory was not created before compiling. | Run `if not exist bin mkdir bin` before calling `gcc`. |
| **Engine crashes on launch** | Corrupted build or antivirus false positive. | Whitelist the project folder in Windows Defender. Recompile cleanly with `gcc -O3 -fomit-frame-pointer src/Mite.c -o bin/Mite.exe`. |
| **Cute Chess hangs on start** | Incorrect protocol specified for the opponent. | Verify protocol: `Mite` requires `proto=uci`. `TSCP` and `Fairy-Max` require `proto=xboard`. |
| **Fairy-Max fails to make moves** | Missing `fmax.ini` configuration file. | Ensure `fmax.ini` is located in the working directory specified by `dir=...`. |
| **Inaccurate time losses / timeouts** | High concurrency causing Windows CPU scheduling delays. | Always run matches with `-concurrency 1` when benchmarking strict time controls. |
| **Missing opening book error** | File path to openings PGN is incorrect or format is wrong. | Check relative path to opening file and confirm format matches (`format=pgn` or `format=epd`). |
| **Cute Chess game terminates with error** | Engine crashed during search. | Include `-recover` in your Cute Chess CLI command so remaining match games continue. Inspect stdout/stderr logs. |

---

## 8. Suggested Testing Workflow

Follow this standard workflow when testing modifications or validating playing strength:

1. **Compile**: Rebuild `bin/Mite.exe` using `gcc -O3 -fomit-frame-pointer src/Mite.c -o bin/Mite.exe`.
2. **Verify Size**: Confirm the compiled binary is approximately ~115 KB and under 220 KB.
3. **Verify UCI**: Send `uci` and `isready` via pipe to verify standard communication.
4. **Smoke Test**: Run a quick 4-game match with Cute Chess CLI (`-games 4 -repeat`) to confirm clock handling and move execution.
5. **Full Benchmark**: Execute a complete 200-game match with `-repeat`, `-concurrency 1`, and `-recover` at your target time control (`tc 1`, `tc 5`, `tc 10`, etc.).
6. **Archive**: Save all games to a descriptive PGN file in `benchmarks/`.
7. **Document**: Calculate exact scores using $\frac{W + 0.5 \times D}{N} \times 100$ and record the verified data in [`benchmarks/overview.md`](benchmarks/overview.md).

## Developer and Contact

Developed by **[Viraj Ravani](https://virajravani.in/)**.

- **Portfolio**: [virajravani.in](https://virajravani.in/)
- **Email**: [viraj.ravani@somaiya.edu](mailto:viraj.ravani@somaiya.edu)
- **LinkedIn**: [linkedin.com/in/virajravani](https://www.linkedin.com/in/virajravani/)

---
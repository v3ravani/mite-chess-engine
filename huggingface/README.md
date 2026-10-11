---
title: Mite Chess Engine
emoji: ♟️
colorFrom: yellow
colorTo: green
sdk: static
pinned: false
---

# Mite Chess Engine

**Mite** is a compact chess engine developed by **Viraj Ravani**, compiled directly to **WebAssembly** and hosted as a free, serverless Hugging Face Static Space.

Play live against the engine directly in your web browser with zero installation, zero server backend, and no external API dependencies!

---

## Features

- **Real Engine Logic in WebAssembly**: Powered by the genuine Mite C chess engine compiled via Emscripten into WebAssembly (`mite.wasm`, ~60 KB), calculating over 800,000 nodes/sec in your browser.
- **Dedicated Web Worker**: The chess engine runs in an isolated Web Worker thread, ensuring smooth 60 FPS board animations and uninterrupted clock timing while the engine calculates.
- **Pure UCI Protocol**: Uses standard Universal Chess Interface (`uci`, `isready`, `ucinewgame`, `position`, `go`, `stop`) communication.
- **Professional Chess Interface**: Fully responsive, clean dark-themed chessboard interface with high-quality PNG piece assets, legal move indicators, check highlights, and last-move highlights.
- **Accurate Clocks & Increments**: Bullet (1+0, 1+1), Blitz (3+0, 3+2, 5+0, 5+3), Rapid (10+0, 10+5, 15+10), Custom controls, and Casual unlimited time.
- **Synthesized Audio**: Latency-free Web Audio API sound effects for moves, captures, castling, checks, and game termination.
- **Captured Piece Trays & Material Counter**: Visual captured pieces display with material advantage tally.
- **Full Rules Enforcement**: Accurate handling of castling, en passant, pawn promotion selection, three-fold repetition, 50-move rule, checkmate, and stalemate.

---

## Playing Against Mite

1. **Select Color & Time**: Click **New Game** to choose playing as White, Black, or Random, alongside your desired time control and engine thinking preset.
2. **Move Pieces**: Click on a piece to highlight all legal destination squares and capture rings. Click a valid target square to execute your move.
3. **Controls**:
   - **Undo**: Retract the last move sequence and resynchronize the board and engine.
   - **Flip Board**: Invert perspective between White and Black.
   - **Sound Toggle**: Mute or enable sound effects.
   - **Resign / Draw**: Forfeit or claim a draw under repetition or 50-move rules.

---

## Repository Structure

```text
huggingface/
├── README.md              # Space configuration and project overview
├── SETUP.md               # Build, local running, and deployment guide
├── index.html             # Application entry point
├── styles/
│   └── style.css          # Responsive styling and board theme
├── scripts/
│   ├── chess.js           # Legal chess move validation and rules
│   ├── game.js            # Clocks, Web Audio, and UCI coordinator
│   ├── worker.js          # Web Worker running Mite WebAssembly
│   └── app.js             # UI board interactions and modals
├── engine/
│   ├── mite.c             # WebAssembly-adapted C engine source
│   ├── mite.js            # Emscripten JS loader
│   └── mite.wasm          # Compiled WebAssembly binary (~60 KB)
├── assets/
│   ├── mite_logo.png      # Engine branding
│   └── pieces/            # 12 high-resolution SVG chess pieces
└── build/
    ├── build.sh           # Linux / macOS Emscripten compilation script
    └── build.bat          # Windows Emscripten compilation script
```

---

## Deployment to Hugging Face Static Space

This space is built exclusively with standard Web standards (HTML5, Vanilla CSS, JavaScript, WebAssembly, Web Workers, and Web Audio API). It requires no Docker image, no server runtime, and no paid hardware tier.

For full build instructions and step-by-step deployment directions, refer to [SETUP.md](SETUP.md).

---

## License & Credits

- **Author**: Viraj Ravani
- **Engine**: Mite Chess Engine
- **License**: MIT

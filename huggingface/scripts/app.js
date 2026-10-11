/**
 * app.js - Professional UI Controller for Mite Chess Space
 * Responsive, restrained design, SVG icons, zero emojis, full rules integration.
 */

(function () {
  'use strict';

  // State
  let chess = null;
  let audio = null;
  let clock = null;
  let engine = null;

  const state = {
    playerColor: 'w',      // 'w' | 'b'
    boardFlipped: false,   // true if Black perspective
    timeControl: {
      type: 'blitz-3-2',
      minutes: 3,
      increment: 2,
      isUnlimited: false
    },
    engineSetting: {
      mode: 'fixed',
      movetime: 500
    },
    soundEnabled: true,
    isEngineThinking: false,
    selectedSquare: null,
    legalMovesForSelected: [],
    lastMove: null,        // { from, to }
    pendingPromotion: null,// { from, to }
    gameActive: false,
    undosRemaining: 3,     // max 3 undos per match
    moveHistoryUci: []     // ['e2e4', 'e7e5', ...]
  };

  // DOM Elements
  const el = {};

  function init() {
    // Cache Elements
    el.board = document.getElementById('board');
    el.notationList = document.getElementById('notation-list');
    el.gameStatusText = document.getElementById('game-status-text');
    el.turnDot = document.getElementById('turn-dot');
    
    // Tools
    el.soundBtn = document.getElementById('sound-btn');
    el.soundIconOn = document.getElementById('sound-icon-on');
    el.soundIconOff = document.getElementById('sound-icon-off');
    el.flipBtn = document.getElementById('flip-btn');
    el.newGameBtn = document.getElementById('new-game-btn');
    el.undoBtn = document.getElementById('undo-btn');
    el.undoCount = document.getElementById('undo-count');
    el.drawBtn = document.getElementById('draw-btn');
    el.resignBtn = document.getElementById('resign-btn');

    // Player Strips
    el.topName = document.getElementById('top-name');
    el.topBadge = document.getElementById('top-badge');
    el.topStatusPill = document.getElementById('top-status-pill');
    el.topClock = document.getElementById('top-clock');
    el.topCapturedBox = document.getElementById('top-captured-box');
    el.topMaterialLead = document.getElementById('top-material-lead');

    el.bottomName = document.getElementById('bottom-name');
    el.bottomBadge = document.getElementById('bottom-badge');
    el.bottomClock = document.getElementById('bottom-clock');
    el.bottomCapturedBox = document.getElementById('bottom-captured-box');
    el.bottomMaterialLead = document.getElementById('bottom-material-lead');

    // Diagnostics
    el.diagEval = document.getElementById('diag-eval');
    el.diagDepth = document.getElementById('diag-depth');
    el.diagNodes = document.getElementById('diag-nodes');
    el.diagNps = document.getElementById('diag-nps');

    // Modals
    el.newGameModal = document.getElementById('new-game-modal');
    el.closeNewGameBtn = document.getElementById('close-new-game-btn');
    el.startGameBtn = document.getElementById('start-game-btn');
    el.customTimeBox = document.getElementById('custom-time-box');
    el.customMinutes = document.getElementById('custom-minutes');
    el.customIncrement = document.getElementById('custom-increment');

    el.promotionModal = document.getElementById('promotion-modal');
    el.promoChoices = document.getElementById('promo-choices');

    el.resignModal = document.getElementById('resign-modal');
    el.confirmResignBtn = document.getElementById('confirm-resign-btn');
    el.cancelResignBtn = document.getElementById('cancel-resign-btn');

    el.drawModal = document.getElementById('draw-modal');
    el.drawModalHeading = document.getElementById('draw-modal-heading');
    el.drawModalDesc = document.getElementById('draw-modal-desc');
    el.closeDrawBtn = document.getElementById('close-draw-btn');

    el.gameOverModal = document.getElementById('game-over-modal');
    el.gameOverTitle = document.getElementById('game-over-title');
    el.gameOverReason = document.getElementById('game-over-reason');
    el.reviewBtn = document.getElementById('review-btn');
    el.rematchBtn = document.getElementById('rematch-btn');

    // Initialize dependencies
    chess = new Chess();
    audio = new ChessAudio();

    clock = new ChessClock(180, 2, (wMs, bMs) => {
      renderClocks(wMs, bMs);
    }, (flaggedColor) => {
      handleTimeFlag(flaggedColor);
    });

    engine = new MiteEngineClient();
    engine.onLoaded = () => {
      console.log('Mite WebAssembly Engine ready');
      el.topStatusPill.textContent = 'Ready';
      el.topStatusPill.className = 'status-pill';
    };
    engine.onBestMove = (moveStr) => {
      handleEngineMove(moveStr);
    };
    engine.onInfo = (info) => {
      updateDiagnostics(info);
    };
    engine.onError = (err) => {
      console.error('Engine error:', err);
      el.gameStatusText.textContent = 'Engine error: ' + err;
    };
    engine.init();

    // Attach UI Events
    attachEvents();

    // Start default match: White, 3+2 Blitz
    startMatch({
      playerColor: 'w',
      timeControl: { type: 'blitz-3-2', minutes: 3, increment: 2, isUnlimited: false },
      engineSetting: { mode: 'fixed', movetime: 500 }
    });
  }

  function attachEvents() {
    // Toolbar buttons
    el.newGameBtn.addEventListener('click', () => el.newGameModal.classList.add('is-open'));
    el.closeNewGameBtn.addEventListener('click', () => el.newGameModal.classList.remove('is-open'));
    el.startGameBtn.addEventListener('click', onConfirmNewGame);

    el.flipBtn.addEventListener('click', () => {
      state.boardFlipped = !state.boardFlipped;
      renderBoard();
    });

    el.soundBtn.addEventListener('click', () => {
      state.soundEnabled = !state.soundEnabled;
      audio.enabled = state.soundEnabled;
      el.soundBtn.classList.toggle('active', state.soundEnabled);
      el.soundIconOn.style.display = state.soundEnabled ? 'block' : 'none';
      el.soundIconOff.style.display = state.soundEnabled ? 'none' : 'block';
    });

    el.undoBtn.addEventListener('click', onUndoMove);

    el.drawBtn.addEventListener('click', onDrawClick);
    el.closeDrawBtn.addEventListener('click', () => el.drawModal.classList.remove('is-open'));

    el.resignBtn.addEventListener('click', () => {
      if (!state.gameActive) return;
      el.resignModal.classList.add('is-open');
    });
    el.confirmResignBtn.addEventListener('click', onConfirmResign);
    el.cancelResignBtn.addEventListener('click', () => el.resignModal.classList.remove('is-open'));

    el.reviewBtn.addEventListener('click', () => el.gameOverModal.classList.remove('is-open'));
    el.rematchBtn.addEventListener('click', () => {
      el.gameOverModal.classList.remove('is-open');
      el.newGameModal.classList.add('is-open');
    });

    // Time choice change
    const timeRadios = document.querySelectorAll('input[name="time-choice"]');
    timeRadios.forEach(r => {
      r.addEventListener('change', (e) => {
        if (e.target.value === 'custom') {
          el.customTimeBox.classList.remove('is-hidden');
        } else {
          el.customTimeBox.classList.add('is-hidden');
        }
      });
    });

    // Promotion piece select
    el.promoChoices.addEventListener('click', (e) => {
      const btn = e.target.closest('.promo-btn');
      if (!btn) return;
      const piece = btn.getAttribute('data-piece');
      if (state.pendingPromotion) {
        completePromotion(piece);
      }
    });

    // Board click
    el.board.addEventListener('click', onBoardSquareClick);

    // Keyboard shortcuts
    window.addEventListener('keydown', (e) => {
      if (e.key === 'f' || e.key === 'F') {
        state.boardFlipped = !state.boardFlipped;
        renderBoard();
      }
    });
  }

  function onConfirmNewGame() {
    const colorChoice = document.querySelector('input[name="color-choice"]:checked').value;
    let chosenColor = colorChoice;
    if (colorChoice === 'random') {
      chosenColor = Math.random() < 0.5 ? 'w' : 'b';
    }

    const timeChoice = document.querySelector('input[name="time-choice"]:checked').value;
    let mins = 3;
    let inc = 2;
    let isUnlimited = false;

    if (timeChoice === 'bullet-1-0') { mins = 1; inc = 0; }
    else if (timeChoice === 'bullet-1-1') { mins = 1; inc = 1; }
    else if (timeChoice === 'blitz-3-0') { mins = 3; inc = 0; }
    else if (timeChoice === 'blitz-3-2') { mins = 3; inc = 2; }
    else if (timeChoice === 'blitz-5-0') { mins = 5; inc = 0; }
    else if (timeChoice === 'blitz-5-3') { mins = 5; inc = 3; }
    else if (timeChoice === 'rapid-10-0') { mins = 10; inc = 0; }
    else if (timeChoice === 'rapid-15-10') { mins = 15; inc = 10; }
    else if (timeChoice === 'unlimited') { mins = 0; inc = 0; isUnlimited = true; }
    else if (timeChoice === 'custom') {
      mins = Math.max(0, parseInt(el.customMinutes.value, 10) || 5);
      inc = Math.max(0, parseInt(el.customIncrement.value, 10) || 0);
      if (mins === 0 && inc === 0) isUnlimited = true;
    }

    const speedChoice = document.querySelector('input[name="speed-choice"]:checked').value;
    let movetime = 500;
    let mode = 'fixed';

    if (speedChoice === 'fast') movetime = 150;
    else if (speedChoice === 'standard') movetime = 500;
    else if (speedChoice === 'deep') movetime = 1500;
    else if (speedChoice === 'tournament') movetime = 2500;
    else if (speedChoice === 'dynamic') mode = 'dynamic';

    el.newGameModal.classList.remove('is-open');

    startMatch({
      playerColor: chosenColor,
      timeControl: { type: timeChoice, minutes: mins, increment: inc, isUnlimited: isUnlimited },
      engineSetting: { mode: mode, movetime: movetime }
    });
  }

  function startMatch(config) {
    state.playerColor = config.playerColor;
    state.boardFlipped = (state.playerColor === 'b');
    state.timeControl = config.timeControl;
    state.engineSetting = config.engineSetting;
    state.selectedSquare = null;
    state.legalMovesForSelected = [];
    state.lastMove = null;
    state.pendingPromotion = null;
    state.gameActive = true;
    state.isEngineThinking = false;
    state.undosRemaining = 3;
    state.moveHistoryUci = [];

    // Reset chess logic
    chess.reset();

    // Reset clock
    clock.reset(config.timeControl.minutes * 60, config.timeControl.increment);
    renderClocks(clock.whiteTimeMs, clock.blackTimeMs);
    updateUndoUI();

    // Notify Mite
    engine.newGame();

    // Update Player Strips
    updatePlayerStripsHeader();

    // Reset Notation Sheet
    el.notationList.innerHTML = '';
    el.gameStatusText.textContent = 'White to move';
    el.turnDot.className = 'turn-dot';

    // Clear diagnostics
    el.diagEval.textContent = '0.00';
    el.diagDepth.textContent = 'd: -';
    el.diagNodes.textContent = '- n';
    el.diagNps.textContent = '- k/s';

    // Render board and captured trays
    renderBoard();
    updateCapturedTrays();

    // If player is Black, Mite plays first
    if (state.playerColor === 'b') {
      triggerEngineTurn();
    }
  }

  function updatePlayerStripsHeader() {
    const isWhite = (state.playerColor === 'w');

    // Bottom strip is always the Human
    el.bottomName.textContent = isWhite ? 'You (White)' : 'You (Black)';
    el.bottomBadge.className = 'player-badge ' + (isWhite ? 'badge-white' : 'badge-black');

    // Top strip is always Mite
    el.topName.textContent = isWhite ? 'Mite 2.0 (Black)' : 'Mite 2.0 (White)';
    el.topBadge.className = 'player-badge ' + (isWhite ? 'badge-black' : 'badge-white');
    el.topStatusPill.textContent = 'Ready';
    el.topStatusPill.className = 'status-pill';
  }

  function renderClocks(wMs, bMs) {
    const isWhite = (state.playerColor === 'w');
    const playerMs = isWhite ? wMs : bMs;
    const engineMs = isWhite ? bMs : wMs;

    el.bottomClock.textContent = clock.formatTime(playerMs);
    el.topClock.textContent = clock.formatTime(engineMs);

    // Active clock highlight
    const turn = chess.turn();
    const isPlayerActive = (turn === state.playerColor) && state.gameActive;
    const isEngineActive = (turn !== state.playerColor) && state.gameActive;

    el.bottomClock.classList.toggle('active-clock', isPlayerActive);
    el.topClock.classList.toggle('active-clock', isEngineActive);

    // Low time warning (< 30s)
    if (!state.timeControl.isUnlimited) {
      el.bottomClock.classList.toggle('low-time', playerMs < 30000 && playerMs > 0);
      el.topClock.classList.toggle('low-time', engineMs < 30000 && engineMs > 0);
    } else {
      el.bottomClock.classList.remove('low-time');
      el.topClock.classList.remove('low-time');
    }
  }

  function handleTimeFlag(flaggedColor) {
    if (!state.gameActive) return;
    state.gameActive = false;
    engine.stop();

    const winner = flaggedColor === 'w' ? 'Black' : 'White';
    audio.playGameOver();
    showGameOverDialog(`${winner} Won on Time`, `${flaggedColor === 'w' ? 'White' : 'Black'} ran out of time.`);
    updateStatusDisplay();
  }

  function onConfirmResign() {
    el.resignModal.classList.remove('is-open');
    if (!state.gameActive) return;

    state.gameActive = false;
    clock.pause();
    engine.stop();

    const winner = state.playerColor === 'w' ? 'Black (Mite)' : 'White (Mite)';
    audio.playGameOver();
    showGameOverDialog(`${winner} Won by Resignation`, 'You resigned the game.');
    updateStatusDisplay();
  }

  function onDrawClick() {
    if (!state.gameActive) return;

    // Check threefold repetition
    if (chess.in_threefold_repetition()) {
      state.gameActive = false;
      clock.pause();
      engine.stop();
      audio.playGameOver();
      showGameOverDialog('Draw by Repetition', 'Threefold repetition reached.');
      updateStatusDisplay();
      return;
    }

    // Check 50-move rule
    if (chess.in_draw()) {
      state.gameActive = false;
      clock.pause();
      engine.stop();
      audio.playGameOver();
      showGameOverDialog('Draw Claimed', '50-move rule satisfied.');
      updateStatusDisplay();
      return;
    }

    // Mutual draw request: explain draw condition
    el.drawModalHeading.textContent = 'Draw Offer Declined';
    el.drawModalDesc.textContent = 'Mite declines the draw offer. In competitive play, you may claim a draw when threefold repetition or the 50-move rule occurs.';
    el.drawModal.classList.add('is-open');
  }

  function updateUndoUI() {
    const count = state.undosRemaining;
    if (el.undoCount) {
      el.undoCount.textContent = count;
    }
    if (count <= 0) {
      el.undoBtn.disabled = true;
      el.undoBtn.classList.add('disabled');
      el.undoBtn.title = 'No undos remaining (max 3 per game)';
    } else {
      el.undoBtn.disabled = false;
      el.undoBtn.classList.remove('disabled');
      el.undoBtn.title = `Take back move (${count} remaining)`;
    }
  }

  function onUndoMove() {
    if (!state.gameActive) return;
    if (state.undosRemaining <= 0) return;

    // If engine is calculating, abort search
    if (state.isEngineThinking) {
      engine.stop();
      state.isEngineThinking = false;
      el.topStatusPill.textContent = 'Ready';
      el.topStatusPill.className = 'status-pill';
    }

    // Undo 2 half-moves if it's currently player's turn (reverting to player's previous turn)
    const isPlayerTurn = (chess.turn() === state.playerColor);
    const count = isPlayerTurn ? 2 : 1;

    let undone = 0;
    for (let i = 0; i < count; i++) {
      if (chess.history().length > 0) {
        chess.undo();
        state.moveHistoryUci.pop();
        undone++;
      }
    }

    if (undone > 0) {
      state.undosRemaining--;
      updateUndoUI();

      state.selectedSquare = null;
      state.legalMovesForSelected = [];

      const hist = chess.history({ verbose: true });
      if (hist.length > 0) {
        const last = hist[hist.length - 1];
        state.lastMove = { from: last.from, to: last.to };
      } else {
        state.lastMove = null;
      }

      rebuildNotationUI();
      renderBoard();
      updateCapturedTrays();
      updateStatusDisplay();

      clock.pause();
      if (chess.history().length > 0) {
        clock.start(chess.turn());
      }

      // If it's Mite's turn after undo, trigger search
      if (chess.turn() !== state.playerColor) {
        triggerEngineTurn();
      }
    }
  }

  function renderBoard() {
    el.board.innerHTML = '';

    const files = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];
    const ranks = ['8', '7', '6', '5', '4', '3', '2', '1'];

    const displayRanks = state.boardFlipped ? [...ranks].reverse() : ranks;
    const displayFiles = state.boardFlipped ? [...files].reverse() : files;

    const boardState = chess.board();
    const inCheck = chess.in_check();
    const turn = chess.turn();

    for (let r = 0; r < 8; r++) {
      const rank = displayRanks[r];
      const rankIdx = 8 - parseInt(rank, 10);

      for (let f = 0; f < 8; f++) {
        const file = displayFiles[f];
        const fileIdx = files.indexOf(file);
        const sq = file + rank;

        const sqDiv = document.createElement('div');
        sqDiv.className = 'square ' + ((r + f) % 2 === 1 ? 'dark' : 'light');
        sqDiv.setAttribute('data-sq', sq);

        // Highlight: last move
        if (state.lastMove && (state.lastMove.from === sq || state.lastMove.to === sq)) {
          sqDiv.classList.add('is-last-move');
        }

        // Highlight: selected square
        if (state.selectedSquare === sq) {
          sqDiv.classList.add('is-selected');
        }

        // Highlight: legal move hints
        const legal = state.legalMovesForSelected.some(m => m.to === sq);
        if (legal) {
          const target = chess.get(sq);
          sqDiv.classList.add(target ? 'hint-capture' : 'hint-move');
        }

        // Highlight: king in check
        const piece = boardState[rankIdx][fileIdx];
        if (piece && piece.type === 'k' && piece.color === turn && inCheck) {
          sqDiv.classList.add('is-check');
        }

        // Render Chess Piece PNG
        if (piece) {
          const img = document.createElement('img');
          const color = piece.color;
          const type = piece.type.toUpperCase();
          img.src = `assets/pieces/${color}${type}.png`;
          img.alt = `${color === 'w' ? 'White' : 'Black'} ${type}`;
          img.className = 'piece-img';
          img.draggable = false;
          sqDiv.appendChild(img);
        }

        // Coordinates
        if (f === 0) {
          const rSpan = document.createElement('span');
          rSpan.className = 'coord coord-rank';
          rSpan.textContent = rank;
          sqDiv.appendChild(rSpan);
        }
        if (r === 7) {
          const fSpan = document.createElement('span');
          fSpan.className = 'coord coord-file';
          fSpan.textContent = file;
          sqDiv.appendChild(fSpan);
        }

        el.board.appendChild(sqDiv);
      }
    }
  }

  function onBoardSquareClick(e) {
    if (!state.gameActive || state.isEngineThinking) return;
    if (chess.turn() !== state.playerColor) return;

    const sqEl = e.target.closest('.square');
    if (!sqEl) return;

    const sq = sqEl.getAttribute('data-sq');
    const piece = chess.get(sq);

    // If square is already selected
    if (state.selectedSquare) {
      const legal = state.legalMovesForSelected.find(m => m.to === sq);
      if (legal) {
        // Pawn Promotion Check
        const moving = chess.get(state.selectedSquare);
        const isPromo = moving && moving.type === 'p' &&
          ((moving.color === 'w' && sq.endsWith('8')) || (moving.color === 'b' && sq.endsWith('1')));

        if (isPromo) {
          promptPromotion(state.selectedSquare, sq);
          return;
        }

        executeMove({ from: state.selectedSquare, to: sq });
        return;
      }

      // Switch selection to own piece
      if (piece && piece.color === state.playerColor) {
        selectSquare(sq);
        return;
      }

      // Deselect
      clearSelection();
      renderBoard();
      return;
    }

    // Select piece
    if (piece && piece.color === state.playerColor) {
      selectSquare(sq);
    }
  }

  function selectSquare(sq) {
    state.selectedSquare = sq;
    state.legalMovesForSelected = chess.moves({ square: sq, verbose: true });
    renderBoard();
  }

  function clearSelection() {
    state.selectedSquare = null;
    state.legalMovesForSelected = [];
  }

  function promptPromotion(from, to) {
    state.pendingPromotion = { from, to };
    const color = state.playerColor;

    el.promoChoices.innerHTML = '';
    const promoTypes = ['q', 'r', 'b', 'n'];

    promoTypes.forEach(p => {
      const btn = document.createElement('button');
      btn.className = 'promo-btn';
      btn.setAttribute('data-piece', p);
      btn.innerHTML = `<img src="assets/pieces/${color}${p.toUpperCase()}.png" alt="${p}" />`;
      el.promoChoices.appendChild(btn);
    });

    el.promotionModal.classList.add('is-open');
  }

  function completePromotion(promotedPiece) {
    el.promotionModal.classList.remove('is-open');
    if (!state.pendingPromotion) return;

    const { from, to } = state.pendingPromotion;
    state.pendingPromotion = null;

    executeMove({ from, to, promotion: promotedPiece });
  }

  function executeMove(moveData) {
    const move = chess.move(moveData);
    if (!move) {
      clearSelection();
      renderBoard();
      return;
    }

    const uci = move.from + move.to + (move.promotion || '');
    state.moveHistoryUci.push(uci);
    state.lastMove = { from: move.from, to: move.to };
    clearSelection();

    // Audio
    if (move.flags.includes('c') || move.flags.includes('e')) {
      audio.playCapture();
    } else if (move.flags.includes('k') || move.flags.includes('q')) {
      audio.playCastle();
    } else if (chess.in_check()) {
      audio.playCheck();
    } else {
      audio.playMove();
    }

    // Clock
    if (chess.history().length === 1) {
      clock.start('b');
    } else {
      clock.switchTurn('b');
    }

    appendMoveToHistory(move);
    updateCapturedTrays();
    renderBoard();
    updateStatusDisplay();

    if (checkTerminalState()) return;

    triggerEngineTurn();
  }

  function triggerEngineTurn() {
    state.isEngineThinking = true;
    el.topStatusPill.textContent = 'Thinking';
    el.topStatusPill.className = 'status-pill thinking';
    el.turnDot.className = 'turn-dot thinking';
    el.gameStatusText.textContent = "Mite's turn";

    const options = {};
    if (state.engineSetting.mode === 'fixed') {
      options.movetime = state.engineSetting.movetime;
    } else {
      options.wtime = clock.whiteTimeMs;
      options.btime = clock.blackTimeMs;
      options.winc = clock.incrementMs;
      options.binc = clock.incrementMs;
    }

    engine.requestMove(state.moveHistoryUci, options);
  }

  function handleEngineMove(bestMoveStr) {
    state.isEngineThinking = false;
    el.topStatusPill.textContent = 'Ready';
    el.topStatusPill.className = 'status-pill';

    if (!state.gameActive) return;
    if (!bestMoveStr || bestMoveStr === '(none)') {
      checkTerminalState();
      return;
    }

    const from = bestMoveStr.substring(0, 2);
    const to = bestMoveStr.substring(2, 4);
    const promotion = bestMoveStr.length > 4 ? bestMoveStr.substring(4, 5).toLowerCase() : undefined;

    const move = chess.move({ from, to, promotion });
    if (!move) {
      console.error('Engine move rejected by rules:', bestMoveStr);
      return;
    }

    state.moveHistoryUci.push(bestMoveStr);
    state.lastMove = { from, to };

    if (move.flags.includes('c') || move.flags.includes('e')) {
      audio.playCapture();
    } else if (move.flags.includes('k') || move.flags.includes('q')) {
      audio.playCastle();
    } else if (chess.in_check()) {
      audio.playCheck();
    } else {
      audio.playMove();
    }

    clock.switchTurn('w');

    appendMoveToHistory(move);
    updateCapturedTrays();
    renderBoard();
    updateStatusDisplay();

    checkTerminalState();
  }

  function checkTerminalState() {
    if (chess.in_checkmate()) {
      state.gameActive = false;
      clock.pause();
      const winner = chess.turn() === 'w' ? 'Black' : 'White';
      audio.playGameOver();
      showGameOverDialog(`${winner} Won by Checkmate`, `${chess.turn() === 'w' ? 'White' : 'Black'} has no legal moves.`);
      updateStatusDisplay();
      return true;
    }

    if (chess.in_stalemate()) {
      state.gameActive = false;
      clock.pause();
      audio.playGameOver();
      showGameOverDialog('Draw by Stalemate', 'No legal moves available and not in check.');
      updateStatusDisplay();
      return true;
    }

    if (chess.in_threefold_repetition()) {
      state.gameActive = false;
      clock.pause();
      audio.playGameOver();
      showGameOverDialog('Draw by Repetition', 'Position occurred three times.');
      updateStatusDisplay();
      return true;
    }

    if (chess.insufficient_material()) {
      state.gameActive = false;
      clock.pause();
      audio.playGameOver();
      showGameOverDialog('Draw by Insufficient Material', 'Neither side has mating material.');
      updateStatusDisplay();
      return true;
    }

    if (chess.in_draw()) {
      state.gameActive = false;
      clock.pause();
      audio.playGameOver();
      showGameOverDialog('Draw by 50-Move Rule', '50 moves played without a capture or pawn move.');
      updateStatusDisplay();
      return true;
    }

    return false;
  }

  function showGameOverDialog(title, reason) {
    el.gameOverTitle.textContent = title;
    el.gameOverReason.textContent = reason;
    el.gameOverModal.classList.add('is-open');
  }

  function updateStatusDisplay() {
    const turn = chess.turn();
    const isPlayer = (turn === state.playerColor);

    if (chess.in_checkmate()) {
      el.gameStatusText.textContent = `Checkmate - ${turn === 'w' ? 'Black' : 'White'} wins`;
      el.turnDot.className = 'turn-dot ended';
    } else if (chess.in_draw()) {
      el.gameStatusText.textContent = 'Game Drawn';
      el.turnDot.className = 'turn-dot waiting';
    } else if (chess.in_check()) {
      el.gameStatusText.textContent = `${turn === 'w' ? 'White' : 'Black'} in Check`;
      el.turnDot.className = 'turn-dot ended';
    } else if (state.isEngineThinking) {
      el.gameStatusText.textContent = "Mite's turn";
      el.turnDot.className = 'turn-dot thinking';
    } else {
      el.gameStatusText.textContent = isPlayer ? 'Your turn' : "Mite's turn";
      el.turnDot.className = 'turn-dot';
    }
  }

  function updateDiagnostics(info) {
    if (info.depth !== undefined) {
      el.diagDepth.textContent = `d: ${info.depth}`;
    }
    if (info.score !== undefined) {
      if (info.scoreType === 'mate') {
        el.diagEval.textContent = `M${info.score}`;
      } else {
        const val = (info.score / 100).toFixed(2);
        el.diagEval.textContent = info.score > 0 ? `+${val}` : val;
      }
    }
    if (info.nodes !== undefined) {
      el.diagNodes.textContent = `${(info.nodes / 1000).toFixed(1)}k n`;
    }
    if (info.nps !== undefined) {
      el.diagNps.textContent = `${Math.round(info.nps / 1000)}k/s`;
    }
  }

  function appendMoveToHistory(move) {
    const hist = chess.history();
    const idx = hist.length;
    const moveNum = Math.ceil(idx / 2);
    const isWhite = (idx % 2 === 1);

    if (isWhite) {
      const row = document.createElement('div');
      row.className = 'notation-row';

      const nSpan = document.createElement('span');
      nSpan.className = 'notation-num';
      nSpan.textContent = `${moveNum}.`;

      const wSpan = document.createElement('span');
      wSpan.className = 'notation-move active';
      wSpan.textContent = move.san;

      const bSpan = document.createElement('span');
      bSpan.className = 'notation-move';

      row.appendChild(nSpan);
      row.appendChild(wSpan);
      row.appendChild(bSpan);
      el.notationList.appendChild(row);
    } else {
      const rows = el.notationList.querySelectorAll('.notation-row');
      const lastRow = rows[rows.length - 1];
      if (lastRow) {
        const wSpan = lastRow.querySelectorAll('.notation-move')[0];
        const bSpan = lastRow.querySelectorAll('.notation-move')[1];
        if (wSpan) wSpan.classList.remove('active');
        if (bSpan) {
          bSpan.textContent = move.san;
          bSpan.classList.add('active');
        }
      }
    }
    el.notationList.scrollTop = el.notationList.scrollHeight;
  }

  function rebuildNotationUI() {
    el.notationList.innerHTML = '';
    const moves = chess.history();
    for (let i = 0; i < moves.length; i++) {
      const moveNum = Math.ceil((i + 1) / 2);
      const isWhite = (i % 2 === 0);

      if (isWhite) {
        const row = document.createElement('div');
        row.className = 'notation-row';

        const nSpan = document.createElement('span');
        nSpan.className = 'notation-num';
        nSpan.textContent = `${moveNum}.`;

        const wSpan = document.createElement('span');
        wSpan.className = 'notation-move';
        wSpan.textContent = moves[i];

        const bSpan = document.createElement('span');
        bSpan.className = 'notation-move';

        row.appendChild(nSpan);
        row.appendChild(wSpan);
        row.appendChild(bSpan);
        el.notationList.appendChild(row);
      } else {
        const rows = el.notationList.querySelectorAll('.notation-row');
        const lastRow = rows[rows.length - 1];
        if (lastRow) {
          const bSpan = lastRow.querySelectorAll('.notation-move')[1];
          if (bSpan) bSpan.textContent = moves[i];
        }
      }
    }
    el.notationList.scrollTop = el.notationList.scrollHeight;
  }

  function updateCapturedTrays() {
    const starting = { p: 8, n: 2, b: 2, r: 2, q: 1 };
    const onBoard = {
      w: { p: 0, n: 0, b: 0, r: 0, q: 0 },
      b: { p: 0, n: 0, b: 0, r: 0, q: 0 }
    };

    const board = chess.board();
    for (let r = 0; r < 8; r++) {
      for (let f = 0; f < 8; f++) {
        const p = board[r][f];
        if (p && p.type !== 'k') {
          onBoard[p.color][p.type]++;
        }
      }
    }

    const values = { p: 1, n: 3, b: 3, r: 5, q: 9 };
    let whiteMat = 0;
    let blackMat = 0;

    // Pieces lost by White = captured by Black
    const capturedByBlack = [];
    // Pieces lost by Black = captured by White
    const capturedByWhite = [];

    ['q', 'r', 'b', 'n', 'p'].forEach(t => {
      const lostWhite = Math.max(0, starting[t] - onBoard.w[t]);
      for (let i = 0; i < lostWhite; i++) capturedByBlack.push(t);

      const lostBlack = Math.max(0, starting[t] - onBoard.b[t]);
      for (let i = 0; i < lostBlack; i++) capturedByWhite.push(t);

      whiteMat += onBoard.w[t] * values[t];
      blackMat += onBoard.b[t] * values[t];
    });

    const isWhite = (state.playerColor === 'w');
    // Human captured pieces = pieces taken from opponent
    const playerCaptured = isWhite ? capturedByWhite : capturedByBlack;
    const engineCaptured = isWhite ? capturedByBlack : capturedByWhite;

    // The piece images shown in the tray are of the OPPONENT's color
    const playerTrayColor = isWhite ? 'b' : 'w';
    const engineTrayColor = isWhite ? 'w' : 'b';

    renderTray(el.bottomCapturedBox, playerCaptured, playerTrayColor);
    renderTray(el.topCapturedBox, engineCaptured, engineTrayColor);

    const playerLead = isWhite ? (whiteMat - blackMat) : (blackMat - whiteMat);
    el.bottomMaterialLead.textContent = playerLead > 0 ? `+${playerLead}` : '';
    el.topMaterialLead.textContent = playerLead < 0 ? `+${Math.abs(playerLead)}` : '';
  }

  function renderTray(boxEl, piecesList, pieceColor) {
    boxEl.innerHTML = '';
    if (piecesList.length === 0) return;

    // Group by piece type for clean presentation
    const order = ['q', 'r', 'b', 'n', 'p'];
    order.forEach(t => {
      const matches = piecesList.filter(p => p === t);
      if (matches.length > 0) {
        const group = document.createElement('span');
        group.className = 'captured-group';
        matches.forEach(() => {
          const img = document.createElement('img');
          img.src = `assets/pieces/${pieceColor}${t.toUpperCase()}.png`;
          img.className = 'captured-img';
          img.alt = t;
          group.appendChild(img);
        });
        boxEl.appendChild(group);
      }
    });
  }

  // Auto initialize on DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();

/**
 * game.js - Core chess game coordinator for Mite Chess Engine Space
 * Manages chess rules, Web Worker engine communication, clocks, and audio synthesis.
 */

class ChessAudio {
  constructor() {
    this.ctx = null;
    this.enabled = true;
  }

  _initContext() {
    if (!this.ctx) {
      const AudioContext = window.AudioContext || window.webkitAudioContext;
      if (AudioContext) {
        this.ctx = new AudioContext();
      }
    }
    if (this.ctx && this.ctx.state === 'suspended') {
      this.ctx.resume();
    }
  }

  playMove() {
    if (!this.enabled) return;
    this._initContext();
    if (!this.ctx) return;

    // Resonant wooden snap
    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'triangle';
    osc.frequency.setValueAtTime(420, t);
    osc.frequency.exponentialRampToValueAtTime(140, t + 0.06);

    gain.gain.setValueAtTime(0.45, t);
    gain.gain.exponentialRampToValueAtTime(0.001, t + 0.06);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.065);
  }

  playCapture() {
    if (!this.enabled) return;
    this._initContext();
    if (!this.ctx) return;

    // Thicker punchy thud
    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const osc2 = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sawtooth';
    osc.frequency.setValueAtTime(320, t);
    osc.frequency.exponentialRampToValueAtTime(80, t + 0.09);

    osc2.type = 'sine';
    osc2.frequency.setValueAtTime(180, t);
    osc2.frequency.exponentialRampToValueAtTime(60, t + 0.09);

    gain.gain.setValueAtTime(0.55, t);
    gain.gain.exponentialRampToValueAtTime(0.001, t + 0.09);

    osc.connect(gain);
    osc2.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc2.start(t);
    osc.stop(t + 0.095);
    osc2.stop(t + 0.095);
  }

  playCheck() {
    if (!this.enabled) return;
    this._initContext();
    if (!this.ctx) return;

    // Crisp high double warning tone
    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(660, t);
    osc.frequency.setValueAtTime(880, t + 0.08);

    gain.gain.setValueAtTime(0.35, t);
    gain.gain.exponentialRampToValueAtTime(0.001, t + 0.22);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.23);
  }

  playCastle() {
    if (!this.enabled) return;
    this._initContext();
    if (!this.ctx) return;

    // Rapid double wood slide
    this.playMove();
    setTimeout(() => {
      this.playMove();
    }, 75);
  }

  playGameOver() {
    if (!this.enabled) return;
    this._initContext();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(392, t);       // G4
    osc.frequency.setValueAtTime(523.25, t + 0.12); // C5
    osc.frequency.setValueAtTime(659.25, t + 0.24); // E5

    gain.gain.setValueAtTime(0.3, t);
    gain.gain.exponentialRampToValueAtTime(0.001, t + 0.45);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.46);
  }
}

class ChessClock {
  constructor(initialSeconds = 300, incrementSeconds = 0, onTick = null, onFlag = null) {
    this.initialMs = initialSeconds * 1000;
    this.incrementMs = incrementSeconds * 1000;
    this.whiteTimeMs = this.initialMs;
    this.blackTimeMs = this.initialMs;
    this.activeColor = null; // 'w' | 'b' | null
    this.lastTimestamp = null;
    this.timerId = null;
    this.onTick = onTick;
    this.onFlag = onFlag;
    this.isUnlimited = (initialSeconds <= 0);
  }

  start(color = 'w') {
    if (this.isUnlimited) return;
    this.activeColor = color;
    this.lastTimestamp = performance.now();
    if (this.timerId) clearInterval(this.timerId);

    this.timerId = setInterval(() => {
      this._update();
    }, 100);
  }

  pause() {
    if (this.timerId) {
      clearInterval(this.timerId);
      this.timerId = null;
    }
    if (this.activeColor && this.lastTimestamp && !this.isUnlimited) {
      const now = performance.now();
      const elapsed = now - this.lastTimestamp;
      if (this.activeColor === 'w') {
        this.whiteTimeMs = Math.max(0, this.whiteTimeMs - elapsed);
      } else {
        this.blackTimeMs = Math.max(0, this.blackTimeMs - elapsed);
      }
      if (this.onTick) {
        this.onTick(this.whiteTimeMs, this.blackTimeMs);
      }
    }
    this.activeColor = null;
    this.lastTimestamp = null;
  }

  switchTurn(newColor) {
    if (this.isUnlimited) return;
    this._update();

    // Add increment to the player who just completed their turn
    if (this.activeColor === 'w') {
      this.whiteTimeMs += this.incrementMs;
    } else if (this.activeColor === 'b') {
      this.blackTimeMs += this.incrementMs;
    }

    this.activeColor = newColor;
    this.lastTimestamp = performance.now();
    if (this.onTick) {
      this.onTick(this.whiteTimeMs, this.blackTimeMs);
    }
  }

  _update() {
    if (!this.activeColor || !this.lastTimestamp || this.isUnlimited) return;
    const now = performance.now();
    const elapsed = now - this.lastTimestamp;
    this.lastTimestamp = now;

    let flagged = null;
    if (this.activeColor === 'w') {
      this.whiteTimeMs = Math.max(0, this.whiteTimeMs - elapsed);
      if (this.whiteTimeMs <= 0) flagged = 'w';
    } else {
      this.blackTimeMs = Math.max(0, this.blackTimeMs - elapsed);
      if (this.blackTimeMs <= 0) flagged = 'b';
    }

    if (this.onTick) {
      this.onTick(this.whiteTimeMs, this.blackTimeMs);
    }

    if (flagged) {
      this.pause();
      if (this.onFlag) this.onFlag(flagged);
    }
  }

  reset(initialSeconds = null, incrementSeconds = null) {
    this.pause();
    if (initialSeconds !== null) {
      this.initialMs = initialSeconds * 1000;
      this.isUnlimited = (initialSeconds <= 0);
    }
    if (incrementSeconds !== null) {
      this.incrementMs = incrementSeconds * 1000;
    }
    this.whiteTimeMs = this.initialMs;
    this.blackTimeMs = this.initialMs;
    if (this.onTick) {
      this.onTick(this.whiteTimeMs, this.blackTimeMs);
    }
  }

  formatTime(ms) {
    if (this.isUnlimited) return '∞';
    const totalSecs = Math.max(0, Math.ceil(ms / 1000));
    const mins = Math.floor(totalSecs / 60);
    const secs = totalSecs % 60;
    if (mins >= 60) {
      const hours = Math.floor(mins / 60);
      const remMins = mins % 60;
      return `${hours}:${remMins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
    }
    return `${mins}:${secs.toString().padStart(2, '0')}`;
  }
}

class MiteEngineClient {
  constructor() {
    this.worker = null;
    this.isLoaded = false;
    this.isSearching = false;
    this.onLoaded = null;
    this.onBestMove = null;
    this.onInfo = null;
    this.onError = null;
    this.searchTimeoutId = null;
  }

  init() {
    try {
      this.worker = new Worker('scripts/worker.js');
      this.worker.onmessage = (e) => this._handleMessage(e.data);
      this.worker.onerror = (err) => {
        console.error('Engine worker error:', err);
        if (this.onError) this.onError(err.message || 'Worker error');
      };
    } catch (err) {
      console.error('Failed to create worker:', err);
      if (this.onError) this.onError(err.message || 'Failed to spawn worker');
    }
  }

  _handleMessage(data) {
    if (!data) return;

    if (data.type === 'loaded') {
      this.isLoaded = true;
      this.sendCommand('uci');
      this.sendCommand('isready');
      if (this.onLoaded) this.onLoaded();
    } else if (data.type === 'uci') {
      this._parseUciLine(data.line);
    } else if (data.type === 'error') {
      console.error('Engine runtime error:', data.error);
      if (this.onError) this.onError(data.error);
    }
  }

  _parseUciLine(line) {
    if (!line) return;
    const trimmed = line.trim();

    if (trimmed.startsWith('bestmove')) {
      this.isSearching = false;
      if (this.searchTimeoutId) {
        clearTimeout(this.searchTimeoutId);
        this.searchTimeoutId = null;
      }
      const parts = trimmed.split(/\s+/);
      const moveStr = parts[1]; // e.g. "e7e5" or "(none)"
      if (this.onBestMove) {
        this.onBestMove(moveStr);
      }
    } else if (trimmed.startsWith('info')) {
      // Parse info depth <d> score cp <s> nodes <n> pv <pv>
      const info = { raw: trimmed };
      const depthMatch = trimmed.match(/depth\s+(\d+)/);
      const scoreMatch = trimmed.match(/score\s+(cp|mate)\s+(-?\d+)/);
      const nodesMatch = trimmed.match(/nodes\s+(\d+)/);
      const npsMatch = trimmed.match(/nps\s+(\d+)/);
      const pvMatch = trimmed.match(/pv\s+(.*)/);

      if (depthMatch) info.depth = parseInt(depthMatch[1], 10);
      if (scoreMatch) {
        info.scoreType = scoreMatch[1];
        info.score = parseInt(scoreMatch[2], 10);
      }
      if (nodesMatch) info.nodes = parseInt(nodesMatch[1], 10);
      if (npsMatch) info.nps = parseInt(npsMatch[1], 10);
      if (pvMatch) info.pv = pvMatch[1];

      if (this.onInfo) {
        this.onInfo(info);
      }
    }
  }

  sendCommand(cmd) {
    if (!this.worker) return;
    this.worker.postMessage({ type: 'command', cmd: cmd });
  }

  newGame() {
    this.stop();
    this.sendCommand('ucinewgame');
    this.sendCommand('isready');
  }

  stop() {
    if (this.isSearching) {
      this.sendCommand('stop');
      this.isSearching = false;
    }
    if (this.searchTimeoutId) {
      clearTimeout(this.searchTimeoutId);
      this.searchTimeoutId = null;
    }
  }

  requestMove(moveHistoryList, options = {}) {
    if (this.isSearching) {
      this.stop();
    }

    let posCmd = 'position startpos';
    if (moveHistoryList && moveHistoryList.length > 0) {
      posCmd += ' moves ' + moveHistoryList.join(' ');
    }
    this.sendCommand(posCmd);

    // Format go command
    let goCmd = 'go';
    if (options.movetime) {
      goCmd += ` movetime ${options.movetime}`;
    } else if (options.wtime !== undefined && options.btime !== undefined) {
      goCmd += ` wtime ${Math.floor(options.wtime)} btime ${Math.floor(options.btime)}`;
      if (options.winc !== undefined) goCmd += ` winc ${Math.floor(options.winc)}`;
      if (options.binc !== undefined) goCmd += ` binc ${Math.floor(options.binc)}`;
    } else {
      goCmd += ' movetime 500';
    }

    this.isSearching = true;
    this.sendCommand(goCmd);

    // Safety timeout in case engine hangs
    const maxExpectedMs = (options.movetime || 2500) + 3000;
    this.searchTimeoutId = setTimeout(() => {
      if (this.isSearching) {
        console.warn('Engine search safety timeout triggered.');
        this.stop();
      }
    }, maxExpectedMs);
  }
}

// Export for app.js
window.ChessAudio = ChessAudio;
window.ChessClock = ChessClock;
window.MiteEngineClient = MiteEngineClient;

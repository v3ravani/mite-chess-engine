// Mite Web Worker: Runs the WebAssembly chess engine in a background thread
// Ensures calculating moves does not block the UI or chess clock.

let engineInstance = null;
let commandQueue = [];
let isReady = false;

try {
  // Import the Emscripten-generated Mite loader
  importScripts('../engine/mite.js');

  MiteModule({
    locateFile: function (path, prefix) {
      try {
        return new URL('../engine/' + path, self.location.href).href;
      } catch (e) {
        return '../engine/' + path;
      }
    },
    print: function (text) {
      self.postMessage({ type: 'uci', line: text });
    },
    printErr: function (text) {
      self.postMessage({ type: 'err', line: text });
    }
  }).then(function (instance) {
    engineInstance = instance;
    isReady = true;
    self.postMessage({ type: 'loaded' });

    // Process any UCI commands queued during initialization
    while (commandQueue.length > 0) {
      var cmd = commandQueue.shift();
      engineInstance.ccall('process_command', null, ['string'], [cmd]);
    }
  }).catch(function (err) {
    self.postMessage({ type: 'error', error: err ? err.toString() : 'Failed to initialize engine' });
  });
} catch (e) {
  self.postMessage({ type: 'error', error: e ? e.toString() : 'Worker script error' });
}

self.onmessage = function (event) {
  var data = event.data;
  if (!data) return;

  if (data.type === 'command' && typeof data.cmd === 'string') {
    if (isReady && engineInstance) {
      engineInstance.ccall('process_command', null, ['string'], [data.cmd]);
    } else {
      commandQueue.push(data.cmd);
    }
  }
};

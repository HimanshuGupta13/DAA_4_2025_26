/* ────────────────────────────────────────────
   SUDOKU — script.js
   C++-style engine: backtracking solver,
   unique-solution puzzle generator, validator
   ──────────────────────────────────────────── */

"use strict";

/* ── Constants ── */
const SIZE = 9;
const BOX  = 3;
const REMOVE = { easy: 36, medium: 46, hard: 54, expert: 60 };

/* ── Game state ── */
let grid      = [];   // current board (0 = empty)
let solution  = [];   // full solved board
let given     = [];   // boolean mask: true = original clue cell
let notes     = [];   // array of Sets, one per cell
let selected  = -1;   // index of selected cell (-1 = none)
let inputMode = 'normal'; // 'normal' | 'notes'
let difficulty = 'easy';

let mistakes  = 0;
let score     = 0;
let hints     = 3;
let history   = [];   // undo stack
let timerSec  = 0;
let timerID   = null;
let gameOver  = false;

/* ════════════════════════════════════════════
   CORE ENGINE  (mirrors C++ logic)
   ════════════════════════════════════════════ */

function makeGrid()  { return new Array(81).fill(0); }
function getRow(i)   { return Math.floor(i / SIZE); }
function getCol(i)   { return i % SIZE; }
function getBox(i)   { return Math.floor(getRow(i) / BOX) * BOX + Math.floor(getCol(i) / BOX); }

/** True if value v can be placed at index i without conflict */
function canPlace(g, i, v) {
  const r = getRow(i), c = getCol(i);
  for (let j = 0; j < SIZE; j++) {
    if (g[r * SIZE + j] === v) return false;   // row
    if (g[j * SIZE + c] === v) return false;   // col
  }
  const br = Math.floor(r / BOX) * BOX;
  const bc = Math.floor(c / BOX) * BOX;
  for (let dr = 0; dr < BOX; dr++)
    for (let dc = 0; dc < BOX; dc++)
      if (g[(br + dr) * SIZE + (bc + dc)] === v) return false;
  return true;
}

/** Backtracking solver — shuffles candidates when shuffle=true (for generation) */
function solve(g, shuffle = false) {
  for (let i = 0; i < 81; i++) {
    if (g[i] !== 0) continue;
    const nums = [1,2,3,4,5,6,7,8,9];
    if (shuffle) nums.sort(() => Math.random() - 0.5);
    for (const v of nums) {
      if (canPlace(g, i, v)) {
        g[i] = v;
        if (solve(g, shuffle)) return true;
        g[i] = 0;
      }
    }
    return false;   // no valid number → backtrack
  }
  return true;      // all cells filled
}

/** Count solutions (capped at `limit` to stay fast) */
function countSolutions(g, limit = 2) {
  for (let i = 0; i < 81; i++) {
    if (g[i] !== 0) continue;
    let count = 0;
    for (let v = 1; v <= 9; v++) {
      if (canPlace(g, i, v)) {
        g[i] = v;
        count += countSolutions([...g], limit);
        g[i] = 0;
        if (count >= limit) return count;
      }
    }
    return count;
  }
  return 1;
}

/** Generate a puzzle with exactly one solution by removing `remove` cells */
function makePuzzle(remove) {
  // 1. Build a fully-solved random grid
  const sol = makeGrid();
  solve(sol, true);

  // 2. Remove cells one by one, checking uniqueness
  const puzzle = [...sol];
  const positions = [...Array(81).keys()].sort(() => Math.random() - 0.5);
  let removed = 0;

  for (const i of positions) {
    if (removed >= remove) break;
    const backup = puzzle[i];
    puzzle[i] = 0;
    if (countSolutions([...puzzle]) === 1) {
      removed++;
    } else {
      puzzle[i] = backup; // restore — would create multiple solutions
    }
  }

  return { puzzle, solution: sol };
}

/** Return a Set of all cell indices in the same row/col/box as i */
function getPeers(i) {
  const r = getRow(i), c = getCol(i), b = getBox(i);
  const peers = new Set();
  for (let j = 0; j < 81; j++) {
    if (j === i) continue;
    if (getRow(j) === r || getCol(j) === c || getBox(j) === b) peers.add(j);
  }
  return peers;
}

/* ════════════════════════════════════════════
   GAME LOGIC
   ════════════════════════════════════════════ */

function newGame() {
  clearInterval(timerID);
  timerSec = 0; mistakes = 0; score = 0; hints = 3;
  history = []; gameOver = false; selected = -1;

  const { puzzle, solution: sol } = makePuzzle(REMOVE[difficulty]);
  grid     = [...puzzle];
  solution = sol;
  given    = grid.map(v => v !== 0);
  notes    = Array.from({ length: 81 }, () => new Set());

  // Reset UI
  updateScoreUI();
  updateTimerUI();
  setMessage('', '');
  document.getElementById('hint-count').textContent = hints;

  timerID = setInterval(() => { timerSec++; updateTimerUI(); }, 1000);
  render();
}

function inputNumber(v) {
  if (selected < 0 || gameOver) return;
  if (given[selected]) return;

  // Save snapshot for undo
  const snap = {
    index: selected,
    prevVal: grid[selected],
    prevNotes: new Set(notes[selected])
  };

  if (inputMode === 'notes') {
    if (v === 0) {
      notes[selected].clear();
    } else {
      notes[selected].has(v) ? notes[selected].delete(v) : notes[selected].add(v);
    }
  } else {
    if (v === 0) {
      grid[selected] = 0;
      notes[selected].clear();
    } else {
      // Toggle off if same number clicked again
      if (grid[selected] === v) {
        grid[selected] = 0;
      } else {
        grid[selected] = v;
        notes[selected].clear();

        // Remove this number from notes in peers
        for (const p of getPeers(selected)) notes[p].delete(v);

        // Check correctness
        if (v !== solution[selected]) {
          mistakes++;
          score = Math.max(0, score - 10);
          flashCell(selected, 'error');
        } else {
          score += 5;
          flashCell(selected, 'correct-flash');
        }
        updateScoreUI();
      }
    }
  }

  history.push(snap);
  checkWin();
  render();
}

function undoMove() {
  if (!history.length) return;
  const { index, prevVal, prevNotes } = history.pop();
  grid[index]  = prevVal;
  notes[index] = prevNotes;
  render();
}

function useHint() {
  if (hints <= 0 || gameOver) return;

  // Find unsolved incorrect cells
  const candidates = [];
  for (let i = 0; i < 81; i++) {
    if (!given[i] && grid[i] !== solution[i]) candidates.push(i);
  }
  if (!candidates.length) return;

  const i = candidates[Math.floor(Math.random() * candidates.length)];
  grid[i]  = solution[i];
  given[i] = true;
  notes[i].clear();

  hints--;
  score = Math.max(0, score - 15);
  document.getElementById('hint-count').textContent = hints;
  updateScoreUI();
  flashCell(i, 'correct-flash');
  checkWin();
  render();
}

function autoSolve() {
  if (gameOver) return;
  clearInterval(timerID);
  for (let i = 0; i < 81; i++) {
    grid[i]  = solution[i];
    given[i] = true;
  }
  gameOver = true;
  setMessage('Puzzle auto-solved!', 'info');
  render();
}

function checkWin() {
  if (grid.every((v, i) => v === solution[i])) {
    gameOver = true;
    clearInterval(timerID);
    const bonus = Math.max(0, 500 - timerSec * 2 - mistakes * 20);
    score += bonus;
    updateScoreUI();
    showWinOverlay();
  }
}

/* ════════════════════════════════════════════
   RENDER
   ════════════════════════════════════════════ */

function render() {
  const board    = document.getElementById('board');
  const peerSet  = selected >= 0 ? getPeers(selected) : new Set();
  const selVal   = selected >= 0 ? grid[selected] : 0;

  board.innerHTML = '';

  for (let i = 0; i < 81; i++) {
    const cell = document.createElement('div');
    const classes = ['cell'];

    if (given[i]) classes.push('given');
    else if (grid[i]) classes.push('player');

    if (i === selected) classes.push('selected');
    else if (peerSet.has(i)) classes.push('peer');

    // Highlight same number
    if (selVal > 0 && grid[i] === selVal && i !== selected) classes.push('same-num');

    // Error highlight
    if (!given[i] && grid[i] !== 0 && grid[i] !== solution[i]) classes.push('error');

    cell.className = classes.join(' ');

    // Content
    if (grid[i] !== 0) {
      cell.textContent = grid[i];
    } else if (notes[i].size > 0) {
      const ng = document.createElement('div');
      ng.className = 'notes-grid';
      for (let n = 1; n <= 9; n++) {
        const nd = document.createElement('div');
        nd.className = 'note';
        nd.textContent = notes[i].has(n) ? n : '';
        ng.appendChild(nd);
      }
      cell.appendChild(ng);
    }

    cell.addEventListener('click', () => {
      selected = (selected === i) ? -1 : i;
      render();
    });

    board.appendChild(cell);
  }
}

/* ════════════════════════════════════════════
   UI HELPERS
   ════════════════════════════════════════════ */

function updateTimerUI() {
  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');
  document.getElementById('timer').textContent = `${m}:${s}`;
}

function updateScoreUI() {
  document.getElementById('mistakes').textContent = mistakes;
  document.getElementById('score').textContent    = score;
}

function setMessage(text, type) {
  const el = document.getElementById('msg');
  el.textContent  = text;
  el.className    = 'msg' + (type ? ' ' + type : '');
}

function flashCell(i, cls) {
  const cells = document.querySelectorAll('.cell');
  if (!cells[i]) return;
  cells[i].classList.add(cls);
  setTimeout(() => cells[i].classList.remove(cls), 600);
}

function showWinOverlay() {
  const m = String(Math.floor(timerSec / 60)).padStart(2, '0');
  const s = String(timerSec % 60).padStart(2, '0');

  const overlay = document.createElement('div');
  overlay.className = 'overlay';
  overlay.innerHTML = `
    <div class="overlay-box">
      <h2>🎉 Solved!</h2>
      <p>Great work on ${difficulty} difficulty</p>
      <div class="overlay-stats">
        <div class="overlay-stat">
          <div class="val">${m}:${s}</div>
          <div class="lbl">Time</div>
        </div>
        <div class="overlay-stat">
          <div class="val">${mistakes}</div>
          <div class="lbl">Mistakes</div>
        </div>
        <div class="overlay-stat">
          <div class="val">${score}</div>
          <div class="lbl">Score</div>
        </div>
      </div>
      <button class="overlay-btn" id="overlay-new">Play Again</button>
    </div>
  `;
  document.body.appendChild(overlay);
  document.getElementById('overlay-new').addEventListener('click', () => {
    overlay.remove();
    newGame();
  });
}

/* ════════════════════════════════════════════
   BUILD NUMPAD
   ════════════════════════════════════════════ */

function buildNumpad() {
  const np = document.getElementById('numpad');
  np.innerHTML = '';

  for (let v = 1; v <= 9; v++) {
    const btn = document.createElement('button');
    btn.className   = 'num-btn';
    btn.textContent = v;
    btn.addEventListener('click', () => inputNumber(v));
    np.appendChild(btn);
  }

  const erase = document.createElement('button');
  erase.className   = 'num-btn erase';
  erase.textContent = 'Erase';
  erase.addEventListener('click', () => inputNumber(0));
  np.appendChild(erase);
}

/* ════════════════════════════════════════════
   EVENT LISTENERS
   ════════════════════════════════════════════ */

// Keyboard navigation & input
document.addEventListener('keydown', (e) => {
  if (e.key >= '1' && e.key <= '9') { inputNumber(parseInt(e.key)); return; }
  if (e.key === 'Backspace' || e.key === 'Delete' || e.key === '0') { inputNumber(0); return; }
  if (e.key === 'z' && (e.ctrlKey || e.metaKey)) { undoMove(); return; }
  if (selected < 0) return;
  if (e.key === 'ArrowRight') { selected = Math.min(80, selected + 1); render(); }
  if (e.key === 'ArrowLeft')  { selected = Math.max(0,  selected - 1); render(); }
  if (e.key === 'ArrowDown')  { selected = Math.min(80, selected + 9); render(); }
  if (e.key === 'ArrowUp')    { selected = Math.max(0,  selected - 9); render(); }
});

// Mode buttons
document.getElementById('btn-normal').addEventListener('click', () => {
  inputMode = 'normal';
  document.getElementById('btn-normal').classList.add('active');
  document.getElementById('btn-notes').classList.remove('active');
});

document.getElementById('btn-notes').addEventListener('click', () => {
  inputMode = 'notes';
  document.getElementById('btn-notes').classList.add('active');
  document.getElementById('btn-normal').classList.remove('active');
});

// Action buttons
document.getElementById('btn-new').addEventListener('click', newGame);
document.getElementById('btn-undo').addEventListener('click', undoMove);
document.getElementById('btn-hint').addEventListener('click', useHint);
document.getElementById('btn-solve').addEventListener('click', autoSolve);

// Difficulty buttons
document.querySelectorAll('.diff-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    difficulty = btn.dataset.d;
    document.querySelectorAll('.diff-btn').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    newGame();
  });
});

/* ════════════════════════════════════════════
   INIT
   ════════════════════════════════════════════ */

buildNumpad();
newGame();
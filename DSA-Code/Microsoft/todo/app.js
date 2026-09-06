// Application State
let allProblems = [];
let completedIds = new Set();
let starredIds = new Set();
let activeFile = 'all';
let activeStatus = 'all';
let searchQuery = '';

// DOM Elements
const problemsList = document.getElementById('problemsList');
const emptyState = document.getElementById('emptyState');
const searchInput = document.getElementById('searchInput');
const clearSearchBtn = document.getElementById('clearSearchBtn');
const filePillsContainer = document.getElementById('filePills');
const statusTogglesContainer = document.getElementById('statusToggles');

const statTotal = document.getElementById('statTotal');
const statDone = document.getElementById('statDone');
const statRemaining = document.getElementById('statRemaining');
const statStarred = document.getElementById('statStarred');
const statStarredMeta = document.getElementById('statStarredMeta');
const progressBarFill = document.getElementById('progressBarFill');
const progressPercent = document.getElementById('progressPercent');
const markAllBtn = document.getElementById('markAllBtn');
const resetAllBtn = document.getElementById('resetAllBtn');

// Helper to get badge class by file name
function getFileBadgeClass(file) {
  switch (file) {
    case '1.cpp': return 'f-1';
    case '2.cpp': return 'f-2';
    case '3.cpp': return 'f-3';
    case '4.cpp': return 'f-4';
    case '5.cpp': return 'f-5';
    case 'greedy.md': return 'f-greedy';
    case 'pyq.cpp':
    case 'pyq1.cpp': return 'f-pyq1';
    case 'pyq2.cpp': return 'f-pyq2';
    default: return 'f-1';
  }
}

// Load problems from SQLite API or fallback
async function loadProblemsFromDB() {
  // One-time cleanup to purge legacy auto-marked 86 problems from browser localStorage
  if (localStorage.getItem('ms_dsa_legacy_cleaned_v1') !== 'true') {
    localStorage.removeItem('ms_dsa_completed_problems');
    localStorage.setItem('ms_dsa_legacy_cleaned_v1', 'true');
  }

  const savedCompleted = JSON.parse(localStorage.getItem('ms_dsa_completed_problems') || '[]');
  const savedStarred = JSON.parse(localStorage.getItem('ms_dsa_starred_problems') || '[]');
  
  try {
    const res = await fetch('/api/problems');
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    const data = await res.json();
    allProblems = data.problems || [];
    
    // Populate completed & starred IDs from database
    completedIds.clear();
    starredIds.clear();

    allProblems.forEach(p => {
      if (p.completed) completedIds.add(p.id);
      if (p.starred) starredIds.add(p.id);
    });

    localStorage.setItem('ms_dsa_completed_problems', JSON.stringify([...completedIds]));
    localStorage.setItem('ms_dsa_starred_problems', JSON.stringify([...starredIds]));

    updatePillCounts();
    updateStats();
    renderList();
  } catch (err) {
    console.warn("API request failed, loading fallback local dataset:", err);
    if (typeof PROBLEMS_DATA !== 'undefined') {
      allProblems = PROBLEMS_DATA;
      completedIds = new Set(savedCompleted);
      starredIds = new Set(savedStarred.length > 0 ? savedStarred : allProblems.filter(p => p.starred).map(p => p.id));
      localStorage.setItem('ms_dsa_completed_problems', JSON.stringify([...completedIds]));
      localStorage.setItem('ms_dsa_starred_problems', JSON.stringify([...starredIds]));
      updatePillCounts();
      updateStats();
      renderList();
    }
  }
}

// Update counts on filter pills
function updatePillCounts() {
  const counts = {
    'all': allProblems.length,
    '1.cpp': 0,
    '2.cpp': 0,
    '3.cpp': 0,
    '4.cpp': 0,
    '5.cpp': 0,
    'greedy.md': 0,
    'pyq1.cpp': 0,
    'pyq2.cpp': 0
  };

  allProblems.forEach(p => {
    if (counts[p.file] !== undefined) {
      counts[p.file]++;
    }
  });

  filePillsContainer.querySelectorAll('.gh-subnav-item').forEach(btn => {
    const file = btn.dataset.file;
    if (counts[file] !== undefined) {
      const label = file === 'all' ? 'All' : file;
      btn.innerHTML = `${label} <span class="gh-counter">${counts[file]}</span>`;
    }
  });
}

// Update Top Progress Bar & Counters
function updateStats() {
  const total = allProblems.length;
  const done = completedIds.size;
  const remaining = total - done;
  const starred = starredIds.size;
  const percent = total === 0 ? 0 : Math.round((done / total) * 100);

  if (statTotal) statTotal.textContent = total;
  if (statDone) statDone.textContent = done;
  if (statRemaining) statRemaining.textContent = remaining;
  if (statStarred) statStarred.textContent = starred;
  if (statStarredMeta) statStarredMeta.textContent = starred;
  if (progressBarFill) progressBarFill.style.width = `${percent}%`;
  if (progressPercent) progressPercent.textContent = `${percent}%`;
}

// Filter dataset based on current state
function getFilteredProblems() {
  return allProblems.filter(item => {
    // File filter
    if (activeFile !== 'all' && item.file !== activeFile) return false;

    // Status filter
    const isDone = completedIds.has(item.id);
    const isStarred = starredIds.has(item.id);

    if (activeStatus === 'todo' && isDone) return false;
    if (activeStatus === 'completed' && !isDone) return false;
    if (activeStatus === 'starred' && !isStarred) return false;

    // Search query filter
    if (searchQuery) {
      const q = searchQuery.toLowerCase();
      const matchName = (item.name || '').toLowerCase().includes(q);
      const matchPattern = (item.pattern || '').toLowerCase().includes(q);
      const matchCategory = (item.category || '').toLowerCase().includes(q);
      const matchLc = (item.lcNum || '').toLowerCase().includes(q);
      const matchClass = (item.classNum || '').toLowerCase().includes(q);
      const matchFile = (item.file || '').toLowerCase().includes(q);
      if (!matchName && !matchPattern && !matchCategory && !matchLc && !matchClass && !matchFile) {
        return false;
      }
    }

    return true;
  });
}

// Helper to escape HTML special characters
function escapeHtml(str) {
  if (!str) return '';
  return str
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#039;');
}

// Dracula C++ Syntax Highlighter
function highlightCpp(code) {
  if (!code) return '';
  const tokenRegex = /(\/\/[^\n]*|\/\*[\s\S]*?\*\/)|("(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')|(#\s*\w+)|(\b(?:class|struct|public|private|protected|virtual|override|return|if|else|for|while|do|switch|case|default|break|continue|new|delete|sizeof|typedef|using|namespace|auto|const|static|inline|explicit|template|typename|operator)\b)|(\b(?:int|long|short|char|bool|float|double|void|size_t|uint64_t|int64_t|vector|string|stack|queue|deque|priority_queue|set|unordered_set|multiset|map|unordered_map|multimap|pair|tuple|ListNode|TreeNode|Node)\b)|(\b(?:true|false|nullptr|NULL)\b)|(\b\d+(?:\.\d+)?(?:LL|ULL|L|U|f)?\b)|(\b[a-zA-Z_]\w*(?=\s*\())|([&|<>=!+\-*\/%^~?:]+)/g;

  let lastIndex = 0;
  let html = '';
  let match;

  while ((match = tokenRegex.exec(code)) !== null) {
    if (match.index > lastIndex) {
      html += escapeHtml(code.slice(lastIndex, match.index));
    }
    const [raw, comment, str, prep, kw, type, lit, num, fn, op] = match;
    if (comment) {
      html += '<span class="tok-comment">' + escapeHtml(comment) + '</span>';
    } else if (str) {
      html += '<span class="tok-string">' + escapeHtml(str) + '</span>';
    } else if (prep) {
      html += '<span class="tok-prep">' + escapeHtml(prep) + '</span>';
    } else if (kw) {
      html += '<span class="tok-keyword">' + escapeHtml(kw) + '</span>';
    } else if (type) {
      html += '<span class="tok-type">' + escapeHtml(type) + '</span>';
    } else if (lit) {
      html += '<span class="tok-lit">' + escapeHtml(lit) + '</span>';
    } else if (num) {
      html += '<span class="tok-num">' + escapeHtml(num) + '</span>';
    } else if (fn) {
      html += '<span class="tok-fn">' + escapeHtml(fn) + '</span>';
    } else if (op) {
      html += '<span class="tok-op">' + escapeHtml(op) + '</span>';
    }
    lastIndex = tokenRegex.lastIndex;
  }
  if (lastIndex < code.length) {
    html += escapeHtml(code.slice(lastIndex));
  }
  return html;
}

// Copy problem solution code to clipboard
window.copyProblemCode = async function(event, id) {
  if (event) {
    event.preventDefault();
    event.stopPropagation();
  }
  const problem = allProblems.find(p => p.id === id);
  if (!problem || !problem.code) return;

  const btn = event.currentTarget;
  const originalHtml = btn.innerHTML;

  try {
    if (navigator.clipboard && window.isSecureContext) {
      await navigator.clipboard.writeText(problem.code);
    } else {
      const textarea = document.createElement('textarea');
      textarea.value = problem.code;
      textarea.style.position = 'fixed';
      textarea.style.left = '-9999px';
      document.body.appendChild(textarea);
      textarea.focus();
      textarea.select();
      document.execCommand('copy');
      document.body.removeChild(textarea);
    }
    btn.classList.add('copied');
    btn.innerHTML = `
      <svg height="12" viewBox="0 0 16 16" width="12" fill="currentColor">
        <path d="M13.78 4.22a.75.75 0 0 1 0 1.06l-7.25 7.25a.75.75 0 0 1-1.06 0L2.22 9.28a.751.751 0 0 1 .018-1.042.751.751 0 0 1 1.042-.018L6 10.94l6.72-6.72a.75.75 0 0 1 1.06 0Z"></path>
      </svg>
      <span class="copy-label">Copied!</span>
    `;
    setTimeout(() => {
      btn.classList.remove('copied');
      btn.innerHTML = originalHtml;
    }, 1800);
  } catch (err) {
    console.error('Failed to copy code:', err);
  }
};

// Render problem rows
function renderList() {
  const filtered = getFilteredProblems();
  const filteredCountEl = document.getElementById('filteredCount');
  if (filteredCountEl) {
    filteredCountEl.innerHTML = `<strong>${filtered.length}</strong> problem${filtered.length === 1 ? '' : 's'}`;
  }

  if (filtered.length === 0) {
    problemsList.innerHTML = '';
    emptyState.style.display = 'block';
    return;
  }

  emptyState.style.display = 'none';

  problemsList.innerHTML = filtered.map(item => {
    const isChecked = completedIds.has(item.id);
    const isStarred = starredIds.has(item.id);
    const badgeClass = getFileBadgeClass(item.file);

    return `
      <article class="gh-row ${isChecked ? 'completed' : ''}" data-id="${item.id}" id="problem-${item.id}" onclick="handleRowClick(event, ${item.id})">
        <div class="gh-row-main">
          <label class="gh-check-wrapper" title="${isChecked ? 'Mark todo' : 'Mark done'}">
            <input type="checkbox" class="gh-checkbox" ${isChecked ? 'checked' : ''} onchange="toggleProblem(${item.id})">
            <span class="gh-custom-check"></span>
          </label>

          <button class="gh-star-btn ${isStarred ? 'starred' : ''}" onclick="toggleStar(event, ${item.id})" title="${isStarred ? 'Unstar problem' : 'Star for revision'}" aria-label="Star problem">
            <svg height="15" viewBox="0 0 16 16" width="15" fill="currentColor">
              <path d="m8 .25 2.062 5.093 5.438.423-4.14 3.567 1.259 5.337L8 11.838l-4.619 2.832 1.259-5.337-4.14-3.567 5.438-.423L8 .25Z"></path>
            </svg>
          </button>
          
          <div class="gh-row-content">
            <div class="gh-row-header">
              <span class="gh-id">#${item.id}</span>
              ${item.url ? `
                <a href="${item.url}" target="_blank" rel="noopener noreferrer" class="gh-title" title="Open on LeetCode (${item.lcNum})">
                  ${item.name}
                  <svg class="gh-external-icon" height="11" viewBox="0 0 16 16" width="11" fill="currentColor">
                    <path d="M3.75 2h3.5a.75.75 0 0 1 0 1.5h-3.5a.25.25 0 0 0-.25.25v8.5c0 .138.112.25.25.25h8.5a.25.25 0 0 0 .25-.25v-3.5a.75.75 0 0 1 1.5 0v3.5A1.75 1.75 0 0 1 12.25 14h-8.5A1.75 1.75 0 0 1 2 12.25v-8.5C2 2.784 2.784 2 3.75 2Zm6.75-.75a.75.75 0 0 1 .75-.75h3.5a.75.75 0 0 1 .75.75v3.5a.75.75 0 0 1-1.5 0V3.56l-4.22 4.22a.751.751 0 0 1-1.042-.018.751.751 0 0 1-.018-1.042L12.94 2.5h-1.69a.75.75 0 0 1-.75-.75Z"></path>
                  </svg>
                </a>
              ` : `
                <span class="gh-title">${item.name}</span>
              `}
              <span class="gh-label gh-label-lc">${item.lcNum}</span>
              <span class="gh-label gh-label-file ${badgeClass}">${item.file}</span>
            </div>
            
            <div class="gh-row-meta">
              ${item.classNum ? `<span class="meta-item code-font">${item.classNum}</span><span class="meta-divider">&bull;</span>` : ''}
              <span class="meta-item">${item.pattern}</span>
              <span class="meta-divider">&bull;</span>
              <span class="meta-item code-font">${item.time}</span>
              <span class="meta-divider">|</span>
              <span class="meta-item code-font">${item.space}</span>
            </div>

            <div class="gh-row-details">
              <div class="gh-detail-card">
                <div class="gh-detail-meta-bar">
                  <div class="detail-meta-pill">
                    <span class="detail-meta-label">Pattern</span>
                    <span class="detail-meta-val">${item.pattern || 'Optimal Approach'}</span>
                  </div>
                  <div class="detail-meta-pill">
                    <span class="detail-meta-label">Time</span>
                    <code class="detail-meta-code">${item.time}</code>
                  </div>
                  <div class="detail-meta-pill">
                    <span class="detail-meta-label">Space</span>
                    <code class="detail-meta-code">${item.space}</code>
                  </div>
                </div>
                <div class="gh-code-card dracula-theme">
                  <div class="gh-code-header">
                    <div class="dracula-header-left">
                      <div class="dracula-dots" aria-hidden="true">
                        <span class="dracula-dot dot-red"></span>
                        <span class="dracula-dot dot-yellow"></span>
                        <span class="dracula-dot dot-green"></span>
                      </div>
                      <span class="gh-code-lang">
                        ${item.classNum ? item.classNum + ' &bull; C++' : 'C++ Solution'}
                      </span>
                    </div>
                    <button class="gh-copy-btn dracula-copy-btn" onclick="copyProblemCode(event, ${item.id})" title="Copy code to clipboard">
                      <svg height="12" viewBox="0 0 16 16" width="12" fill="currentColor">
                        <path d="M0 6.75C0 5.784.784 5 1.75 5h1.5a.75.75 0 0 1 0 1.5h-1.5a.25.25 0 0 0-.25.25v7.5c0 .138.112.25.25.25h7.5a.25.25 0 0 0 .25-.25v-1.5a.75.75 0 0 1 1.5 0v1.5A1.75 1.75 0 0 1 9.25 16h-7.5A1.75 1.75 0 0 1 0 14.25Z"></path>
                        <path d="M5 1.75C5 .784 5.784 0 6.75 0h7.5C15.216 0 16 .784 16 1.75v7.5A1.75 1.75 0 0 1 14.25 11h-7.5A1.75 1.75 0 0 1 5 9.25Zm1.75-.25a.25.25 0 0 0-.25.25v7.5c0 .138.112.25.25.25h7.5a.25.25 0 0 0 .25-.25v-7.5a.25.25 0 0 0-.25-.25Z"></path>
                      </svg>
                      <span class="copy-label">Copy</span>
                    </button>
                  </div>
                  <pre class="gh-code-pre dracula-pre"><code class="gh-code-content dracula-code">${highlightCpp(item.code || '// Solution code not available')}</code></pre>
                </div>
              </div>
            </div>
          </div>

          <div class="gh-row-actions">
            <button class="gh-expand-btn" title="Toggle details" aria-label="Toggle details">
              <svg height="16" viewBox="0 0 16 16" width="16" fill="currentColor">
                <path d="m4.427 7.427 3.396 3.396a.25.25 0 0 0 .354 0l3.396-3.396A.25.25 0 0 0 11.396 7H4.604a.25.25 0 0 0-.177.427Z"></path>
              </svg>
            </button>
          </div>
        </div>
      </article>
    `;
  }).join('');
}

// Toggle problem completion
window.toggleProblem = async function(id) {
  const isDone = completedIds.has(id);
  if (isDone) {
    completedIds.delete(id);
  } else {
    completedIds.add(id);
  }
  updateStats();
  
  const card = document.getElementById(`problem-${id}`);
  if (card) {
    card.classList.toggle('completed', completedIds.has(id));
  }

  // Dual persistence: localStorage
  localStorage.setItem('ms_dsa_completed_problems', JSON.stringify([...completedIds]));

  // Dual persistence: SQLite DB
  try {
    await fetch('/api/toggle', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ id })
    });
  } catch (e) {
    // Already in localStorage
  }

  // Re-filter if on todo / completed tab
  if (activeStatus !== 'all') {
    renderList();
  }
};

// Toggle star mark
window.toggleStar = async function(event, id) {
  if (event) {
    event.preventDefault();
    event.stopPropagation();
  }

  const isStarred = starredIds.has(id);
  if (isStarred) {
    starredIds.delete(id);
  } else {
    starredIds.add(id);
  }
  updateStats();

  const card = document.getElementById(`problem-${id}`);
  if (card) {
    const btn = card.querySelector('.gh-star-btn');
    if (btn) {
      btn.classList.toggle('starred', starredIds.has(id));
      btn.title = starredIds.has(id) ? 'Unstar problem' : 'Star for revision';
    }
  }

  // Dual persistence: localStorage
  localStorage.setItem('ms_dsa_starred_problems', JSON.stringify([...starredIds]));

  // Dual persistence: SQLite DB
  try {
    await fetch('/api/star', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ id })
    });
  } catch (e) {
    // Already in localStorage
  }

  // Re-filter if on starred tab
  if (activeStatus === 'starred') {
    renderList();
  }
};

// Expand / collapse details
window.toggleDetails = function(id) {
  const card = document.getElementById(`problem-${id}`);
  if (card) {
    card.classList.toggle('expanded');
  }
};

// Click anywhere on problem row to toggle dropdown (except on checkbox, star, problem name, or details)
window.handleRowClick = function(event, id) {
  // If clicking on checkbox, star button, problem name, or inside the solution details, do NOT toggle dropdown
  if (event.target.closest('.gh-check-wrapper, .gh-star-btn, .gh-title, .gh-row-details, .dracula-copy-btn')) {
    return;
  }

  // Clicking anywhere else on the row toggles details dropdown
  toggleDetails(id);
};

// Search handling
searchInput.addEventListener('input', (e) => {
  searchQuery = e.target.value.trim();
  clearSearchBtn.style.display = searchQuery ? 'block' : 'none';
  renderList();
});

clearSearchBtn.addEventListener('click', () => {
  searchInput.value = '';
  searchQuery = '';
  clearSearchBtn.style.display = 'none';
  searchInput.focus();
  renderList();
});

// File pill filters
filePillsContainer.addEventListener('click', (e) => {
  const btn = e.target.closest('.gh-subnav-item');
  if (!btn) return;

  filePillsContainer.querySelectorAll('.gh-subnav-item').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  activeFile = btn.dataset.file;
  renderList();
});

// Status filters
statusTogglesContainer.addEventListener('click', (e) => {
  const btn = e.target.closest('.gh-status-btn');
  if (!btn) return;

  statusTogglesContainer.querySelectorAll('.gh-status-btn').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  activeStatus = btn.dataset.status;
  renderList();
});

// Mark all filtered as completed
markAllBtn.addEventListener('click', async () => {
  const filtered = getFilteredProblems();
  if (filtered.length === 0) return;
  const ids = filtered.map(p => p.id);
  ids.forEach(id => completedIds.add(id));
  updateStats();
  renderList();

  localStorage.setItem('ms_dsa_completed_problems', JSON.stringify([...completedIds]));

  try {
    await fetch('/api/mark-all', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ ids, completed: true })
    });
  } catch (e) {
    // persisted locally
  }
});

// Reset progress
resetAllBtn.addEventListener('click', async () => {
  if (confirm('Reset all completed problems?')) {
    completedIds.clear();
    updateStats();
    renderList();

    localStorage.removeItem('ms_dsa_completed_problems');

    try {
      await fetch('/api/reset', { method: 'POST' });
    } catch (e) {
      // persisted locally
    }
  }
});

// Keyboard shortcut: Ctrl + K or Cmd + K to focus search
document.addEventListener('keydown', (e) => {
  if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 'k') {
    e.preventDefault();
    searchInput.focus();
    searchInput.select();
  }
});

// Sticky header stuck state detection
const milestoneCard = document.getElementById('milestoneCard');
if (milestoneCard) {
  const onScroll = () => {
    if (window.scrollY > 35) {
      milestoneCard.classList.add('is-stuck');
    } else {
      milestoneCard.classList.remove('is-stuck');
    }
  };
  window.addEventListener('scroll', onScroll, { passive: true });
  onScroll();
}

// Initialize on page load
loadProblemsFromDB();

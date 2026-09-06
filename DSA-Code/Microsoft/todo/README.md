# Microsoft DSA Practice Tracker & Review Hub

A local-first, full-stack GitHub-styled web application to systematically track, filter, star, and review 221 high-yield Microsoft DSA and interview questions.

---

## Directory Structure

```text
todo/
├── index.html            # Main UI page (GitHub Dark theme, progress bar, subnav pills)
├── app.js                # Frontend client logic, state management, Dracula syntax highlighting
├── style.css             # GitHub Dark Theme design system tokens and responsive layouts
├── server.py             # Python HTTP + SQLite API server (port 8088)
├── setup_db.py           # Database migration & initialization script
├── problems.db           # SQLite database (stores completion, stars, intuition, code)
├── problems.json         # Source of truth dataset (221 curated problems)
├── problems.js           # Offline client fallback dataset (PROBLEMS_DATA)
│
├── backups/              # Historical backups and caches
│   ├── backup_5cpp_problems.json
│   ├── backup_easy_greedy_problems.json
│   └── leetcode_slugs_cache.json
│
└── scripts/              # Migration, intuition generation & enrichment utilities
    ├── apply_class_solutions.py
    ├── apply_leetcode_urls.py
    └── generate_intuitions.py
```

---

## Quickstart

### 1. Launch the Server
```bash
cd todo
python3 server.py
```
Open **`http://localhost:8088`** in your browser.

### 2. Rebuild or Sync Database
If problems in `problems.json` are modified or added:
```bash
python3 setup_db.py
```
> *Note: `setup_db.py` automatically preserves your completed and starred checkboxes across rebuilds.*

---

## Features

- **GitHub Dark Aesthetics**: Authentic GitHub colors, pills, counters, tooltips, and Dracula-style C++ code blocks.
- **Persistent Progress**: Every completion toggle and star is saved in `problems.db` (SQLite) with automatic local-storage backup.
- **Instant Search & Filters**: Filter by keyword, category, LeetCode number, file origin (`1.cpp`, `pyq1.cpp`, `pyq2.cpp`, `greedy.md`), or star status.
- **Keyboard Shortcuts**:
  - `⌘K` or `Ctrl+K`: Focus search bar.
  - `Esc`: Clear search / close active filters.

---

## API Reference

The backend runs on port `8088` with REST endpoints:

| Method | Endpoint | Description |
|---|---|---|
| `GET` | `/api/problems` | Returns all 221 problems with status, intuition, and code |
| `POST` | `/api/toggle` | Toggle completion for problem `{ "id": <number> }` |
| `POST` | `/api/star` | Toggle star bookmark for problem `{ "id": <number> }` |
| `POST` | `/api/mark-all` | Batch update completion for `{ "ids": [...], "completed": bool }` |
| `POST` | `/api/reset` | Reset all problems to uncompleted |

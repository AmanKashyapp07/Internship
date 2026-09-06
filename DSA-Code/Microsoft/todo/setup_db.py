import sqlite3
import json
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, 'problems.db')
JSON_PATH = os.path.join(BASE_DIR, 'problems.json')

def init_db(force=False):
    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    completed_ids = set()
    starred_ids = set()
    if not force:
        try:
            cursor.execute("SELECT id FROM problems WHERE completed = 1")
            completed_ids = {row[0] for row in cursor.fetchall()}
        except sqlite3.OperationalError:
            pass
        try:
            cursor.execute("SELECT id FROM problems WHERE starred = 1")
            starred_ids = {row[0] for row in cursor.fetchall()}
        except sqlite3.OperationalError:
            pass

    cursor.execute('DROP TABLE IF EXISTS problems')
    cursor.execute('''
    CREATE TABLE problems (
        id INTEGER PRIMARY KEY,
        source TEXT,
        class_num TEXT,
        lc_num TEXT,
        name TEXT,
        category TEXT,
        pattern TEXT,
        time_complexity TEXT,
        space_complexity TEXT,
        summary TEXT,
        intuition TEXT,
        url TEXT,
        code TEXT,
        completed INTEGER DEFAULT 0,
        starred INTEGER DEFAULT 0
    )
    ''')

    if not os.path.exists(JSON_PATH):
        raise FileNotFoundError(f"{JSON_PATH} not found")

    with open(JSON_PATH, 'r', encoding='utf-8') as f:
        problems = json.load(f)

    for p in problems:
        pid = p['id']
        is_completed = 1 if (p.get('completed') or pid in completed_ids) else 0
        is_starred = 1 if (p.get('starred') or pid in starred_ids) else 0
        cursor.execute('''
        INSERT INTO problems (id, source, class_num, lc_num, name, category, pattern, time_complexity, space_complexity, summary, intuition, url, code, completed, starred)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (
            pid,
            p.get('file', ''),
            p.get('classNum', ''),
            p.get('lcNum', ''),
            p.get('name', ''),
            p.get('category', ''),
            p.get('pattern', ''),
            p.get('time', ''),
            p.get('space', ''),
            p.get('summary', p.get('pattern', '')),
            json.dumps(p.get('intuition', [])),
            p.get('url', ''),
            p.get('code', ''),
            is_completed,
            is_starred
        ))

    conn.commit()
    cursor.execute('SELECT COUNT(*) FROM problems')
    total = cursor.fetchone()[0]
    conn.close()
    print(f"Initialized database with {total} problems, intuition, and LeetCode URLs.")
    return total

if __name__ == '__main__':
    init_db()

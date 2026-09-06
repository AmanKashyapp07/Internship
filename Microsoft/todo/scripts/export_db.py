import sqlite3
import json
import os

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DB_PATH = os.path.join(BASE_DIR, 'problems.db')
JSON_PATH = os.path.join(BASE_DIR, 'problems.json')
JS_PATH = os.path.join(BASE_DIR, 'problems.js')

def export_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM problems ORDER BY id ASC')
    rows = cursor.fetchall()
    
    problems = []
    for r in rows:
        intuition = []
        if 'intuition' in r.keys() and r['intuition']:
            try:
                intuition = json.loads(r['intuition'])
            except Exception:
                intuition = []
        
        p = {
            'id': r['id'],
            'file': r['source'],
            'classNum': r['class_num'] or '',
            'lcNum': r['lc_num'] or '',
            'name': r['name'] or '',
            'category': r['category'] or '',
            'pattern': r['pattern'] or '',
            'time': r['time_complexity'] or '',
            'space': r['space_complexity'] or '',
            'intuition': intuition,
            'completed': bool(r['completed']),
            'starred': bool(r['starred']) if 'starred' in r.keys() else False,
            'url': r['url'] or '',
            'code': r['code'] or ''
        }
        if 'summary' in r.keys() and r['summary']:
            p['summary'] = r['summary']
        problems.append(p)
    
    conn.close()

    with open(JSON_PATH, 'w', encoding='utf-8') as f:
        json.dump(problems, f, indent=2)

    with open(JS_PATH, 'w', encoding='utf-8') as f:
        f.write('const PROBLEMS_DATA = ' + json.dumps(problems, indent=2) + ';\n')

    print(f"Exported {len(problems)} problems from SQLite to problems.json and problems.js")
    print(f"Completed: {sum(1 for p in problems if p['completed'])}, Starred: {sum(1 for p in problems if p['starred'])}")

if __name__ == '__main__':
    export_db()

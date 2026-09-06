import http.server
import socketserver
import sqlite3
import json
import os
import urllib.parse
from setup_db import init_db, DB_PATH

PORT = 8088
DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def get_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

class RequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

    def end_headers(self):
        self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Expires', '0')
        super().end_headers()

    def _send_json(self, data, status=200):
        self.send_response(status)
        self.send_header('Content-Type', 'application/json; charset=utf-8')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode('utf-8'))

    def do_GET(self):
        parsed_url = urllib.parse.urlparse(self.path)
        if parsed_url.path == '/api/problems':
            conn = get_db()
            cursor = conn.cursor()
            cursor.execute('SELECT * FROM problems ORDER BY id ASC')
            rows = cursor.fetchall()
            problems = []
            for r in rows:
                problems.append({
                    "id": r["id"],
                    "source": r["source"],
                    "file": r["source"],
                    "classNum": r["class_num"],
                    "lcNum": r["lc_num"],
                    "name": r["name"],
                    "category": r["category"],
                    "pattern": r["pattern"],
                    "time": r["time_complexity"],
                    "space": r["space_complexity"],
                    "summary": r["summary"],
                    "intuition": json.loads(r["intuition"]) if ("intuition" in r.keys() and r["intuition"]) else [],
                    "url": r["url"] if ("url" in r.keys() and r["url"]) else "",
                    "code": r["code"] if ("code" in r.keys() and r["code"]) else "",
                    "completed": bool(r["completed"]),
                    "starred": bool(r["starred"]) if "starred" in r.keys() else False
                })
            conn.close()
            self._send_json({"total": len(problems), "problems": problems})
            return

        # Fallback to serving static files
        super().do_GET()

    def do_POST(self):
        parsed_url = urllib.parse.urlparse(self.path)
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length).decode('utf-8')
        body = json.loads(post_data) if post_data else {}

        if parsed_url.path == '/api/toggle':
            problem_id = body.get('id')
            if not problem_id:
                self._send_json({"error": "Missing id"}, status=400)
                return
            conn = get_db()
            cursor = conn.cursor()
            cursor.execute('UPDATE problems SET completed = 1 - completed WHERE id = ?', (problem_id,))
            conn.commit()
            cursor.execute('SELECT completed FROM problems WHERE id = ?', (problem_id,))
            row = cursor.fetchone()
            conn.close()
            new_val = bool(row[0]) if row else False
            self._send_json({"success": True, "id": problem_id, "completed": new_val})
            return

        elif parsed_url.path == '/api/star':
            problem_id = body.get('id')
            if not problem_id:
                self._send_json({"error": "Missing id"}, status=400)
                return
            conn = get_db()
            cursor = conn.cursor()
            cursor.execute('UPDATE problems SET starred = 1 - starred WHERE id = ?', (problem_id,))
            conn.commit()
            cursor.execute('SELECT starred FROM problems WHERE id = ?', (problem_id,))
            row = cursor.fetchone()
            conn.close()
            new_val = bool(row[0]) if row else False
            self._send_json({"success": True, "id": problem_id, "starred": new_val})
            return

        elif parsed_url.path == '/api/mark-all':
            ids = body.get('ids', [])
            completed = 1 if body.get('completed', True) else 0
            if ids:
                conn = get_db()
                cursor = conn.cursor()
                cursor.executemany('UPDATE problems SET completed = ? WHERE id = ?', [(completed, pid) for pid in ids])
                conn.commit()
                conn.close()
            self._send_json({"success": True, "count": len(ids)})
            return

        elif parsed_url.path == '/api/reset':
            conn = get_db()
            cursor = conn.cursor()
            cursor.execute('UPDATE problems SET completed = 0')
            conn.commit()
            conn.close()
            self._send_json({"success": True, "message": "All progress reset"})
            return

        self._send_json({"error": "Not found"}, status=404)

if __name__ == '__main__':
    # Initialize DB (preserves completed states)
    init_db(force=True)

    socketserver.TCPServer.allow_reuse_address = True
    with socketserver.TCPServer(("", PORT), RequestHandler) as httpd:
        print(f"DSA Tracker API & Web Server running at http://localhost:{PORT}")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down server.")

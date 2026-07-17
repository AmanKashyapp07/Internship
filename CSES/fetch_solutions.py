import os
import re
import sys
import time
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin
from concurrent.futures import ThreadPoolExecutor, as_completed

try:
    import browser_cookie3
    HAS_BROWSER_COOKIE = True
except ImportError:
    HAS_BROWSER_COOKIE = False

BASE_URL   = "https://cses.fi/problemset/"
CSES_DIR   = os.path.dirname(os.path.abspath(__file__))
MAX_WORKERS = 5   # parallel tasks fetched at once

FOLDER_MAP = {"Additional Problems": "Additional Problems I"}
LANG_EXT   = [('c++', '.cpp'), ('python', '.py'), ('java', '.java'),
              ('rust', '.rs'), (' c ', '.c')]

# ─── Helpers ──────────────────────────────────────────────────────────────────

def sanitize(name):
    return re.sub(r'[\\/*?:"<>|]', "", name).strip()

def get_folder_name(cat):
    return FOLDER_MAP.get(cat, cat)

def get_ext(row_text):
    for key, ext in LANG_EXT:
        if key in row_text:
            return ext
    return '.txt'

def is_downloaded(task_name, category):
    folder = os.path.join(CSES_DIR, get_folder_name(category))
    if not os.path.isdir(folder):
        return False
    stem = sanitize(task_name)
    return any(os.path.splitext(f)[0] == stem for f in os.listdir(folder))

def get_with_retry(session, url, retries=3, backoff=1):
    for attempt in range(retries):
        try:
            r = session.get(url, timeout=15)
            r.raise_for_status()
            return r
        except Exception as e:
            if attempt == retries - 1:
                raise
            time.sleep(backoff * (2 ** attempt))

# ─── Session ──────────────────────────────────────────────────────────────────

def get_session():
    session = requests.Session()
    phpsessid = os.environ.get("CSES_SESSION")

    if phpsessid:
        print("Using PHPSESSID from environment variable...")
    elif len(sys.argv) > 1:
        phpsessid = sys.argv[1].strip()
        print("Using PHPSESSID from command-line argument...")
    elif HAS_BROWSER_COOKIE:
        try:
            print("Loading cookies from Brave browser...")
            cj = browser_cookie3.brave(domain_name='cses.fi')
            session.cookies.update(cj)
            if 'PHPSESSID' in session.cookies.get_dict():
                print("Cookies loaded!")
                return session
        except Exception as e:
            print(f"Could not load Brave cookies: {e}")

    if not phpsessid:
        print("\n--- PHPSESSID REQUIRED ---")
        phpsessid = input("Paste your PHPSESSID here: ").strip()

    session.cookies.set('PHPSESSID', phpsessid, domain='cses.fi')
    return session

# ─── Per-task fetcher (runs in thread pool) ───────────────────────────────────

def fetch_one(session, task_name, task_url, category):
    """Fetches and saves the latest accepted submission for one task.
    Returns a status string for printing."""
    try:
        # 1. Task page → find Results link (/view/)
        task_page = get_with_retry(session, task_url)
        task_soup = BeautifulSoup(task_page.text, 'html.parser')

        submissions_url = None
        for a in task_soup.find_all('a'):
            href = a.get('href', '')
            text = a.text.lower()
            if 'results' in text or 'submissions' in text or '/view/' in href:
                submissions_url = urljoin(task_url, href)
                break

        if not submissions_url:
            return f"  [-] {task_name}: no submissions link"

        # 2. Submissions list → find latest accepted row
        sub_page = get_with_retry(session, submissions_url)
        sub_soup = BeautifulSoup(sub_page.text, 'html.parser')

        result_url = None
        ext = '.txt'
        table = sub_soup.find('table')
        if table:
            for row in table.find_all('tr'):
                link = row.find('a', href=lambda h: h and '/result/' in h)
                if not link:
                    continue
                row_text = row.get_text(' ', strip=True).lower()
                if row.select('.full, .valid') or 'accepted' in row_text:
                    result_url = urljoin(submissions_url, link['href'])
                    ext = get_ext(row_text)
                    break

        if not result_url:
            return f"  [-] {task_name}: no accepted submission"

        # 3. Result detail page → extract code
        result_page = get_with_retry(session, result_url)
        result_soup = BeautifulSoup(result_page.text, 'html.parser')
        pre = result_soup.find('pre', class_='linenums') or result_soup.find('pre')
        if not pre:
            return f"  [-] {task_name}: code block not found"

        # 4. Write file
        cat_folder = get_folder_name(category)
        out_dir    = os.path.join(CSES_DIR, cat_folder)
        os.makedirs(out_dir, exist_ok=True)
        filepath   = os.path.join(out_dir, sanitize(task_name) + ext)
        comment = f"# Link: {task_url}\n\n" if ext == ".py" else f"// Link: {task_url}\n\n"
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(comment + pre.text)

        return f"  [+] Saved → {cat_folder}/{sanitize(task_name)}{ext}"

    except Exception as e:
        return f"  [!] {task_name}: {e}"

# ─── Main ─────────────────────────────────────────────────────────────────────

def main():
    session = get_session()

    print("\nFetching solved tasks list...")
    try:
        resp = get_with_retry(session, urljoin(BASE_URL, "list/"))
    except Exception as e:
        print(f"Error: {e}")
        return

    soup   = BeautifulSoup(resp.text, 'html.parser')
    solved = []

    for h2 in soup.find_all('h2'):
        category = h2.text.strip()
        if category == "General":
            continue
        ul = h2.find_next_sibling('ul', class_='task-list')
        if not ul:
            continue
        for li in ul.find_all('li', class_='task'):
            if li.select_one('span.task-score.full, span.task-score.valid'):
                a = li.find('a')
                if a:
                    solved.append((a.text.strip(), urljoin(BASE_URL, a['href']), category))

    if not solved:
        print("No solved tasks found. Check your PHPSESSID.")
        return

    # Skip tasks already on disk
    new_tasks = [(n, u, c) for n, u, c in solved if not is_downloaded(n, c)]

    print(f"Found {len(solved)} solved tasks.")
    print(f"  → {len(solved) - len(new_tasks)} already downloaded, skipped.")
    print(f"  → {len(new_tasks)} new tasks to fetch.\n")

    if not new_tasks:
        print("Everything is up to date!")
        return

    # Fetch new tasks concurrently
    done = 0
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as pool:
        futures = {
            pool.submit(fetch_one, session, n, u, c): n
            for n, u, c in new_tasks
        }
        for future in as_completed(futures):
            done += 1
            print(f"[{done}/{len(new_tasks)}] {future.result()}")

    print("\nAll done!")

if __name__ == "__main__":
    main()

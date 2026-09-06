import json
import re
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CACHE_PATH = os.path.join(BASE_DIR, 'leetcode_slugs_cache.json')
PROBLEMS_JSON = os.path.join(BASE_DIR, 'problems.json')
BACKUP_5CPP = os.path.join(BASE_DIR, 'backup_5cpp_problems.json')
PROBLEMS_JS = os.path.join(BASE_DIR, 'problems.js')

with open(CACHE_PATH, 'r', encoding='utf-8') as f:
    lc_cache = json.load(f)

def add_urls(file_path):
    if not os.path.exists(file_path):
        return 0
    with open(file_path, 'r', encoding='utf-8') as f:
        probs = json.load(f)
    
    count = 0
    for p in probs:
        lc = p.get('lcNum', '')
        m = re.search(r'LC\s*(\d+)', lc)
        if m:
            qid = m.group(1)
            if qid in lc_cache:
                slug, _ = lc_cache[qid]
                p['url'] = f'https://leetcode.com/problems/{slug}/'
                count += 1
            else:
                p['url'] = ''
        else:
            p['url'] = ''

    with open(file_path, 'w', encoding='utf-8') as f:
        json.dump(probs, f, indent=2)
    return count

cnt1 = add_urls(PROBLEMS_JSON)
print(f"Updated {cnt1} LeetCode URLs in problems.json")

cnt2 = add_urls(BACKUP_5CPP)
print(f"Updated {cnt2} LeetCode URLs in backup_5cpp_problems.json")

# Update problems.js
with open(PROBLEMS_JSON, 'r', encoding='utf-8') as f:
    data = json.load(f)

with open(PROBLEMS_JS, 'w', encoding='utf-8') as f:
    f.write('const PROBLEMS_DATA = ' + json.dumps(data, indent=2) + ';\n')

print("Updated problems.js")

import json
import re
import os

BASE_DIR = '/Users/amankashyap/Documents/internship/DSA-Code/Microsoft'
TODO_DIR = os.path.join(BASE_DIR, 'todo')
PROBLEMS_JSON = os.path.join(TODO_DIR, 'problems.json')
PROBLEMS_JS = os.path.join(TODO_DIR, 'problems.js')

with open(PROBLEMS_JSON, 'r', encoding='utf-8') as f:
    probs = json.load(f)

# Load 1.cpp, 2.cpp, 3.cpp, 4.cpp
cpp_contents = {}
for fn in ['1.cpp', '2.cpp', '3.cpp', '4.cpp']:
    with open(os.path.join(BASE_DIR, fn), 'r', encoding='utf-8') as f:
        cpp_contents[fn] = f.read()

# Load g1..g5 blocks
g_blocks = {}
for g_idx in range(1, 6):
    fn = f'g{g_idx}.cpp'
    with open(os.path.join(BASE_DIR, fn), 'r', encoding='utf-8') as f:
        txt = f.read()
    sections = re.split(r'//\s*={40,}\s*\n', txt)
    for i in range(1, len(sections), 2):
        if i + 1 >= len(sections):
            break
        header = sections[i].strip()
        body = sections[i + 1]
        m_num = re.search(r'//\s*(\d+)\.', header)
        if m_num:
            num = int(m_num.group(1))
            code_part = body.split('// Interview Explanation:')[0].strip()
            code_part = re.sub(r'//\s*-{40,}.*$', '', code_part, flags=re.DOTALL).strip()
            if not code_part.strip().startswith('class'):
                code_part = 'class Solution {\npublic:\n' + '    ' + code_part.replace('\n', '\n    ') + '\n};'
            g_blocks[num] = code_part

# Load greedy.md
with open(os.path.join(BASE_DIR, 'greedy.md'), 'r', encoding='utf-8') as f:
    g_md = f.read()

md_num_by_name = {}
for line in g_md.split('\n'):
    m = re.match(r'^(\d+)\.\s*\*\*([^*]+)\*\*', line.strip())
    if m:
        md_num_by_name[m.group(2).strip().lower()] = int(m.group(1))

manual_override = {
    97: 13,   # Partitioning Into Minimum Deci-Binary
    131: 52,  # Minimum Moves to Make Array Complementary
    158: 81,  # Minimum Number of Refueling Stops
    166: 80   # Patch Array -> Patching Array
}

for p in probs:
    pid = p['id']
    file = p['file']
    code = ''
    if file in cpp_contents:
        class_num = p.get('classNum', '')
        if class_num:
            pattern = r'(class\s+' + re.escape(class_num) + r'[\s\S]*?\n\};)'
            m_class = re.search(pattern, cpp_contents[file])
            if m_class:
                code = m_class.group(1).strip()
    elif file == 'greedy.md':
        if pid in manual_override:
            code = g_blocks[manual_override[pid]]
        else:
            p_name = p['name'].strip().lower()
            num = md_num_by_name.get(p_name)
            if not num:
                for k, v in md_num_by_name.items():
                    if k in p_name or p_name in k:
                        num = v
                        break
            if num and num in g_blocks:
                code = g_blocks[num]
    
    p['code'] = code

with open(PROBLEMS_JSON, 'w', encoding='utf-8') as f:
    json.dump(probs, f, indent=2)

with open(PROBLEMS_JS, 'w', encoding='utf-8') as f:
    f.write('const PROBLEMS_DATA = ' + json.dumps(probs, indent=2) + ';\n')

print(f"Successfully populated full class Solution code for {len(probs)} problems in problems.json and problems.js!")

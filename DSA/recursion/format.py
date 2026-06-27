#!/usr/bin/env python3
import os
import re

def format_cpp_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Normalize line endings to \n
    content = content.replace('\r\n', '\n')

    # Remove trailing spaces on every line
    lines = [line.rstrip() for line in content.split('\n')]
    content = '\n'.join(lines)

    # Replace tabs with 4 spaces
    content = content.replace('\t', '    ')

    # Collapse multiple consecutive blank lines to at most one blank line
    content = re.sub(r'\n\n\n+', '\n\n', content)

    # Remove empty lines right after a '{' (with optional spaces or comments)
    content = re.sub(r'({\s*(?://.*)?)\n\n+', r'\1\n', content)

    # Remove empty lines right before a '}'
    content = re.sub(r'\n\n+(\s*})', r'\n\1', content)

    # Remove empty lines right after class access specifiers
    content = re.sub(r'((?:public|private|protected):\s*(?://.*)?)\n\n+', r'\1\n', content)

    # Strip leading and trailing newlines/whitespace
    content = content.strip()
    
    # Ensure exactly one newline at the end of the file
    content += '\n'

    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(content)

def main():
    current_dir = os.getcwd()
    print(f"Scanning directory: {current_dir}")
    formatted_count = 0
    for file_name in os.listdir(current_dir):
        if file_name.endswith('.cpp') or file_name.endswith('.h'):
            file_path = os.path.join(current_dir, file_name)
            print(f"Formatting {file_name}...")
            format_cpp_file(file_path)
            formatted_count += 1
    print(f"Successfully formatted {formatted_count} file(s).")

if __name__ == '__main__':
    main()

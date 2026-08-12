#!/usr/bin/env python3
"""Strip comments from source files (C/C++/Java-like and Python).

Usage examples:
  python3 scripts/strip_comments.py -l c -o out.c in.c
  python3 scripts/strip_comments.py -l py --minify -i script.py

Features:
  - Removes // and /* */ comments for C-like languages (preserves strings/char literals).
  - Removes Python `#` comments using the `tokenize` module (safe for triple-quoted strings).
  - Optional `--minify` collapses consecutive blank lines and trims trailing spaces.
  - Can process files in-place or write to an output file.
"""
from __future__ import annotations

import argparse
import glob
import io
import sys
import tokenize
from typing import Iterable


def strip_comments_c_like(text: str) -> str:
    out = []
    i = 0
    n = len(text)
    state = "NORMAL"
    while i < n:
        ch = text[i]
        if state == "NORMAL":
            if ch == "/" and i + 1 < n and text[i + 1] == "/":
                i += 2
                state = "SINGLE_COMMENT"
                continue
            if ch == "/" and i + 1 < n and text[i + 1] == "*":
                i += 2
                state = "MULTI_COMMENT"
                continue
            if ch == '"':
                out.append(ch)
                i += 1
                state = "STRING"
                continue
            if ch == "'":
                out.append(ch)
                i += 1
                state = "CHAR"
                continue
            out.append(ch)
            i += 1

        elif state == "STRING":
            out.append(ch)
            if ch == "\\":
                # escape next char
                i += 1
                if i < n:
                    out.append(text[i])
                    i += 1
            elif ch == '"':
                i += 1
                state = "NORMAL"
            else:
                i += 1

        elif state == "CHAR":
            out.append(ch)
            if ch == "\\":
                i += 1
                if i < n:
                    out.append(text[i])
                    i += 1
            elif ch == "'":
                i += 1
                state = "NORMAL"
            else:
                i += 1

        elif state == "SINGLE_COMMENT":
            if ch == "\n":
                out.append(ch)
                i += 1
                state = "NORMAL"
            else:
                i += 1

        elif state == "MULTI_COMMENT":
            if ch == "*" and i + 1 < n and text[i + 1] == "/":
                i += 2
                state = "NORMAL"
            else:
                i += 1
    return ''.join(out)


def strip_comments_python(text: str) -> str:
    # Use the tokenize module to remove COMMENT tokens; preserves structure and indentation
    out_lines = []
    try:
        bytes_io = io.BytesIO(text.encode('utf-8'))
        tokens = tokenize.tokenize(bytes_io.readline)
        last_lineno = -1
        last_col = 0
        cur_line = []
        for tok in tokens:
            ttype = tok.type
            tstring = tok.string
            start = tok.start
            end = tok.end
            if ttype == tokenize.ENCODING:
                continue
            if ttype == tokenize.COMMENT:
                # skip comments entirely
                continue
            if ttype == tokenize.NL:
                # non-significant newline inside multi-line constructs
                out_lines.append(''.join(cur_line) + '\n')
                cur_line = []
                last_lineno = -1
                last_col = 0
                continue
            if ttype == tokenize.ENDMARKER:
                break
            sline, scol = start
            if sline != last_lineno:
                # new physical line
                if cur_line:
                    out_lines.append(''.join(cur_line))
                cur_line = []
                last_lineno = sline
                last_col = 0
            # preserve spacing between tokens
            gap = scol - last_col
            if gap > 0:
                cur_line.append(' ' * gap)
            cur_line.append(tstring)
            last_col = end[1]
        if cur_line:
            out_lines.append(''.join(cur_line))
    except Exception:
        # fallback: remove '#' not inside strings naively
        lines = []
        in_triple = False
        for line in text.splitlines(True):
            if not in_triple:
                # naive split
                idx = None
                try:
                    idx = line.index('#')
                except ValueError:
                    lines.append(line)
                    continue
                lines.append(line[:idx].rstrip() + '\n')
            else:
                lines.append(line)
        return ''.join(lines)
    return '\n'.join(l.rstrip() for l in out_lines) + ('\n' if text.endswith('\n') else '')


def minify_whitespace(text: str) -> str:
    # Trim trailing spaces and collapse multiple blank lines to a single blank line
    lines = [ln.rstrip() for ln in text.splitlines()]
    out = []
    blank = False
    for ln in lines:
        if ln == "":
            if not blank:
                out.append("")
                blank = True
            else:
                continue
        else:
            out.append(ln)
            blank = False
    result = "\n".join(out)
    if text.endswith('\n'):
        result += '\n'
    return result


def process_text(text: str, lang: str, minify: bool) -> str:
    if lang in ("c", "cpp", "java", "c-like"):
        out = strip_comments_c_like(text)
    elif lang in ("py", "python"):
        out = strip_comments_python(text)
    else:
        raise ValueError(f"unknown language: {lang}")
    if minify:
        out = minify_whitespace(out)
    return out


def iter_input_files(patterns: Iterable[str]) -> Iterable[str]:
    for p in patterns:
        if any(ch in p for ch in "*?["):
            for m in glob.glob(p, recursive=True):
                yield m
        else:
            yield p


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description="Strip comments from source files")
    p.add_argument("inputs", nargs="*", help="Input file(s) or glob patterns; if empty, read stdin")
    p.add_argument("-l", "--lang", default="c", help="Language: c/cpp/java or py/python")
    p.add_argument("--minify", action="store_true", help="Trim trailing spaces and collapse blank lines")
    p.add_argument("-i", "--inplace", action="store_true", help="Edit files inplace")
    p.add_argument("-o", "--output", help="Write single output file (or stdout if omitted)")
    args = p.parse_args(argv)

    if not args.inputs:
        data = sys.stdin.read()
        out = process_text(data, args.lang, args.minify)
        if args.output:
            with open(args.output, "w", encoding="utf-8") as f:
                f.write(out)
        else:
            sys.stdout.write(out)
        return 0

    files = list(iter_input_files(args.inputs))
    if args.output and args.inplace:
        print("Cannot use --output and --inplace together", file=sys.stderr)
        return 2

    if args.output and len(files) > 1:
        # concatenate all processed files into the single output
        all_out = []
        for fn in files:
            with open(fn, "r", encoding="utf-8", errors="ignore") as f:
                all_out.append(process_text(f.read(), args.lang, args.minify))
        with open(args.output, "w", encoding="utf-8") as f:
            f.write('\n'.join(all_out))
        return 0

    for fn in files:
        with open(fn, "r", encoding="utf-8", errors="ignore") as f:
            out = process_text(f.read(), args.lang, args.minify)
        if args.inplace:
            with open(fn, "w", encoding="utf-8") as f:
                f.write(out)
        elif args.output:
            with open(args.output, "w", encoding="utf-8") as f:
                f.write(out)
        else:
            # print to stdout with a header comment
            sys.stdout.write(f"/* file: {fn} */\n")
            sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

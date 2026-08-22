# Merkle DAGs & Git Storage Internals: Systems Architecture Guide

> **Core Concept:** A Merkle DAG (Directed Acyclic Graph) is a graph data structure where every node is uniquely identified by the cryptographic hash of its contents and the hashes of its child edge links. It forms the foundational storage engine of **Git, IPFS, and Blockchain ledgers**, enabling tamper-proof verification, content deduplication, and sub-second history comparison.

---

## 💬 "Say It Out Loud" in an Interview (The 30-Second Elevator Pitch)

> **When the interviewer asks:** *"What is MERKLE and why/when do we use it?"*
>
> **You say:** *"A Merkle DAG is a graph where parent nodes contain the cryptographic hashes of their children. It allows checking if two complex directory trees are identical in O(1) time by comparing their top-level root hashes, forming the core tamper-proof storage model of Git and distributed content systems."*

---

## 1. What It Is in Plain English

Most people think Git is a system that saves file diffs (deltas). **It is not.**

Git is fundamentally a **Content-Addressable Object Store and a Merkle DAG**. Every time you commit, Git creates a complete point-in-time snapshot of your entire project directory. If 99 out of 100 files were not modified in that commit, Git does not duplicate those 99 files; it simply points the new Merkle directory tree to the existing cryptographic SHA-1/SHA-256 hashes of the untouched files.

---

## 2. The 4 Fundamental Git Object Types

```
+-----------------------------------------------------------------------------------+
|                              THE GIT OBJECT HIERARCHY                             |
+-----------------------------------------------------------------------------------+

                    [ COMMIT OBJECT: sha1("commit ...") ]
                    - tree: "a1b2c3..." (Root directory snapshot)
                    - parent: "f9e8d7..." (Previous commit hash)
                    - author: "Developer <dev@example.com>"
                    - message: "Add pipeline scheduler"
                                      |
                                      v
                     [ ROOT TREE OBJECT: sha1("tree ...") ]
                     - 100644 blob "44aa..." package.json
                     - 040000 tree "88bb..." src/
                                       |
                 +---------------------+---------------------+
                 |                                           |
                 v                                           v
[ BLOB OBJECT: sha1("blob ...") ]           [ SUB-TREE OBJECT: sha1("tree ...") ]
- Content: { "name": "my-app" }             - 100644 blob "99cc..." App.tsx
                                            - 100644 blob "11dd..." index.css
```

1. **`blob` (Binary Large Object):** Stores raw file contents (zero metadata; does not store file name or timestamp).
2. **`tree`:** Represents a directory. Stores file mode permissions (`100644`), object type, file name, and the SHA hash of the child `blob` or sub-`tree`.
3. **`commit`:** Points to a root `tree` object and zero, one, or more parent `commit` hashes, along with author metadata.
4. **`tag`:** A permanent, human-readable GPG-signed reference pointing to a specific commit.

---

## 3. Analogy for Live Interviews

> *"Imagine you take a photograph of your living room every day. Instead of buying a new room full of furniture for each photo (full duplicate storage), you keep one master warehouse containing numbered pieces of furniture (the CAS Blob store). Each photo is just an index card (the Tree object) that lists: 'Sofa #42 on the left, Lamp #88 on the right'. If you only replace the lamp, tomorrow's index card points to Sofa #42 and new Lamp #89. The living room history is completely preserved with minimal storage."*

---

## 4. Merkle Tree vs. Standard Tree Comparison

| Dimension | Merkle DAG (Git / IPFS) | Standard File System Tree (ext4 / NTFS) |
| :--- | :--- | :--- |
| **Node Identity** | Cryptographic hash of content (`SHA-256`) | Mutable Path string (`/home/user/app.ts`) |
| **Equality Check** | **$O(1)$ constant time (Compare root hashes)** | $O(N)$ recursive byte-by-byte comparison |
| **Tamper Evidence** | **100% Tamper-proof (Any bit change changes root)**| Vulnerable to silent bit rot or hidden file edits |
| **Deduplication** | **Automatic & native across the whole repo** | None (Duplicate files take duplicate disk blocks) |

---

## 5. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What happens under the hood when you run `git add` and `git commit`?
> **Answer:**
> 1. **`git add <file>`:** Git computes the SHA hash of the file content, compresses it with `zlib`, writes a new `blob` object to `.git/objects/`, and updates the staging area (**The Index file**: `.git/index`) mapping the file path to that blob hash.
> 2. **`git commit -m "msg"`:** Git writes a new `tree` object representing the current state of `.git/index`, writes a new `commit` object pointing to that tree hash and the current `HEAD` commit as parent, and moves the current branch reference pointer (`.git/refs/heads/main`) to the new commit hash.

### Q2: What is a Git Packfile and why is it needed?
> **Answer:** If a repository contains 100,000 commits where a 10MB file had 1 line changed per commit, storing 100,000 full 10MB loose blob objects would consume 1TB of disk.
> - **Packfiles (`.pack`):** Git periodically runs `git gc` to pack loose objects into a single binary file. It uses **Delta Compression**: it stores one full baseline object and records subsequent revisions as small reverse sliding-window diffs, achieving up to 95% storage compression.

### Q3: What is the difference between `git merge` and `git rebase`?
> **Answer:**
> - **`git merge`:** Combines two branches by creating a new **3-way Merge Commit** with two parent commit hashes. It preserves exact historical chronological order, but produces a non-linear "railroad track" commit graph.
> - **`git rebase`:** Takes the commits from the feature branch and **replays them one-by-one onto the tip of the base branch**, rewriting new commit hashes. It creates a clean, linear commit history, but alters original commit timestamps and author hashes.

### Q4: How does Git detect that two directories are 100% identical in $O(1)$ time?
> **Answer:** By comparing their **Tree SHA hashes**. Because the hash of a `tree` object is calculated from the sorted hashes of all its child blobs and sub-trees, if two directories have the exact same contents, their root tree hashes are guaranteed to be identical. If the top-level tree hashes match, Git skips inspecting any of the thousands of subdirectories or files inside.

---

## 6. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Does Git track file renames explicitly?"
- **The Answer:** **No.** Git does not store "rename" metadata anywhere in its object database. When you rename `foo.ts` to `bar.ts`, Git simply writes a new `tree` object pointing the name `bar.ts` to the existing unchanged `blob` hash. When running `git log` or `git status`, Git's diff engine dynamically detects renames on the fly using a similarity heuristic (if an added file shares >50% content with a deleted file).

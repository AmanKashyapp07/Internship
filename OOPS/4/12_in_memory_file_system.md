# LLD: In-Memory File System (File vs. Folder Hierarchy)

> **Core Category:** Hierarchical & Aggregation Modeling  
> **Key Design Pattern:** Composite Pattern (Leaf vs. Composite) + Visitor Pattern (Operations like Search / Disk Usage)  
> **Target Concepts:** Uniform Tree Node Interface, Recursive Size & Path Calculation, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"Design an In-Memory File System (like Linux ext4 or Windows NTFS). The file system contains `File` objects (store raw binary/text data, size, extension) and `Directory` objects (hold a collection of files and nested subdirectories). Both files and directories must respond uniformly to common operations like `getSize()`, `getAbsolutePath()`, and `delete()`. Only directories can support `addNode(INode)` and `getChild(name)`. How do you design this hierarchy cleanly without scattering `dynamic_cast<Directory*>` and `instanceof` checks everywhere?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Fat Base Node (LSP & ISP Violation)

```
+-------------------------------------------------------------------------+
|                             FileSystemNode                              |
+-------------------------------------------------------------------------+
| - isDirectory: bool                                                     |
| + getSize(): int                                                        |
| + addNode(node: Node): void      -> File throws NotSupportedException!  |
| + getChildren(): vector<Node>    -> File throws NotSupportedException!  |
| + writeContent(data: str): void  -> Directory throws NotSupportedExc!   |
+-------------------------------------------------------------------------+
```

#### Why the interviewer rejects this:
1. **Liskov Substitution Principle (LSP) Violation:** A client calling `node->addNode(...)` on an arbitrary `FileSystemNode` crashes if the node happens to be a `File`.
2. **Interface Pollution:** A `File` has dummy container methods; a `Directory` has dummy file-content methods.

---

### ❌ Anti-Pattern 2: Downcasting & `instanceof` Hell

```cpp
// ❌ Anti-Pattern in client code:
void calculateDiskUsage(FileSystemNode* node) {
    if (auto dir = dynamic_cast<Directory*>(node)) {
        for (auto child : dir->getChildren()) calculateDiskUsage(child);
    } else if (auto file = dynamic_cast<File*>(node)) {
        total += file->getSize();
    }
}
```

#### Why the interviewer rejects this:
- Violates **Open-Closed Principle (OCP)**: Adding new node types (e.g. `Symlink`, `ZipArchive`, `HardLink`) requires modifying every recursive traversal function across the codebase.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Apply the GoF Composite Pattern. Treat `File` (Leaf) and `Directory` (Composite) uniformly via `INode`. Composite `Directory` delegates common operations recursively to its children."*

```
+-----------------------------------------------------------------------------------+
|                            <<interface>> INode                                    |
+-----------------------------------------------------------------------------------+
| + getName()*: string                                                              |
| + getSize()*: size_t                                                              |
| + getAbsolutePath()*: string                                                      |
| + printHierarchy(indent: int)*: void                                              |
| + isDirectory()*: bool                                                            |
| + setParent(parent: INode*)*: void                                                |
+-----------------------------------------------------------------------------------+
           ^                                                      ^
           | implements (Leaf)                                    | implements (Composite)
           |                                                      |
+------------------------------------+         +------------------------------------+
|                File                |         |             Directory              |
+------------------------------------+         +------------------------------------+
| - name: string                     |         | - name: string                     |
| - content: string                  |         | - parent: INode*                   |
| - parent: INode*                   |         | - children: map<string,            |
+------------------------------------+         |             unique_ptr<INode>>     |
| + getSize(): content.size()        |         +------------------------------------+
| + read(): string                   |         | + getSize(): sum(child.getSize())  |
| + write(data): void                |         | + addEntry(node: unique_ptr<INode>)|
+------------------------------------+         | + deleteEntry(name: string): void  |
                                               | + getEntry(name): INode*           |
                                               +------------------------------------+
                                                                  |
                                                                  | 1 CONTAINS MANY
                                                                  +------> (Tree Recursion)
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Uniform Component Interface (Composite Pattern)
class INode {
public:
    virtual ~INode() = default;
    virtual std::string getName() const = 0;
    virtual size_t getSize() const = 0;
    virtual bool isDirectory() const = 0;
    virtual std::string getPath() const = 0;
};

// 2. Leaf Component: File
class File : public INode {
private:
    std::string name;
    std::string content;
    INode* parent;

public:
    File(std::string name, std::string content = "", INode* parent = nullptr)
        : name(std::move(name)), content(std::move(content)), parent(parent) {}

    std::string getName() const override { return name; }
    size_t getSize() const override { return content.size(); } // Byte size
    bool isDirectory() const override { return false; }
    std::string getPath() const override {
        return (parent ? parent->getPath() : "") + "/" + name;
    }
    void writeContent(const std::string& data) { content = data; }
    std::string readContent() const { return content; }
};

// 3. Composite Component: Directory (Holds and Aggregates Children)
class Directory : public INode {
private:
    std::string name;
    INode* parent;
    std::unordered_map<std::string, std::unique_ptr<INode>> children;

public:
    Directory(std::string name, INode* parent = nullptr) : name(std::move(name)), parent(parent) {}

    std::string getName() const override { return name; }
    bool isDirectory() const override { return true; }
    std::string getPath() const override {
        if (!parent) return name.empty() ? "/" : name; // Root directory
        return parent->getPath() + "/" + name;
    }

    // Recursive Aggregation across the tree
    size_t getSize() const override {
        size_t total = 0;
        for (const auto& [childName, childNode] : children) {
            total += childNode->getSize(); // Uniform recursive call!
        }
        return total;
    }

    // Directory-Specific Mutation
    void addEntry(std::unique_ptr<INode> node) {
        children[node->getName()] = std::move(node);
    }
    INode* getEntry(const std::string& childName) {
        return children.count(childName) ? children[childName].get() : nullptr;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we put container methods like `addEntry()` and file methods like `writeContent()` in a single base class, we violate the **Liskov Substitution Principle (LSP)** and **Interface Segregation Principle (ISP)** by forcing `File` to throw exceptions."*
> 2. *"If we separate `File` and `Directory` without a uniform interface, client code becomes littered with fragile **`dynamic_cast` and `instanceof` downcasts**."*
> 3. *"Instead, I apply the **GoF Composite Pattern**. Both `File` (Leaf) and `Directory` (Composite) implement the common `INode` interface (`getName()`, `getSize()`, `getPath()`)."*
> 4. *"When a client calls `rootDirectory->getSize()`, the directory recursively iterates over its child `INode` pointers without needing to know whether a given child is a file, another directory, or a symlink."*
> 5. *"This provides $O(1)$ extensibility: Adding a new node type like `Symlink` or `CompressedArchive` requires zero changes to the directory traversal engine."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you support complex external operations like Antivirus Scanning or Search?"** | Apply the **Visitor Pattern** (`INodeVisitor` with `visit(File&)` and `visit(Directory&)`) to add new operations without polluting `INode`. |
| **"How do you handle symbolic links (Symlinks) without infinite recursion?"** | Create a `Symlink` class inheriting `INode` that points to a target `INode*`, and maintain a `visitedSet` during recursive traversals to detect circular loops. |
| **"How do you optimize `getSize()` if directories contain millions of files?"** | Cache `cachedSize` at each directory node and invalidate parent caches on file write/delete operations (Eager / Lazy Write-Through caching). |

# LLD: Smart Device Features (Printer, Scanner, Copier, Fax)

> **Core Category:** Role & Behavior Combinations / Capability Modeling  
> **Key Design Pattern:** Interface Segregation Principle (ISP) + Component Strategy / Composite Capabilities  
> **Target Concepts:** Interface Segregation Principle (ISP), Liskov Substitution Principle (LSP), Decorator/Pipeline (Scan -> Print = Copy)

---

## 1. The Interview Question

> *"An office equipment manufacturer sells various devices: `BasicPrinter` (prints only), `PhotoScanner` (scans only), `MultiFunctionPrinter` (prints, scans, copies), and `EnterpriseHub` (prints, scans, copies, faxes, network-shares). How do you model device capabilities cleanly without bloated interfaces or combinatorial subclasses?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Monolithic "Fat" Interface (ISP & LSP Violation)

```
+-------------------------------------------------------------------------+
|                              <<interface>>                              |
|                              ISmartDevice                               |
+-------------------------------------------------------------------------+
| + printDocument(doc: Document): void                                    |
| + scanDocument(): RawImage                                              |
| + sendFax(number: string, doc: Document): void                          |
| + shareOverNetwork(ip: string): void                                    |
+-------------------------------------------------------------------------+
                                     ^
                                     | implements
                +--------------------+--------------------+
                |                                         |
+-------------------------------+         +-------------------------------+
|         BasicPrinter          |         |         PhotoScanner          |
+-------------------------------+         +-------------------------------+
| + printDocument(doc): void    |         | + printDocument(doc): throw!  |
| + scanDocument(): throw Err!  |         | + scanDocument(): RawImage    |
| + sendFax(...): throw Err!    |         | + sendFax(...): throw Err!    |
| + shareOverNetwork(...): throw|         | + shareOverNetwork(...): throw|
+-------------------------------+         +-------------------------------+
```

#### Why the interviewer rejects this:
1. **Interface Pollution (ISP Violation):** Clients of `BasicPrinter` are forced to depend on methods like `scanDocument()` and `sendFax()` that the hardware physically cannot perform.
2. **Runtime Fragility (LSP Violation):** If a client writes polymorphic code:
   ```cpp
   void digitizePaper(ISmartDevice* device) {
       device->scanDocument(); // CRASHES if a BasicPrinter is passed!
   }
   ```
   Subclasses cannot be safely substituted for their base interface without checking types or catching runtime exceptions.

---

### ❌ Anti-Pattern 2: Combinatorial Multiple Inheritance ($2^N$ Subclasses)

```
                 [PrintOnly]       [ScanOnly]       [FaxOnly]
                      \               /                /
                       \             /                /
                     [PrinterScanner]                /
                            \                       /
                             \                     /
                         [PrinterScannerFax]  <-- Combinatorial Class Explosion!
```

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight:
1. **Segregate Interfaces:** Define pure, single-purpose capability contracts (`IPrinter`, `IScanner`, `IFax`, `INetworkShareable`).
2. **Decompose Hardware Engines:** Realize that a "Copy" operation is simply a **pipeline** of `IScanner::scanDocument()` $\to$ `IPrinter::printDocument()`.

```
+------------------+    +------------------+    +------------------+    +------------------+
|  <<interface>>   |    |  <<interface>>   |    |  <<interface>>   |    |  <<interface>>   |
|     IPrinter     |    |     IScanner     |    |      IFax        |    | INetworkShareable|
+------------------+    +------------------+    +------------------+    +------------------+
| + print(doc)*    |    | + scan()*: Image |    | + fax(num, doc)* |    | + share(ip)*     |
+------------------+    +------------------+    +------------------+    +------------------+
         ^                       ^                       ^                       ^
         | implements            | implements            | implements            | implements
         |                       |                       |                       |
+--------+--------+     +--------+--------+     +--------+--------+              |
|  BasicPrinter   |     |  PhotoScanner   |     |  MultiFunction  |--------------+
+-----------------+     +-----------------+     |     Device      |
| - printEngine   |     | - scanEngine    |     +-----------------+
+-----------------+     +-----------------+     | - printEngine   |
| + print(doc)    |     | + scan(): Image |     | - scanEngine    |
+-----------------+     +-----------------+     | - faxEngine     |
                                                +-----------------+
                                                | + print(doc)    |
                                                | + scan(): Image |
                                                | + copy(copies)  | <--- Pipeline: scan() -> print()
                                                | + fax(num, doc) |
                                                | + share(ip)     |
                                                +-----------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Segregated, Single-Responsibility Interfaces (ISP Compliant)
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void printDocument(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual std::string scanDocument() = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void sendFax(const std::string& phone, const std::string& doc) = 0;
};

// 2. Simple Leaf Device (Implements ONLY what it supports)
class BasicPrinter : public IPrinter {
public:
    void printDocument(const std::string& doc) override {
        // Output via thermal/laser print engine
    }
};

// 3. Multi-Function Device (Composes multiple capabilities & implements matching interfaces)
class MultiFunctionDevice : public IPrinter, public IScanner, public IFax {
private:
    std::unique_ptr<IPrinter> printEngine;
    std::unique_ptr<IScanner> scanEngine;
    std::unique_ptr<IFax> faxEngine;

public:
    MultiFunctionDevice(std::unique_ptr<IPrinter> p, 
                        std::unique_ptr<IScanner> s, 
                        std::unique_ptr<IFax> f)
        : printEngine(std::move(p)), scanEngine(std::move(s)), faxEngine(std::move(f)) {}

    void printDocument(const std::string& doc) override {
        printEngine->printDocument(doc);
    }

    std::string scanDocument() override {
        return scanEngine->scanDocument();
    }

    void sendFax(const std::string& phone, const std::string& doc) override {
        faxEngine->sendFax(phone, doc);
    }

    // High-Level Value-Add Operation: Copying is a Scan -> Print pipeline!
    void copyDocument(int copies = 1) {
        std::string scannedImage = scanEngine->scanDocument();
        for (int i = 0; i < copies; ++i) {
            printEngine->printDocument(scannedImage);
        }
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"The primary mistake in designing smart devices is creating a monolithic `ISmartDevice` interface with `print()`, `scan()`, and `fax()`. This forces a `BasicPrinter` to throw runtime errors for `scan()`, directly violating the **Interface Segregation Principle (ISP)** and **Liskov Substitution Principle (LSP)**."*
> 2. *"Instead, we break down capabilities into fine-grained interfaces: `IPrinter`, `IScanner`, and `IFax`. A `BasicPrinter` implements only `IPrinter`, giving clients compile-time type safety."*
> 3. *"For a `MultiFunctionDevice`, we implement all three interfaces, but delegate the underlying work to modular hardware engines (`printEngine`, `scanEngine`, `faxEngine`)."*
> 4. *"Notice how **Copying** is modeled: We don't need a separate `ICopier` hardware engine. Copying is naturally modeled as a behavioral pipeline that calls `scanEngine->scanDocument()` and feeds the result into `printEngine->printDocument()`."*
> 5. *"This architecture enables pluggable hardware engines (e.g. swapping a Laser Printer engine for an Inkjet engine) with zero changes to client code."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you expose these devices to a network print server?"** | Create an `INetworkShareable` interface or a `NetworkDeviceAdapter` that wraps any `IPrinter` or `IScanner` with an IP/TCP transport layer. |
| **"What if the device runs out of ink during a copy job?"** | Use the **Observer Pattern**: Hardware engines publish `EngineStatusEvent` (OutOfPaper, LowInk, PaperJam) to a central `DeviceHealthMonitor`. |
| **"Can you add duplex (double-sided) printing without modifying `IPrinter`?"** | Use the **Decorator Pattern** (`DuplexPrintDecorator` wrapping `IPrinter`) or pass a `PrintOptions` configuration struct to `printDocument(doc, options)`. |

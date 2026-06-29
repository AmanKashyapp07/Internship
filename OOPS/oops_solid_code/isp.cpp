/**
 * @file isp.cpp
 * @brief Demonstration of the Interface Segregation Principle (ISP)
 * 
 * ISP: Clients should not be forced to depend on methods they do not use.
 */

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// ============================================================================
// ❌ VIOLATION: A single, bloated interface for all device operations.
// Devices that can only print are forced to implement scan and fax.
// ============================================================================
class IMultiFunctionDevice {
public:
    virtual ~IMultiFunctionDevice() = default;
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
};

class BadOldFashionedPrinter : public IMultiFunctionDevice {
public:
    void print() override {
        cout << "Printing document..." << endl;
    }

    // Forced to implement methods that the hardware does not support!
    void scan() override {
        throw runtime_error("Error: Scan feature is not supported by this printer!");
    }

    void fax() override {
        throw runtime_error("Error: Fax feature is not supported by this printer!");
    }
};

// ============================================================================
// ✅ RESOLUTION: Segregate the interfaces into small, cohesive, focused units.
// ============================================================================

class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print() = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan() = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax() = 0;
};

// Simple Printer only depends on printing interface
class SimplePrinter : public IPrinter {
public:
    void print() override {
        cout << "SimplePrinter: Printing document..." << endl;
    }
};

// Advanced Multi-Function Printer implements multiple interfaces
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax {
public:
    void print() override {
        cout << "MFP: Printing document..." << endl;
    }

    void scan() override {
        cout << "MFP: Scanning document..." << endl;
    }

    void fax() override {
        cout << "MFP: Faxing document..." << endl;
    }
};

int main() {
    cout << "=== ISP Demonstration ===" << endl << endl;

    // ❌ Testing Bad Design
    cout << "[Bad Design Example]" << endl;
    BadOldFashionedPrinter badPrinter;
    badPrinter.print();
    try {
        badPrinter.scan(); // Throws exception at runtime
    } catch (const exception& e) {
        cout << "Caught expected exception: " << e.what() << endl;
    }

    cout << endl;

    // ✅ Testing Good Design (ISP Compliant)
    cout << "[Good Design Example]" << endl;
    SimplePrinter simplePrinter;
    simplePrinter.print(); // Simple, clean, no unsupported stubs needed.

    cout << endl;

    MultiFunctionPrinter mfp;
    mfp.print();
    mfp.scan();
    mfp.fax();

    return 0;
}

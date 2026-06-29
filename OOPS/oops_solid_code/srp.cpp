/**
 * @file srp.cpp
 * @brief Demonstration of the Single Responsibility Principle (SRP)
 * 
 * SRP: A class should have one, and only one, reason to change.
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Struct to represent a simple item
struct Book {
    string title;
    double price;
};

// ============================================================================
// ❌ VIOLATION: The Invoice class handles data, printing, and file persistence.
// It has multiple reasons to change: if invoice calculation changes, if formatting
// changes, or if persistence mechanism changes.
// ============================================================================
class BadInvoice {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& book : books) {
            total += book.price;
        }
        return total;
    }

    // Reason to change #1: Printing / Formatting logic changes
    void printInvoice() const {
        cout << "--- BadInvoice ---" << endl;
        for (const auto& book : books) {
            cout << book.title << " : $" << book.price << endl;
        }
        cout << "Total: $" << calculateTotal() << endl;
    }

    // Reason to change #2: Storage / Persistence mechanism changes
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "--- Invoice ---" << endl;
            for (const auto& book : books) {
                file << book.title << "," << book.price << endl;
            }
            file << "Total: " << calculateTotal() << endl;
            file.close();
            cout << "Saved invoice to " << filename << endl;
        }
    }
};

// ============================================================================
// ✅ RESOLUTION: Split the class into cohesive units with single responsibilities.
// ============================================================================

// Responsibility 1: Manage invoice data and logic
class Invoice {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    const vector<Book>& getBooks() const {
        return books;
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& book : books) {
            total += book.price;
        }
        return total;
    }
};

// Responsibility 2: Handle printing and formatting of the invoice
class InvoicePrinter {
public:
    void print(const Invoice& invoice) const {
        cout << "--- GoodInvoice ---" << endl;
        for (const auto& book : invoice.getBooks()) {
            cout << book.title << " : $" << book.price << endl;
        }
        cout << "Total: $" << invoice.calculateTotal() << endl;
    }
};

// Responsibility 3: Handle saving and persistence of the invoice
class InvoiceRepository {
public:
    void saveToFile(const Invoice& invoice, const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "--- Invoice ---" << endl;
            for (const auto& book : invoice.getBooks()) {
                file << book.title << "," << book.price << endl;
            }
            file << "Total: " << invoice.calculateTotal() << endl;
            file.close();
            cout << "Saved invoice to " << filename << " via InvoiceRepository" << endl;
        }
    }
};

int main() {
    cout << "=== SRP Demonstration ===" << endl << endl;

    Book b1{"Introduction to Algorithms", 89.99};
    Book b2{"Clean Code", 39.99};

    // ❌ Testing Bad Design
    cout << "[Bad Design Example]" << endl;
    BadInvoice badInvoice;
    badInvoice.addBook(b1);
    badInvoice.addBook(b2);
    badInvoice.printInvoice();
    badInvoice.saveToFile("bad_invoice.txt");

    cout << endl;

    // ✅ Testing Good Design (SRP Compliant)
    cout << "[Good Design Example]" << endl;
    Invoice goodInvoice;
    goodInvoice.addBook(b1);
    goodInvoice.addBook(b2);

    InvoicePrinter printer;
    printer.print(goodInvoice);

    InvoiceRepository repo;
    repo.saveToFile(goodInvoice, "good_invoice.txt");

    return 0;
}

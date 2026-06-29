/**
 * @file dip.cpp
 * @brief Demonstration of the Dependency Inversion Principle (DIP)
 * 
 * DIP: High-level modules should not depend on low-level modules. Both should 
 * depend on abstractions. Abstractions should not depend on details. Details 
 * should depend on abstractions.
 */

#include <iostream>
#include <string>
#include <memory>

using namespace std;

// ============================================================================
// ❌ VIOLATION: PasswordMessenger (high-level) directly depends on EmailSender
// (low-level, concrete class). Tightly coupled.
// ============================================================================
class BadEmailSender {
public:
    void sendEmail(const string& message) const {
        cout << "EmailSender: Sending email -> " << message << endl;
    }
};

class BadPasswordMessenger {
private:
    BadEmailSender emailSender; // Direct dependency on low-level detail!

public:
    void sendPasswordResetLink(const string& email) const {
        string msg = "Click here to reset your password: http://reset.link/123";
        // Cannot easily swap emailSender with SMSSender, or Mock for unit tests
        emailSender.sendEmail(msg);
    }
};

// ============================================================================
// ✅ RESOLUTION: Both depend on the abstraction IMessageSender.
// ============================================================================

// The Abstraction Interface
class IMessageSender {
public:
    virtual ~IMessageSender() = default;
    virtual void sendMessage(const string& message) const = 0;
};

// Low-level detail 1: Email Sender
class EmailSender : public IMessageSender {
public:
    void sendMessage(const string& message) const override {
        cout << "EmailSender: Sending email -> " << message << endl;
    }
};

// Low-level detail 2: SMS Sender (added easily!)
class SmsSender : public IMessageSender {
public:
    void sendMessage(const string& message) const override {
        cout << "SmsSender: Sending SMS -> " << message << endl;
    }
};

// High-level module: depends on the abstraction IMessageSender
class PasswordMessenger {
private:
    shared_ptr<IMessageSender> sender; // Dependency on abstraction

public:
    // Dependency Injection (Constructor Injection)
    PasswordMessenger(shared_ptr<IMessageSender> s) : sender(s) {}

    void sendPasswordResetLink() const {
        string msg = "Click here to reset your password: http://reset.link/123";
        if (sender) {
            sender->sendMessage(msg);
        }
    }
};

int main() {
    cout << "=== DIP Demonstration ===" << endl << endl;

    // ❌ Testing Bad Design
    cout << "[Bad Design Example]" << endl;
    BadPasswordMessenger badMessenger;
    badMessenger.sendPasswordResetLink("user@example.com");

    cout << endl;

    // ✅ Testing Good Design (DIP Compliant)
    cout << "[Good Design Example]" << endl;
    
    // Injecting Email sender
    cout << "--- Sending via Email ---" << endl;
    auto emailSender = make_shared<EmailSender>();
    PasswordMessenger emailMessenger(emailSender);
    emailMessenger.sendPasswordResetLink();

    cout << endl;

    // Injecting SMS sender without changing the high-level PasswordMessenger class!
    cout << "--- Sending via SMS ---" << endl;
    auto smsSender = make_shared<SmsSender>();
    PasswordMessenger smsMessenger(smsSender);
    smsMessenger.sendPasswordResetLink();

    return 0;
}

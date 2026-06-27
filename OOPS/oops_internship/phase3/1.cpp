/**
 * File: 1.cpp
 * Demonstrates: Encapsulation & Access Modifiers
 * Description: Demonstrates restricting access to internal variables (private) and exposing them through validated setters and getters (public).
 * Interview Takeaway: Encapsulation protects class invariants and prevents unauthorized state corruption.
 */

// SRP demonstration: Non-SRP vs SRP with different classes
#include <iostream>
#include <string>

using namespace std;

// -------------------- Non-SRP --------------------
// One class has multiple reasons to change.
class UserNonSRP { // Class definition
private: // Inaccessible from outside the class
    string username;
    string email;

public: // Accessible from anywhere in the program
    UserNonSRP(string uname, string mail) : username(uname), email(mail) {}

    string getEmail() const { return email; }
    string getUsername() const { return username; }
    void updateProfile(string newEmail) { email = newEmail; }

    void saveToDatabase() const {
        cout << "[Non-SRP] Connecting to database...\n";
        cout << "[Non-SRP] Saving user " << username << " to SQL table.\n";
    }

    void sendWelcomeEmail() const {
        cout << "[Non-SRP] Connecting to SMTP server...\n";
        cout << "[Non-SRP] Sending welcome newsletter to " << email << "\n";
    }
};

// -------------------- SRP --------------------
// 1) Only profile data and profile operations.
class UserProfile { // Class definition
private: // Inaccessible from outside the class
    string username;
    string email;

public: // Accessible from anywhere in the program
    UserProfile(string uname, string mail) : username(uname), email(mail) {}

    string getEmail() const { return email; }
    string getUsername() const { return username; }
    void updateEmail(string newEmail) { email = newEmail; }
};

// 2) Only persistence responsibility.
class UserRepository { // Class definition
public: // Accessible from anywhere in the program
    void save(const UserProfile& user) const {
        cout << "[SRP] Connecting to database...\n";
        cout << "[SRP] Saving user " << user.getUsername() << " to SQL table.\n";
    }
};

// 3) Only notification responsibility.
class EmailService { // Class definition
public: // Accessible from anywhere in the program
    void sendWelcome(const UserProfile& user) const {
        cout << "[SRP] Connecting to SMTP server...\n";
        cout << "[SRP] Sending welcome newsletter to " << user.getEmail() << "\n";
    }
};

int main() { // Entry point of the program
    cout << "--- Non-SRP flow ---\n";
    UserNonSRP user1("Aman", "aman@mail.com");
    user1.saveToDatabase();
    user1.sendWelcomeEmail();

    cout << "\n--- SRP flow (different classes) ---\n";
    UserProfile user2("Aman", "aman@mail.com");
    UserRepository repo;
    EmailService mailer;

    repo.save(user2);
    mailer.sendWelcome(user2);

    return 0;
}
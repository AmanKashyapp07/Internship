# LLD: Banking Account Types (Savings, Current, Overdraft, Multi-Currency)

> **Core Category:** Dynamic State & Entity Evolution / Non-Invasive Feature Augmentation  
> **Key Design Pattern:** Decorator Pattern / Account Capability Wrappers (or Strategy / State Pattern)  
> **Target Concepts:** Preserving Account Invariants & Transaction Logs, Non-Invasive Feature Upgrades, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"A customer opens a standard `SavingsAccount` with account number `ACC-101` and generates hundreds of deposit and withdrawal transactions over 3 years. Later, the bank approves them for an `OverdraftFacility` (allows balance to go down to -$5,000 with interest) and a `MultiCurrencyWallet` (hold balances in USD, EUR, GBP). How do you model this so existing transaction ledgers and account numbers remain intact without deleting/reallocating the core account object?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Static Class Hierarchy Replacement

```
                           +------------------------+
                           |      BankAccount       |  (accountNumber, ledger, balance)
                           +------------------------+
                             /          |         \
                            /           |          \
                 +---------------+ +-----------+ +---------------+
                 |SavingsAccount | |CurrentAcc | |OverdraftAcc   |
                 +---------------+ +-----------+ +---------------+
                                                        \
                                                         \
                                                  +---------------+
                                                  |MultiCurrOver..|  <-- Combinatorial Explosion!
                                                  +---------------+
```

#### Why the interviewer rejects this:
1. **The Broken Ledger Catastrophe:** When `ACC-101` is upgraded to `OverdraftAccount`, you cannot change the runtime class of `SavingsAccount`. If you delete the `SavingsAccount` object and instantiate `OverdraftAccount`:
   - Past transaction pointers are severed.
   - External services (Direct Debit, Bill Pay, Credit Cards) holding references to the old instance will experience dangling pointers or invalid state.
2. **Feature Combinations:** What about `SavingsWithOverdraft` vs `CurrentWithOverdraftAndForex`? You end up with $2^N$ subclasses.

---

### ❌ Anti-Pattern 2: The God Account Class

```
+-------------------------------------------------------------------------+
|                               BankAccount                               |
+-------------------------------------------------------------------------+
| - hasOverdraft: bool                                                    |
| - overdraftLimit: double                                                |
| - hasMultiCurrency: bool                                                |
| - foreignBalances: map<Currency, double>                                |
| + withdraw(amount, currency): void -> massive spaghetti if/else tree!  |
+-------------------------------------------------------------------------+
```

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"The Core Account manages ledger balance and account identity. Features like Overdraft and Multi-Currency are non-invasive Decorator Wrappers or Pluggable Capability Strategies around the withdrawal pipeline."*

```
+-----------------------------------------------------------------------------------+
|                                   <<interface>>                                   |
|                                   IBankAccount                                    |
+-----------------------------------------------------------------------------------+
| + getAccountNumber()*: string                                                     |
| + getAvailableBalance()*: double                                                  |
| + deposit(amount: double, currency: string)*: void                                |
| + withdraw(amount: double, currency: string)*: bool                               |
| + printStatement()*: void                                                         |
+-----------------------------------------------------------------------------------+
           ^                                                      ^
           | implements                                           | implements & wraps
           |                                                      |
+------------------------------------+         +------------------------------------+
|          BaseBankAccount           |         |       AccountDecorator (Base)      |
+------------------------------------+         +------------------------------------+
| - accountNumber: string            |         | # wrappedAccount: unique_ptr<IBank>|
| - balance: double                  |         +------------------------------------+
| - ledger: vector<Transaction>      |         | + withdraw(...) -> delegate        |
+------------------------------------+         +------------------------------------+
| + deposit(amount, currency): void  |                            ^
| + withdraw(amount, currency): bool |                            | extends
+------------------------------------+                 +----------+----------+
                                                       |                     |
                                            +---------------------+ +---------------------+
                                            |  OverdraftDecorator | | MultiCurrDecorator  |
                                            +---------------------+ +---------------------+
                                            | - overdraftLimit:dbl| | - foreignBalances   |
                                            | - interestRate: dbl | | - exchangeService   |
                                            +---------------------+ +---------------------+
                                            | + withdraw() {      | | + withdraw() {      |
                                            |     allow negative  | |     convert currency|
                                            |     up to limit     | |     or draw FX wallet
                                            |   }                 | |   }                 |
                                            +---------------------+ +---------------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Core Bank Account Interface
class IBankAccount {
public:
    virtual ~IBankAccount() = default;
    virtual std::string getAccountNumber() const = 0;
    virtual double getAvailableBalance() const = 0;
    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;
};

// 2. Concrete Base Account (Identity + Pure Ledger)
class BaseSavingsAccount : public IBankAccount {
private:
    std::string accountNumber;
    double balance;

public:
    BaseSavingsAccount(std::string accNo, double initBalance)
        : accountNumber(std::move(accNo)), balance(initBalance) {}

    std::string getAccountNumber() const override { return accountNumber; }
    double getAvailableBalance() const override { return balance; }
    void deposit(double amount) override { balance += amount; }

    bool withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false; // Insufficient funds in standard savings
    }
};

// 3. Decorator Base (Wraps another IBankAccount transparently)
class AccountDecorator : public IBankAccount {
protected:
    std::unique_ptr<IBankAccount> innerAccount;
public:
    explicit AccountDecorator(std::unique_ptr<IBankAccount> acc) : innerAccount(std::move(acc)) {}
    std::string getAccountNumber() const override { return innerAccount->getAccountNumber(); }
    double getAvailableBalance() const override { return innerAccount->getAvailableBalance(); }
    void deposit(double amount) override { innerAccount->deposit(amount); }
    bool withdraw(double amount) override { return innerAccount->withdraw(amount); }
};

// 4. Overdraft Feature Decorator (Augments Withdrawal Pipeline)
class OverdraftAccountDecorator : public AccountDecorator {
private:
    double overdraftLimit; // e.g. $5,000

public:
    OverdraftAccountDecorator(std::unique_ptr<IBankAccount> acc, double limit)
        : AccountDecorator(std::move(acc)), overdraftLimit(limit) {}

    double getAvailableBalance() const override {
        return innerAccount->getAvailableBalance() + overdraftLimit;
    }

    bool withdraw(double amount) override {
        // First attempt standard deduction from real balance
        if (innerAccount->withdraw(amount)) return true;

        // If real balance is insufficient, check overdraft headroom
        double currentBal = innerAccount->getAvailableBalance();
        if (currentBal + overdraftLimit >= amount) {
            innerAccount->deposit(-amount); // Force balance into negative
            return true;
        }
        return false;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model `OverdraftAccount` via class inheritance, upgrading a 3-year-old savings account forces us to delete the original object and create a new instance, which breaks **ledger auditability and object references** across banking microservices."*
> 2. *"Instead, I apply the **Decorator Pattern** (or pluggable Policy Strategy). The core `BaseSavingsAccount` encapsulates the account number and base ledger."*
> 3. *"When the customer is approved for an overdraft facility, we wrap the existing `BaseSavingsAccount` inside an `OverdraftAccountDecorator`. The decorator transparently intercepts `withdraw()` requests to allow negative balances up to the authorized limit."*
> 4. *"This satisfies the **Open-Closed Principle (OCP)**: We can dynamically chain multiple feature wrappers at runtime (e.g. `MultiCurrencyDecorator(OverdraftDecorator(BaseSavingsAccount))` with **zero modifications to the core banking ledger**."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle interest calculation on negative overdraft balances?"** | Add a scheduled `applyMonthlyInterest()` method on the decorator that queries `if (balance < 0) chargeFee(balance * rate)`. |
| **"How do you prevent unauthorized decorators from being attached?"** | Use a `BankingAccountFactory` or `AccountBuilder` that validates customer credit scores and KYC status before assembling the decorator chain. |
| **"What if the customer wants to cancel the overdraft facility later?"** | Simply peel off the decorator: `std::unique_ptr<IBankAccount> standardAccount = overdraftDecorator->unwrap()`. |

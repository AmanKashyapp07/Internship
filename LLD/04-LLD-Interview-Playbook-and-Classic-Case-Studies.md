# 04. LLD Interview Playbook & 5 Classic Case Studies

> **Interview Mindset**: In a 45-minute Low-Level Design (LLD) interview, the interviewer does not just look at your final code—they observe **how you structure ambiguity, isolate responsibilities, and apply design patterns in modern C++ without over-engineering**. 
> Following a predictable 5-step framework guarantees you never freeze or get lost mid-interview.

---

## 1. The 5-Step LLD Interview Framework

```
Step 1: Clarify Scope & Requirements (5 mins)
   │
   ▼
Step 2: Identify Core Entities & Attributes (5 mins)
   │
   ▼
Step 3: Map Relationships & Design Patterns (Rectangle Box Diagrams) (10 mins)
   │
   ▼
Step 4: Implement Clean C++ Code & Abstract Interfaces (20 mins)
   │
   ▼
Step 5: Address Extensibility & Concurrency Edge Cases (5 mins)
```

---

## 2. Case Study 1: Design a Parking Lot

### 1. Requirements & Clarifications
- Multi-floor parking lot with different spot types (`Compact`, `Large`, `Handicapped`).
- Different vehicle types (`Motorcycle`, `Car`, `Truck`).
- Dynamic fee calculation via Strategy Pattern (Hourly vs Flat rate).
- Entry and exit gates generating and processing tickets.

### 2. Class Diagram

```
┌────────────────────────────────────────────────────────┐
│                 ParkingLot (Singleton)                 │
└───────────────────────────┬────────────────────────────┘
                            │ Has-A vector of floors
                            ▼
┌────────────────────────────────────────────────────────┐
│                      ParkingFloor                      │
└───────────────────────────┬────────────────────────────┘
                            │ Has-A vector of spots
                            ▼
┌────────────────────────────────────────────────────────┐
│             << Abstract >> ParkingSpot                 │
└───────────────────────────┬────────────────────────────┘
                            │
          ┌─────────────────┼─────────────────┐
          ▼                 ▼                 ▼
    ┌───────────┐     ┌───────────┐     ┌───────────┐
    │CompactSpot│     │ LargeSpot │     │Handicapped│
    └───────────┘     └───────────┘     └───────────┘

┌──────────────────────────────────────────────┐
│                    Ticket                    │
│      (EntryTime, SpotID, Vehicle Details)    │
└──────────────────────┬───────────────────────┘
                       │ Calculates fee using
                       ▼
┌──────────────────────────────────────────────┐
│          << Interface >> FeeStrategy         │
└──────────────────────┬───────────────────────┘
                       │
          ┌────────────┴────────────┐
          ▼                         ▼
┌───────────────────┐     ┌───────────────────┐
│ HourlyFeeStrategy │     │FlatRateFeeStrategy│
└───────────────────┘     └───────────────────┘
```

### 3. Clean C++ Implementation

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>

// 1. Vehicle and Spot Enums
enum class VehicleType { MOTORCYCLE, CAR, TRUCK };
enum class SpotType { COMPACT, LARGE, HANDICAPPED };

class Vehicle {
private:
    std::string licensePlate;
    VehicleType type;
public:
    Vehicle(std::string plate, VehicleType t) : licensePlate(std::move(plate)), type(t) {}
    VehicleType getType() const { return type; }
    const std::string& getPlate() const { return licensePlate; }
};

// 2. Parking Spot Hierarchy
class ParkingSpot {
protected:
    std::string id;
    SpotType type;
    bool isFree{true};
    std::shared_ptr<Vehicle> currentVehicle{nullptr};

public:
    ParkingSpot(std::string spotId, SpotType t) : id(std::move(spotId)), type(t) {}
    virtual ~ParkingSpot() = default;

    bool isAvailable() const { return isFree; }
    
    void park(std::shared_ptr<Vehicle> v) {
        currentVehicle = std::move(v);
        isFree = false;
    }
    
    void vacate() {
        currentVehicle = nullptr;
        isFree = true;
    }

    virtual bool canFitVehicle(const Vehicle& v) const = 0;
};

class CompactSpot : public ParkingSpot {
public:
    explicit CompactSpot(std::string spotId) : ParkingSpot(std::move(spotId), SpotType::COMPACT) {}
    bool canFitVehicle(const Vehicle& v) const override {
        return v.getType() == VehicleType::CAR || v.getType() == VehicleType::MOTORCYCLE;
    }
};

// 3. Fee Strategy (Strategy Pattern)
class FeeStrategy {
public:
    virtual double calculateFee(long long durationSeconds) = 0;
    virtual ~FeeStrategy() = default;
};

class HourlyFeeStrategy : public FeeStrategy {
private:
    double hourlyRate;
public:
    explicit HourlyFeeStrategy(double rate) : hourlyRate(rate) {}
    double calculateFee(long long durationSeconds) override {
        double hours = std::max(1.0, durationSeconds / 3600.0);
        return hours * hourlyRate;
    }
};

// 4. Ticket Entity
class Ticket {
private:
    std::string ticketId;
    std::chrono::system_clock::time_point entryTime;
    std::shared_ptr<ParkingSpot> spot;
    std::shared_ptr<Vehicle> vehicle;

public:
    Ticket(std::string id, std::shared_ptr<ParkingSpot> s, std::shared_ptr<Vehicle> v)
        : ticketId(std::move(id)), entryTime(std::chrono::system_clock::now()), 
          spot(std::move(s)), vehicle(std::move(v)) {}

    long long getDurationSeconds() const {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - entryTime).count();
    }

    std::shared_ptr<ParkingSpot> getSpot() const { return spot; }
};
```

---

## 3. Case Study 2: Design Tic-Tac-Toe

### 1. Requirements
- $N \times N$ board (typically $3 \times 3$).
- 2 players with pieces (`X` and `O`).
- Turn-taking queue (`std::deque`).
- $O(1)$ win checking condition (tracking row, column, and diagonal counts rather than scanning all rows on every move).

### 2. Class Diagram

```
┌────────────────────────────────────────┐
│             TicTacToeGame              │
└───────────────┬────────────────┬───────┘
                │                │
      Has-A     │                │ Manages turn queue
                ▼                ▼
┌───────────────────────┐ ┌───────────────────────┐
│         Board         │ │     std::deque        │
│      (N x N Grid)     │ │     <Player>          │
└───────────────────────┘ └──────────┬────────────┘
                                     │
                                     ▼
                          ┌───────────────────────┐
                          │        Player         │
                          │ (Name, Piece: X or O) │
                          └───────────────────────┘
```

### 3. $O(1)$ Win-Check C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <cmath>

enum class PieceType { X, O };

class Player {
private:
    std::string name;
    PieceType piece;
public:
    Player(std::string n, PieceType p) : name(std::move(n)), piece(p) {}
    const std::string& getName() const { return name; }
    PieceType getPiece() const { return piece; }
};

class TicTacToeGame {
private:
    int n;
    std::vector<int> rowCounts;
    std::vector<int> colCounts;
    int diagCount{0};
    int antiDiagCount{0};
    std::vector<std::vector<int>> board;
    std::deque<Player> players;

public:
    TicTacToeGame(int size, Player p1, Player p2) 
        : n(size), rowCounts(size, 0), colCounts(size, 0), 
          board(size, std::vector<int>(size, 0)) {
        players.push_back(std::move(p1));
        players.push_back(std::move(p2));
    }

    // Returns true if this move results in a win (O(1) time complexity)
    bool makeMove(int r, int c, const Player& player) {
        if (r < 0 || r >= n || c < 0 || c >= n || board[r][c] != 0) {
            throw std::invalid_argument("Invalid board move coordinates!");
        }

        int val = (player.getPiece() == PieceType::X) ? 1 : -1;
        board[r][c] = val;

        rowCounts[r] += val;
        colCounts[c] += val;
        if (r == c) diagCount += val;
        if (r + c == n - 1) antiDiagCount += val;

        return std::abs(rowCounts[r]) == n ||
               std::abs(colCounts[c]) == n ||
               std::abs(diagCount) == n ||
               std::abs(antiDiagCount) == n;
    }
};
```

---

## 4. Case Study 3: Design an Elevator System

### 1. Requirements
- Multiple elevator cars inside a building.
- External requests (calling elevator up/down) and Internal requests (target floor).
- State transitions (`IDLE`, `MOVING_UP`, `MOVING_DOWN`).
- Request queues using ordered sets (`std::set`).

### 2. Class Diagram

```
┌────────────────────────────────────────┐
│           ElevatorController           │
└───────────────────┬────────────────────┘
                    │ Dispatches requests to
                    ▼
┌────────────────────────────────────────┐
│              ElevatorCar               │
└───────────────────┬────────────────────┘
                    │ Holds current state
                    ▼
┌────────────────────────────────────────┐
│     << Interface >> ElevatorState      │
└───────────────────┬────────────────────┘
                    │
       ┌────────────┼────────────┐
       ▼            ▼            ▼
 ┌───────────┐┌───────────┐┌───────────┐
 │ IdleState ││MovingUpSt.││MovingDnSt.│
 └───────────┘└───────────┘└───────────┘
```

### 3. State Pattern C++ Implementation

```cpp
#include <iostream>
#include <set>
#include <memory>

enum class Direction { UP, DOWN, IDLE };

class ElevatorCar; // Forward declaration

class ElevatorState {
public:
    virtual void handleFloorRequest(ElevatorCar& car, int floor) = 0;
    virtual void step(ElevatorCar& car) = 0;
    virtual ~ElevatorState() = default;
};

class ElevatorCar {
private:
    int currentFloor{0};
    Direction dir{Direction::IDLE};
    std::unique_ptr<ElevatorState> state;
    std::set<int> upRequests;
    std::set<int, std::greater<int>> downRequests;

public:
    ElevatorCar();

    void setState(std::unique_ptr<ElevatorState> newState) {
        state = std::move(newState);
    }

    void addUpRequest(int floor) { upRequests.insert(floor); }
    void addDownRequest(int floor) { downRequests.insert(floor); }
    
    int getCurrentFloor() const { return currentFloor; }
    void setCurrentFloor(int f) { currentFloor = f; }
    
    void processStep() {
        if (state) state->step(*this);
    }
};

class MovingUpState : public ElevatorState {
public:
    void handleFloorRequest(ElevatorCar& car, int floor) override {
        if (floor >= car.getCurrentFloor()) {
            car.addUpRequest(floor);
        } else {
            car.addDownRequest(floor);
        }
    }

    void step(ElevatorCar& car) override {
        car.setCurrentFloor(car.getCurrentFloor() + 1);
        std::cout << "Elevator moved up to floor: " << car.getCurrentFloor() << "\n";
    }
};
```

---

## 5. Case Study 4: Design Splitwise (Expense Sharing)

### 1. Requirements
- Users can create groups and add expenses.
- Different split types: `EQUAL`, `EXACT` amount, and `PERCENTAGE`.
- Strategy Pattern to validate and calculate amounts.

### 2. Class Diagram

```
┌────────────────────────────────────────┐
│                 Group                  │
└───────────────────┬────────────────────┘
                    │ Contains expenses
                    ▼
┌────────────────────────────────────────┐
│                Expense                 │
│        (Amount, PaidBy, Splits)        │
└─────────┬────────────────────┬─────────┘
          │                    │
          │ Uses Strategy      │ Has-A vector of
          ▼                    ▼
┌───────────────────┐┌───────────────────┐
│   SplitStrategy   ││       Split       │
│ (Equal/Exact/Pct) ││(User, AmountOwed) │
└───────────────────┘└───────────────────┘
```

### 3. Strategy Pattern C++ Implementation

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>

class User {
private:
    std::string id;
    std::string name;
public:
    User(std::string uid, std::string uname) : id(std::move(uid)), name(std::move(uname)) {}
    const std::string& getName() const { return name; }
};

class Split {
private:
    std::shared_ptr<User> user;
    double amount{0.0};
public:
    explicit Split(std::shared_ptr<User> u) : user(std::move(u)) {}
    void setAmount(double amt) { amount = amt; }
    double getAmount() const { return amount; }
    std::shared_ptr<User> getUser() const { return user; }
};

class SplitStrategy {
public:
    virtual void calculate(double totalAmount, std::vector<std::shared_ptr<Split>>& splits) = 0;
    virtual ~SplitStrategy() = default;
};

class EqualSplitStrategy : public SplitStrategy {
public:
    void calculate(double totalAmount, std::vector<std::shared_ptr<Split>>& splits) override {
        if (splits.empty()) return;
        double splitAmount = totalAmount / splits.size();
        for (auto& s : splits) {
            s->setAmount(splitAmount);
        }
    }
};

class ExactSplitStrategy : public SplitStrategy {
public:
    void calculate(double totalAmount, std::vector<std::shared_ptr<Split>>& splits) override {
        double sum = 0.0;
        for (const auto& s : splits) {
            sum += s->getAmount();
        }
        if (std::abs(sum - totalAmount) > 0.01) {
            throw std::invalid_argument("Exact split amounts do not sum up to total expense!");
        }
    }
};
```

---

## 6. Case Study 5: Design a Vending Machine

### 1. Requirements
- Select item, accept money (coins/cash), dispense item, return change.
- Handles edge cases: Insufficient funds, out of stock, refund on cancellation.
- State machine: `NoMoneyState` $\to$ `HasMoneyState` $\to$ `DispenseState` $\to$ `SoldOutState`.

### 2. State Machine Diagram

```
┌───────────────┐     Insert Coin     ┌───────────────┐     Select Item & OK     ┌───────────────┐
│1. NoMoneyState├────────────────────►│2. HasMoneyState├────────────────────────►│3.DispenseState│
└───────▲───────┘                     └───────┬───────┘                          └───────┬───────┘
        │                                     │                                          │
        │           Cancel Button             │                                          │ Dispense Complete
        └─────────────────────────────────────┘                                          ▼
        │                           Refund & Reset                       ┌───────────────┴──┐
        └────────────────────────────────────────────────────────────────┤ Next State or    │
                                                                         │ 4. SoldOutState  │
                                                                         └──────────────────┘
```

### 3. State Pattern C++ Implementation

```cpp
#include <iostream>
#include <memory>
#include <string>

class VendingMachine; // Forward declaration

class VendingState {
public:
    virtual void insertMoney(VendingMachine& machine, double amount) = 0;
    virtual void selectItem(VendingMachine& machine, const std::string& code) = 0;
    virtual void dispense(VendingMachine& machine) = 0;
    virtual void refund(VendingMachine& machine) = 0;
    virtual ~VendingState() = default;
};

class VendingMachine {
private:
    double balance{0.0};
    std::unique_ptr<VendingState> state;

public:
    VendingMachine(std::unique_ptr<VendingState> initial) : state(std::move(initial)) {}

    void setState(std::unique_ptr<VendingState> s) { state = std::move(s); }
    void addBalance(double amt) { balance += amt; }
    double getBalance() const { return balance; }
    void resetBalance() { balance = 0.0; }

    void insertMoney(double amt) { state->insertMoney(*this, amt); }
    void selectItem(const std::string& code) { state->selectItem(*this, code); }
    void dispense() { state->dispense(*this); }
    void refund() { state->refund(*this); }
};

class HasMoneyState : public VendingState {
public:
    void insertMoney(VendingMachine& m, double amount) override {
        m.addBalance(amount);
        std::cout << "Added $" << amount << ". Total balance: $" << m.getBalance() << "\n";
    }

    void selectItem(VendingMachine& m, const std::string& code) override {
        std::cout << "Item " << code << " selected. Validating inventory & balance...\n";
    }

    void dispense(VendingMachine& m) override {
        std::cout << "Please select an item before dispensing.\n";
    }

    void refund(VendingMachine& m) override {
        std::cout << "Refunding $" << m.getBalance() << " to user.\n";
        m.resetBalance();
    }
};
```

---

## 7. "Say This, Not That" in LLD Interviews

| Instead of saying / doing... ❌ | Say and do this like a senior C++ candidate... ✅ |
| :--- | :--- |
| *Jumping immediately into writing raw pointers (`T*`) and manual `delete`.* | *"I will use smart pointers (`std::unique_ptr` for exclusive ownership and `std::shared_ptr` for shared lifetimes) to adhere to RAII and guarantee zero memory leaks."* |
| *Writing raw C-style arrays (`int arr[100]`) or raw string buffers.* | *"I'll use STL containers like `std::vector` and `std::string` for dynamic safety, and `std::unordered_map` for $O(1)$ lookups."* |
| *Using a giant `switch (type)` statement to handle behaviors.* | *"I will apply the Strategy Pattern with pure virtual interfaces so we can add new algorithms in the future without modifying existing code (Open/Closed Principle)."* |
| *Forgetting `virtual ~Base() = default;` on polymorphic base classes.* | *"I'll mark the base class destructor as virtual so derived class cleanup triggers properly during polymorphic deletion."* |
| *Ignoring multi-threading completely.* | *"In a real production environment, multiple threads could call this concurrently. I will guard the critical section with `std::mutex` or `std::lock_guard` to prevent race conditions."* |

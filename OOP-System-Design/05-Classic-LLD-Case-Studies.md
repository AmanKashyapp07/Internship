# 05. Classic LLD Case Studies: Production C++ Blueprint

> **The LLD Interview Template**:
> In every 45-minute LLD interview, structure your whiteboard time as follows:
> 1. **Clarify Requirements & Constraints** (5 mins)
> 2. **Identify Entities & Draw ASCII Class Diagram** (10 mins)
> 3. **Define Abstract Interfaces & Design Patterns** (5 mins)
> 4. **Write Clean C++ Code with Smart Pointers & Concurrency** (20 mins)
> 5. **Discuss Edge Cases & Extensibility** (5 mins)

---

# Case Study 1: Parking Lot System

### 1. Requirements & Core Entities
- Multi-floor parking lot with multiple spot types: `COMPACT`, `LARGE`, `MOTORCYCLE`.
- Vehicles: `Motorcycle` fits any spot; `Car` fits `COMPACT` or `LARGE`; `Truck` fits only `LARGE`.
- Entry gate issues a `Ticket` with entry timestamp; Exit gate calculates fee using a dynamic `FeeStrategy`.

### 2. Class Diagram
```
┌─────────────────────────────────────┐
│             ParkingLot              │
├─────────────────────────────────────┤
│ - floors_: vector<ParkingFloor>     │
│ - feeStrategy_: unique_ptr<IFee>    │
│ - mtx_: mutex                       │
├─────────────────────────────────────┤
│ + parkVehicle(Vehicle&): Ticket     │
│ + exitVehicle(Ticket&): double      │
└──────────────────┬──────────────────┘
                   │ 1:N (◆)
                   ▼
┌─────────────────────────────────────┐
│            ParkingFloor             │
├─────────────────────────────────────┤
│ - floorId_: int                     │
│ - spots_: vector<ParkingSpot>       │
├─────────────────────────────────────┤
│ + findAndOccupy(Vehicle&): Spot*    │
└──────────────────┬──────────────────┘
                   │ 1:N (◆)
                   ▼
┌─────────────────────────────────────┐
│             ParkingSpot             │
├─────────────────────────────────────┤
│ - spotId_: int                      │
│ - spotType_: SpotType               │
│ - isOccupied_: bool                 │
│ - parkedVehicle_: Vehicle*          │
└─────────────────────────────────────┘
```

### 3. Production C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>

enum class VehicleType { MOTORCYCLE, CAR, TRUCK };
enum class SpotType { MOTORCYCLE, COMPACT, LARGE };

class Vehicle {
protected:
    std::string licensePlate_;
    VehicleType type_;
public:
    Vehicle(std::string plate, VehicleType type) 
        : licensePlate_(std::move(plate)), type_(type) {}
    virtual ~Vehicle() = default;
    VehicleType getType() const { return type_; }
    const std::string& getPlate() const { return licensePlate_; }
};

class Car : public Vehicle {
public:
    explicit Car(std::string plate) : Vehicle(std::move(plate), VehicleType::CAR) {}
};

class ParkingSpot {
private:
    int id_;
    SpotType type_;
    bool occupied_{false};
    Vehicle* vehicle_{nullptr};

public:
    ParkingSpot(int id, SpotType type) : id_(id), type_(type) {}

    bool canFit(const Vehicle& v) const {
        if (occupied_) return false;
        if (v.getType() == VehicleType::MOTORCYCLE) return true;
        if (v.getType() == VehicleType::CAR) return type_ == SpotType::COMPACT || type_ == SpotType::LARGE;
        if (v.getType() == VehicleType::TRUCK) return type_ == SpotType::LARGE;
        return false;
    }

    bool occupy(Vehicle& v) {
        if (!canFit(v)) return false;
        vehicle_ = &v;
        occupied_ = true;
        return true;
    }

    void vacate() {
        vehicle_ = nullptr;
        occupied_ = false;
    }

    int getId() const { return id_; }
};

struct Ticket {
    std::string ticketId;
    std::string plateNumber;
    int spotId;
    std::chrono::system_clock::time_point entryTime;
};

// Strategy Pattern for Parking Fees
class IFeeStrategy {
public:
    virtual ~IFeeStrategy() = default;
    virtual double calculate(std::chrono::hours hours, VehicleType type) const = 0;
};

class FlatRateFeeStrategy : public IFeeStrategy {
public:
    double calculate(std::chrono::hours hours, VehicleType type) const override {
        double ratePerHour = (type == VehicleType::TRUCK) ? 10.0 : (type == VehicleType::CAR ? 5.0 : 2.0);
        return std::max(1.0, static_cast<double>(hours.count())) * ratePerHour;
    }
};

class ParkingLot {
private:
    std::vector<ParkingSpot> spots_;
    std::unique_ptr<IFeeStrategy> feeStrategy_;
    std::mutex mtx_;

public:
    explicit ParkingLot(std::unique_ptr<IFeeStrategy> feeStrategy)
        : feeStrategy_(std::move(feeStrategy)) {
        // Initialize sample spots
        spots_.emplace_back(1, SpotType::MOTORCYCLE);
        spots_.emplace_back(2, SpotType::COMPACT);
        spots_.emplace_back(3, SpotType::LARGE);
    }

    Ticket issueTicket(Vehicle& vehicle) {
        std::lock_guard<std::mutex> lock(mtx_); // Thread-safe spot assignment
        for (auto& spot : spots_) {
            if (spot.occupy(vehicle)) {
                return Ticket{
                    "TCK-" + vehicle.getPlate(),
                    vehicle.getPlate(),
                    spot.getId(),
                    std::chrono::system_clock::now()
                };
            }
        }
        throw std::runtime_error("Parking Lot Full!");
    }

    double vacateSpot(const Ticket& ticket, VehicleType type) {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto& spot : spots_) {
            if (spot.getId() == ticket.spotId) {
                spot.vacate();
                auto duration = std::chrono::duration_cast<std::chrono::hours>(
                    std::chrono::system_clock::now() - ticket.entryTime);
                return feeStrategy_->calculate(duration, type);
            }
        }
        return 0.0;
    }
};
```

---

# Case Study 2: Library Management System

### 1. Key Concept: `Book` vs `BookItem`
- **Book**: Metadata (Title, Author, ISBN, Subject).
- **BookItem**: The physical copy in the library with a unique barcode and rack location.

### 2. Class Diagram
```
┌─────────────────────────┐               ┌─────────────────────────┐
│          Book           │1             N│        BookItem         │
├─────────────────────────┤──────────────►├─────────────────────────┤
│ - isbn_: string         │               │ - barcode_: string      │
│ - title_: string        │               │ - status_: BookStatus   │
│ - author_: string       │               │ - rackNumber_: string   │
└─────────────────────────┘               └────────────┬────────────┘
                                                       │
                                                       │ Borrowed By
                                                       ▼
┌─────────────────────────┐               ┌─────────────────────────┐
│     LibraryAccount      │1             N│       BookLending       │
├─────────────────────────┤──────────────►├─────────────────────────┤
│ - memberId_: string     │               │ - dueDate_: time_point  │
│ - name_: string         │               │ - returnDate_: time_pt  │
└─────────────────────────┘               └─────────────────────────┘
```

### 3. Production C++ Implementation

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

enum class BookStatus { AVAILABLE, LOANED, RESERVED, LOST };

class Book {
    std::string isbn_, title_, author_;
public:
    Book(std::string isbn, std::string title, std::string author)
        : isbn_(std::move(isbn)), title_(std::move(title)), author_(std::move(author)) {}
    const std::string& getTitle() const { return title_; }
};

class BookItem {
private:
    std::string barcode_;
    std::shared_ptr<Book> metadata_;
    BookStatus status_{BookStatus::AVAILABLE};

public:
    BookItem(std::string barcode, std::shared_ptr<Book> book)
        : barcode_(std::move(barcode)), metadata_(std::move(book)) {}

    bool checkout() {
        if (status_ != BookStatus::AVAILABLE) return false;
        status_ = BookStatus::LOANED;
        return true;
    }

    void returnBook() {
        status_ = BookStatus::AVAILABLE;
    }

    const std::string& getBarcode() const { return barcode_; }
    BookStatus getStatus() const { return status_; }
};

class LendingRecord {
public:
    std::string memberId;
    std::string barcode;
    std::chrono::system_clock::time_point checkoutDate;
    std::chrono::system_clock::time_point dueDate;

    double calculateFine() const {
        auto now = std::chrono::system_clock::now();
        if (now <= dueDate) return 0.0;
        auto daysLate = std::chrono::duration_cast<std::chrono::hours>(now - dueDate).count() / 24;
        return daysLate * 1.50; // $1.50 per day late fine
    }
};
```

---

# Case Study 3: ATM System

### 1. Requirements & Patterns
- **State Machine Pattern**: Handles states cleanly: `IdleState`, `HasCardState`, `AuthenticatedState`, `DispensingState`.
- **Chain of Responsibility**: Cash dispenser chain ($100 bills ──► $50 bills ──► $20 bills).

### 2. State Machine Diagram
```
┌──────────────┐   Insert Card    ┌──────────────┐   Enter Valid PIN   ┌──────────────┐
│  IdleState   ├─────────────────►│ HasCardState ├────────────────────►│ Authenticated│
└──────▲───────┘                  └──────┬───────┘                     └──────┬───────┘
       │                                 │ Invalid PIN                        │ Request Cash
       │ Transaction Complete            ▼                                    ▼
       └─────────────────────────── [Eject Card] ◄──────────────────── ┌──────────────┐
                                                                       │  Dispensing  │
                                                                       └──────────────┘
```

### 3. Production C++ Implementation

```cpp
#include <iostream>
#include <memory>

class ATMContext;

// Abstract State Interface
class IATMState {
public:
    virtual ~IATMState() = default;
    virtual void insertCard(ATMContext& ctx) = 0;
    virtual void authenticatePIN(ATMContext& ctx, int pin) = 0;
    virtual void withdrawCash(ATMContext& ctx, int amount) = 0;
    virtual void ejectCard(ATMContext& ctx) = 0;
};

// Forward Declarations of Concrete States
class IdleState;
class HasCardState;
class AuthenticatedState;

class ATMContext {
private:
    std::unique_ptr<IATMState> currentState_;
    int availableCash_{50000};

public:
    explicit ATMContext();

    void setState(std::unique_ptr<IATMState> newState) {
        currentState_ = std::move(newState);
    }

    void insertCard() { currentState_->insertCard(*this); }
    void authenticatePIN(int pin) { currentState_->authenticatePIN(*this, pin); }
    void withdrawCash(int amount) { currentState_->withdrawCash(*this, amount); }
    void ejectCard() { currentState_->ejectCard(*this); }

    void deductCash(int amount) { availableCash_ -= amount; }
    int getAvailableCash() const { return availableCash_; }
};

class IdleState : public IATMState {
public:
    void insertCard(ATMContext& ctx) override;
    void authenticatePIN(ATMContext&, int) override { std::cout << "Insert card first!\n"; }
    void withdrawCash(ATMContext&, int) override { std::cout << "Insert card first!\n"; }
    void ejectCard(ATMContext&) override { std::cout << "No card inside!\n"; }
};

class HasCardState : public IATMState {
public:
    void insertCard(ATMContext&) override { std::cout << "Card already inserted!\n"; }
    void authenticatePIN(ATMContext& ctx, int pin) override;
    void withdrawCash(ATMContext&, int) override { std::cout << "Authenticate PIN first!\n"; }
    void ejectCard(ATMContext& ctx) override;
};

class AuthenticatedState : public IATMState {
public:
    void insertCard(ATMContext&) override { std::cout << "Card already in session!\n"; }
    void authenticatePIN(ATMContext&, int) override { std::cout << "Already authenticated!\n"; }
    void withdrawCash(ATMContext& ctx, int amount) override;
    void ejectCard(ATMContext& ctx) override;
};

// State Transitions
void IdleState::insertCard(ATMContext& ctx) {
    std::cout << "[ATM]: Card inserted. Please enter PIN.\n";
    ctx.setState(std::make_unique<HasCardState>());
}

void HasCardState::authenticatePIN(ATMContext& ctx, int pin) {
    if (pin == 1234) {
        std::cout << "[ATM]: PIN Verified successfully.\n";
        ctx.setState(std::make_unique<AuthenticatedState>());
    } else {
        std::cout << "[ATM]: Invalid PIN. Ejecting card.\n";
        ctx.setState(std::make_unique<IdleState>());
    }
}

void HasCardState::ejectCard(ATMContext& ctx) {
    std::cout << "[ATM]: Card ejected.\n";
    ctx.setState(std::make_unique<IdleState>());
}

void AuthenticatedState::withdrawCash(ATMContext& ctx, int amount) {
    if (amount <= ctx.getAvailableCash()) {
        ctx.deductCash(amount);
        std::cout << "[ATM]: Dispensed $" << amount << ". Remaining ATM cash: $" 
                  << ctx.getAvailableCash() << "\n";
    } else {
        std::cout << "[ATM]: Insufficient cash inside ATM!\n";
    }
    ejectCard(ctx);
}

void AuthenticatedState::ejectCard(ATMContext& ctx) {
    std::cout << "[ATM]: Thank you. Card returned.\n";
    ctx.setState(std::make_unique<IdleState>());
}

ATMContext::ATMContext() : currentState_(std::make_unique<IdleState>()) {}
```

---

# Case Study 4: Elevator Control System

### 1. Requirements & Algorithms
- Multi-elevator system. Each elevator has a direction (`UP`, `DOWN`, `IDLE`).
- Internal car buttons (destination floor) + Hall buttons (Up/Down call).
- **LOOK / SCAN Algorithm**: Continues in current direction serving all requests until none remain, then reverses direction.

### 2. Class Diagram
```
┌─────────────────────────────┐
│       ElevatorController    │
├─────────────────────────────┤
│ - elevators_: vector<Car>   │
├─────────────────────────────┤
│ + requestElevator(flr, dir) │
│ + stepSimulation()          │
└──────────────┬──────────────┘
               │ 1:N (◆)
               ▼
┌─────────────────────────────┐
│         ElevatorCar         │
├─────────────────────────────┤
│ - currentFloor_: int        │
│ - direction_: Direction     │
│ - upRequests_: set<int>     │
│ - downRequests_: set<int>   │
├─────────────────────────────┤
│ + addDestination(floor)     │
│ + moveNext()                │
└─────────────────────────────┘
```

### 3. Production C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <set>
#include <cmath>

enum class Direction { UP, DOWN, IDLE };

class ElevatorCar {
private:
    int id_;
    int currentFloor_{0};
    Direction direction_{Direction::IDLE};
    std::set<int> upRequests_;   // Floors requested going UP
    std::set<int> downRequests_; // Floors requested going DOWN

public:
    explicit ElevatorCar(int id) : id_(id) {}

    void addRequest(int floor) {
        if (floor > currentFloor_) {
            upRequests_.insert(floor);
            if (direction_ == Direction::IDLE) direction_ = Direction::UP;
        } else if (floor < currentFloor_) {
            downRequests_.insert(floor);
            if (direction_ == Direction::IDLE) direction_ = Direction::DOWN;
        }
    }

    void step() {
        if (direction_ == Direction::UP) {
            if (!upRequests_.empty()) {
                currentFloor_ = *upRequests_.begin();
                upRequests_.erase(upRequests_.begin());
                std::cout << "Elevator " << id_ << " reached Floor " << currentFloor_ << " [UP]\n";
            } else if (!downRequests_.empty()) {
                direction_ = Direction::DOWN;
            } else {
                direction_ = Direction::IDLE;
            }
        } else if (direction_ == Direction::DOWN) {
            if (!downRequests_.empty()) {
                currentFloor_ = *downRequests_.rbegin();
                downRequests_.erase(--downRequests_.end());
                std::cout << "Elevator " << id_ << " reached Floor " << currentFloor_ << " [DOWN]\n";
            } else if (!upRequests_.empty()) {
                direction_ = Direction::UP;
            } else {
                direction_ = Direction::IDLE;
            }
        }
    }

    int getCurrentFloor() const { return currentFloor_; }
    Direction getDirection() const { return direction_; }
    int getId() const { return id_; }
};
```

---

# Case Study 5: Vending Machine System

### 1. Requirements & State Pattern
- States: `IdleState`, `HasCoinState`, `DispensingState`, `SoldOutState`.
- Accepts coins, returns balance, handles out-of-stock items.

### 2. Production C++ Implementation

```cpp
#include <iostream>
#include <unordered_map>
#include <memory>

class VendingMachine;

class IVendingState {
public:
    virtual ~IVendingState() = default;
    virtual void insertCoin(VendingMachine& vm, double amount) = 0;
    virtual void selectProduct(VendingMachine& vm, const std::string& code) = 0;
    virtual void refund(VendingMachine& vm) = 0;
};

class VendingMachine {
private:
    std::unordered_map<std::string, double> prices_;
    std::unordered_map<std::string, int> inventory_;
    double currentBalance_{0.0};
    std::unique_ptr<IVendingState> state_;

public:
    VendingMachine();
    void setState(std::unique_ptr<IVendingState> s) { state_ = std::move(s); }
    void addBalance(double b) { currentBalance_ += b; }
    double getBalance() const { return currentBalance_; }
    void resetBalance() { currentBalance_ = 0.0; }

    bool hasItem(const std::string& code) const {
        auto it = inventory_.find(code);
        return it != inventory_.end() && it->second > 0;
    }

    double getPrice(const std::string& code) const {
        return prices_.at(code);
    }

    void dispense(const std::string& code) {
        inventory_[code]--;
        currentBalance_ -= prices_[code];
        std::cout << "[Dispensed]: Item " << code << "! Change returned: $" << currentBalance_ << "\n";
        resetBalance();
    }

    void insertCoin(double a) { state_->insertCoin(*this, a); }
    void selectProduct(const std::string& c) { state_->selectProduct(*this, c); }
    void refund() { state_->refund(*this); }
};

class VendingIdleState : public IVendingState {
public:
    void insertCoin(VendingMachine& vm, double amount) override;
    void selectProduct(VendingMachine&, const std::string&) override {
        std::cout << "Please insert coins first!\n";
    }
    void refund(VendingMachine&) override {
        std::cout << "No coins to refund!\n";
    }
};

class VendingHasCoinState : public IVendingState {
public:
    void insertCoin(VendingMachine& vm, double amount) override {
        vm.addBalance(amount);
        std::cout << "Balance updated: $" << vm.getBalance() << "\n";
    }

    void selectProduct(VendingMachine& vm, const std::string& code) override {
        if (!vm.hasItem(code)) {
            std::cout << "Item " << code << " is OUT OF STOCK!\n";
            return;
        }
        if (vm.getBalance() < vm.getPrice(code)) {
            std::cout << "Insufficient funds! Price is $" << vm.getPrice(code) << "\n";
            return;
        }
        vm.dispense(code);
        vm.setState(std::make_unique<VendingIdleState>());
    }

    void refund(VendingMachine& vm) override {
        std::cout << "Refunded: $" << vm.getBalance() << "\n";
        vm.resetBalance();
        vm.setState(std::make_unique<VendingIdleState>());
    }
};

void VendingIdleState::insertCoin(VendingMachine& vm, double amount) {
    vm.addBalance(amount);
    std::cout << "Coin accepted. Balance: $" << vm.getBalance() << "\n";
    vm.setState(std::make_unique<VendingHasCoinState>());
}

VendingMachine::VendingMachine() : state_(std::make_unique<VendingIdleState>()) {
    prices_["A1"] = 1.50;
    inventory_["A1"] = 5;
}
```

---

# Case Study 6: Ride Sharing Service (Uber / Lyft LLD)

### 1. Requirements & Patterns
- Riders request rides; nearest available Driver is matched and notified.
- **Strategy Pattern**: Driver Matching Strategy (`NearestDriverStrategy`, `HighestRatedStrategy`) & Dynamic Surge Pricing (`SurgePricingStrategy`).
- **State Pattern**: `TripState` (`REQUESTED`, `ACCEPTED`, `IN_PROGRESS`, `COMPLETED`).
- **Thread Safety**: Mutex locks driver to prevent two simultaneous riders from matching the same driver.

### 2. Class Diagram
```
┌──────────────────────────────┐
│         RideService          │
├──────────────────────────────┤
│ - drivers_: vector<Driver>   │
│ - matchingStrategy_: IStrategy
│ - pricingStrategy_: IPricing │
├──────────────────────────────┤
│ + bookRide(rider, src, dst)  │
└──────────────┬───────────────┘
               │ Matches
               ▼
┌──────────────────────────────┐
│            Driver            │
├──────────────────────────────┤
│ - id_: string                │
│ - isAvailable_: bool         │
│ - mtx_: mutex                │
├──────────────────────────────┤
│ + lockAndAccept(): bool      │
│ + release(): void            │
└──────────────────────────────┘
```

### 3. Production C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <cmath>

struct Location {
    double lat{0.0};
    double lon{0.0};

    double distanceTo(const Location& other) const {
        return std::sqrt(std::pow(lat - other.lat, 2) + std::pow(lon - other.lon, 2));
    }
};

class Driver {
private:
    std::string id_;
    Location location_;
    bool isAvailable_{true};
    std::mutex mtx_; // Mutex protects driver availability from race conditions

public:
    Driver(std::string id, Location loc) : id_(std::move(id)), location_(loc) {}

    bool tryLockAndAssign() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!isAvailable_) return false;
        isAvailable_ = false;
        return true;
    }

    void completeTrip(Location newLoc) {
        std::lock_guard<std::mutex> lock(mtx_);
        location_ = newLoc;
        isAvailable_ = true;
    }

    bool isAvailable() const { return isAvailable_; }
    const Location& getLocation() const { return location_; }
    const std::string& getId() const { return id_; }
};

// Strategy for Driver Matching
class IDriverMatchingStrategy {
public:
    virtual ~IDriverMatchingStrategy() = default;
    virtual Driver* matchDriver(const Location& pickup, std::vector<std::shared_ptr<Driver>>& drivers) = 0;
};

class NearestDriverStrategy final : public IDriverMatchingStrategy {
public:
    Driver* matchDriver(const Location& pickup, std::vector<std::shared_ptr<Driver>>& drivers) override {
        Driver* bestDriver = nullptr;
        double minDistance = 1e9;

        for (auto& driver : drivers) {
            if (driver->isAvailable()) {
                double d = pickup.distanceTo(driver->getLocation());
                if (d < minDistance) {
                    minDistance = d;
                    bestDriver = driver.get();
                }
            }
        }
        return bestDriver;
    }
};

// Ride Dispatcher Service
class RideService {
private:
    std::vector<std::shared_ptr<Driver>> drivers_;
    std::unique_ptr<IDriverMatchingStrategy> matcher_;

public:
    explicit RideService(std::unique_ptr<IDriverMatchingStrategy> matcher)
        : matcher_(std::move(matcher)) {}

    void registerDriver(std::shared_ptr<Driver> driver) {
        drivers_.push_back(std::move(driver));
    }

    bool requestRide(const std::string& riderId, Location src, Location dst) {
        std::cout << "[RideService]: Rider " << riderId << " requested ride...\n";
        
        Driver* matched = matcher_->matchDriver(src, drivers_);
        if (!matched || !matched->tryLockAndAssign()) {
            std::cout << "[RideService]: No available drivers nearby. Try again!\n";
            return false;
        }

        std::cout << "[RideService]: Driver " << matched->getId() 
                  << " matched and locked for Rider " << riderId << "!\n";
        return true;
    }
};
```

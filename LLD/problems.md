# Classic LLD Interview Problems 📝

Here is a list of 10 classic Low-Level Design (LLD) / Object-Oriented Design (OOD) interview problems, along with the core entities to define and the typical design patterns applied to solve them.

---

### 1. Design a Parking Lot
* **Core Entities:** `ParkingLot`, `Level`, `ParkingSpot`, `Vehicle` (Car, Truck, Motorcycle), `Ticket`, `PaymentGateway`.
* **Key Patterns:**
  * **Factory Method:** To instantiate different types of `Vehicle` or `ParkingSpot`.
  * **Strategy:** To calculate parking fees based on different pricing strategies (e.g., hourly, flat rate, weekend surge).
  * **Singleton:** Ensuring only one instance of the `ParkingLot` system manages the overarching state.

### 2. Design a Library Management System
* **Core Entities:** `Library`, `Book`, `BookItem` (physical copy), `User` (Member, Librarian), `LendingRecord`, `FineCalculator`.
* **Key Patterns:**
  * **State:** To track the state of a `BookItem` (Available, Reserved, Loaned, Lost).
  * **Observer:** To notify a user when a reserved book becomes available.

### 3. Design an E-Commerce Platform (Amazon)
* **Core Entities:** `Customer`, `Product`, `ShoppingCart`, `Order`, `Payment`, `Shipment`.
* **Key Patterns:**
  * **Decorator:** To apply varying layers of discounts or taxes to an `Order`.
  * **Command:** To implement the "Add to Cart" or "Checkout" actions, enabling easy undo/redo (e.g., "Remove from Cart").
  * **Strategy:** For handling multiple `Payment` methods (Credit Card, Net Banking, UPI).

### 4. Design a Vending Machine
* **Core Entities:** `VendingMachine`, `State` (Idle, HasMoney, Dispensing), `Product`, `Coin/Note`, `Inventory`.
* **Key Patterns:**
  * **State:** (Crucial here) The machine behaves differently based on its state. If it's in the `Idle` state, pressing a product button prompts for money. If it's in the `HasMoney` state, it dispenses.

### 5. Design Tic-Tac-Toe
* **Core Entities:** `Board`, `Cell`, `Player`, `Game`, `Piece` (X, O).
* **Key Patterns:**
  * **Factory:** To create player instances (Human vs. AI).
  * **Strategy:** If implementing an AI player, you can plug in different algorithms (Random move, Minimax strategy).
  * **Observer:** To update UI clients whenever the `Board` state changes.

### 6. Design a Movie Ticket Booking System (BookMyShow)
* **Core Entities:** `Cinema`, `Screen`, `Movie`, `Show`, `Seat`, `Booking`, `Payment`.
* **Key Patterns:**
  * **Concurrency/Locking:** Not strictly a GoF pattern, but handling pessimistic or optimistic locking on a `Seat` is the core challenge.
  * **State:** Tracking seat status (Available, Locked, Booked).

### 7. Design an ATM System
* **Core Entities:** `ATM`, `CardReader`, `CashDispenser`, `Screen`, `BankService`, `Transaction`.
* **Key Patterns:**
  * **State:** The ATM cycles through states (Ready, CardInserted, Authenticated, TransactionPending).
  * **Chain of Responsibility:** For the `CashDispenser`. If the user requests $130, the $100 dispenser handles what it can, then passes the remaining $30 request to the $50 dispenser (skips), then to the $20 dispenser, etc.

### 8. Design Stack Overflow
* **Core Entities:** `User`, `Question`, `Answer`, `Comment`, `Tag`, `Vote`.
* **Key Patterns:**
  * **Observer:** To send notifications when a user's question receives an answer or upvote.
  * **Strategy:** For different badge earning or reputation calculation algorithms.

### 9. Design a Snake and Ladder Game
* **Core Entities:** `Game`, `Board`, `Player`, `Dice`, `Jumper` (Snake or Ladder).
* **Key Patterns:**
  * **Factory:** For generating dice with different numbers of sides if extending the game.
  * **Singleton:** To manage the central `Game` state and turn coordination.

### 10. Design an Elevator System
* **Core Entities:** `ElevatorSystem`, `ElevatorCar`, `Door`, `FloorButton`, `ElevatorButton`, `Dispatcher`.
* **Key Patterns:**
  * **Strategy:** To define the dispatching algorithm (e.g., Shortest Seek Time First (SSTF), SCAN algorithm, LOOK algorithm).
  * **State:** To track the `ElevatorCar` state (MovingUp, MovingDown, Idle, Maintenance).

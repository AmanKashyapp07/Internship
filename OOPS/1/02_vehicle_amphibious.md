# LLD: Vehicle / Amphibious Car / Flying Car (Hybrid Vehicles)

> **Core Category:** Role & Behavior Combinations / Multiple Inheritance Traps  
> **Key Design Pattern:** Bridge Pattern / Capability-Based Composition (Strategy Pattern)  
> **Target Concepts:** Diamond Inheritance Prevention, Interface Segregation Principle (ISP), Liskov Substitution Principle (LSP), Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"You need to model land vehicles (`Car`), watercraft (`Boat`), and aircraft (`Plane`). An `AmphibiousCar` drives on roads and floats on water; a `FlyingCar` drives and flies; a `Triphibian` can drive, sail, and fly. How do you design this hierarchy without falling into diamond inheritance?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Diamond of Death (Multiple Concrete Inheritance)

```
                     +-----------------------+
                     |        Vehicle        |  (VIN, speed, fuelTank, odometer)
                     +-----------------------+
                           /           \
                          /             \
             +---------------+       +---------------+
             |   LandCar     |       |   WaterBoat   |
             +---------------+       +---------------+
             | + drive()     |       | + sail()      |
             +---------------+       +---------------+
                          \             /
                           \           /
                     +-----------------------+
                     |     AmphibiousCar     |  <-- DEADLY DIAMOND!
                     +-----------------------+
```

#### Why the interviewer rejects this:
1. **State Duplication & Slicing:** `AmphibiousCar` inherits **two separate copies** of `fuelTank`, `odometer`, and `VIN` from `LandCar` and `WaterBoat`. Refueling `LandCar::fuelTank` leaves `WaterBoat::fuelTank` empty!
2. **Method Ambiguity & Collision:** Both `LandCar` and `WaterBoat` define `startEngine()`, `steer()`, and `brake()`. Calling `amphibiousCar.steer()` produces a compiler ambiguity error (`error: request for member 'steer' is ambiguous`).
3. **Combinatorial Explosion:** 
   - Land + Water = `AmphibiousCar`
   - Land + Air = `FlyingCar`
   - Water + Air = `Seaplane`
   - Land + Water + Air = `TriphibianVehicle` ($2^N - 1$ distinct subclasses for $N=3$ mediums).

---

### ❌ Anti-Pattern 2: The Fat Base Class (LSP & ISP Violation)

```
+-------------------------------------------------------------+
|                           Vehicle                           |
| + drive(): void  -> Car implements, Boat throws Exception   |
| + sail(): void   -> Boat implements, Car throws Exception   |
| + fly(): void    -> Plane implements, Car throws Exception  |
+-------------------------------------------------------------+
```

#### Why the interviewer rejects this:
- Violates **Liskov Substitution Principle (LSP)**: Calling `vehicle->fly()` on a `Car` crashes at runtime.
- Violates **Interface Segregation Principle (ISP)**: Classes are forced to depend on methods they don't support.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A Vehicle IS a single physical entity with an engine and chassis, which HAS one or more Medium Propulsion Capabilities."*

```
+-----------------------------------------------------------------------------------+
|                                      Vehicle                                      |
+-----------------------------------------------------------------------------------+
| - vin: string                                                                     |
| - currentSpeed: double                                                            |
| - landCapability: unique_ptr<ILandPropulsion>                                     |
| - waterCapability: unique_ptr<IWaterPropulsion>                                   |
| - airCapability: unique_ptr<IAirPropulsion>                                       |
+-----------------------------------------------------------------------------------+
| + driveOnRoad(speed: double): void                                                |
| + navigateWater(speed: double): void                                              |
| + flyInAir(speed: double, altitude: double): void                                 |
| + isAmphibious(): bool                                                            |
| + isFlyingCar(): bool                                                             |
| + setLandCapability(cap: unique_ptr<ILandPropulsion>): void                       |
| + setWaterCapability(cap: unique_ptr<IWaterPropulsion>): void                     |
| + setAirCapability(cap: unique_ptr<IAirPropulsion>): void                         |
+-----------------------------------------------------------------------------------+
       |                                |                               |
       | HAS-A                          | HAS-A                         | HAS-A
       v                                v                               v
+-----------------------+    +-----------------------+    +-----------------------+
|     <<interface>>     |    |     <<interface>>     |    |     <<interface>>     |
|    ILandPropulsion    |    |   IWaterPropulsion    |    |    IAirPropulsion     |
+-----------------------+    +-----------------------+    +-----------------------+
| + drive(speed)*: void |    | + sail(speed)*: void  |    | + fly(speed, alt)*    |
+-----------------------+    +-----------------------+    +-----------------------+
            ^                            ^                            ^
            |                            |                            |
      +-----+-----+                +-----+-----+                +-----+-----+
      |           |                |           |                |           |
+----------+ +----------+    +----------+ +----------+    +----------+ +----------+
|WheelDrive| |TrackDrive|    |Propeller | | Hydrojet |    |JetEngine | |RotorBlade|
|  (Car)   | |  (Tank)  |    |  (Boat)  | | (JetSki) |    | (Plane)  | |(Helicop) |
+----------+ +----------+    +----------+ +----------+    +----------+ +----------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Segregated Propulsion Capability Interfaces (ISP)
class ILandPropulsion {
public:
    virtual ~ILandPropulsion() = default;
    virtual void drive(double speed) = 0;
};

class IWaterPropulsion {
public:
    virtual ~IWaterPropulsion() = default;
    virtual void sail(double speed) = 0;
};

class IAirPropulsion {
public:
    virtual ~IAirPropulsion() = default;
    virtual void fly(double speed, double altitude) = 0;
};

// 2. Concrete Strategy Implementations
class WheelDrive : public ILandPropulsion {
public:
    void drive(double speed) override { /* Engage 4-wheel drive on asphalt */ }
};

class PropellerDrive : public IWaterPropulsion {
public:
    void sail(double speed) override { /* Spin twin water propellers */ }
};

class JetEngineDrive : public IAirPropulsion {
public:
    void fly(double speed, double alt) override { /* Engage turbofan at altitude */ }
};

// 3. The Core Vehicle Entity (Composition Root)
class Vehicle {
private:
    std::string vin;
    double currentSpeed = 0.0;

    // Optional modular capabilities
    std::unique_ptr<ILandPropulsion> landMover;
    std::unique_ptr<IWaterPropulsion> waterMover;
    std::unique_ptr<IAirPropulsion> airMover;

public:
    Vehicle(std::string vin) : vin(std::move(vin)) {}

    // Capability Attachment
    void setLandCapability(std::unique_ptr<ILandPropulsion> cap) { landMover = std::move(cap); }
    void setWaterCapability(std::unique_ptr<IWaterPropulsion> cap) { waterMover = std::move(cap); }
    void setAirCapability(std::unique_ptr<IAirPropulsion> cap) { airMover = std::move(cap); }

    // Type Querying without dynamic_cast
    bool isAmphibious() const { return landMover && waterMover; }
    bool isFlyingCar() const { return landMover && airMover; }

    // Safe Delegated Operations
    void driveOnRoad(double speed) {
        if (!landMover) throw std::logic_error("Vehicle cannot drive on land!");
        currentSpeed = speed;
        landMover->drive(speed);
    }

    void navigateWater(double speed) {
        if (!waterMover) throw std::logic_error("Vehicle cannot navigate water!");
        currentSpeed = speed;
        waterMover->sail(speed);
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model an `AmphibiousCar` by inheriting from both `Car` and `Boat`, we trigger the **Diamond Problem**. The vehicle inherits duplicate state (two odometers, two fuel tanks) and method name collisions on common actions like `steer()` or `startEngine()`."*
> 2. *"If we instead put all methods (`drive()`, `sail()`, `fly()`) in a fat `Vehicle` base class, we violate the **Liskov Substitution Principle (LSP)** and **Interface Segregation Principle (ISP)** because calling `fly()` on a standard `Car` throws a runtime exception."*
> 3. *"The robust architectural solution is the **Bridge Pattern / Capability Composition**. A `Vehicle` is a single physical entity holding common state (`vin`, `fuelLevel`, `speed`), while movement across land, water, and air are decoupled into pluggable capability interfaces (`ILandPropulsion`, `IWaterPropulsion`, `IAirPropulsion`)."*
> 4. *"An `AmphibiousCar` is simply a `Vehicle` initialized with both `WheelDrive` and `PropellerDrive`. If we later build a `Triphibian` or change a propulsion engine from wheels to caterpillar tracks, we simply plug in the new strategy with **zero inheritance changes and zero diamond ambiguity**."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle steering when transitioning from road to water?"** | Inject a `SteeringController` strategy that routes steering inputs to the rudder when in water mode and to the front axle when in road mode (State/Strategy pattern). |
| **"What if a vehicle has dynamic deployable wings (transforms mid-trip)?"** | Add/enable the `IAirPropulsion` component at runtime via `vehicle->setAirCapability(deployWings())`. |
| **"How do you prevent code duplication across different cars (e.g. Electric vs Petrol engine)?"** | Further decompose `Engine` as an `IEngine` strategy (`ElectricEngine`, `CombustionEngine`) injected into the `Vehicle`. |

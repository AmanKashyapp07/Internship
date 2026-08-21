# LLD: Cricket Player Roles (Batsman, Bowler, All-Rounder, Wicket-Keeper)

> **Core Category:** Role & Behavior Combinations / Dynamic Entity Evolution  
> **Key Design Pattern:** Role Object Pattern / Capability-Based Composition (Strategy Pattern)  
> **Target Concepts:** Avoiding Diamond Inheritance, Composition over Inheritance, Single Responsibility Principle (SRP), Interface Segregation Principle (ISP)

---

## 1. The Interview Question

> *"Design a player model for a sports simulation/cricket game supporting Batsmen, Bowlers, Wicket-Keepers, and All-Rounders. How would you structure these classes?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Diamond of Death (Multiple Inheritance)

```
                 +-------------------+
                 |      Player       |  (id, name, age, stats)
                 +-------------------+
                       /       \
                      /         \
         +---------------+   +---------------+
         |    Batsman    |   |    Bowler     |
         +---------------+   +---------------+
                      \         /
                       \       /
                 +-------------------+
                 |    AllRounder     |  <-- DEADLY DIAMOND!
                 +-------------------+
```

#### Why the interviewer rejects this:
1. **Ambiguity & State Duplication:** `AllRounder` inherits duplicate copies of `Player` fields (`name`, `age`, `stats`) unless virtual inheritance is used.
2. **Combinatorial Explosion ($2^N$ Subclasses):**
   - What about `WicketKeeperBatsman`? `CaptainBatsman`? `CaptainAllRounder`?
   - For $N=4$ roles (Bat, Bowl, Keep, Captain), you would need $2^4 = 16$ distinct classes!
3. **The "Steve Smith" Trap (No Runtime Role Mutation):**
   - A player who starts as a specialist Bowler and later develops world-class batting cannot change their C++ concrete class type at runtime.
   - You would have to **delete the Bowler object and re-instantiate an AllRounder**, which breaks every pointer/ID held by `Team`, `Match`, and `Tournament` tables.

---

### ❌ Anti-Pattern 2: The Monolithic God Object

```
+-----------------------------------------------------+
|                       Player                        |
| - isBatsman: bool                                   |
| - isBowler: bool                                    |
| - isKeeper: bool                                    |
| + bat(): void   -> if (!isBatsman) throw error      |
| + bowl(): void  -> if (!isBowler) throw error       |
+-----------------------------------------------------+
```

#### Why the interviewer rejects this:
- Violates **Single Responsibility (SRP)** and **Interface Segregation (ISP)**.
- Full of brittle `if/else` checks for every action.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A Player HAS Batting/Bowling Capabilities, they ARE NOT hardcoded subclasses"*

```
+-----------------------------------------------------------------------------------+
|                                      Player                                       |
+-----------------------------------------------------------------------------------+
| - id: int                                                                         |
| - name: string                                                                    |
| - battingSkill: unique_ptr<IBattingSkill>                                         |
| - bowlingSkill: unique_ptr<IBowlingSkill>                                         |
| - keeperSkill: unique_ptr<IWicketKeepingSkill>                                    |
| - captaincyRole: unique_ptr<ICaptaincyRole>                                       |
+-----------------------------------------------------------------------------------+
| + isBatsman(): bool                                                               |
| + isBowler(): bool                                                                |
| + isAllRounder(): bool                                                            |
| + performBatting(): void                                                          |
| + performBowling(): void                                                          |
| + setBattingSkill(skill: unique_ptr<IBattingSkill>): void                         |
| + setBowlingSkill(skill: unique_ptr<IBowlingSkill>): void                         |
+-----------------------------------------------------------------------------------+
       |                        |                       |                    |
       | HAS-A                  | HAS-A                 | HAS-A              | HAS-A
       v                        v                       v                    v
+------------------+   +------------------+   +-------------------+  +------------------+
|  <<interface>>   |   |  <<interface>>   |   |   <<interface>>   |  |  <<interface>>   |
|  IBattingSkill   |   |  IBowlingSkill   |   |IWicketKeepingSkill|  |  ICaptaincyRole  |
+------------------+   +------------------+   +-------------------+  +------------------+
| + playShot()     |   | + deliverBall()  |   | + takeCatch()     |  | + setField()     |
| + getStyle()     |   | + getStyle()     |   | + doStumping()    |  | + rotateBowlers()|
+------------------+   +------------------+   +-------------------+  +------------------+
         ^                      ^                       ^                    ^
         |                      |                       |                    |
   +-----+-----+          +-----+-----+                 |                    |
   |           |          |           |                 |                    |
+----------+ +--------+ +----------+ +--------+   +---------------+  +-----------------+
|Aggressive| | Anchor | | FastPace | |SpinBow |   |StandardKeeper |  |TacticalCaptain  |
| Batting  | |Batting | | Bowling  | |  ling  |   +---------------+  +-----------------+
+----------+ +--------+ +----------+ +--------+
```

---

## 4. Concise Paper / Whiteboard Code (What to Write)

```cpp
// 1. Granular Capability Interfaces (ISP Compliant)
class IBattingSkill {
public:
    virtual ~IBattingSkill() = default;
    virtual void playShot() = 0;
};

class IBowlingSkill {
public:
    virtual ~IBowlingSkill() = default;
    virtual void deliverBall() = 0;
};

class IWicketKeepingSkill {
public:
    virtual ~IWicketKeepingSkill() = default;
    virtual void doStumping() = 0;
};

// 2. Concrete Strategy Implementations
class AggressiveBatting : public IBattingSkill {
public:
    void playShot() override { /* Hit boundary */ }
};

class FastBowling : public IBowlingSkill {
public:
    void deliverBall() override { /* 145 km/h Yorker */ }
};

// 3. The Stable Core Entity (Composition Root)
class Player {
private:
    int id;
    std::string name;
    
    // Composable capabilities (nullptr if player lacks skill)
    std::unique_ptr<IBattingSkill> battingSkill;
    std::unique_ptr<IBowlingSkill> bowlingSkill;
    std::unique_ptr<IWicketKeepingSkill> keeperSkill;

public:
    Player(int id, std::string name) : id(id), name(std::move(name)) {}

    // Runtime Role / Capability Checks
    bool isBatsman() const { return battingSkill != nullptr; }
    bool isBowler() const { return bowlingSkill != nullptr; }
    bool isAllRounder() const { return isBatsman() && isBowler(); }

    // Dynamic Skill Attachment (Zero Object Reallocation!)
    void setBattingSkill(std::unique_ptr<IBattingSkill> skill) {
        battingSkill = std::move(skill);
    }
    void setBowlingSkill(std::unique_ptr<IBowlingSkill> skill) {
        bowlingSkill = std::move(skill);
    }

    // Behavioral Delegation
    void bat() {
        if (battingSkill) battingSkill->playShot();
    }
    void bowl() {
        if (bowlingSkill) bowlingSkill->deliverBall();
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model `AllRounder` via multiple inheritance inheriting from `Batsman` and `Bowler`, we run straight into the **Diamond Problem** and suffer a **$2^N$ class explosion** for players who also keep wickets or captain."*
> 2. *"More critically, inheritance is **static at compile-time**. If a bowler evolves into a batsman later in their career, static inheritance forces us to delete and re-instantiate the player, breaking external object references."*
> 3. *"Instead, I use the **Role Object / Capability-Based Composition Pattern**. `Player` is an entity that HAS a batting skill, bowling skill, or keeping skill. An All-Rounder is simply a `Player` with both batting and bowling skills populated."*
> 4. *"This satisfies the **Open-Closed Principle (OCP)** because new batting techniques or player roles (e.g. Captaincy, Impact Player) can be added without modifying the core `Player` class."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"What if match rules introduce a temporary 'Impact Player' role?"** | Simply create an `IImpactPlayerRole` and attach it to the `Player` at runtime via `player->setImpactRole(...)`. |
| **"How do you query all batsmen in a Team?"** | Filter `team.getPlayers()` using the predicate `player->isBatsman()`. |
| **"What if batting tactics change mid-match (Defense $\to$ Aggression)?"** | Swap the `battingSkill` pointer from `AnchorBatting` to `AggressiveBatting` (Strategy Pattern). |

# LLD: Game Character Class System (Warrior, Mage, Paladin)

> **Core Category:** Dynamic State & Entity Evolution / Component-Based Modeling  
> **Key Design Pattern:** Component Pattern (Lightweight ECS) + Skill Tree / Strategy Pattern  
> **Target Concepts:** Respecialization (Respec) Without Re-Instantiating Entities, Hybrid Classes, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"In an RPG game, players choose from archetypes like `Warrior` (melee combat, rage resource), `Mage` (spellcasting, mana resource), and hybrid classes like `Paladin` (both sword combat and holy healing spells). During gameplay, characters level up, unlock cross-class talent trees, and can visit a trainer to reset their skills (respecialize). How do you design the character class system using composition rather than a rigid inheritance hierarchy?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Rigid Subclassing Hierarchy

```
                             +--------------------+
                             |     Character      |  (name, level, health, inventory)
                             +--------------------+
                                /              \
                               /                \
                    +----------------+    +----------------+
                    |    Warrior     |    |      Mage      |
                    +----------------+    +----------------+
                    | + slashSword() |    | + castFireball()|
                    +----------------+    +----------------+
                               \                /
                                \              /
                             +--------------------+
                             |      Paladin       |  <-- DEADLY DIAMOND!
                             +--------------------+
```

#### Why the interviewer rejects this:
1. **The Respecialization (Respec) Failure:** If a `Warrior` wants to respec into a `Mage` at level 50, static inheritance requires deleting the character object and creating a new `Mage`, losing character metadata, quest state, and inventory bindings.
2. **Hybrid Class Explosion:** Paladins, Battlemages, Spellblades, and Necro-knights would all require distinct multiple-inheritance classes.
3. **Talent Tree Rigidity:** You cannot dynamically equip or unlock individual abilities (e.g., a Warrior learning a minor `Heal` spell from a quest scroll).

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A Character is an Entity (holds identity, HP, stats) containing pluggable Components (`CombatComponent`, `MagicComponent`) and a dynamic collection of `Skill` objects (Strategy / Command Pattern)."*

```
+-----------------------------------------------------------------------------------+
|                                     Character                                     |
+-----------------------------------------------------------------------------------+
| - name: string                                                                    |
| - level: int                                                                      |
| - health: int                                                                     |
| - components: unordered_map<ComponentType, unique_ptr<ICharacterComponent>>      |
| - activeSkills: vector<shared_ptr<ISkill>>                                        |
+-----------------------------------------------------------------------------------+
| + addComponent(comp: unique_ptr<ICharacterComponent>): void                       |
| + removeComponent(type: ComponentType): void                                      |
| + learnSkill(skill: shared_ptr<ISkill>): void                                     |
| + resetSkills(): void (Respec!)                                                   |
| + useSkill(skillName: string, target: Character): void                            |
+-----------------------------------------------------------------------------------+
           |                                                      |
           | 1 HAS MANY                                           | 1 HAS MANY
           v                                                      v
+------------------------------------+         +------------------------------------+
|           <<interface>>            |         |           <<interface>>            |
|        ICharacterComponent         |         |               ISkill               |
+------------------------------------+         +------------------------------------+
| + getComponentType()*: CompType    |         | + getName()*: string               |
| + update(deltaTime)*: void         |         | + getManaCost()*: int              |
+------------------------------------+         | + execute(caster, target)*: void   |
           ^                                   +------------------------------------+
           | implements                                           ^
     +-----+-----+                                                | implements
     |           |                                       +--------+--------+
+----------+ +----------+                                |                 |
|  Combat  | |  Magic   |                        +---------------+ +---------------+
|Component | |Component |                        |  MeleeSlash   | | FireballSpell |
+----------+ +----------+                        +---------------+ +---------------+
|- rage:int| |- mana:int|                        | + execute()   | | + execute()   |
+----------+ +----------+                        +---------------+ +---------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Decoupled Skill Strategy / Command
class Character; // Forward declaration

class ISkill {
public:
    virtual ~ISkill() = default;
    virtual std::string getName() const = 0;
    virtual int getResourceCost() const = 0;
    virtual void cast(Character& caster, Character& target) = 0;
};

class SwordSlashSkill : public ISkill {
public:
    std::string getName() const override { return "Whirlwind Slash"; }
    int getResourceCost() const override { return 20; } // Rage cost
    void cast(Character& caster, Character& target) override {
        // Deal physical weapon damage to target
    }
};

class FireballSkill : public ISkill {
public:
    std::string getName() const override { return "Fireball"; }
    int getResourceCost() const override { return 50; } // Mana cost
    void cast(Character& caster, Character& target) override {
        // Deal magical fire damage over time
    }
};

// 2. Character Entity (Composite / Component Host)
class Character {
private:
    std::string name;
    int level = 1;
    int health = 100;
    int mana = 100;
    int rage = 0;
    std::vector<std::shared_ptr<ISkill>> skillBook;

public:
    Character(std::string name) : name(std::move(name)) {}

    // Dynamic Skill Tree Progression
    void learnSkill(std::shared_ptr<ISkill> skill) {
        skillBook.push_back(std::move(skill));
    }

    // Zero-Cost Full Respecialization (Respec)
    void respecialize() {
        skillBook.clear(); // Safely reset skills without modifying character identity or gear!
    }

    // Dynamic Archetype Querying
    bool isPaladin() const {
        bool hasMelee = false, hasMagic = false;
        for (const auto& s : skillBook) {
            if (s->getName() == "Whirlwind Slash") hasMelee = true;
            if (s->getName() == "Fireball" || s->getName() == "HolyHeal") hasMagic = true;
        }
        return hasMelee && hasMagic;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"Using inheritance for game classes like `class Paladin : public Warrior, public Mage` causes the classic **Diamond Problem** and fails completely when a player wants to **respecialize (respec)** from Warrior to Mage."*
> 2. *"In modern game architecture, entities are decoupled from behaviors using the **Component Pattern (Lightweight ECS) and Strategy/Command Pattern**."*
> 3. *"A `Character` is a stable entity holding base stats and inventory. Skills (`SwordSlash`, `Fireball`, `HolyHeal`) are modeled as `ISkill` command objects inside the player's `SkillBook`."*
> 4. *"A `Warrior` is simply a character with physical combat skills; a `Mage` has spell skills; a `Paladin` is a hybrid holding both. When the player visits a trainer to respec, we simply clear the `SkillBook` and repopulate it with **zero object reallocation**."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you implement cooldowns and global cooldowns (GCD)?"** | Add a `CooldownTracker` component on `Character` that records `lastCastTimestamp[skillId]` and validates before `cast()`. |
| **"How do you handle status effects like Stun or Poison (Buffs/Debuffs)?"** | Use the **Decorator Pattern or Observer Pattern**: A `StatusEffectComponent` hooks into damage calculation pipelines (`onTakeDamage`, `onTick`). |
| **"What if equipment (e.g. Magic Staff) unlocks exclusive skills?"** | Equipment items implement a `grantSkills(Character&)` method that dynamically attaches skills when equipped and detaches them when unequipped. |

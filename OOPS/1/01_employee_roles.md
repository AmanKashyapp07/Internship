# LLD: Employee, Manager, Director & Tech Lead (Role Object Pattern)

> **Core Category:** Role & Behavior Combinations / Multiple Inheritance Traps  
> **Key Design Pattern:** Role Object Pattern / Dynamic Role Composition  
> **Target Concepts:** Avoiding $2^N$ Subclass Explosion, Composition over Inheritance, Open-Closed Principle (OCP), Single Responsibility Principle (SRP)

---

## 1. The Interview Question

> *"In an organization, an employee can be an Individual Contributor (`IC`), a `Manager` (manages reports and conducts 1-on-1s), a `Director` (manages departments and budgets), or a `TechLead` (both writes code and performs management/code reviews). How do you model this without creating exponential subclass combinations or breaking object identity during promotions?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Combinatorial Subclassing & Diamond Traps

```
                              +--------------------+
                              |      Employee      |  (id, name, baseSalary)
                              +--------------------+
                                 /              \
                                /                \
                     +----------------+    +----------------+
                     |  DeveloperIC   |    |    Manager     |
                     +----------------+    +----------------+
                     | + writeCode()  |    | + conduct1on1()|
                     +----------------+    +----------------+
                                \                /
                                 \              /
                              +--------------------+
                              |      TechLead      |  <-- DEADLY DIAMOND!
                              +--------------------+
                                         |
                              +--------------------+
                              |  DirectorTechLead  |  <-- Combinatorial Class Explosion ($2^N$)
                              +--------------------+
```

#### Why the interviewer rejects this:
1. **Diamond of Death:** `TechLead` inherits duplicate copies of `Employee` identity (`id`, `name`, `baseSalary`) from both `DeveloperIC` and `Manager`.
2. **Exponential Subclasses ($2^N$):** With roles like `IC`, `Manager`, `Director`, `Architect`, and `ScrumMaster`, you would need dozens of hybrid classes (`ManagerArchitect`, `DirectorTechLead`, etc.).
3. **The Promotion / Demotion Trap (Object Mutation):** When a Senior Developer is promoted to Tech Lead or Manager, you cannot change their concrete C++ type at runtime. You would have to delete the `DeveloperIC` object and instantiate a `Manager`, destroying all historical audit logs, references in Jira, Slack, and HR systems!

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"An Employee is a permanent physical person (identity & payroll ledger) who dynamically HOLDS one or more active Roles (IC, Manager, Director, TechLead)."*

```
+-----------------------------------------------------------------------------------+
|                                     Employee                                      |
+-----------------------------------------------------------------------------------+
| - id: int                                                                         |
| - name: string                                                                    |
| - baseSalary: double                                                              |
| - activeRoles: unordered_map<RoleType, unique_ptr<IRole>>                         |
+-----------------------------------------------------------------------------------+
| + addRole(role: unique_ptr<IRole>): void                                          |
| + removeRole(type: RoleType): void                                                |
| + hasRole(type: RoleType): bool                                                   |
| + getRole<T>(type: RoleType): T*                                                  |
| + calculateTotalCompensation(): double                                            |
| + performWork(): void                                                             |
+-----------------------------------------------------------------------------------+
                                         |
                                         | 1 HAS MANY (0..*)
                                         v
                      +--------------------------------------+
                      |            <<interface>>             |
                      |                IRole                 |
                      +--------------------------------------+
                      | + getRoleType()*: RoleType           |
                      | + getRoleBonus()*: double            |
                      | + executeDuties()*: void             |
                      +--------------------------------------+
                                         ^
                                         | implements
         +-------------------------------+-------------------------------+
         |                               |                               |
+-----------------+             +-----------------+             +-----------------+
|     ICRole      |             |   ManagerRole   |             |  DirectorRole   |
+-----------------+             +-----------------+             +-----------------+
| - techStack     |             | - directReports |             | - departments   |
+-----------------+             +-----------------+             +-----------------+
| + writeCode()   |             | + conduct1on1() |             | + allocateBudget|
| + reviewPR()    |             | + approveLeave()|             | + approveHiring |
+-----------------+             +-----------------+             +-----------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Role Type Identifiers
enum class RoleType { IC, MANAGER, DIRECTOR, ARCHITECT };

// 2. Pure Role Interface
class IRole {
public:
    virtual ~IRole() = default;
    virtual RoleType getType() const = 0;
    virtual double getBonus() const = 0;
    virtual void executeDuties() = 0;
};

// 3. Concrete Role Implementations
class ICRole : public IRole {
public:
    RoleType getType() const override { return RoleType::IC; }
    double getBonus() const override { return 10000.0; }
    void executeDuties() override { /* Push code commits & review PRs */ }
};

class ManagerRole : public IRole {
    std::vector<int> directReportIds;
public:
    RoleType getType() const override { return RoleType::MANAGER; }
    double getBonus() const override { return 25000.0; }
    void executeDuties() override { /* Conduct 1-on-1s & performance reviews */ }
    void addReport(int empId) { directReportIds.push_back(empId); }
};

// 4. Stable Employee Entity (Holds Roles Dynamically)
class Employee {
private:
    int id;
    std::string name;
    double baseSalary;
    std::unordered_map<RoleType, std::unique_ptr<IRole>> roles;

public:
    Employee(int id, std::string name, double salary)
        : id(id), name(std::move(name)), baseSalary(salary) {}

    // Dynamic Role Management
    void attachRole(std::unique_ptr<IRole> role) {
        roles[role->getType()] = std::move(role);
    }
    void detachRole(RoleType type) { roles.erase(type); }
    bool hasRole(RoleType type) const { return roles.count(type) > 0; }

    // Dynamic Capabilities (Tech Lead = hasRole(IC) && hasRole(MANAGER))
    bool isTechLead() const {
        return hasRole(RoleType::IC) && hasRole(RoleType::MANAGER);
    }

    // Aggregation Across Roles
    double calculateTotalCompensation() const {
        double total = baseSalary;
        for (const auto& [type, role] : roles) total += role->getBonus();
        return total;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model `TechLead` as a class inheriting from `Developer` and `Manager`, we trigger the **Diamond Problem** (duplicate employee IDs and salaries) and create a **$2^N$ combinatorial explosion** as new roles like `Architect` or `Director` are introduced."*
> 2. *"More importantly, an employee's role is **dynamic throughout their career**. If a developer is promoted to Tech Lead or Manager, static inheritance forces us to destroy the old object and create a new one, breaking references in payroll and HR databases."*
> 3. *"Instead, I use the **Role Object Pattern**. An `Employee` has a stable identity (`id`, `name`, `baseSalary`) and maintains a dynamic map of active `IRole` instances (`ICRole`, `ManagerRole`, `DirectorRole`)."*
> 4. *"A `TechLead` is simply an `Employee` who currently holds both an `ICRole` and a `ManagerRole`. Adding, upgrading, or revoking a role is an $O(1)$ runtime operation with **zero object reallocation**."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle matrix management (reporting to multiple managers)?"** | In `ManagerRole`, store `vector<shared_ptr<Employee>> directReports`, and in `Employee` store `vector<int> managerIds` without altering role classes. |
| **"What if only Directors are allowed to approve budgets over $100k?"** | Implement a **Policy/Specification pattern**: Query `employee->getRole<DirectorRole>(RoleType::DIRECTOR)` and invoke `director->approveBudget(amount)`. |
| **"How do you log role change history for auditing?"** | Use the **Observer / Event Sourcing Pattern**: When `attachRole()` or `detachRole()` is called, publish an `EmployeeRoleChangedEvent` to an audit logger. |

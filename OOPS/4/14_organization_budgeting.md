# LLD: Organization Chart & Department Budgeting

> **Core Category:** Hierarchical & Aggregation Modeling  
> **Key Design Pattern:** Composite Pattern (Employee Leaf vs. Department Composite) + Visitor Pattern  
> **Target Concepts:** Recursive Financial Aggregation, Headcount Calculation, Tree Traversal, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"An enterprise organizational hierarchy consists of individual employees (with salaries, titles, and bonuses) and departments (e.g. Engineering, Mobile Team, QA, Sales). A department contains individual employees as well as nested sub-departments (e.g. `Engineering` contains `BackendDept`, `FrontendDept`, and `DevOpsDept`). Corporate leadership needs to query total operational budget, total headcount, and generate reporting directories across any branch or entire company. How do you design this hierarchy uniformly?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Disjoint Data Structures with Manual Recursion

```cpp
// ❌ Anti-Pattern: Separate classes with manual recursive traversal logic
class Employee { double salary; };
class Department {
    vector<Employee> employees;
    vector<Department> subDepartments;
};

// Client code littered with bespoke traversal functions:
double calculateDeptBudget(const Department& dept) {
    double total = 0;
    for (const auto& emp : dept.employees) total += emp.salary;
    for (const auto& sub : dept.subDepartments) total += calculateDeptBudget(sub);
    return total;
}
```

#### Why the interviewer rejects this:
1. **No Uniform Interface:** An `Employee` and a `Department` cannot be treated polymorphically (e.g. in a payroll disbursement queue or org chart visualizer).
2. **Duplication of Reporting Logic:** Every new organizational metric (e.g., diversity headcount, bonus allocation, equipment budget) requires writing another customized recursive loop.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Apply the GoF Composite Pattern. Treat `Employee` (Leaf) and `Department` (Composite) uniformly via `IOrgUnit`. `Department` aggregates budgets and headcounts recursively across its nested units."*

```
+-----------------------------------------------------------------------------------+
|                           <<interface>> IOrgUnit                                  |
+-----------------------------------------------------------------------------------+
| + getName()*: string                                                              |
| + getMonthlyBudget()*: double                                                     |
| + getHeadcount()*: int                                                            |
| + printOrgStructure(depth: int)*: void                                            |
+-----------------------------------------------------------------------------------+
           ^                                                      ^
           | implements (Leaf)                                    | implements (Composite)
           |                                                      |
+------------------------------------+         +------------------------------------+
|              Employee              |         |             Department             |
|               (Leaf)               |         |            (Composite)             |
+------------------------------------+         +------------------------------------+
| - id: int                          |         | - departmentName: string           |
| - name: string                     |         | - operationalOverhead: double      |
| - monthlySalary: double            |         | - members: vector<                 |
+------------------------------------+         |             shared_ptr<IOrgUnit>>  |
| + getMonthlyBudget():              |         +------------------------------------+
|     return monthlySalary;          |         | + getMonthlyBudget():              |
| + getHeadcount(): return 1;        |         |     sum(member.getMonthlyBudget()) |
+------------------------------------+         |     + operationalOverhead          |
                                               | + getHeadcount():                  |
                                               |     sum(member.getHeadcount())     |
                                               | + addUnit(unit: shared_ptr<IOrg>): |
                                               +------------------------------------+
                                                                  |
                                                                  | 1 CONTAINS MANY
                                                                  +------> (Tree Recursion)
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Uniform Organizational Unit Interface (Composite Pattern)
class IOrgUnit {
public:
    virtual ~IOrgUnit() = default;
    virtual std::string getName() const = 0;
    virtual double getMonthlyBudget() const = 0;
    virtual int getHeadcount() const = 0;
    virtual void printTree(int indent = 0) const = 0;
};

// 2. Leaf Component: Employee
class Employee : public IOrgUnit {
private:
    std::string name;
    std::string title;
    double monthlySalary;

public:
    Employee(std::string name, std::string title, double salary)
        : name(std::move(name)), title(std::move(title)), monthlySalary(salary) {}

    std::string getName() const override { return name; }
    double getMonthlyBudget() const override { return monthlySalary; }
    int getHeadcount() const override { return 1; }

    void printTree(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "- [Emp] " << name << " (" << title << ") - $" << monthlySalary << "\n";
    }
};

// 3. Composite Component: Department (Aggregates Employees & Sub-Departments)
class Department : public IOrgUnit {
private:
    std::string departmentName;
    double operationalOverhead; // Rent, software licenses, server costs
    std::vector<std::shared_ptr<IOrgUnit>> members;

public:
    Department(std::string name, double overhead = 0.0)
        : departmentName(std::move(name)), operationalOverhead(overhead) {}

    void addUnit(std::shared_ptr<IOrgUnit> unit) {
        members.push_back(std::move(unit));
    }

    std::string getName() const override { return departmentName; }

    // Recursive Budget Calculation across sub-trees
    double getMonthlyBudget() const override {
        double total = operationalOverhead;
        for (const auto& member : members) {
            total += member->getMonthlyBudget(); // Uniform recursive call!
        }
        return total;
    }

    // Recursive Headcount Aggregation
    int getHeadcount() const override {
        int count = 0;
        for (const auto& member : members) {
            count += member->getHeadcount();
        }
        return count;
    }

    void printTree(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "+ [Dept] " << departmentName << " (Overhead: $" << operationalOverhead << ")\n";
        for (const auto& member : members) {
            member->printTree(indent + 4);
        }
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"An enterprise organization chart is a classic hierarchical tree where branches can be individual employees or entire nested sub-departments."*
> 2. *"To avoid writing duplicated recursive functions for every financial query, I apply the **GoF Composite Pattern**."*
> 3. *"Both `Employee` (Leaf) and `Department` (Composite) implement the uniform `IOrgUnit` interface (`getMonthlyBudget()`, `getHeadcount()`, `printTree()`)."*
> 4. *"When executive management queries `engineeringDept->getMonthlyBudget()`, the department sums its own fixed operational overhead and recursively queries each member `IOrgUnit`. It doesn't need to know whether a member is an individual senior engineer or an entire QA department."*
> 5. *"This design satisfies the **Open-Closed Principle (OCP)**: We can introduce contractor units, offshore vendor teams, or project squads without changing how budget and headcount aggregation works."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you export the org chart to JSON or perform custom salary audits?"** | Apply the **Visitor Pattern** (`IOrgVisitor` with `visit(Employee&)` and `visit(Department&)`) to add external reporting features without modifying entity classes. |
| **"How do you handle employees who belong to multiple departments (Matrix Management)?"** | Split the employee's budget allocation via a `MatrixAllocation` proxy or link `shared_ptr<Employee>` across departments with percentage weights. |
| **"How do you support moving a sub-department or employee to another department?"** | Provide `department->removeUnit(unitId)` and `newDept->addUnit(unit)` using standard container manipulation. |

# LLD: Restaurant Order Items (Customizable Food Items)

> **Core Category:** Dynamic State & Entity Evolution / Combinatorial Customization  
> **Key Design Pattern:** Decorator Pattern (Dynamic Add-ons) + Builder Pattern (Fluent Configuration)  
> **Target Concepts:** Recursive Composition, Dynamic Pricing Calculation, Nutrition/Calorie Aggregation, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"In a restaurant POS / food delivery app (like DoorDash, UberEats, Zomato), customers customize items: A `ClassicBurger` can have extra cheese (+$1.50), double patty (+$3.00), a gluten-free bun upgrade (+$2.00), or 'no mayo' (removes allergen). Customers can add 3x cheese or customize pizzas with dozens of toppings. How do you model pricing, calorie calculation, and ingredient lists dynamically without creating thousands of subclass combinations?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Subclass Explosion for Every Topping Combination

```
                                  +-------------------+
                                  |    BaseBurger     |  ($5.00, 500 kcal)
                                  +-------------------+
                                     /             \
                                    /               \
                       +------------------+   +------------------+
                       |   CheeseBurger   |   | DoublePattyBurger|
                       +------------------+   +------------------+
                                \                     /
                                 \                   /
                           +-------------------------------+
                           | DoublePattyCheeseBurgerWith.. |  <-- Combinatorial Explosion!
                           +-------------------------------+
```

#### Why the interviewer rejects this:
1. **$O(2^N)$ Combinatorial Nightmare:** With 10 possible toppings, you would need $2^{10} = 1,024$ distinct classes!
2. **Multipliers are Impossible:** What if a customer orders *Triple Cheese*? Inheritance cannot model repeating the same topping 3 times.
3. **Menu Inflexibility:** Adding a new topping like "Jalapeno" requires modifying or multiplying existing subclasses.

---

### ❌ Anti-Pattern 2: God Burger Class with Boolean Arrays

```
+-------------------------------------------------------------------------+
|                               FoodItem                                  |
+-------------------------------------------------------------------------+
| - basePrice: double                                                     |
| - hasExtraCheese: bool                                                  |
| - hasDoublePatty: bool                                                  |
| - hasGlutenFreeBun: bool                                                |
| - hasBacon: bool                                                        |
| + getPrice(): double -> 50-line switch/case adding up every bool flag!  |
+-------------------------------------------------------------------------+
```

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A Base Food Item (`Burger`, `Pizza`) and its Toppings (`ExtraCheese`, `Bacon`) share the same `IFoodItem` interface. Toppings recursively wrap the base item (Decorator Pattern)."*

```
+-----------------------------------------------------------------------------------+
|                                   <<interface>>                                   |
|                                     IFoodItem                                     |
+-----------------------------------------------------------------------------------+
| + getDescription()*: string                                                       |
| + getPrice()*: double                                                             |
| + getCalories()*: int                                                             |
| + getIngredients()*: vector<string>                                               |
+-----------------------------------------------------------------------------------+
           ^                                                      ^
           | implements                                           | implements & wraps
           |                                                      |
+------------------------------------+         +------------------------------------+
|            ClassicBurger           |         |       FoodToppingDecorator (Base)  |
+------------------------------------+         +------------------------------------+
| - basePrice = 5.99                 |         | # innerItem: unique_ptr<IFoodItem> |
| - baseCalories = 550               |         +------------------------------------+
+------------------------------------+         | + getPrice() -> innerItem->getPrice|
| + getDescription(): "Classic Burger|         +------------------------------------+
| + getPrice(): 5.99                 |                            ^
| + getCalories(): 550               |                            | extends
+------------------------------------+                 +----------+----------+
                                                       |                     |
                                            +---------------------+ +---------------------+
                                            | ExtraCheeseDecorator| | DoublePattyDecorator|
                                            +---------------------+ +---------------------+
                                            | + getPrice():       | | + getPrice():       |
                                            |     inner + 1.50    | |     inner + 3.00    |
                                            | + getCalories():    | | + getCalories():    |
                                            |     inner + 120 kcal| |     inner + 300 kcal|
                                            +---------------------+ +---------------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Uniform Component Interface
class IFoodItem {
public:
    virtual ~IFoodItem() = default;
    virtual std::string getDescription() const = 0;
    virtual double getPrice() const = 0;
    virtual int getCalories() const = 0;
};

// 2. Concrete Base Item
class ClassicBurger : public IFoodItem {
public:
    std::string getDescription() const override { return "Classic Beef Burger"; }
    double getPrice() const override { return 6.00; }
    int getCalories() const override { return 550; }
};

// 3. Base Topping Decorator
class FoodToppingDecorator : public IFoodItem {
protected:
    std::unique_ptr<IFoodItem> innerItem;
public:
    explicit FoodToppingDecorator(std::unique_ptr<IFoodItem> item) : innerItem(std::move(item)) {}
    std::string getDescription() const override { return innerItem->getDescription(); }
    double getPrice() const override { return innerItem->getPrice(); }
    int getCalories() const override { return innerItem->getCalories(); }
};

// 4. Concrete Topping Decorators (Add-ons)
class ExtraCheeseDecorator : public FoodToppingDecorator {
public:
    explicit ExtraCheeseDecorator(std::unique_ptr<IFoodItem> item) : FoodToppingDecorator(std::move(item)) {}
    std::string getDescription() const override { return innerItem->getDescription() + ", Extra Cheese"; }
    double getPrice() const override { return innerItem->getPrice() + 1.50; }
    int getCalories() const override { return innerItem->getCalories() + 120; }
};

class BaconDecorator : public FoodToppingDecorator {
public:
    explicit BaconDecorator(std::unique_ptr<IFoodItem> item) : FoodToppingDecorator(std::move(item)) {}
    std::string getDescription() const override { return innerItem->getDescription() + ", Crispy Bacon"; }
    double getPrice() const override { return innerItem->getPrice() + 2.00; }
    int getCalories() const override { return innerItem->getCalories() + 180; }
};

// 5. Fluent Builder for Easy Client Assembly
class BurgerBuilder {
    std::unique_ptr<IFoodItem> burger;
public:
    BurgerBuilder() : burger(std::make_unique<ClassicBurger>()) {}
    BurgerBuilder& addCheese() { burger = std::make_unique<ExtraCheeseDecorator>(std::move(burger)); return *this; }
    BurgerBuilder& addBacon() { burger = std::make_unique<BaconDecorator>(std::move(burger)); return *this; }
    std::unique_ptr<IFoodItem> build() { return std::move(burger); }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model customized food items using inheritance (e.g. `CheeseBurgerWithBacon`), we encounter an **$O(2^N)$ subclass explosion** and cannot support multiple quantities of the same topping (e.g. Triple Cheese)."*
> 2. *"If we put boolean flags in a `Burger` class, we violate the **Open-Closed Principle (OCP)** every time the restaurant introduces a seasonal topping."*
> 3. *"Instead, I apply the **Decorator Pattern** with a **Builder Pattern** wrapper. The base `ClassicBurger` implements `IFoodItem`. Each topping (`ExtraCheeseDecorator`, `BaconDecorator`) is a decorator wrapping another `IFoodItem`."*
> 4. *"When `getPrice()` or `getCalories()` is invoked on the outermost decorator, the call propagates recursively down the chain, aggregating prices and nutritional data seamlessly."*
> 5. *"This design makes adding new toppings trivial: We simply write a new `IToppingDecorator` class with **zero changes to existing menu items**."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle ingredient REMOVALS (e.g. 'No Onions', 'No Pickles')?"** | Create an `IngredientRemovalDecorator` holding a list of excluded items that subtracts allergens/calories or filters the recipe printout. |
| **"How do you enforce maximum limits (e.g. Maximum 4 extra patties)?"** | In the `BurgerBuilder`, track a count map of applied decorators and throw an `InvalidOrderException` if limits are exceeded. |
| **"How do you support size variations (Small, Medium, Large)?"** | Add a `SizeMultiplierDecorator` (e.g., `LargeSizeDecorator` multiplies entire inner price by $1.3\times$). |

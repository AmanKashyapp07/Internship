# LLD: UI Component Tree (Widgets, Containers, Event Bubbling)

> **Core Category:** Hierarchical & Aggregation Modeling  
> **Key Design Pattern:** Composite Pattern (Leaf Widgets vs. Layout Containers) + Chain of Responsibility (Event Bubbling)  
> **Target Concepts:** Recursive Rendering Hierarchy, Coordinate Geometry Propagation, Event Dispatching, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"You are designing a graphical user interface (GUI) framework (like the browser DOM, React, Qt, or Flutter). The UI is composed of leaf widgets (`Button`, `Label`, `TextBox`) and layout containers (`Panel`, `Dialog`, `Window`) that contain other widgets and nested panels. When the root `Window` is told to `render()`, `resize()`, or `handleMouseEvent(event)`, how do you design the hierarchy so rendering propagates recursively down the tree while user click events bubble up the hierarchy to their parent containers?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Concrete Downcasting & Type Checking

```cpp
// ❌ Anti-Pattern: Manual type switching for layout and event propagation
void renderUI(Widget* widget) {
    if (auto panel = dynamic_cast<Panel*>(widget)) {
        for (auto child : panel->getChildren()) renderUI(child);
    } else if (auto btn = dynamic_cast<Button*>(widget)) {
        btn->drawButton();
    } else if (auto lbl = dynamic_cast<Label*>(widget)) {
        lbl->drawLabel();
    }
}
```

#### Why the interviewer rejects this:
1. **Open-Closed Principle (OCP) Violation:** Adding a new UI widget (e.g. `Slider`, `Dropdown`, `Canvas`) requires modifying every rendering and event loop switch-statement in the application.
2. **Tight Coupling:** Parent containers are tightly coupled to the exact concrete types of their children.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Apply the Composite Pattern for recursive tree rendering and layout, combined with the Chain of Responsibility Pattern for event bubbling up to parent containers."*

```
+-----------------------------------------------------------------------------------+
|                            <<interface>> UIComponent                              |
+-----------------------------------------------------------------------------------+
| # parent: UIComponent*                                                            |
| # x, y, width, height: int                                                        |
+-----------------------------------------------------------------------------------+
| + render(renderer: GraphicsContext)*: void                                        |
| + onMouseEvent(event: MouseEvent)*: bool  <-- Returns true if handled!             |
| + setParent(parent: UIComponent*): void                                           |
| + getParent(): UIComponent*                                                       |
+-----------------------------------------------------------------------------------+
           ^                                                      ^
           | implements (Leaf)                                    | implements (Composite)
           |                                                      |
+------------------------------------+         +------------------------------------+
|               Button               |         |            UIContainer             |
|              (Leaf)                |         |            (Composite)             |
+------------------------------------+         +------------------------------------+
| - labelText: string                |         | - children: vector<                |
| - onClickListener: function        |         |             unique_ptr<UIComponent>|
+------------------------------------+         +------------------------------------+
| + render(ctx): Draw button graphic |         | + render(ctx):                     |
| + onMouseEvent(e):                 |         |     for child in children:         |
|     if (contains(e.x, e.y)) {      |         |         child->render(ctx)         |
|         onClick(); return true;    |         +------------------------------------+
|     }                              |         | + onMouseEvent(e):                 |
|     return parent->onMouseEvent(e);|         |     1. Dispatch to child hitting pt|
|     (Chain of Responsibility!)     |         |     2. If unhandled, handle self   |
+------------------------------------+         +------------------------------------+
                                                                  |
                                                                  | 1 HOLDS MANY
                                                                  +------> (Tree Recursion)
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Common UI Event Structure
struct MouseEvent {
    int x, y;
    enum class Type { CLICK, HOVER, DRAG } type;
};

// 2. Uniform UI Component Interface (Composite Pattern)
class UIComponent {
protected:
    UIComponent* parent = nullptr;
    int x = 0, y = 0, width = 0, height = 0;

public:
    virtual ~UIComponent() = default;
    virtual void render() const = 0;
    virtual bool onMouseEvent(const MouseEvent& event) = 0;

    void setParent(UIComponent* p) { parent = p; }
    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

// 3. Leaf Component: Button
class Button : public UIComponent {
    std::string text;
    std::function<void()> onClick;

public:
    Button(std::string text, std::function<void()> callback)
        : text(std::move(text)), onClick(std::move(callback)) {}

    void render() const override {
        // Draw button rectangle & text
    }

    bool onMouseEvent(const MouseEvent& event) override {
        if (contains(event.x, event.y) && event.type == MouseEvent::Type::CLICK) {
            if (onClick) onClick();
            return true; // Event consumed!
        }
        // Event Bubbling (Chain of Responsibility)
        return parent ? parent->onMouseEvent(event) : false;
    }
};

// 4. Composite Component: Panel / Window (Layout Container)
class UIContainer : public UIComponent {
private:
    std::vector<std::unique_ptr<UIComponent>> children;

public:
    void addChild(std::unique_ptr<UIComponent> child) {
        child->setParent(this);
        children.push_back(std::move(child));
    }

    // Top-down recursive rendering
    void render() const override {
        // Render container background
        for (const auto& child : children) {
            child->render(); // Uniform recursive dispatch!
        }
    }

    // Top-down routing + Bottom-up bubbling
    bool onMouseEvent(const MouseEvent& event) override {
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->contains(event.x, event.y)) {
                if ((*it)->onMouseEvent(event)) return true; // Handled by child!
            }
        }
        // If no child handled it, container handles or bubbles to parent
        return parent ? parent->onMouseEvent(event) : false;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"A GUI framework is inherently tree-structured. Instead of writing separate rendering loops for buttons, panels, and windows, I apply the **GoF Composite Pattern**."*
> 2. *"Both simple widgets (`Button`, `Label`) and containers (`Panel`, `Window`) share the uniform `UIComponent` base interface (`render()`, `onMouseEvent()`)."*
> 3. *"When `render()` is called on the root window, it recursively triggers rendering on all child widgets without needing to know their concrete types."*
> 4. *"For user interaction, I integrate the **Chain of Responsibility Pattern (Event Bubbling)**: When a mouse click occurs, the event is routed top-down to the targeted child. If the child doesn't handle it (or once it finishes handling), the event bubbles up to the parent container."*
> 5. *"This design satisfies the **Open-Closed Principle (OCP)**: We can build rich composite widgets (like a `SearchBox` containing a `TextBox` and a `SearchButton`) without modifying the core GUI event loop."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you implement Layout Managers (e.g. Flexbox, Grid, Absolute)?"** | Use the **Strategy Pattern**: Inject an `ILayoutStrategy` (`FlexLayout`, `GridLayout`) into `UIContainer` to calculate child bounds before rendering. |
| **"How do you prevent rendering off-screen or occluded components (Repaint optimization)?"** | Maintain a `dirtyBounds` bounding box and clip child render calls using the `GraphicsContext::clipRect()`. |
| **"How do you handle focus management (Tab key navigation)?"** | Maintain a `FocusManager` holding an ordered list of `UIComponent*` leaves, delegating keyboard events to the currently focused component. |

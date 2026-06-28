# Common Design Patterns 🧩

Design patterns are typical solutions to commonly occurring problems in software design. They are like pre-made blueprints that you can customize to solve a recurring design problem in your code.

They are categorized into three main groups: **Creational**, **Structural**, and **Behavioral**.

---

## 1. Creational Patterns
These patterns deal with object creation mechanisms, trying to create objects in a manner suitable to the situation, instead of basic instantiation.

### A. Singleton
Ensures a class has only one instance, and provides a global point of access to it.
* **Use Case:** Database connections, Logging services, Configuration managers.
* **Implementation:** Private constructor, static private instance variable, public static `getInstance()` method.

### B. Factory Method
Provides an interface for creating objects in a superclass, but allows subclasses to alter the type of objects that will be created.
* **Use Case:** UI libraries where you need to create cross-platform buttons (WindowsButton vs MacButton) without tightly coupling your code to the specific classes.

### C. Abstract Factory
Produces families of related objects without specifying their concrete classes.
* **Use Case:** Creating a whole suite of UI elements (Button, Checkbox, TextField) that must match a specific theme (Light Theme vs Dark Theme) together.

### D. Builder
Separates the construction of a complex object from its representation. Allows you to construct complex objects step by step.
* **Use Case:** Creating complex objects like an `HttpRequest` or a `Pizza` with many optional parameters (avoiding telescoping constructors).

---

## 2. Structural Patterns
These patterns explain how to assemble objects and classes into larger structures, while keeping these structures flexible and efficient.

### A. Adapter
Allows objects with incompatible interfaces to collaborate. It acts as a wrapper.
* **Use Case:** Integrating a legacy XML-based billing system with a modern JSON-based analytics platform.

### B. Decorator
Lets you attach new behaviors to objects by placing these objects inside special wrapper objects that contain the behaviors.
* **Use Case:** Adding features to a text stream (encryption, compression) dynamically without subclassing. (e.g., `Java.io` classes).

### C. Facade
Provides a simplified interface to a library, a framework, or any other complex set of classes.
* **Use Case:** Providing a simple `VideoConverter.convert(file, format)` method that hides the complexity of interacting with internal classes like `AudioMixer`, `CodecFactory`, and `BitrateReader`.

### D. Proxy
Provides a substitute or placeholder for another object to control access to it.
* **Use Case:** Lazy loading a massive image file (Virtual Proxy), or checking permissions before allowing access to a secure database object (Protection Proxy).

---

## 3. Behavioral Patterns
These patterns are concerned with algorithms and the assignment of responsibilities between objects.

### A. Observer (Pub/Sub)
Defines a subscription mechanism to notify multiple objects about any events that happen to the object they're observing.
* **Use Case:** MVC architectures (View updates when Model changes), Mailing list subscriptions, UI Event Listeners.

### B. Strategy
Lets you define a family of algorithms, put each of them into a separate class, and make their objects interchangeable at runtime.
* **Use Case:** Implementing different payment methods (Credit Card, PayPal, Crypto) or sorting algorithms that can be swapped dynamically.

### C. Command
Turns a request into a stand-alone object that contains all information about the request.
* **Use Case:** Implementing Undo/Redo functionality in text editors, or queuing requests/jobs to execute them later.

### D. State
Lets an object alter its behavior when its internal state changes. It appears as if the object changed its class.
* **Use Case:** Vending machines, Document workflows (Draft $\rightarrow$ Review $\rightarrow$ Published), Media players (Play, Pause, Stop behaviors).

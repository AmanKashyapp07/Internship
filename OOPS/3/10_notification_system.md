# LLD: Notification System (Channels × Formats)

> **Core Category:** Polymorphic Operations & Multi-Axis Variations  
> **Key Design Pattern:** Bridge Pattern (Delivery Channels × Message Formatters) + Adapter / Strategy Pattern  
> **Target Concepts:** Decoupling Content Presentation from Network Delivery, Preventing $M \times N$ Subclassing, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"You are building an enterprise notification engine (like Twilio / SendGrid / Firebase). Alerts must be sent across multiple delivery channels (`SMS`, `Email`, `PushNotification`, `Slack`) and formatted across multiple markup formats (`PlainText`, `HTML`, `Markdown`, `JSON`). An email alert needs rich HTML, an SMS requires concise PlainText (under 160 characters), and a Slack bot alert needs Markdown. How do you design this architecture without creating combinatorial classes like `EmailHtmlNotification` or `SmsPlainTextNotification`?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Multi-Axis Class Explosion ($M \times N$ Subclasses)

```
                               +----------------------------+
                               |        Notification        |
                               +----------------------------+
                                  /            |           \
                                 /             |            \
                   +----------------+  +----------------+  +----------------+
                   |   EmailAlert   |  |    SmsAlert    |  |   SlackAlert   |
                   +----------------+  +----------------+  +----------------+
                      /     |    \        /     |    \        /     |    \
                     /      |     \      /      |     \      /      |     \
               +-----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+
               |Html | |MD  | |JSON| |Html| |MD  | |JSON| |Html| |MD  | |JSON|
               |Email| |Em..| |Em..| |SMS | |SMS | |SMS | |Sl..| |Sl..| |Sl..|
               +-----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+
```

#### Why the interviewer rejects this:
1. **$M \times N$ Redundant Classes:** If you support 4 channels and 4 formatting styles, you end up writing **16 classes**. Adding a new channel (e.g. `WhatsApp`) requires creating 4 more classes.
2. **Formatting Logic Duplication:** HTML rendering logic is duplicated across `HtmlEmail` and `HtmlSlack`.
3. **Template Rigidity:** You cannot switch an email from PlainText to HTML dynamically based on user preferences.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Apply the Bridge Pattern. Separate the Delivery Channel (`INotificationChannel` - SMTP, Twilio, APNs, Webhook) from the Message Formatter (`IMessageFormatter` - PlainText, HTML, Markdown)."*

```
+-----------------------------------------------------------------------------------+
|                        <<abstraction>> Notification                               |
+-----------------------------------------------------------------------------------+
| # channel: shared_ptr<INotificationChannel>                                       |
| # formatter: shared_ptr<IMessageFormatter>                                        |
+-----------------------------------------------------------------------------------+
| + send(recipient: string, rawTitle: string, rawBody: string)*: void               |
| + setFormatter(fmt: shared_ptr<IMessageFormatter>): void                          |
| + setChannel(ch: shared_ptr<INotificationChannel>): void                          |
+-----------------------------------------------------------------------------------+
           |                                                      |
           | HAS-A (Channel Bridge)                               | HAS-A (Formatter Bridge)
           v                                                      v
+------------------------------------+         +------------------------------------+
|           <<interface>>            |         |           <<interface>>            |
|        INotificationChannel        |         |          IMessageFormatter         |
+------------------------------------+         +------------------------------------+
| + deliver(to, payload)*: bool      |         | + format(title, body)*: string     |
| + getChannelName()*: string        |         | + getContentType()*: string        |
+------------------------------------+         +------------------------------------+
           ^                                                      ^
           | implements                                           | implements
     +-----+-----+                                          +-----+-----+
     |           |                                          |           |
+----------+ +----------+                              +----------+ +----------+
|  Email   | |   SMS    |                              |   HTML   | | Markdown |
| Channel  | | Channel  |                              |Formatter | |Formatter |
+----------+ +----------+                              +----------+ +----------+
| - smtpSrv| | - twilio |                              |<b>bold</b>| |**bold**  |
+----------+ +----------+                              +----------+ +----------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Bridge Dimension A: Message Formatter Interface (Presentation)
class IMessageFormatter {
public:
    virtual ~IMessageFormatter() = default;
    virtual std::string format(const std::string& title, const std::string& body) = 0;
};

class HtmlFormatter : public IMessageFormatter {
public:
    std::string format(const std::string& title, const std::string& body) override {
        return "<html><body><h1>" + title + "</h1><p>" + body + "</p></body></html>";
    }
};

class PlainTextFormatter : public IMessageFormatter {
public:
    std::string format(const std::string& title, const std::string& body) override {
        return "[" + title + "] " + body;
    }
};

// 2. Bridge Dimension B: Delivery Channel Interface (Transport)
class INotificationChannel {
public:
    virtual ~INotificationChannel() = default;
    virtual bool deliver(const std::string& recipient, const std::string& payload) = 0;
};

class EmailChannel : public INotificationChannel {
public:
    bool deliver(const std::string& to, const std::string& payload) override {
        // Dispatch over SMTP / SendGrid API
        return true;
    }
};

class SmsChannel : public INotificationChannel {
public:
    bool deliver(const std::string& to, const std::string& payload) override {
        // Dispatch over Twilio REST API
        return true;
    }
};

// 3. The Decoupled Notification Entity (Composition Root)
class Notification {
private:
    std::shared_ptr<INotificationChannel> channel;
    std::shared_ptr<IMessageFormatter> formatter;

public:
    Notification(std::shared_ptr<INotificationChannel> ch, std::shared_ptr<IMessageFormatter> fmt)
        : channel(std::move(ch)), formatter(std::move(fmt)) {}

    void send(const std::string& recipient, const std::string& title, const std::string& body) {
        // 1. Format payload according to formatting strategy
        std::string payload = formatter->format(title, body);
        // 2. Deliver payload over target transport channel
        channel->deliver(recipient, payload);
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model notifications using inheritance (e.g. `HtmlEmailNotification`, `MarkdownSlackNotification`), we create an **$M \times N$ class explosion**. Adding a WhatsApp channel or JSON formatter forces us to write multiple redundant subclasses."*
> 2. *"Instead, I apply the **Bridge Pattern** to decouple the **Presentation Layer (`IMessageFormatter`)** from the **Delivery Transport Layer (`INotificationChannel`)**."*
> 3. *"A `Notification` holds references to both. An email alert is simply a `Notification` composed of `EmailChannel` and `HtmlFormatter`. An SMS alert pairs `SmsChannel` with `PlainTextFormatter`."*
> 4. *"This gives us $O(M + N)$ scalability: Adding a new channel like `TelegramChannel` requires writing exactly **1 class** and immediately works with all existing formatters (HTML, PlainText, Markdown)."*
> 5. *"This also enables dynamic user customization: If a user selects 'Plain Text only' in their profile settings, we can swap `HtmlFormatter` for `PlainTextFormatter` at runtime with zero code duplication."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle broadcast notifications (e.g. Send to Email AND SMS simultaneously)?"** | Use the **Composite Pattern**: Create a `CompositeChannel` holding a `vector<shared_ptr<INotificationChannel>>` that fans out to each channel. |
| **"How do you implement rate limiting (e.g. Max 5 SMS per minute)?"** | Wrap channels in a **Decorator Pattern** (`RateLimitingChannelDecorator` using a Token Bucket algorithm) before delegating to `deliver()`. |
| **"How do you handle channel retry policies and transient network errors?"** | Add a `RetryChannelDecorator` with exponential backoff or forward failed alerts to a Dead Letter Queue (DLQ). |

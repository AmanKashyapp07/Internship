# LLD: Database Query Builder & Connection Engine

> **Core Category:** Polymorphic Operations & Multi-Axis Variations  
> **Key Design Pattern:** Strategy Pattern (SQL Dialect) × Bridge Pattern (Transport Protocol) + Abstract Factory  
> **Target Concepts:** Decoupling Query Generation from Database Execution, Abstract Factory for Family Creation, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"You are designing a cross-database ORM client (like Hibernate, SQLAlchemy, or Prisma) supporting `PostgreSQL`, `MySQL`, and `SQLite`. Each database has: (1) A unique SQL dialect (e.g. Postgres uses `LIMIT x OFFSET y` and `ILIKE`; SQL Server/Oracle use `TOP x`; Postgres supports `ON CONFLICT DO UPDATE`, MySQL uses `ON DUPLICATE KEY UPDATE`), and (2) A unique network transport protocol (Postgres uses a binary TCP socket, MySQL uses its own wire protocol, SQLite uses in-process local C-bindings). How do you decouple the SQL query builder from the database connection logic without creating monolithic database drivers?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Monolithic Database Driver Hierarchy

```
                                  +---------------------+
                                  |    DatabaseDriver   |
                                  +---------------------+
                                     /        |        \
                                    /         |         \
                    +----------------+ +---------------+ +----------------+
                    | PostgresDriver | |  MySQLDriver  | |  SQLiteDriver  |
                    +----------------+ +---------------+ +----------------+
                    | + buildSelect()| | + buildSelect()| | + buildSelect()|
                    | + connectTCP() | | + connectTCP()| | + openLocalFile|
                    | + executeSQL() | | + executeSQL()| | + executeSQL() |
                    +----------------+ +---------------+ +----------------+
```

#### Why the interviewer rejects this:
1. **Violation of Single Responsibility Principle (SRP):** `PostgresDriver` is responsible for BOTH SQL string syntax compilation AND low-level socket I/O.
2. **Untestable Code:** You cannot unit test your SQL query generator without establishing a live network socket connection to a PostgreSQL database!
3. **No Code Reuse:** Common SQL query building logic (e.g., standard ANSI SQL `WHERE`, `JOIN`, `GROUP BY`) cannot be reused across drivers.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Separate the two independent concerns: (1) The Query Compiler (`ISqlDialect` - Strategy Pattern for building SQL strings), and (2) The Connection Transport (`IDbConnection` - Bridge Pattern for executing queries over wire/file). Use an `AbstractFactory` to pair compatible families."*

```
+-----------------------------------------------------------------------------------+
|                                  DatabaseClient                                   |
+-----------------------------------------------------------------------------------+
| - dialect: unique_ptr<ISqlDialect>                                                |
| - connection: unique_ptr<IDbConnection>                                           |
+-----------------------------------------------------------------------------------+
| + query(table: string): QueryBuilder                                              |
| + executeQuery(query: QueryObj): ResultSet                                        |
+-----------------------------------------------------------------------------------+
           |                                                      |
           | HAS-A (Dialect Strategy)                             | HAS-A (Connection Bridge)
           v                                                      v
+------------------------------------+         +------------------------------------+
|           <<interface>>            |         |           <<interface>>            |
|            ISqlDialect             |         |           IDbConnection            |
+------------------------------------+         +------------------------------------+
| + buildSelect(table, limit, off)*  |         | + openConnection(connStr)*: void   |
| + buildUpsert(table, cols, keys)*  |         | + executeRawSql(sql)*: ResultSet   |
| + quoteIdentifier(name)*: string   |         | + closeConnection()*: void         |
+------------------------------------+         +------------------------------------+
           ^                                                      ^
           | implements                                           | implements
     +-----+-----+                                          +-----+-----+
     |           |                                          |           |
+----------+ +----------+                              +----------+ +----------+
| Postgres | |  MySQL   |                              |TCP Socket| | SQLite C |
| Dialect  | | Dialect  |                              |Connection| | Binding  |
+----------+ +----------+                              +----------+ +----------+
| "ILIKE"  | "LIKE"     |                              | (Postgres| | (In-proc |
| LIMIT/OFF| LIMIT/OFF  |                              |  / MySQL)| |  Memory) |
+----------+ +----------+                              +----------+ +----------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Common Query Data Structures
struct Query {
    std::string table;
    std::vector<std::string> columns;
    int limit = -1, offset = -1;
};

// 2. Strategy Dimension A: SQL Dialect Compiler
class ISqlDialect {
public:
    virtual ~ISqlDialect() = default;
    virtual std::string compileSelect(const Query& q) const = 0;
    virtual std::string quote(const std::string& id) const = 0;
};

class PostgresDialect : public ISqlDialect {
public:
    std::string quote(const std::string& id) const override { return "\"" + id + "\""; }
    std::string compileSelect(const Query& q) const override {
        std::string sql = "SELECT * FROM " + quote(q.table);
        if (q.limit > 0) sql += " LIMIT " + std::to_string(q.limit);
        if (q.offset > 0) sql += " OFFSET " + std::to_string(q.offset);
        return sql;
    }
};

// 3. Bridge Dimension B: Connection Transport Layer
class IDbConnection {
public:
    virtual ~IDbConnection() = default;
    virtual void connect(const std::string& connectionString) = 0;
    virtual void executeSql(const std::string& sql) = 0;
};

class TcpSocketConnection : public IDbConnection {
public:
    void connect(const std::string& connStr) override { /* Open TCP socket over port 5432 / 3306 */ }
    void executeSql(const std::string& sql) override { /* Send binary packet over wire */ }
};

// 4. Abstract Factory: Pairs matching Dialect + Connection
class IDatabaseFactory {
public:
    virtual ~IDatabaseFactory() = default;
    virtual std::unique_ptr<ISqlDialect> createDialect() = 0;
    virtual std::unique_ptr<IDbConnection> createConnection() = 0;
};

class PostgresDatabaseFactory : public IDatabaseFactory {
public:
    std::unique_ptr<ISqlDialect> createDialect() override { return std::make_unique<PostgresDialect>(); }
    std::unique_ptr<IDbConnection> createConnection() override { return std::make_unique<TcpSocketConnection>(); }
};

// 5. Clean Database Client Orchestrator
class DatabaseClient {
    std::unique_ptr<ISqlDialect> dialect;
    std::unique_ptr<IDbConnection> connection;
public:
    explicit DatabaseClient(std::unique_ptr<IDatabaseFactory> factory)
        : dialect(factory->createDialect()), connection(factory->createConnection()) {}

    void execute(const Query& query) {
        std::string sql = dialect->compileSelect(query); // 1. Compile SQL
        connection->executeSql(sql);                     // 2. Transmit over wire
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"Monolithic database drivers tightly couple **SQL dialect string generation** with **network socket transport**. This violates the **Single Responsibility Principle (SRP)** and makes unit-testing query builders impossible without spinning up a live database server."*
> 2. *"To solve this, I decouple the architecture into two independent axes: **`ISqlDialect` (Strategy Pattern)** for dialect-specific SQL syntax compilation, and **`IDbConnection` (Bridge Pattern)** for physical connection management."*
> 3. *"We tie compatible dialect and connection pairs together using an **Abstract Factory (`IDatabaseFactory`)**."*
> 4. *"This gives us complete testability: We can unit-test `PostgresDialect` by asserting generated SQL strings in pure memory with zero network overhead."*
> 5. *"It also satisfies the **Open-Closed Principle (OCP)**: Adding support for CockroachDB or SQLite requires writing a new Dialect/Connection class without touching the core `DatabaseClient`."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you implement Connection Pooling (reuse open sockets)?"** | Use the **Object Pool Pattern**: Create a `ConnectionPool` that manages an active queue of `unique_ptr<IDbConnection>` instances to eliminate TCP handshake overhead. |
| **"How do you support Transactions (`BEGIN`, `COMMIT`, `ROLLBACK`)?"** | Create a `TransactionContext` object implementing `RAII`: It sends `BEGIN` on construction and automatically triggers `ROLLBACK` in its destructor if an unhandled exception occurs before `commit()`. |
| **"How do you prevent SQL Injection attacks?"** | The `ISqlDialect` compiler generates parameterized queries (`SELECT * FROM users WHERE id = $1`) and passes parameters separately to `connection->executeParameterizedSql(sql, params)`. |

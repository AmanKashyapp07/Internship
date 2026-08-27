# Types of Databases — Interview Notes

A guide to discuss database categories with the confidence of an engineer who
has made (and lived with) real trade-off decisions, not just recited a taxonomy.

---

## 1. Relational / SQL Databases (e.g., PostgreSQL, MySQL, SQL Server)

**What it is:** Data stored in structured tables with predefined schemas,
relationships enforced via foreign keys, and queried via SQL.

**Strengths**
- ACID guarantees (Atomicity, Consistency, Isolation, Durability) — strong
  correctness for transactional workloads.
- Mature query optimizer, joins, and a declarative query language (SQL) that's
  a universal skill.
- Strong data integrity via constraints, foreign keys, normalization.
- Decades of tooling: ORMs, migrations, backups, monitoring.

**Weaknesses**
- Vertical scaling is the default; horizontal scaling (sharding) is manual and
  operationally painful compared to many NoSQL systems.
- Rigid schema — schema changes on large tables can be costly/slow.
- Joins at scale can become a performance bottleneck.

**When to use:** Anything where data integrity and relationships matter more
than raw horizontal scale — financial systems, order management, anything with
complex, evolving relational queries and where transactions must be reliable.

**Interview-ready line:** "I default to Postgres unless there's a specific
reason not to — the ACID guarantees and mature tooling remove a whole class of
bugs you'd otherwise have to build yourself at the application layer."

---

## 2. Key-Value Stores (e.g., Redis, DynamoDB, Riak)

**What it is:** The simplest NoSQL model — data stored as a key mapped to an
opaque value, retrieved only by key.

**Strengths**
- Extremely fast reads/writes, often sub-millisecond (especially in-memory
  stores like Redis).
- Simple to scale horizontally — easy to shard by key.
- Great for caching, session storage, rate limiting, leaderboards.

**Weaknesses**
- No querying by value — you must know the key. No joins, no secondary indexes
  in the simplest implementations.
- Not suited for complex relationships or ad-hoc queries.

**When to use:** Caching layers, session stores, feature flags, real-time
counters/leaderboards — anywhere access pattern is "give me the value for this
exact key" and speed is the priority.

**Interview-ready line:** "Key-value stores aren't a general-purpose database
substitute — I use them as a targeted cache or lookup layer in front of a
system-of-record, not as the source of truth for complex data."

---

## 3. Document Databases (e.g., MongoDB, Couchbase, Firestore)

**What it is:** Semi-structured data stored as JSON/BSON-like documents, each
document can have a flexible/nested schema.

**Strengths**
- Schema flexibility — good fit for evolving or heterogeneous data models.
- Natural mapping to application objects (less ORM friction than relational).
- Easier horizontal scaling (sharding) than traditional RDBMS, built in from
  the start in most document DBs.

**Weaknesses**
- Weaker consistency guarantees historically (though modern MongoDB supports
  multi-document ACID transactions now, at a performance cost).
- Denormalization can lead to data duplication and update anomalies if not
  modeled carefully.
- Joins across documents are limited/expensive compared to SQL joins.

**When to use:** Content management systems, product catalogs, user profiles
with varying attributes — data that's naturally hierarchical/nested and whose
schema evolves quickly during early product development.

**Interview-ready line:** "Document databases give you schema agility, but
that flexibility shifts data-integrity responsibility from the database to
your application code — I only choose that trade-off deliberately."

---

## 4. Column-Family / Wide-Column Stores (e.g., Cassandra, HBase, Bigtable)

**What it is:** Data organized by column families rather than rows, optimized
for writing and reading massive volumes of data across distributed clusters.

**Strengths**
- Excellent write throughput and horizontal scalability — built for massive
  scale (petabytes, thousands of nodes).
- Tunable consistency (Cassandra lets you choose consistency level per query).
- No single point of failure — masterless, peer-to-peer architecture (Cassandra).

**Weaknesses**
- Query flexibility is limited — you generally must design your table around
  your query patterns upfront (query-first modeling), unlike SQL's flexible
  ad-hoc querying.
- Eventual consistency by default in many configurations — not ideal where
  strict consistency is required.
- Operationally complex to run and tune well.

**When to use:** Time-series/IoT data at massive scale, write-heavy workloads
like logging/analytics/event tracking, systems that must stay available across
multiple data centers.

**Interview-ready line:** "Cassandra-style stores force you to model around
your queries up front — that's a real cost, but it's what buys you linear
write scalability across a distributed cluster."

---

## 5. Graph Databases (e.g., Neo4j, Amazon Neptune)

**What it is:** Data modeled as nodes and edges (relationships), optimized for
traversing connections rather than tabular joins.

**Strengths**
- Extremely efficient for relationship-heavy queries (e.g., "friends of
  friends," shortest path) — traversal is O(1) per hop, not join-explosion.
- Natural fit for fraud detection, recommendation engines, social networks,
  knowledge graphs.

**Weaknesses**
- Niche — not a good general-purpose database; overkill if relationships
  aren't the core of your query patterns.
- Smaller talent pool and tooling ecosystem than SQL/NoSQL mainstream options.
- Scaling graph databases horizontally is harder than most other NoSQL types.

**When to use:** Social networks, recommendation systems, fraud/risk detection,
knowledge graphs — anywhere the relationships between entities are the primary
thing you're querying, not just an attribute of the data.

**Interview-ready line:** "I'd reach for a graph database only when the
question I'm answering is fundamentally about relationships and traversal
depth — otherwise it's added operational complexity for no real benefit."

---

## 6. Columnar / OLAP Warehouses (e.g., ClickHouse, Snowflake, DuckDB)

**What it is:** Databases that store data by column rather than by row on disk,
optimized for heavy aggregate analytical queries over billions of rows.

**Strengths**
- Ultra-fast aggregations (`COUNT`, `SUM`, `AVG`) — reads only the specific
  columns needed for the query, skipping irrelevant columns entirely.
- Extreme data compression (10:1 ratio) because values within a single column
  share identical data types and patterns.
- SIMD vectorized execution: evaluates mathematical queries in parallel chunks
  directly on CPU hardware registers.

**Weaknesses**
- Poor performance for transactional single-row lookups (`SELECT * WHERE id = 1`).
- Single-row updates and deletes are slow and expensive; writes must be batched
  in large append-only blocks.
- Not suited as an OLTP transactional system of record.

**When to use:** Analytical dashboards, Business Intelligence (BI) reporting,
clickstream event analysis, or massive historical logs where you read millions
of rows across a few columns.

**Interview-ready line:** "Row-oriented databases are built for updating single
records; Columnar databases are built for aggregating across millions of
records — you pick columnar when your queries are aggregate-heavy and OLAP-driven."

---

## 7. Time-Series Databases (e.g., InfluxDB, TimescaleDB, Prometheus)

**What it is:** Databases optimized specifically for timestamped data — high
write throughput, efficient range queries over time, and built-in
downsampling/retention policies.

**Strengths**
- Purpose-built compression and indexing for time-ordered data — far more
  efficient than a general RDBMS for this pattern.
- Native support for aggregation functions over time windows, retention
  policies, and downsampling.

**Weaknesses**
- Narrow use case — not suited for general transactional or relational data.
- Smaller ecosystem than mainstream SQL/NoSQL databases.

**When to use:** Monitoring/observability (metrics, logs), IoT sensor data,
financial tick data — anything where "value over time" is the dominant access
pattern.

**Interview-ready line:** "Time-series workloads have a very specific access
pattern — mostly-append writes and range-over-time reads — and purpose-built
databases exploit that far better than a general RDBMS would."

---

## 8. Search Engines (e.g., Elasticsearch, OpenSearch, Solr)

**What it is:** Not a primary database in the traditional sense, but a
specialized store built for full-text search, relevance ranking, and faceted
filtering, using inverted indexes.

**Strengths**
- Extremely fast full-text search, fuzzy matching, relevance scoring.
- Great for faceted search, log aggregation/analysis (ELK stack), and
  autocomplete.

**Weaknesses**
- Not ACID-compliant, not meant to be a system of record.
- Eventual consistency — data indexed here can lag the source of truth.
- Operationally heavy to run and tune (shard sizing, cluster management).

**When to use:** Product search, log analytics, autocomplete/typeahead —
always paired with a primary database as the source of truth, syncing into
the search engine as a read-optimized index.

**Interview-ready line:** "Elasticsearch is a search index, not a database of
record — I always keep a real source of truth behind it and treat the search
cluster as rebuildable."

---

## 9. Vector Databases (e.g., Pinecone, Milvus, Qdrant, pgvector)

**What it is:** Specialized stores built to index and query high-dimensional
vector embeddings generated by AI/ML models (e.g., text, image, audio vectors)
using similarity metrics.

**Strengths**
- Approximate Nearest Neighbor (ANN) search across millions of dense vectors in
  sub-50ms using indexing algorithms like HNSW (Hierarchical Navigable Small World).
- Native support for semantic similarity search via Cosine Similarity, Dot
  Product, or Euclidean distance.
- Core foundation for LLM Retrieval-Augmented Generation (RAG) and semantic memory.

**Weaknesses**
- Queries are approximate (ANN trade-off between recall accuracy and search speed).
- Memory-heavy: HNSW graphs and high-dimensional vectors (e.g., 1536 floats per
  doc) require massive RAM capacity.
- Immature ecosystem compared to decades-old SQL engines.

**When to use:** LLM RAG pipelines, semantic document search, image similarity,
and recommendation systems where relevance is based on conceptual meaning rather
than exact keyword matching.

**Interview-ready line:** "Vector databases don't replace relational databases;
they serve as semantic indexing engines for unstructured data and AI embeddings —
I pair pgvector or Pinecone with Postgres as the system of record."

---

## Side-by-Side Comparison

| Criteria | Relational (SQL) | Key-Value | Document | Column-Family | Graph | Columnar (OLAP) | Time-Series | Search Engine | Vector DB |
|---|---|---|---|---|---|---|---|---|---|
| Data model | Tables/rows | Key → value | JSON documents | Column families | Nodes/edges | Columns | Timestamped points | Inverted index | High-dim vectors |
| Consistency | Strong (ACID) | Configurable | Configurable/tunable | Tunable, eventual | Strong (mostly) | Eventual/Append | Usually eventual | Eventual | Eventual |
| Scaling pattern | Vertical, hard shard | Easy horizontal | Easy horizontal | Excellent horizontal | Harder horizontal | Distributed MPP | Good horizontal | Horizontal (sharded) | Horizontal (sharded) |
| Query flexibility | High (SQL, joins) | Low (key only) | Medium | Low (query-first) | High (traversal) | High (aggregations) | Low (time-range) | High (text/fuzzy) | High (vector similarity)|
| Best for | Transactions, integrity | Caching, sessions | Flexible/nested data | Massive write scale | Relationship traversal | BI & Big Data analytics | Metrics, IoT, logs | Full-text search | LLM RAG, embeddings |
| Weak point | Horizontal scale | No complex queries | Weaker joins/integrity | Rigid query modeling | Niche, complex ops | Slow single-row updates | Narrow use case | Not system of record | High RAM, approximate |

---

## The CAP Theorem — the framework interviewers love to probe

In a distributed system you can only fully guarantee two of three:
**Consistency, Availability, Partition tolerance.** Since network partitions
are a fact of life in distributed systems, the real-world choice is usually
**CP vs AP**:

- **CP (Consistency + Partition tolerance):** e.g., traditional RDBMS
  clusters, MongoDB (with majority writes), HBase — favors correctness over
  uptime during a partition.
- **AP (Availability + Partition tolerance):** e.g., Cassandra, DynamoDB
  (default mode), Riak — favors staying up and accepting eventual consistency
  during a partition.

**Interview-ready line:** "CAP isn't a menu you pick from once — most modern
systems let you tune consistency per-operation (e.g., Cassandra's consistency
levels, DynamoDB's strong vs eventually consistent reads), so the real skill
is knowing which operations need strong consistency and which don't."

---

## Decision Framework (say this out loud in interviews)

1. **Need transactions, strong integrity, complex relational queries** → SQL/Relational.
2. **Need a fast cache or simple lookup by key** → Key-Value.
3. **Data is naturally document-shaped and schema evolves fast** → Document DB.
4. **Need massive write throughput across a distributed cluster** → Column-Family.
5. **Core problem is relationships/traversal** → Graph.
6. **Core problem is heavy aggregations over billions of historical rows** → Columnar (OLAP).
7. **Data is timestamped and mostly append + range-query** → Time-Series.
8. **Core problem is full-text search/relevance** → Search Engine (paired with a system of record).
9. **Core problem is AI semantic similarity / LLM RAG embeddings** → Vector DB.

---

## What separates a senior answer from a junior one

### 1. Storage Engine Physics & Disk Mechanics
- **Junior:** "Databases just store data; some are fast and some are slow."
- **Senior:** *"A senior understands the **physical mechanical tradeoffs** of each storage engine:
  - **B+ Trees (Postgres / MySQL):** Optimized for fast random point reads and range scans via 8KB/16KB disk pages, but heavy writes cause high write amplification and random disk I/O.
  - **LSM-Trees (Cassandra / RocksDB):** Converts random writes into sequential in-memory MemTable appends flushed to immutable SSTable disk files, achieving 250k+ writes/sec at the cost of background compaction overhead.
  - **Column-Major Layout (ClickHouse / Snowflake):** Stores columns in separate contiguous compressed files, enabling SIMD CPU vectorization across billions of rows for OLAP aggregations, but single-row updates are prohibitively expensive.
  - **Index-Free Adjacency (Neo4j):** Nodes hold direct physical memory pointers to neighbor edges, enabling $O(k)$ graph hops instead of expensive relational $O(N \log N)$ table joins.
  - **HNSW Graphs (Vector DBs / Pinecone / pgvector):** Multi-layer graph structures enable sub-50ms Approximate Nearest Neighbor (ANN) search across 1536-dimensional floating-point embeddings."*

### 2. Consistency Guarantees & Real-World CAP Trade-offs
- **Junior:** "NoSQL databases don't support ACID transactions, so you can never use them for real apps."
- **Senior:** *"CAP is not a binary choice made once at the database level—most modern distributed databases allow **tunable consistency per operation**. A senior knows when to choose strong consistency (CP) for financial balances vs. eventual consistency (AP) for social activity feeds. In Cassandra, you tune quorums ($W + R > N$) per query: write with `LOCAL_QUORUM` for low-latency multi-datacenter replication, and read with `LOCAL_QUORUM` to guarantee immediate consistency only when strictly required."*

### 3. Polyglot Persistence Architecture
- **Junior:** "We should migrate our whole stack from PostgreSQL to DynamoDB or MongoDB to be modern."
- **Senior:** *"A senior never replaces a relational source of truth with a single NoSQL database. We architect **polyglot persistence ecosystems**:
  - **PostgreSQL:** System of record for users, financial ledgers, and transactions.
  - **Redis:** Sub-millisecond session store, distributed locks (`SET NX`), and rate limiting counters.
  - **Meilisearch / Elasticsearch:** Inverted index for sub-50ms fuzzy text search.
  - **ClickHouse:** Columnar warehouse for BI metrics and aggregating 1B+ analytics events.
  - **Pinecone / pgvector:** Semantic vector embeddings for Generative AI RAG memory."*

### 4. Operational Cost & Maintenance Reality
- **Junior:** "Let's use a Graph DB, an LSM Store, and an Elasticsearch cluster for our new 5-person project."
- **Senior:** *"A senior factors in **operational maintenance overhead and team cognitive load**. Running distributed Cassandra clusters (tombstone compactions, repair storms), Elasticsearch clusters (JVM garbage collection, shard sizing), or specialized graph databases requires dedicated SRE expertise. A senior defaults to **Postgres + JSONB + pgvector + Redis** until scale and access patterns mathematically demand dedicated distributed engines."*

### 5. Schema Evolution & Integrity Boundaries
- **Junior:** "Document databases are great because there is no schema and you can store anything."
- **Senior:** *"A 'schemaless' database does not mean there is no schema—it means **the burden of schema enforcement shifts from the database engine to application code**. If application validation slips, document databases end up with corrupt, heterogeneous data models and update anomalies. A senior enforces strict schema validation (Zod/Mongoose) at the application layer when using Document stores, and uses PostgreSQL with JSONB when relational integrity is paramount."*
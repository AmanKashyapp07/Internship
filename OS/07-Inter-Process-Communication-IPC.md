# Master Guide 07: Inter-Process Communication (IPC)

> **Focus:** The Master IPC Comparison Matrix, Shared Memory vs Message Passing, Anonymous & Named Pipes (FIFOs), Message Queues, UNIX Domain Sockets & SCM_RIGHTS, Signals & Async-Signal Safety, and Zero-Copy I/O (sendfile/mmap).
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [The Master IPC Comparison Matrix](#1-the-master-ipc-comparison-matrix)
2. [Shared Memory & mmap (Fastest Zero-Copy IPC)](#2-shared-memory--mmap-fastest-zero-copy-ipc)
3. [UNIX Domain Sockets vs. Network Sockets](#3-unix-domain-sockets-vs-network-sockets)
4. [Anonymous Pipes vs. Named Pipes (FIFOs)](#4-anonymous-pipes-vs-named-pipes-fifos)
5. [POSIX & System V Message Queues](#5-posix--system-v-message-queues)
6. [Signals & Async-Signal Safety](#6-signals--async-signal-safety)
7. [Zero-Copy I/O Architecture (sendfile vs read/write)](#7-zero-copy-io-architecture-sendfile-vs-readwrite)
8. [High-Frequency Interview Drill & Verbal Q&A](#8-high-frequency-interview-drill--verbal-qa)

---

# 1. The Master IPC Comparison Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| IPC MECHANISM          | THROUGHPUT / SPEED | KERNEL INVOLVEMENT  | SYNC REQUIRED?   | PRIMARY INTERVIEW USE CASE  |
+--------------------------------------------------------------------------------------------------------------------+
| 1. Shared Memory       | Fastest (GB/s)     | Setup only (mmap)   | YES (Semaphores /| High-Frequency Trading,     |
|    (shm_open / mmap)   | (Direct RAM access)| Zero kernel per byte| Mutex in shared) | Large tensor/video sharing  |
+--------------------------------------------------------------------------------------------------------------------+
| 2. UNIX Domain Sockets | Very High (MB-GB/s)| Kernel socket buffer| Built-in (Kernel | Microservices on same host, |
|    (AF_UNIX / LOCAL)   | (Bypasses TCP/IP)  | (2 context switches)| stream/datagram) | Passing File Descriptors    |
+--------------------------------------------------------------------------------------------------------------------+
| 3. Anonymous Pipes     | High (MB/s)        | Kernel circular pipe| Built-in (Blocks | Shell pipeline (ls | grep), |
|    (pipe syscall)      | (Half-Duplex stream| buffer (usually 64KB| on full/empty)   | Parent-Child process data   |
+--------------------------------------------------------------------------------------------------------------------+
| 4. Named Pipes (FIFOs) | High (MB/s)        | Kernel circular pipe| Built-in (Blocks | Unrelated processes on same |
|    (mkfifo filesystem) | (Half-Duplex stream| buffer              | on open/read)    | machine via filesystem path |
+--------------------------------------------------------------------------------------------------------------------+
| 5. Message Queues      | Medium (MB/s)      | Full kernel queue   | Built-in         | Priority-based discrete     |
|    (POSIX mq_*)        | (Structured packet)| management          | (Message bound)  | packet transmission         |
+--------------------------------------------------------------------------------------------------------------------+
| 6. Signals             | Minimal            | Software Interrupt  | N/A              | Asynchronous notifications  |
|    (kill / sigaction)  | (Carries integer ID| Trap to kernel      |                  | (SIGTERM, SIGKILL, SIGCHLD) |
+--------------------------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** Shared memory is sharing a whiteboard in the same room; pipes are passing notes through a kernel mailbox; sockets are making a phone call.
- **The Interview Trap:** Saying Shared Memory is completely safe out-of-the-box. Shared Memory provides **zero synchronization**; without explicit POSIX semaphores or process-shared mutexes, concurrent writes will corrupt memory instantly.

---

# 2. Shared Memory & mmap (Fastest Zero-Copy IPC)

```
Process A Virtual Memory                                            Process B Virtual Memory
+-----------------------+                                           +-----------------------+
| Virtual Page 0x7FFF00 | -----\                             /----> | Virtual Page 0x600000 |
+-----------------------+       \                           /       +-----------------------+
                                 v                         v
                           +-------------------------------------+
                           | Shared Physical RAM Frame (Zero Copy)|
                           +-------------------------------------+
```

### Setup Lifecycle:
1. `int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);`
2. `ftruncate(fd, SIZE);`
3. `void* ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);`
4. Both processes read/write directly via `ptr` with zero system call overhead per byte transferred.

- **30-Second Verbal Answer:** **"Shared Memory maps the exact same physical RAM frames into the virtual address spaces of multiple processes using `shm_open` and `mmap`. Once mapped, reads and writes occur at hardware memory bus speeds with zero kernel mode switches, but processes must coordinate access using POSIX process-shared semaphores."**

---

# 3. UNIX Domain Sockets vs. Network Sockets

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | UNIX DOMAIN SOCKETS (AF_UNIX)         | NETWORK SOCKETS (AF_INET / TCP)    |
+---------------------------------------------------------------------------------------------------+
| Scope                | Same Host Only                        | Cross-Network / Remote Machines    |
| Protocol Stack       | Bypasses IP, TCP, Checksums, Framing  | Full TCP/IP stack overhead         |
| Performance          | ~2x higher throughput, lower latency  | Subject to network interface queue |
| Special Capability   | Can pass open File Descriptors via    | Only passes serialized data payload|
|                      | `sendmsg()` with `SCM_RIGHTS`         |                                    |
+---------------------------------------------------------------------------------------------------+
```

- **Killer Feature (Passing File Descriptors):** Process A opens a file or database TCP connection and passes the actual open file descriptor over an `AF_UNIX` socket to Process B using `sendmsg()` with `SCM_RIGHTS`. Process B immediately reads from that file descriptor without reopening it.

---

# 4. Anonymous Pipes vs. Named Pipes (FIFOs)

```
Anonymous Pipe (Parent-Child Only):
[ Parent Process ] ---> Write End (fd[1]) ---> [ 64KB Kernel Ring Buffer ] ---> Read End (fd[0]) ---> [ Child Process ]

Named Pipe / FIFO (Any Unrelated Processes via Path /tmp/my_fifo):
[ Process A (Writer) ] ---> (Opens /tmp/my_fifo for Write) ---> [ Kernel Buffer ] ---> (Opens for Read) ---> [ Process B ]
```

### Critical Behavioral Traps:
- **`SIGPIPE` Signal:** Writing to a pipe whose **read end is closed** immediately triggers a `SIGPIPE` signal, terminating the writer process unless caught or ignored (`signal(SIGPIPE, SIG_IGN)`).
- **Blocking on `open()` in FIFOs:** Opening a FIFO for reading blocks until another process opens it for writing (and vice versa), ensuring synchronization at startup.
- **Pipe Capacity:** Typically 64 KB on modern Linux. Writes $\le 4096$ bytes (`PIPE_BUF`) are guaranteed **atomic** (never interleaved with other concurrent writers).

---

# 5. POSIX & System V Message Queues

```
Kernel-Managed Message Queue:
[ Message (Priority=30) ] ---> [ Message (Priority=20) ] ---> [ Message (Priority=5) ]
              ^
              | Highest priority message consumed first, regardless of insertion order!
```

### Key Highlights:
- Messages are **discrete packets** (unlike byte-stream pipes).
- Supports **message priorities**: `mq_receive()` pulls the highest-priority message first.
- POSIX API (`mq_open`, `mq_send`, `mq_receive`, `mq_close`) is preferred over legacy System V (`msgget`, `msgsnd`).

---

# 6. Signals & Async-Signal Safety

```
+---------------------------------------------------------------------------------------------------+
| SIGNAL               | NUMBER | DEFAULT ACTION    | CAN BE CAUGHT / BLOCKED? | COMMON INTERVIEW MEANING   |
+---------------------------------------------------------------------------------------------------+
| SIGHUP               | 1      | Terminate         | YES                      | Terminal hangup / Reload cfg|
| SIGINT               | 2      | Terminate         | YES                      | Ctrl+C from terminal       |
| SIGKILL              | 9      | Uncatchable Kill  | NO (Kernel forces exit)  | Instant uncatchable death  |
| SIGSEGV              | 11     | Core Dump         | YES                      | Invalid memory access      |
| SIGPIPE              | 13     | Terminate         | YES                      | Write to closed pipe/socket|
| SIGTERM              | 15     | Graceful Terminate| YES                      | Polite shutdown request    |
| SIGCHLD              | 17     | Ignore            | YES                      | Child process state change |
+---------------------------------------------------------------------------------------------------+
```

### The Async-Signal Safety Rule (The #1 Signal Trap):
- Signal handlers interrupt execution at arbitrary instruction boundaries.
- **NEVER call non-reentrant functions** inside a signal handler:
  - `malloc()` / `free()` (Can corrupt heap lock if interrupted during another `malloc`).
  - `printf()` / standard I/O (Uses internal locks and static buffers).
- **Safe in handlers:** Only Async-Signal-Safe system calls (e.g. `write()`, `_exit()`, modifying `volatile sig_atomic_t` flags).

---

# 7. Zero-Copy I/O Architecture (sendfile vs read/write)

```
Traditional read() + write() Pipeline (4 Context Switches + 4 Copies):
[ Disk ] --(DMA Copy)--> [ Kernel Page Cache ] --(CPU Copy)--> [ User Buffer ]
                                                                       |
[ Network Socket ] <--(DMA Copy)-- [ Socket Buffer ] <--(CPU Copy)-----+

Zero-Copy sendfile() Pipeline (2 Context Switches + 2 Copies + Zero CPU Copies):
[ Disk ] --(DMA Copy)--> [ Kernel Page Cache ] ------------------------+
                                   | (Copy FD descriptors only)        |
                                   v                                   | (Direct DMA Transfer)
                        [ Socket Buffer (Headers) ]                    v
                                   | -------------------------> [ Network NIC ]
```

- **One-Line Intuition:** Zero-copy transfers data directly from the kernel disk cache to the network card via DMA without wasting CPU cycles bouncing bytes into user-space memory.
- **System Call:** `sendfile(out_fd, in_fd, &offset, count);` (Used internally by Nginx, Kafka, and Netty for massive throughput).

---

# 8. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the fastest IPC mechanism on a single Linux machine and why?
> **Answer:** **Shared Memory (`shm_open` + `mmap`).** Once mapped into the virtual address spaces, data transfer happens via direct CPU RAM instructions without system calls, context switches, or intermediate kernel buffer copies.

### Q2: What happens if a process writes to a pipe whose read end has been closed?
> **Answer:** The write operation fails, the kernel delivers a **`SIGPIPE` signal** to the writing process (which terminates it by default), and the `write()` system call returns `-1` with `errno = EPIPE`.

### Q3: Why is `printf()` forbidden inside a signal handler?
> **Answer:** `printf()` is **non-reentrant and not async-signal-safe**. It acquires an internal mutex lock on `stdout`. If a thread holding that mutex is interrupted by a signal handler that also calls `printf()`, the process **deadlocks with itself**.

### Q4: How does Kafka achieve massive throughput using OS-level features?
> **Answer:** Kafka utilizes the **OS Page Cache** combined with the **`sendfile()` zero-copy system call**, allowing data to be transferred directly from the kernel page cache to the network NIC buffer via DMA without crossing into user-space memory.

### Q5: How can two completely unrelated processes communicate using a pipe?
> **Answer:** By using a **Named Pipe (FIFO)** created with `mkfifo()`. Unlike anonymous pipes which require parent-child inheritance of file descriptors, a FIFO has a presence in the filesystem, allowing any process with proper file permissions to open it by path.

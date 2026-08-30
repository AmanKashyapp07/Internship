# Inter-Process Communication (IPC) & Zero-Copy I/O Theory

> **Scope:** IPC Models (Shared Memory vs. Message Passing), Direct Physical RAM Sharing (`mmap`), UNIX Domain Sockets (`AF_UNIX` & `SCM_RIGHTS`), Anonymous & Named Pipes (FIFOs, `SIGPIPE`), POSIX Message Queues, Asynchronous Signal Handling & Async-Signal Safety, and Zero-Copy I/O Architecture (`sendfile`).

---

# Table of Contents
1. [IPC Paradigms & Mechanism Comparison Matrix](#1-ipc-paradigms--mechanism-comparison-matrix)
2. [Shared Memory Architecture & mmap Mechanics](#2-shared-memory-architecture--mmap-mechanics)
3. [UNIX Domain Sockets vs. Network Sockets](#3-unix-domain-sockets-vs-network-sockets)
4. [Anonymous Pipes vs. Named Pipes (FIFOs)](#4-anonymous-pipes-vs-named-pipes-fifos)
5. [POSIX Message Queues](#5-posix-message-queues)
6. [Asynchronous Signal Architecture & Signal Safety](#6-asynchronous-signal-architecture--signal-safety)
7. [Zero-Copy I/O Architecture (sendfile vs. read/write)](#7-zero-copy-io-architecture-sendfile-vs-readwrite)
8. [Core Theoretical Summary Principles](#8-core-theoretical-summary-principles)

---

# 1. IPC Paradigms & Mechanism Comparison Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| IPC MECHANISM          | DATA TRANSFER RATE | KERNEL INVOLVEMENT  | SYNCHRONIZATION  | PRIMARY APPLICATION DOMAIN  |
+--------------------------------------------------------------------------------------------------------------------+
| Shared Memory          | Hardware Bus Speed | Setup only (mmap)   | External         | High-frequency data streams,|
| (shm_open / mmap)      | (GB/s, Zero-Copy)  | Zero per-byte cost  | (POSIX Semaphores| large tensor arrays         |
+--------------------------------------------------------------------------------------------------------------------+
| UNIX Domain Sockets    | High Throughput    | Kernel socket buffer| Built-in stream/ | Co-located microservices,   |
| (AF_UNIX / LOCAL)      | (Bypasses TCP/IP)  | (Double context sw) | datagram queue   | passing File Descriptors    |
+--------------------------------------------------------------------------------------------------------------------+
| Anonymous Pipes        | Medium (MB/s)      | Kernel ring buffer  | Built-in         | Shell pipelines (stdout/in),|
| (pipe syscall)         | (Half-Duplex stream| (64 KB capacity)    | (Blocks on full) | Parent-child data pipelines |
+--------------------------------------------------------------------------------------------------------------------+
| Named Pipes (FIFOs)    | Medium (MB/s)      | Kernel ring buffer  | Built-in         | Inter-process communication |
| (mkfifo filesystem)    | (Half-Duplex stream|                     | (Blocks on open) | across unrelated processes  |
+---------------------------------------------------------------------------------------------------+
| Message Queues         | Discrete Packets   | Kernel queue        | Built-in message | Priority-based discrete     |
| (POSIX mq_*)           |                    | management          | boundary queues  | message transmission        |
+---------------------------------------------------------------------------------------------------+
| Signals                | Minimal Payload    | Software Interrupt  | Kernel state     | Asynchronous control events |
| (sigaction)            | (Integer identifier| Trap to kernel      | notifications    | (SIGINT, SIGTERM, SIGCHLD)  |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Shared Memory Architecture & mmap Mechanics

Shared memory allows multiple independent processes to map the exact same physical RAM frames into their distinct virtual address spaces.

```
Process A Virtual Address Space                                     Process B Virtual Address Space
+-----------------------+                                           +-----------------------+
| Virtual Page 0x7FFF00 | -----\                             /----> | Virtual Page 0x600000 |
+-----------------------+       \                           /       +-----------------------+
                                 v                         v
                           +-------------------------------------+
                           | Shared Physical RAM Frame (Zero-Copy)|
                           +-------------------------------------+
```

### Lifecycle Execution:
1. `shm_open()` allocates an in-memory POSIX shared memory object file descriptor.
2. `ftruncate()` sets the physical byte capacity.
3. `mmap()` configures the calling process's page table entries to point to the allocated physical frames under `MAP_SHARED`.
4. **Data Transfer Characteristics:** Transfers occur via direct CPU memory bus loads and stores without invoking system calls or intermediate kernel buffer copying.
5. **Synchronization Requirement:** Shared memory provides zero concurrency control. Processes must synchronize access using POSIX process-shared semaphores (`PTHREAD_PROCESS_SHARED`) or spinlocks.

---

# 3. UNIX Domain Sockets vs. Network Sockets

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | UNIX DOMAIN SOCKETS (AF_UNIX)         | NETWORK SOCKETS (AF_INET / TCP)    |
+---------------------------------------------------------------------------------------------------+
| Scope                | Single Operating System Host          | Cross-Network Distributed Hosts    |
| Protocol Stack       | Bypasses IP, TCP, Checksums, Framing  | Full TCP/IP networking stack       |
| Performance          | ~2x higher throughput, lower latency  | Subject to network interface queues|
| File Descriptor Pass | Supported via `sendmsg()` SCM_RIGHTS  | Not supported (Data payloads only) |
+---------------------------------------------------------------------------------------------------+
```

### File Descriptor Transfer (`SCM_RIGHTS`):
- Process $A$ opens a resource (e.g. file, TCP socket) and transmits the file descriptor over an `AF_UNIX` socket using `sendmsg()` with an `SCM_RIGHTS` control message.
- The kernel duplicates the underlying file description in the global system open-file table and assigns a valid new file descriptor integer in Process $B$'s private file descriptor table.

---

# 4. Anonymous Pipes vs. Named Pipes (FIFOs)

```
Anonymous Pipe (Parent-Child Process Hierarchy):
[ Parent Process ] ---> Write Descriptor (fd[1]) ---> [ 64KB Kernel Ring Buffer ] ---> Read Descriptor (fd[0]) ---> [ Child Process ]

Named Pipe / FIFO (Filesystem Node: /tmp/my_fifo):
[ Process A (Writer) ] ---> (Opens /tmp/my_fifo for Write) ---> [ Kernel Buffer ] ---> (Opens for Read) ---> [ Process B (Reader) ]
```

### Behavioral Semantics:
1. **Unidirectional Half-Duplex Stream:** Anonymous pipes provide a unidirectional byte stream. Bi-directional communication requires allocating two independent pipes.
2. **`SIGPIPE` Signal:** Writing to a pipe whose read descriptor is closed generates a `SIGPIPE` signal to the writer, terminating the process unless handled or ignored (`SIG_IGN`), and returning `-1` with `errno = EPIPE`.
3. **Atomic Writes (`PIPE_BUF`):** Write operations $\le 4096$ bytes (`PIPE_BUF` on Linux) are guaranteed atomic; writes exceeding `PIPE_BUF` may have their bytes interleaved with concurrent writers.

---

# 5. POSIX Message Queues

POSIX Message Queues (`mq_open`, `mq_send`, `mq_receive`) provide discrete packet transmission over kernel-managed queues.

```
Kernel-Managed Message Priority Queue:
[ Message (Priority=30) ] ---> [ Message (Priority=20) ] ---> [ Message (Priority=5) ]
              ^
              | Highest priority message consumed first, regardless of FIFO insertion order
```

### Key Properties:
- **Message Boundaries:** Preserves record boundaries; a `receive` call extracts an entire discrete message rather than an arbitrary byte stream.
- **Priority Ordering:** Messages are enqueued and dequeued based on an integer priority value rather than pure FIFO arrival order.

---

# 6. Asynchronous Signal Architecture & Signal Safety

Signals represent software interrupts delivered asynchronously by the kernel to a process.

```
+---------------------------------------------------------------------------------------------------+
| SIGNAL               | NUMBER | DEFAULT DISPOSITION| CATCHABLE / BLOCKABLE? | KERNEL SEMANTIC     |
+---------------------------------------------------------------------------------------------------+
| SIGHUP               | 1      | Process Termination| YES                    | Controlling terminal|
| SIGINT               | 2      | Process Termination| YES                    | Keyboard interrupt  |
| SIGKILL              | 9      | Forced Termination | NO (Kernel-enforced)   | Uncatchable kill    |
| SIGSEGV              | 11     | Core Dump          | YES                    | Invalid memory read |
| SIGPIPE              | 13     | Process Termination| YES                    | Write to broken pipe|
| SIGTERM              | 15     | Graceful Shutdown  | YES                    | Termination request |
| SIGCHLD              | 17     | Ignore             | YES                    | Child state change  |
+---------------------------------------------------------------------------------------------------+
```

### Async-Signal Safety Invariant:
Signal handlers interrupt normal program execution at arbitrary CPU instruction boundaries.
- **Non-Reentrant Functions Forbidden:** Functions acquiring internal locks or utilizing static state (e.g. `malloc()`, `free()`, `printf()`) are **not async-signal-safe**. If an interrupted thread holds an internal heap lock and the signal handler invokes `malloc()`, a recursive self-deadlock occurs.
- **Safe Operations:** Reentrant system calls (e.g. `write()`, `_exit()`) or modifying atomic variables typed `volatile sig_atomic_t`.

---

# 7. Zero-Copy I/O Architecture (sendfile vs. read/write)

```
Traditional read() + write() Data Path (4 Context Switches + 4 Buffer Copies):
[ Disk Storage ] --(DMA)--> [ Kernel Page Cache ] --(CPU Copy)--> [ User Space Buffer ]
                                                                           |
[ Network NIC ]  <--(DMA)-- [ Socket Buffer ]     <--(CPU Copy)------------+

Zero-Copy sendfile() Data Path (2 Context Switches + 2 DMA Copies + ZERO CPU Copies):
[ Disk Storage ] --(DMA)--> [ Kernel Page Cache ] -------------------------+
                                     | (Pass FD descriptors only)          |
                                     v                                     | (Direct DMA Transfer)
                          [ Socket Buffer (Headers) ]                      v
                                     | --------------------------> [ Network NIC ]
```

### System Call Execution:
```c
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```
- Data transfers directly from the kernel disk page cache to the network interface buffer using Direct Memory Access (DMA), eliminating CPU memory copies and reducing context switch overhead by 50%.

---

# 8. Core Theoretical Summary Principles

1. **Shared Memory Throughput:** Achieves optimal IPC bandwidth by eliminating system call and buffer copy overhead, requiring process-shared synchronization primitives.
2. **UNIX Domain Socket Optimization:** Bypasses network protocol encapsulation and checksum calculation, supporting file descriptor passing via `SCM_RIGHTS`.
3. **Pipe Atomicity:** Byte writes below `PIPE_BUF` (4096 bytes) execute atomically; closing read ends triggers `SIGPIPE` upon subsequent write attempts.
4. **Async-Signal Safety:** Signal handlers must strictly avoid non-reentrant runtime functions (`malloc`, `printf`) to prevent recursive self-deadlocks.
5. **Zero-Copy Mechanics:** `sendfile` leverages DMA engine transfers between the page cache and network buffers, eliminating user-space memory copying.

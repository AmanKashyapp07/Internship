# Docker Engine API & Container Sandboxes: Architecture Guide

> **Core Concept:** Docker is an OS-level virtualization platform that packages applications and their complete dependencies into isolated, lightweight **Containers** sharing the host Linux kernel. The **Docker Engine API** is a RESTful HTTP/Unix-socket API that allows backend programs to programmatically create, start, inspect, stream logs from, and destroy container sandboxes on the fly without using the shell CLI.

---

## 1. What It Is in Plain English

Virtual Machines (like VMware or VirtualBox) are heavy: each VM boots an entire independent guest operating system (consuming 1–4GB RAM and taking 30–60 seconds to start).

Docker containers are **isolated Linux processes** running directly on the host kernel using kernel primitives (**Namespaces** for process/network isolation and **Control Groups (cgroups)** for CPU/RAM resource limits). Spawning a Docker container takes **under 300 milliseconds** and uses only the memory the app actually needs.

The **Docker Engine API** (exposed over `/var/run/docker.sock`) lets you control Docker using standard HTTP calls from Node.js (via `dockerode` or raw HTTP):
```ts
const container = await docker.createContainer({
    Image: 'node:18-alpine',
    Cmd: ['/bin/sh'],
    Tty: true,
    HostConfig: { Memory: 512 * 1024 * 1024 } // 512MB limit
});
await container.start();
```

---

## 2. Docker Architecture & Container Lifecycle

```
[ Your Backend Orchestrator (Node.js) ]
               |
               v (Unix Socket: /var/run/docker.sock via Docker Engine API)
+-----------------------------------------------------------------------------------+
|                              DOCKER DAEMON (dockerd)                              |
+-----------------------------------------------------------------------------------+
               |
               v (gRPC)
+------------------------------------+
|             containerd             | (Container runtime supervisor)
+------------------------------------+
               |
               v
+------------------------------------+
|               runc                 | (Low-level OCI container runtime)
+------------------------------------+
               |
               v (Invokes Linux Kernel Primitives)
+-----------------------------------------------------------------------------------+
|                                LINUX HOST KERNEL                                  |
|                                                                                   |
|  [ NAMESPACES (Isolation) ]                   [ CGROUPS v2 (Resource Limits) ]    |
|  - pid (Isolated process tree)                - cpu.max (Throttles CPU cycles)    |
|  - net (Dedicated virtual eth/IP)             - memory.max (Hard RAM limit/OOM)   |
|  - mnt (Isolated OverlayFS rootfs)            - pids.max (Prevents fork bombs)    |
+-----------------------------------------------------------------------------------+
```

---

## 3. How I Used It (NexusIDE & MagnusCI)

- **NexusIDE (Pre-Warming Daemon & Pool Manager):**
  - Built a predictive container pool manager via the Docker Engine API. Instead of forcing a user to wait 3–5 seconds for a cold container to pull and initialize when clicking "Open Workspace", a background daemon maintains a warm pool of pre-initialized, paused Docker sandboxes, slashing workspace launch latency to **< 200ms**.
  - Attached bidirectional raw stream sockets to container TTYs via Docker API `/containers/{id}/attach` with hijacked HTTP TCP streams.
- **MagnusCI (Ephemeral Build Sandboxes):**
  - Spawned disposable, isolated Docker containers for every GitHub webhook build run.
  - Used volume bind-mounts and automated container auto-removal (`HostConfig.AutoRemove: true` / `docker.prune()`) to execute multi-stage build pipelines with **zero host disk pollution**.

---

## 4. Analogy for Live Interviews

> *"A Virtual Machine is like building a completely separate physical house with its own foundation, plumbing, and power generator (heavy and expensive). A Docker Container is like renting a room in a modern apartment building. All rooms share the main building's water pipes and electrical grid (the Host Linux Kernel), but each room has its own lock on the door and private key (Namespaces), and the building landlord limits how much electricity your room can use (cgroups)."*

---

## 5. Docker Containers vs. Virtual Machines (VMs)

| Dimension | Docker Containers | Virtual Machines (VMs) | Firecracker MicroVMs (AWS Lambda) |
| :--- | :--- | :--- | :--- |
| **Isolation Level** | OS-level (Shared Host Kernel) | Hardware-level (Hypervisor / Guest OS) | Minimal Hardware Virtualization |
| **Startup Time** | **Milliseconds (< 300ms)** | Minutes (30s – 2 mins) | **Milliseconds (~5ms)** |
| **Memory Overhead** | **Near zero (MBs)** | High (GBs per guest OS) | Minimal (5MB per microVM) |
| **Security Boundary** | Shared kernel vulnerabilities (requires seccomp) | Hard hypervisor boundary | Strong hypervisor boundary |

---

## 6. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the difference between an Image and a Container?
> **Answer:**
> - **Image:** A read-only, immutable template consisting of stacked **OverlayFS filesystem layers** (defined by a Dockerfile) identified by cryptographic SHA-256 digests.
> - **Container:** A runnable, isolated stateful instance of an image. Docker adds a thin, writable **Container Layer (Copy-On-Write)** on top of the immutable image layers and assigns process namespaces and cgroups.

### Q2: What are Linux Namespaces and what are the 6 core namespaces Docker uses?
> **Answer:** Namespaces provide process virtualization by creating isolated views of global system resources:
> 1. `pid`: Isolates process IDs (the container process sees itself as PID 1).
> 2. `net`: Isolates network interfaces, IP addresses, routing tables, and ports.
> 3. `mnt`: Isolates filesystem mount points (OverlayFS rootfs).
> 4. `ipc`: Isolates Inter-Process Communication (shared memory, semaphores).
> 5. `uts`: Isolates hostname and domain name.
> 6. `user`: Maps container root user (UID 0) to an unprivileged user on the host.

### Q3: How does the OverlayFS Copy-on-Write (CoW) mechanism work in Docker?
> **Answer:** Docker uses **OverlayFS**, which merges multiple read-only image layers (`lowerdir`) with a single top writable layer (`upperdir`) into a unified view (`merged`).
> - When a container reads a file, it reads from the lower read-only image layers.
> - When a container modifies a file, OverlayFS copies the file up to the top writable layer (**Copy-on-Write**) and modifies it there, leaving the underlying base image completely untouched.

### Q4: Why is running Docker-in-Docker (DinD) dangerous, and what is the alternative?
> **Answer:** Docker-in-Docker requires running the outer container in `--privileged` mode, granting it full root access to the host kernel and devices, which completely breaks container security isolation.
> - *Safer Alternative:* **Docker-out-of-Docker (DooD)**: Bind-mount the host's Docker socket (`-v /var/run/docker.sock:/var/run/docker.sock`) into the container, allowing the container to instruct the host daemon to spawn sibling containers instead.

### Q5: What is the difference between `CMD` and `ENTRYPOINT` in a Dockerfile?
> **Answer:**
> - `ENTRYPOINT`: Sets the primary executable command that always runs when the container starts (e.g. `ENTRYPOINT ["python", "app.py"]`).
> - `CMD`: Provides default arguments to the `ENTRYPOINT` (e.g. `CMD ["--port", "8080"]`). Arguments passed in `docker run my_image --port 3000` override `CMD`, but preserve `ENTRYPOINT`.

---

## 7. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why does PID 1 inside a Docker container fail to handle SIGTERM on `docker stop`?"
- **The Trap:** Writing a simple Node.js app as `CMD ["node", "app.js"]` and wondering why `docker stop` hangs for 10 seconds before forcefully killing it with `SIGKILL`.
- **The Reality:** In Linux, PID 1 has special init process responsibilities: it does **not** get default signal handlers assigned by the kernel. Standard Node.js does not handle `SIGTERM` by default.
- **The Fix:** Explicitly handle signals in code (`process.on('SIGTERM', () => server.close())`) or use an init wrapper like **`tini`** (`ENTRYPOINT ["/sbin/tini", "--", "node", "app.js"]`).

### Gotcha 2: "What causes 'Host Disk Pollution' in CI/CD platforms and how do you prevent it?"
- **The Answer:** If containers, dangling anonymous volumes (`-v /tmp`), or stopped build containers are not cleaned up after pipeline execution, host disks run out of space (`No space left on device`).
- **The Fix:** Always pass `HostConfig.AutoRemove: true` on container creation, use temporary `tmpfs` mounts in RAM for throwaway build steps, and run scheduled background pruning (`docker system prune -af --volumes`).

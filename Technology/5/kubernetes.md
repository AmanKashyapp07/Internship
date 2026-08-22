# Kubernetes (K3s) & Container Orchestration: Architecture Guide

> **Core Concept:** Kubernetes (K8s) is an open-source container orchestration platform that automates the deployment, scaling, networking, and self-healing of containerized applications across a cluster of nodes. **K3s** is a lightweight, fully compliant, production-grade Kubernetes distribution packaged into a single binary (< 100MB) by Rancher, replacing heavy etcd with SQLite/embedded storage and optimizing memory footprint for edge, CI/CD, and single-to-multi-node servers.

---

## 1. What It Is in Plain English

Docker manages a single container on a single computer. But if you have 20 microservices running across 5 servers, and one server loses power or a container crashes at 3:00 AM, Docker alone will not detect the dead node, re-route traffic, or restart the container on a healthy machine.

Kubernetes is the automated cluster manager. You declare your desired state in a YAML file (*"I want exactly 3 replicas of the MagnusCI worker running with 1GB RAM"*). Kubernetes constantly monitors the cluster (**Reconciliation Loop**), and if a node crashes, it automatically schedules and starts replacements on another node within seconds.

---

## 2. Kubernetes Architecture: Control Plane vs. Worker Nodes

```
+-----------------------------------------------------------------------------------+
|                           KUBERNETES CONTROL PLANE (Master)                       |
|                                                                                   |
|  [ API SERVER (kube-apiserver) ] <--- Single gateway for kubectl & internal calls |
|              |                                                                    |
|  +-----------+-----------+----------------------+--------------------+            |
|  |                       |                      |                    |            |
|  v                       v                      v                    v            |
| [ etcd / K3s SQLite ] [ SCHEDULER ]       [ CONTROLLER MGR ]   [ CLOUD CONTROLLER]|
| (Cluster state store) (Assigns Pod->Node) (Maintains state)                       |
+-----------------------------------------------------------------------------------+
                                         |
                                         | Manages via gRPC / TLS
                                         v
+-----------------------------------------------------------------------------------+
|                             WORKER NODE (K3s Agent)                               |
|                                                                                   |
|  [ kubelet ]             (Node agent that communicates with API Server)           |
|  [ kube-proxy ]          (Maintains iptables/IPVS network routing rules)          |
|  [ containerd ]          (Container runtime executing OCI containers)             |
|                                                                                   |
|  +-----------------------------------------------------------------------------+  |
|  |                             POD INSTANCES                                   |  |
|  |  [ Pod: Worker-1 ] (Container: Node Worker + Secret Volume Mount)           |  |
|  |  [ Pod: Redis ]    (Container: Redis Cache + PersistentVolumeClaim)         |  |
|  +-----------------------------------------------------------------------------+  |
+-----------------------------------------------------------------------------------+
```

---

## 3. How I Used It (MagnusCI)

- **MagnusCI Deployment & Cluster Architecture:**
  - Deployed MagnusCI microservices (API Server, Redis Queue, Ingress Nginx, and Task Workers) onto a **K3s Kubernetes cluster**.
  - Used **Kubernetes Deployments** with rolling update strategies and liveness/readiness probes to ensure zero-downtime deployments.
  - Configured **ClusterIP Services** and an **Ingress Controller** to route external traffic to internal stateless API pods while keeping Redis and Postgres securely unexposed to the public internet.

---

## 4. Analogy for Live Interviews

> *"Docker is like an individual musician playing an instrument (violin, drums). Kubernetes is the orchestra conductor. The conductor doesn't play the instruments directly, but reads the musical score (your declarative YAML manifest), tells each musician when to play, makes sure the tempo is maintained (load balancing), and if the first violinist faints on stage, immediately gestures to the backup violinist to step in without stopping the symphony (self-healing failover)."*

---

## 5. Standard Kubernetes (K8s) vs. Lightweight K3s

| Dimension | Standard Kubernetes (K8s) | Lightweight K3s (Rancher) |
| :--- | :--- | :--- |
| **Binary Size** | Multiple heavy binaries (~1GB+) | **Single standalone binary (< 100MB)** |
| **Cluster State Store** | High-overhead distributed `etcd` | **SQLite (Single-node) or embedded etcd (HA)** |
| **RAM Footprint** | ~1GB–2GB per control plane node | **~500MB total control plane RAM** |
| **Target Environment** | Massive multi-datacenter clouds (AWS EKS, GKE) | **Edge, IoT, CI/CD runners, Single-node VPS (Azure/Hetzner)**|
| **Conformance** | Official CNCF standard | 100% Certified CNCF Kubernetes compliant |

---

## 6. Core Kubernetes Primitives & Resource Tree

- **Pod:** The smallest deployable unit in K8s (one or more tightly coupled containers sharing the same network namespace/IP and storage volumes).
- **Deployment:** Manages declarative state for Pods, handling automated rolling updates, rollbacks, and scale replicas (`replicas: 3`).
- **Service:** An immutable internal load balancer and DNS entry that routes traffic to dynamic, ephemeral Pod IPs matching a label selector:
  - `ClusterIP` (Default): Internal cluster-only IP address.
  - `NodePort`: Exposes service on a static high port (`30000-32767`) on every node's physical IP.
  - `LoadBalancer`: Provisions an external cloud load balancer (AWS NLB / Azure LB).
- **Ingress:** HTTP/HTTPS reverse proxy router (Layer 7) routing domain paths (e.g. `api.domain.com/v1`) to backend Services.

---

## 7. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the difference between a Liveness Probe and a Readiness Probe?
> **Answer:**
> - **Liveness Probe:** Checks if the container is still alive/healthy. If the probe fails (e.g. app is in a deadlock loop), Kubernetes **restarts the container**.
> - **Readiness Probe:** Checks if the container is ready to accept user network traffic (e.g. finished warming caches and connecting to Postgres). If it fails, Kubernetes **removes the Pod's IP from the Service load balancer**, but does not restart it.

### Q2: What is the Kubernetes Reconciliation Loop (Control Loop)?
> **Answer:** The fundamental operating principle of Kubernetes:
> $$\text{Desired State (YAML)} \quad \longleftrightarrow \quad \text{Current State (Cluster Reality)}$$
> The Controller Manager continuously runs an infinite loop:
> 1. Observe the current state of the cluster.
> 2. Compare it with the desired state specified in the etcd/K3s manifest.
> 3. Execute actions to reconcile any differences (e.g. if desired = 3 pods, but current = 2 pods, it schedules 1 new Pod).

### Q3: How do Pods communicate with each other inside a cluster?
> **Answer:** Kubernetes enforces a flat network model where **every Pod gets its own unique IP address** within the cluster CIDR block. Pods can communicate with every other Pod directly without NAT. Services provide a stable, internal cluster DNS name (`http://redis-service.default.svc.cluster.local:6379`) that load-balances traffic across matching healthy Pod IPs.

### Q4: What is a ConfigMap vs. a Secret?
> **Answer:**
> - **ConfigMap:** Stores non-confidential configuration data as key-value pairs or configuration files mounted into pods (e.g. `PORT: "8080"`, `LOG_LEVEL: "debug"`).
> - **Secret:** Stores sensitive credentials (API keys, passwords, TLS certificates). Encoded in Base64 (and encrypted at rest in etcd), and mounted into pods as temporary in-memory `tmpfs` files to prevent writing passwords to disk.

---

## 8. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "Why shouldn't you run stateful databases in basic Kubernetes Pods without StatefulSets?"
- **The Danger:** Standard Deployments treat Pods as completely stateless, interchangeable cattle. If a Postgres Pod crashes, the Deployment creates a new Pod with a new random hostname and a new IP.
- **The Fix:** Stateful databases require **StatefulSets**:
  1. Predictable, stable network identities (`postgres-0`, `postgres-1`).
  2. Dedicated, persistent volume bindings (`PersistentVolumeClaim`) that re-attach to the exact same storage volume upon pod restart.
  3. Ordered, sequential deployment and graceful termination.

# Azure VMs & Cloud Infrastructure Architecture: Interview Guide

> **Core Concept:** Microsoft Azure Virtual Machines (VMs) provide on-demand, scalable Infrastructure-as-a-Service (IaaS) computing instances running Linux or Windows. Deploying on Azure VMs involves configuring **Virtual Networks (VNet)**, **Network Security Groups (NSGs / Virtual Firewalls)**, **Managed OS/Data Disks (Premium SSDs)**, and SSH cryptographic authentication.

---

## 1. What It Is in Plain English

Instead of buying a physical rack-mounted server and keeping it in your house with a backup power generator, Azure gives you access to enterprise hardware in Microsoft datacenters across the globe.

In under 60 seconds, you can provision a Linux Ubuntu VM with 4 vCPUs and 16GB RAM:
1. Azure attaches a virtual network card with a **Public IP** (for public internet traffic) and a **Private IP** (for internal database communication).
2. A **Network Security Group (NSG)** blocks all unauthorized ports by default, allowing only SSH (port 22) and HTTPS (port 443).
3. You SSH into the server using your private cryptographic key (`ssh -i ~/.ssh/id_rsa azureuser@20.x.x.x`) and deploy your Docker engines, K3s clusters, and Node.js backend services.

---

## 2. Azure VM Infrastructure & Security Topology

```
+-----------------------------------------------------------------------------------+
|                           AZURE CLOUD REGION (e.g. East US)                       |
|                                                                                   |
|  [ VIRTUAL NETWORK (VNet: 10.0.0.0/16) ]                                          |
|                                                                                   |
|  +-----------------------------------------------------------------------------+  |
|  |                     PUBLIC SUBNET (10.0.1.0/24)                             |  |
|  |                                                                             |  |
|  |   [ NETWORK SECURITY GROUP (NSG) ]                                          |  |
|  |   - Allow Inbound Port 22 (SSH - Restricted to your IP)                     |  |
|  |   - Allow Inbound Port 80, 443 (HTTP/HTTPS - Public)                        |  |
|  |   - Deny ALL other inbound ports by default!                                |  |
|  |                         |                                                   |  |
|  |                         v                                                   |  |
|  |   [ AZURE LINUX VM (Ubuntu 22.04 LTS: Standard_D4s_v5) ]                    |  |
|  |   - Public Static IP: 20.198.x.x <---> Private IP: 10.0.1.4                 |  |
|  |   - OS Disk: 64GB Premium SSD (Managed Disk: 5,000 IOPS)                    |  |
|  |   - Runs: Nginx (Port 443) -> K3s Cluster -> Docker Engine -> Node API     |  |
|  +-----------------------------------------------------------------------------+  |
+-----------------------------------------------------------------------------------+
```

---

## 3. How I Used It (NexusIDE & MagnusCI)

- **NexusIDE & MagnusCI Production Cloud Hosting:**
  - Provisioned and configured Azure Linux VMs (Ubuntu 22.04 LTS) using SSH public key authentication (disabling vulnerable password authentication entirely in `sshd_config`).
  - Configured **Azure Network Security Groups (NSGs)** to strictly expose only Ports 80 (HTTP) and 443 (HTTPS) to the public internet, keeping Docker daemon Unix sockets (`/var/run/docker.sock`), Redis (`6379`), and PostgreSQL (`5432`) completely isolated on localhost and private VNet subnets.
  - Attached **Azure Premium SSD Managed Disks** to ensure high read/write IOPS and low disk queue latencies when Docker build containers performed heavy multi-stage compilations and CAS chunk hashing.

---

## 4. Analogy for Live Interviews

> *"A PaaS platform (like Vercel or Heroku) is like staying in a fully furnished hotel room: convenient and clean, but you cannot change the furniture, install your own appliances, or modify the electrical wiring (cannot run custom Docker engines or kernel cgroups). An Azure VM is like renting an empty commercial building: you get the physical walls, electricity, and water pipes (CPU, RAM, Disk), and you have full root permissions to build whatever factory, server racks, or custom operating system configurations you require."*

---

## 5. Azure VMs vs. AWS EC2 vs. GCP Compute Engine

| Dimension | Azure Virtual Machines | AWS EC2 | Google Compute Engine (GCE) |
| :--- | :--- | :--- | :--- |
| **Virtual Firewall** | **Network Security Group (NSG)** | Security Group (SG) | VPC Firewall Rules |
| **Virtual Network** | **Virtual Network (VNet)** | Virtual Private Cloud (VPC) | Virtual Private Cloud (VPC) |
| **Burstable Tier** | **B-Series (e.g. Standard_B2s)** | T3 / T4g Instances | E2-micro / E2-small |
| **Compute Optimized** | **F-Series / D-Series** | C-Series / M-Series | C2 / N2 Series |
| **Storage Solution** | Azure Managed Disks (Premium SSD) | EBS Volumes (gp3 / io2) | Persistent Disks (pd-ssd) |

---

## 6. 5–8 High-Yield Interview Questions & Direct Answers

### Q1: What is the difference between Azure B-Series (Burstable) and D-Series (General Compute) VM SKUs?
> **Answer:**
> - **B-Series (Burstable: e.g. `Standard_B2s`):** Cost-effective VMs for low baseline CPU workloads (e.g. dev servers, small staging apps). When the VM uses less than its baseline CPU, it accumulates **CPU Credits**. When a traffic spike hits, it bursts up to 100% CPU until credits are depleted.
> - **D-Series (General Purpose: e.g. `Standard_D4s_v5`):** Delivers **100% sustained, dedicated vCPU performance** at all times with high memory-to-vCPU ratios. Ideal for production CI/CD build engines (MagnusCI) and container hosts where sustained compilation CPU cannot be throttled.

### Q2: What is a Network Security Group (NSG) and how does rule priority work?
> **Answer:** An NSG is a stateful Layer 4 virtual firewall that filters network traffic to Azure resources.
> - Rules are evaluated in order of **Priority (100 to 4096)**, where **lower numbers take precedence over higher numbers**.
> - The moment a packet matches a rule (e.g. Rule Priority 100: `Allow Port 443`), evaluation stops and the packet is allowed; subsequent rules are ignored.

### Q3: What is the difference between an Ephemeral OS Disk and an Azure Managed Disk?
> **Answer:**
> - **Managed Disk (Standard/Premium SSD):** Stored on remote Azure Storage clusters. Data is durable, backed up, and survives VM shutdowns/deallocations, but has network latency overhead.
> - **Ephemeral OS Disk:** Created directly on the physical host server's local NVMe SSD cache. It delivers ultra-fast read/write IOPS with zero remote storage latency, but **all data is wiped if the VM is deallocated or moved to a different host machine** (ideal for stateless CI runners and batch worker nodes).

---

## 7. Common "Gotcha" Questions Interviewers Ask

### Gotcha 1: "What is the difference between stopping a VM from inside the OS (`sudo shutdown -h now`) vs. 'Stopping (Deallocated)' via the Azure Portal?"
- **The Financial Trap:**
  - Running `sudo shutdown` inside the VM shuts down the guest OS, but the VM status in Azure remains **`Stopped (Allocated)`**. Azure **continues to bill you 100% of the compute costs** because the physical hardware CPU and RAM remain reserved for your instance!
  - You must click "Stop" in the Azure Portal or run `az vm deallocate`, changing the status to **`Stopped (Deallocated)`**, which releases the physical hardware reservation and halts all compute billing.

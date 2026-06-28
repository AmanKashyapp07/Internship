# Phase 2: IP Addressing & Routing 🗺️

This report covers IP networks, address space partitioning, hardware resolution protocols, and routing systems.

---

## 1. IPv4 vs. IPv6

| Feature | IPv4 | IPv6 |
| :--- | :--- | :--- |
| **Address Size** | 32 bits ($2^{32} \approx 4.3$ billion addresses) | 128 bits ($2^{128} \approx 3.4 \times 10^{38}$ addresses) |
| **Format** | Dotted Decimal (e.g., `192.168.1.1`) | Hexadecimal Colon-Separated (e.g., `2001:db8::ff00:42`) |
| **Header Size** | Variable (20 to 60 bytes, depending on options) | Fixed (40 bytes) |
| **Fragmentation** | Performed by source hosts and intermediate routers | Performed strictly by source hosts (using PMTUD) |
| **Built-in Security**| IPsec is optional | IPsec is mandatory in the protocol standard |
| **Autoconfiguration**| Requires DHCP for dynamic assignment | Supports SLAAC (Stateless Address Autoconfiguration) |

### Key IPv6 Header Improvements
* **Fixed Size (40 bytes):** Simplifies router processing hardware.
* **Removed Checksum:** Relies on Layer 2 and Layer 4 check validations, eliminating redundant checksum recalculation overhead at every router hop.
* **No Broadcasts:** Broadcasts are replaced by specialized multicast configurations, reducing network-wide interrupt overhead.

---

## 2. Subnetting & CIDR (Classless Inter-Domain Routing)

Subnetting divides a large physical network into smaller, logical sub-networks.

### CIDR Notation
In CIDR, an IP address is accompanied by a slash followed by a number (the prefix length), indicating how many bits represent the network portion.
* **Example:** `192.168.10.0/24`
  * Network bits = 24. Host bits = $32 - 24 = 8$ bits.
  * Subnet Mask: `255.255.255.0` (24 ones: `11111111.11111111.11111111.00000000`).
  * Total IPs: $2^8 = 256$.
  * Usable IPs: $2^8 - 2 = 254$ (subtracting network ID `192.168.10.0` and broadcast address `192.168.10.255`).

### Practical Subnetting Calculation
If a system design requires 4 subnets from `10.0.0.0/24`:
* To get 4 subnets, we borrow $\log_2(4) = 2$ bits from the host portion.
* The new prefix becomes `/26` ($24 + 2$).
* Host bits remaining = $32 - 26 = 6$ bits. Each subnet gets $2^6 = 64$ IPs ($62$ usable).
* Subnets generated:
  1. `10.0.0.0/26` (Range: `.0` to `.63`)
  2. `10.0.0.64/26` (Range: `.64` to `.127`)
  3. `10.0.0.128/26` (Range: `.128` to `.191`)
  4. `10.0.0.192/26` (Range: `.192` to `.255`)

---

## 3. ARP (Address Resolution Protocol) & RARP

To send a frame on a local wire (Ethernet), a device must encapsulate the IP packet inside a frame containing the destination MAC address.

### ARP (IP $\rightarrow$ MAC)
* **How it works:**
  1. If host A wants to send data to host B (on the same subnet) but lacks B's MAC address, it broadcasts an **ARP Request** containing B's IP address.
  2. Every host on the subnet receives the broadcast frame, but only host B unicasts back an **ARP Reply** containing its MAC.
  3. Host A stores B's IP-to-MAC mapping in its local **ARP Cache** (with a timeout) to avoid repeating broadcasts.

### RARP (MAC $\rightarrow$ IP)
* **Purpose:** Reverse ARP allows diskless workstations (legacy devices) that only know their physical MAC address to query a RARP server to obtain their assigned IP address.
* *Note: RARP is obsolete and has been replaced by DHCP.*

---

## 4. Routing Basics

Routing is the process of selecting paths in a network to send packets along.

### Static vs. Dynamic Routing
* **Static Routing:** Paths are configured manually by network administrators. Extremely low overhead, but fails to recover if a link goes down.
* **Dynamic Routing:** Routers use routing protocols to exchange topology changes and calculate paths dynamically.

### Dynamic Routing Protocols
1. **Distance-Vector (e.g., RIP):** Routers periodically advertise their entire routing tables to neighbors. Path metrics are determined by hop count.
2. **Link-State (e.g., OSPF):** Routers flood local link status changes. Every router builds a complete network graph and runs Dijkstra's algorithm to compute the shortest paths.
3. **Path-Vector (e.g., BGP):** Used to route traffic between Autonomous Systems (AS) on the global Internet.

---

## 5. NAT (Network Address Translation)

NAT maps private local IP addresses to a single public IP address at the perimeter router, slowing IPv4 exhaustion.

```
Local Private Subnet                  NAT Router                      Public Internet
[Host 192.168.1.10] ──► (Port 5000) ──► Maps to: Public IP: 203.0.113.1 ──► [Target Web Server]
                                         (NAT Table entry:
                                          192.168.1.10:5000 <-> 203.0.113.1:8001)
```

### Types of NAT
* **Static NAT:** Maps one private IP to one public IP (1-to-1 mapping).
* **Dynamic NAT:** Maps private IPs to a pool of public IPs.
* **PAT (Port Address Translation / NAT Overload):** Maps thousands of private IPs to a single public IP by modifying the source port of outbound TCP/UDP segments. The router maintains a mapping table of `Private IP + Source Port` $\leftrightarrow$ `Public IP + Translated Port` to demultiplex returning response packets.

# Computer Networks in C

A structured collection of **C programs**, networking concepts, and socket programming implementations built while learning **Computer Networks**. This repository serves as both a learning archive and a practical reference, covering everything from low-level data representation to TCP/UDP socket communication and networking algorithms.

---

## Repository Overview

This repository contains practical implementations and experiments related to computer networking, including:

* **Data Representation & Endianness**

  * Bitwise operations
  * Byte manipulation
  * Pointer arithmetic
  * Little Endian vs. Big Endian detection
  * Byte swapping

* **Socket Programming**

  * TCP client-server communication
  * UDP client-server communication
  * POSIX socket programming
  * Network byte order conversion
  * IPv4 addressing

* **Networking Protocols**

  * Flow control algorithms

    * Stop-and-Wait
    * Go-Back-N
    * Selective Repeat
  * Framing techniques
  * Error detection

    * Parity
    * Checksum
    * CRC

* **Concurrent Networking**

  * Multi-client servers using `fork()`
  * POSIX threads (`pthread`)
  * I/O multiplexing (`select()`, `poll()`)

As I continue learning Computer Networks, more implementations and experiments will be added.

---

## Repository Structure

```text
Computer-Networks/
├── endianness/
│   └── 1.c
├── socket-programming/
├── protocols/
├── tcp/
├── udp/
├── README.md
└── ...
```

> Current implementation:
>
> * `1.c` - Demonstrates byte extraction, bitwise operations, pointer manipulation, and machine endianness detection.

---

## Development Environment

These programs are primarily developed and tested using:

* Ubuntu (WSL2)
* GCC (GNU Compiler Collection)
* Visual Studio Code
* POSIX Socket API

### Requirements

* Ubuntu / Linux (recommended)
* Windows Subsystem for Linux (WSL2)
* GCC or Clang

Install GCC if needed:

```bash
sudo apt update
sudo apt install build-essential
```

Verify installation:

```bash
gcc --version
```

---

## Building & Running Programs

### Compile

```bash
gcc filename.c -o filename
```

### Run

```bash
./filename
```

Example:

```bash
gcc 1.c -o 1
./1
```

---

## Socket Programming

Compile the programs:

```bash
gcc server.c -o server
gcc client.c -o client
```

Run the server:

```bash
./server
```

Open another terminal and run the client:

```bash
./client
```

---

## Topics Covered

| Category      | Concepts                                      | Common APIs                                                                   |
| ------------- | --------------------------------------------- | ----------------------------------------------------------------------------- |
| Fundamentals  | Bitwise Operations, Byte Ordering, Endianness | `htons()`, `htonl()`, `ntohs()`, `ntohl()`                                    |
| TCP           | Connection-oriented communication             | `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()` |
| UDP           | Connectionless communication                  | `socket()`, `bind()`, `sendto()`, `recvfrom()`                                |
| Concurrency   | Multi-client servers                          | `fork()`, `pthread_create()`, `select()`, `poll()`                            |
| Error Control | CRC, Checksum, Parity                         | Custom implementations                                                        |
| Flow Control  | Stop-and-Wait, Go-Back-N, Selective Repeat    | Algorithm implementations                                                     |

---

## Useful Linux Commands

Check listening ports:

```bash
ss -tuln
```

Display IP addresses:

```bash
ip a
```

Test connectivity:

```bash
ping <ip-address>
```

Capture packets:

```bash
sudo tcpdump -i lo port <port>
```

---

## Future Additions

* TCP Socket Programming
* UDP Socket Programming
* Multi-client Chat Server
* HTTP Client
* DNS Lookup
* ARP Simulation
* Routing Algorithms
* Sliding Window Protocols
* Error Detection Algorithms
* Congestion Control
* Network Packet Analysis

---

## Purpose

This repository is maintained as a personal learning resource and implementation archive while studying Computer Networks and systems programming in C. Each program is written to reinforce theoretical concepts through practical implementation.

Contributions, suggestions, and improvements are always welcome.

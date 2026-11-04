# Computer Networks in C

A structured, organized collection of **C programs** that implement core computer networking concepts. Designed as both a personal learning archive and a quick reference for lab sessionals. All code is organized by **client / server** and by **protocol (TCP / UDP / File)** so you can find templates instantly.

---

## Quick Reference for Tomorrow's Lab

If you only have a few minutes before the sessional, open these first:

- [`notes.md`](notes.md) — **Read this.** It has the reusable logic patterns, API reference, bug notes, and a checklist of what tomorrow's 2 codes will likely be.
- `tcp/client/chat_interactive_client.c` + `tcp/server/chat_interactive_server.c` — The standard interactive TCP template (loop + `fgets` + `recv`/`send`).
- `udp/client/udp_simple_sender.c` + `udp/server/udp_receiver.c` — The standard UDP template (`sendto` / `recvfrom`).
- `tcp/server/threaded_multiclient_server.c` — Multi-threaded server (`pthread_create`).
- `file_transfer/server/file_sender_server.c` — File transfer over TCP (`system()` + `fread` + `send`).

---

## Repository Organization (Updated)

Files have been reorganized by role and protocol. Compiled binaries removed; only `.c` source files remain.

```text
Computer-Networks/
├── README.md                          # This file
├── notes.md                           # Full topic list, APIs, logic patterns, bug notes
│
├── basics/
│   └── endianness_byte_swap.c         # Byte extraction, pointer arithmetic, Little/Big Endian
│
├── tcp/
│   ├── client/
│   │   ├── basic_tcp_client.c         # Minimal TCP client (connect → send → recv)
│   │   ├── chat_interactive_client.c  # Interactive loop with exit command
│   │   └── simple_client_socket.c     # Basic socket creation + connect
│   └── server/
│       ├── basic_tcp_server.c         # Standard server: bind → listen → accept loop
│       ├── chat_interactive_server.c  # Interactive server (fgets reply + exit check)
│       ├── specific_ip_server.c       # Binds to explicit IP (inet_aton) instead of ANY
│       └── threaded_multiclient_server.c  # Per-client pthread server
│
├── udp/
│   ├── client/
│   │   ├── udp_sender.c              # Binds, sends, then receives reply
│   │   └── udp_simple_sender.c       # No bind; simple sendto → recvfrom
│   └── server/
│       ├── udp_receiver.c            # Standard UDP server (recvfrom + sendto)
│       └── udp_receiver_port_print.c # Same + prints client port (ntohs)
│
└── file_transfer/
    ├── client/
    │   └── file_receiver_client.c     # Receives TCP stream, writes to file
    └── server/
        └── file_sender_server.c       # Reads file, sends chunks over TCP
```

---

## Topics Actually Covered (From Source Code)

Read `notes.md` for full explanations. This is the quick list.

| Category | Concept | Source File(s) | Key APIs / Logic |
|---|---|---|---|
| **Fundamentals** | Endianness, byte swap, bitwise ops, pointer arithmetic | `basics/endianness_byte_swap.c` | `&`, `>>`, `& 255`, `(unsigned char*)&num` |
| **TCP Client** | Connect → Send → Receive → Close | `tcp/client/*.c` | `socket()`, `connect()`, `send()`, `recv()`, `close()` |
| **TCP Server** | Bind → Listen → Accept → Loop | `tcp/server/*.c` | `bind()`, `listen()`, `accept()`, `recv()`, `send()` |
| **Interactive TCP** | `while(1)` loop, `fgets()`, exit strings | `tcp/client/chat_interactive_client.c`, `tcp/server/chat_interactive_server.c` | `fgets()`, `strncmp()`, `strcspn()` |
| **Threading** | `pthread_create()` per client | `tcp/server/threaded_multiclient_server.c` | `pthread_create()`, `pthread_join()`, `malloc()`, `free()` |
| **Specific IP Bind** | `inet_aton()`, binding to interface | `tcp/server/specific_ip_server.c` | `inet_aton()` instead of `htonl(INADDR_ANY)` |
| **UDP Client** | `sendto()` / `recvfrom()` | `udp/client/*.c` | `sendto()`, `recvfrom()`, `bind()` (optional) |
| **UDP Server** | `recvfrom()` + reply with `sendto()` | `udp/server/*.c` | `recvfrom()`, `sendto()`, `ntohs()` for port display |
| **File Transfer** | Read file / receive over TCP | `file_transfer/*.c` | `system()`, `fopen()`, `fread()`, `fwrite()`, `send()`, `recv()` |
| **Address / Port** | Byte order, network conversion, IP strings | All socket files | `htons()`, `htonl()`, `ntohs()`, `ntohl()`, `inet_addr()`, `inet_aton()`, `inet_ntoa()` |

---

## Development Environment

These programs are developed and tested using:

- **Ubuntu (WSL2)** — Linux environment for POSIX sockets
- **GCC** — `gcc` (GNU Compiler Collection)
- **VS Code** — for editing
- **POSIX Socket API** — `sys/socket.h`, `netinet/in.h`, `arpa/inet.h`, `unistd.h`

### Requirements

```bash
sudo apt update
sudo apt install build-essential
```

Verify GCC:

```bash
gcc --version
```

If using threads (`thread_socket.c` / `threaded_multiclient_server.c`), compile with:

```bash
gcc file.c -o file -lpthread
```

---

## Building & Running (Organized Structure)

### Basics
```bash
gcc basics/endianness_byte_swap.c -o basics/byte_swap
./basics/byte_swap
```

### TCP Client + Server Example
```bash
# Terminal 1: Server
gcc tcp/server/chat_interactive_server.c -o tcp/server/chat_server
./tcp/server/chat_server

# Terminal 2: Client
gcc tcp/client/chat_interactive_client.c -o tcp/client/chat_client
./tcp/client/chat_client
```

### UDP Client + Server Example
```bash
# Terminal 1: Server
gcc udp/server/udp_receiver.c -o udp/server/udp_server
./udp/server/udp_server

# Terminal 2: Client
gcc udp/client/udp_simple_sender.c -o udp/client/udp_client
./udp/client/udp_client
```

### Threaded Server
```bash
gcc tcp/server/threaded_multiclient_server.c -o tcp/server/thread_server -lpthread
./tcp/server/thread_server
```

---

## Essential APIs (Quick Look-up)

| Function | Purpose | Protocol | Notes |
|---|---|---|---|
| `socket(AF_INET, SOCK_STREAM, 0)` | Create TCP socket | TCP | Returns fd |
| `socket(AF_INET, SOCK_DGRAM, 0)` | Create UDP socket | UDP | Returns fd |
| `bind()` | Attach address/port | Both | `sockaddr_in`, `htons()`, `INADDR_ANY` |
| `listen()` | Enable passive mode | TCP only | `backlog` (e.g., 5) |
| `accept()` | Pick up pending connection | TCP | Returns **new fd** for client |
| `connect()` | Initiate connection | TCP | Client only |
| `send()` / `recv()` | Stream data | TCP | Use the **connected fd**, not `listen` fd |
| `sendto()` / `recvfrom()` | Message data | UDP | `recvfrom` writes sender info |
| `close()` | Close fd | Both | Close client fd (`newsockfd`) and listen fd (`ret`) |
| `htons()` / `htonl()` | Host → Network byte order | Address/Port | Always before sending |
| `ntohs()` / `ntohl()` | Network → Host byte order | Address/Port | Always when reading / printing |
| `inet_addr()` / `inet_aton()` | String IP → binary | Setup | `inet_aton()` writes to `sin_addr` directly |
| `inet_ntoa()` | Binary IP → string | Display | For printing client info |

> Full parameter details, common bugs, and reusable logic templates are in [`notes.md`](notes.md).

---

## Common Linux Commands for Lab

```bash
# Compile
gcc file.c -o file
# With threads:
gcc file.c -o file -lpthread

# Run
./file

# Check listening ports
ss -tuln

# Show interfaces / IP
ip a

# Test connectivity
ping 127.0.0.1
ping 8.8.8.8

# Capture packets on loopback (debug send/recv)
sudo tcpdump -i lo port 5000
```

---

## Bugs & Fixes Found in Source (Important for Tomorrow)

These are real errors in the original files. Knowing them prevents you from making the same mistakes.

| File | Issue | Fix / Lesson |
|---|---|---|
| `file_transfer/client/file_receiver_client.c` | Uses `recv(status_connect, ...)` — `status_connect` is `connect()` return value (`0`/`-1`), not socket fd. Should be `recv(ret, ...)`. | Always use the original `socket()` fd for `recv`/`send` unless a new fd was created by `accept()`. |
| `file_receiver_client.c` | Opens file with `"r"` (read) but tries to write received data. Should be `"w"`. | File mode (`r`/`w`) must match operation. |
| `file_receiver_client.c` | Does not define `server_sock` before calling `connect()`. | Always declare and populate `sockaddr_in` before `connect`. |
| `thread_socket.c` (original `thread_socket`) | `pthread_join()` called immediately in `while(1)`. This makes clients sequential, not concurrent. For true concurrency, use `pthread_detach()` or manage threads externally. | Understand the difference between sequential and concurrent threading. |

---

## Future / Missing Topics (To Add After Session)

Some topics mentioned in the original README (Stop-and-Wait, Go-Back-N, Selective Repeat, Sliding Window, CRC, Checksum, Framing, HTTP Client, Routing) are not yet implemented in source. Once the user provides the missing session notes, add sections here. See `notes.md` Section 10 for the checklist.

---

## Purpose

This repo is a personal learning resource. Each file is a minimal, working example of one networking concept. The organization (client / server / protocol) is intended to help during sessional exams when you need to write network code from scratch quickly. Read `notes.md` before coding.

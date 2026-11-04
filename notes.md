# Computer Networks — Lab Prep Notes

> Updated for tomorrow's sessional. Covers all code in this repo, APIs, logic patterns, reusable concepts, and what to expect when writing new network programs.

---

## 1. What This Repo Contains (Organized)

```
basics/
  endianness_byte_swap.c       → byte order, bitwise ops, pointers

tcp/client/
  basic_tcp_client.c          → basic TCP connect → send → recv
  chat_interactive_client.c   → loop with fgets, exit command
  simple_client_socket.c      → basic socket creation + send

tcp/server/
  basic_tcp_server.c          → bind → listen → accept → recv → send loop
  chat_interactive_server.c   → interactive loop with "exit" / "bye"
  specific_ip_server.c        → bind to explicit IP (inet_aton) not ANY
  threaded_multiclient_server.c → pthread_create per client

udp/client/
  udp_sender.c                → sendto + recvfrom (binds first)
  udp_simple_sender.c         → sendto + recvfrom (no bind)

udp/server/
  udp_receiver.c              → recvfrom + sendto (binds, no port print)
  udp_receiver_port_print.c   → same + prints sender port (ntohs)

file_transfer/
  client/file_receiver_client.c  → receives over TCP, writes to file
  server/file_sender_server.c    → reads file, sends over TCP
```

---

## 2. Topics Found in Code (Full List)

### A. Fundamentals
- **Bitwise Operations** (`&`, `>>`, `& 255`)
- **Byte Extraction** from integer (manual + pointer)
- **Pointer Arithmetic** (`(unsigned char *)&num`, `ptr[0]..ptr[3]`)
- **Little Endian vs Big Endian** detection
- **Byte Swapping** (manual swap of indices)
- **Network Byte Order** (`htons`, `htonl`, `ntohs`, `ntohl`)

### B. TCP Socket Programming (Connection-Oriented)
- `socket(AF_INET, SOCK_STREAM, 0)`
- `bind()` — attach address/port
- `listen()` — passive mode, backlog queue
- `accept()` — blocks, returns new socket fd for client
- `connect()` — client initiates connection
- `send()` / `recv()` — stream data (TCP guarantees order)
- `close()` — clean up

### C. TCP Client Logic Patterns
- Create socket
- Define `sockaddr_in` (family, port `htons()`, IP `inet_addr()`)
- `connect()`
- `send()` / `recv()` loop
- `close()`

### D. TCP Server Logic Patterns
- `socket()` → `bind()` → `listen()` → `accept()` loop
- Inner loop: `recv()` → process → `send()` → check disconnect (`recv` returns 0)
- `close()` both listening socket and connected socket (`newsockfd`)

### E. Interactive / Chat TCP Pattern
- `while(1)` loop
- Read user input with `fgets(msg, sizeof(msg), stdin)`
- `send()` message
- Check for exit string (`strncmp(msg, "exit", 4)`) before/after send
- `recv()` reply
- Check reply for exit
- Break loop → `close()`

### F. Multi-threaded TCP Server (`pthread`)
- Create listening socket, bind, listen (standard)
- `accept()` inside `while(1)` loop
- Allocate `int*` with `malloc(sizeof(int))` to pass fd to thread
- `pthread_create(&tid, NULL, start_thread, data_sock)`
- Inside thread: `recv()` → `send()` → `close(data_sock)` → `free(arg)`
- `pthread_join(tid, NULL)` after creation (in this repo it's synchronous per client)

### G. UDP Socket Programming (Connectionless)
- `socket(AF_INET, SOCK_DGRAM, 0)`
- `bind()` (optional for clients, required for servers)
- `sendto()` — needs target `sockaddr_in`
- `recvfrom()` — receives data + stores sender address
- `close()`

### H. File Transfer over TCP
- Server side: `system("ls > result.txt")` → `fopen()` → `fread()` chunks → `send()`
- Client side: `recv()` chunks → `fwrite()` to file
- Note: `recv()` may not return full file at once; loop until `recv` returns 0 or `<= 0`
- Note: use `strlen(buff)` for `send()` length, not `sizeof(buff)`

### I. Address & Port Utilities
- `htons(port)` — host → network byte order (port)
- `htonl(INADDR_ANY)` — bind to any interface
- `inet_addr("127.0.0.1")` — string IP → binary
- `inet_aton("...", &sin_addr)` — alternative string conversion
- `inet_ntoa(...)` — binary IP → string (for printing client info)
- `ntohs(...)` — network → host byte order (e.g., print sender port)

---

## 3. Essential APIs & Parameters (Man-Style Reference)

### `socket()`
```c
int socket(int domain, int type, int protocol);
```
- `AF_INET` — IPv4
- `SOCK_STREAM` — TCP (reliable, ordered)
- `SOCK_DGRAM` — UDP (unreliable, message-based)
- Returns file descriptor (`int`), `-1` on error

### `bind()`
```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- `sockaddr_in`: `sin_family = AF_INET`, `sin_port = htons(PORT)`, `sin_addr.s_addr`
- `INADDR_ANY` (`htonl(INADDR_ANY)`) — listen on all interfaces
- Returns `0` on success, `-1` on error

### `listen()`
```c
int listen(int sockfd, int backlog);
```
- `backlog` (e.g., `5`) — max pending connections in queue
- Only for `SOCK_STREAM`

### `accept()`
```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
- **Blocks** until a client connects
- Returns **new socket fd** (`newsockfd`) for the specific client
- `clientaddr` filled with client's IP/port

### `connect()`
```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
- Client uses listening server's `sockaddr_in`
- Returns `0` on success, `-1` on error

### `send()` / `recv()` (TCP)
```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```
- `flags` usually `0`
- `recv` returns bytes received, `0` if client disconnected cleanly, `-1` on error
- Always null-terminate buffer: `buff[b] = '\0';`

### `sendto()` / `recvfrom()` (UDP)
```c
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```
- `recvfrom` writes sender info into `src_addr`; `addrlen` must be initialized to `sizeof(struct sockaddr_in)`

### `close()`
```c
int close(int fd);
```
- Always close sockets. Close the new fd (`newsockfd`) after client disconnects; close listening fd (`ret`/`sockfd`) when server exits.

---

## 4. Logic Patterns & Concepts (Reusable for Tomorrow)

These are the building blocks. Tomorrow's 2 codes will likely mix some of these.

### Pattern 1: Byte Extraction & Endianness
```c
int byte1 = num & 255;           // mask lower 8 bits
int byte2 = (num >> 8) & 255;    // shift right, then mask
int byte3 = (num >> 16) & 255;
int byte4 = (num >> 24) & 255;
```
- **Concept**: Any multi-byte value can be broken into bytes this way. Useful for packet headers, checksums, protocol design.
- **Pointer version**: `(unsigned char*)&num` lets you read bytes directly from memory.
- **Endianness check**: Compare `byte1 == ptr[0]`. If yes → Little Endian.
- **Swap**: Swap index pairs (`ptr[0]` ↔ `ptr[3]`, `ptr[1]` ↔ `ptr[2]`) to convert Endian.

### Pattern 2: TCP Client Template
```c
int s = socket(AF_INET, SOCK_STREAM, 0);
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = inet_addr("IP");
connect(s, (struct sockaddr*)&addr, sizeof(addr));
send(s, msg, strlen(msg), 0);
char buff[1000];
recv(s, buff, sizeof(buff)-1, 0);
buff[recv_bytes] = '\0';
close(s);
```
- **Reusable logic**: Always check `== -1` after `socket`, `connect`, `send`, `recv`.

### Pattern 3: TCP Server Template
```c
int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
bind(listen_sock, (struct sockaddr*)&my_addr, sizeof(my_addr));
listen(listen_sock, 5);
while(1) {
    int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &len);
    while(1) {
        ssize_t n = recv(client_sock, buff, sizeof(buff)-1, 0);
        if (n <= 0) break; // disconnect
        buff[n] = '\0';
        printf("Client: %s\n", buff);
        send(client_sock, reply, strlen(reply), 0);
    }
    close(client_sock);
}
close(listen_sock);
```
- **Reusable logic**: `accept()` blocks, returns a new fd. Don't send/recv on `listen_sock` — use `client_sock`.

### Pattern 4: Interactive Loop Template
```c
char msg[1000], buff[1000];
while(1) {
    printf("> ");
    fgets(msg, sizeof(msg), stdin);
    msg[strcspn(msg, "\n")] = '\0';  // remove newline
    if (strcmp(msg, "exit") == 0) break;
    send(s, msg, strlen(msg), 0);
    ssize_t b = recv(s, buff, sizeof(buff)-1, 0);
    if (b <= 0) { printf("Disconnected\n"); break; }
    buff[b] = '\0';
    printf("Server: %s\n", buff);
}
```
- **Reusable logic**: `fgets()` includes `\n`; remove it with `strcspn`. Check `recv` return value before using buffer.

### Pattern 5: UDP Template
```c
int s = socket(AF_INET, SOCK_DGRAM, 0);
bind(s, ...); // server must bind; client can bind or skip
// Client sends:
sendto(s, msg, strlen(msg), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
// Server receives:
recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client_addr, &client_len);
// Reply:
sendto(s, reply, strlen(reply), 0, (struct sockaddr*)&client_addr, client_len);
```
- **Reusable logic**: `recvfrom` requires `client_len` to be initialized (`= sizeof(...)`) before call because it writes back the actual length.

### Pattern 6: Threaded Server Template
```c
void* handle_client(void* arg) {
    int fd = *(int*)arg;
    free(arg);
    char buff[1000];
    recv(fd, buff, sizeof(buff)-1, 0);
    send(fd, "ack", 3, 0);
    close(fd);
    return NULL;
}
// In main loop:
int* fd_ptr = malloc(sizeof(int));
*fd_ptr = accept(listen_sock, ...);
pthread_t t;
pthread_create(&t, NULL, handle_client, fd_ptr);
// Note: use pthread_detach(t) or manage threads properly if not joining immediately
```
- **Reusable logic**: Always `free(arg)` inside thread. Don't use `arg` after `free`. If you `join` immediately, you don't need detach; for concurrent clients you need either `detach` or a thread pool.

### Pattern 7: File Transfer Template
```c
FILE* fp = fopen("file.txt", "r");
char chunk[1024];
while (fread(chunk, 1, sizeof(chunk), fp) > 0) {
    send(fd, chunk, strlen(chunk), 0); // careful: strlen on binary is unsafe
}
```
- **Reusable logic**: For text files, `strlen()` works. For binary files, pass the actual `fread` return value to `send()`. Always loop `recv()` until it returns `<= 0`.

---

## 5. Common Errors / Bugs in This Repo (Learn from These)

These are actual mistakes in the source files. Knowing them helps you avoid them tomorrow.

| File | Bug / Issue | Why It Matters |
|---|---|---|
| `file_receiver_client.c` (client) | Uses `recv(status_connect, ...)` instead of `recv(ret, ...)`. `status_connect` is the return value of `connect` (`0` or `-1`), not the socket fd. | You must use the original socket fd (`ret`) for `recv`. |
| `file_receiver_client.c` | Opens file with `"r"` (read mode) but tries to write received data into it. Should be `"w"` or `"wb"`. | File mode must match operation. |
| `file_receiver_client.c` | Does not define `server_sock` before `connect()`. | Always declare `sockaddr_in server_addr` and populate it before connecting. |
| `file_receiver_client.c` | Uses `recv()` on a `SOCK_STREAM` but doesn't handle partial reads in a loop correctly. | TCP is a stream; one `recv` may not get everything. Loop until `recv <= 0`. |
| `udp_receiver_port_print.c` | None major — but note it prints `ntohs(client_addr.sin_port)`. Good practice. | Always use `ntohs` when displaying ports. |
| `udp_simple_sender.c` | Doesn't bind. Works fine for client that only sends/replies, but if server expects a fixed source port, bind may be needed. | Understand when bind is required vs optional. |

---

## 6. Linux / Ubuntu Commands for Lab

```bash
# Compile
gcc file.c -o file
# If using pthread:
gcc file.c -o file -lpthread

# Run
./file

# Check if server is listening
ss -tuln
# Example: shows TCP port 5000

# Check your IP / interfaces
ip a

# Test connectivity
ping 127.0.0.1
ping 8.8.8.8

# Capture packets (useful for debugging send/recv)
sudo tcpdump -i lo port 5000
# -i lo = loopback interface (localhost)

# Check process / kill server
ps aux | grep ./file
kill -9 <PID>
```

---

## 7. Quick Compile & Run Reference (Organized Files)

```bash
# Basics
gcc basics/endianness_byte_swap.c -o basics/byte_swap
./basics/byte_swap

# TCP Client
gcc tcp/client/chat_interactive_client.c -o tcp/client/chat_client
gcc tcp/client/simple_client_socket.c -o tcp/client/simple_client

# TCP Server
gcc tcp/server/threaded_multiclient_server.c -o tcp/server/thread_server -lpthread
gcc tcp/server/chat_interactive_server.c -o tcp/server/chat_server
gcc tcp/server/basic_tcp_server.c -o tcp/server/basic_server

# UDP Client
gcc udp/client/udp_sender.c -o udp/client/udp_client

# UDP Server
gcc udp/server/udp_receiver.c -o udp/server/udp_server

# File Transfer
gcc file_transfer/server/file_sender_server.c -o file_transfer/server/file_server
gcc file_transfer/client/file_receiver_client.c -o file_transfer/client/file_client  # NOTE: broken source, see bugs table
```

---

## 8. Tomorrow's Lab Prep — What to Expect

Based on this repo's patterns, tomorrow's 2 codes are likely variations of:

**Possible Code 1 (TCP-based):**
- Client-server chat with exit command
- OR interactive server that responds to user input
- OR server that accepts multiple clients sequentially
- **Key skills**: `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`, `close`, `while(1)` loop, `fgets()` input handling

**Possible Code 2 (UDP or File Transfer or Thread / Protocol Logic):**
- UDP sender/receiver with `sendto`/`recvfrom`
- File transfer over TCP (`system()`, `fopen`, `fread`, `fwrite`, `send`, `recv`)
- Multi-threaded server (`pthread_create`, `malloc`, `free`)
- Endianness / byte swap / checksum logic
- **Key skills**: protocol selection (`SOCK_DGRAM` vs `SOCK_STREAM`), buffer management, thread argument passing, file I/O integration with sockets

### Approach for Tomorrow
1. **Identify protocol first**: Is it TCP (`SOCK_STREAM`) or UDP (`SOCK_DGRAM`)? This changes everything (`sendto` vs `send`, `recvfrom` vs `recv`).
2. **Draw the flow**:
   - Client: Create → Connect (TCP) or Sendto (UDP) → Send/Recv loop → Close
   - Server: Create → Bind → Listen (TCP) / Bind (UDP) → Accept / Recvfrom → Process → Send / Sendto → Close
3. **Use a standard error-check template**: `if (func(...) == -1) { printf("...\n"); exit(-1); }`
4. **Always null-terminate strings** after `recv`.
5. **Always check `recv` return value**: `> 0` = data, `== 0` = disconnect, `< 0` = error.
6. **For interactive codes**: Use `fgets()` not `scanf()` for strings. Remove `\n` with `strcspn()`.
7. **If threading is involved**: `malloc(sizeof(int))` for fd, pass to thread, `free(arg)` inside thread.
8. **If file is involved**: Open in correct mode (`r` vs `w`), loop `recv` + `fwrite` (or `fread` + `send`), close file after done.

---

## 9. Concepts That Help You Write Any Similar Code

These are the **transferable skills** — not just syntax, but the way to think about network programming.

### A. The Client-Server Model (Universal)
Every network program is a conversation:
- **Client** initiates (`connect` or `sendto`).
- **Server** waits (`accept` or `recvfrom`).
- Both use `send` (or `sendto`) and `recv` (or `recvfrom`).
- Communication is **request → response** in most lab codes.

### B. Protocol = Rules
- **TCP** (`SOCK_STREAM`) = reliable pipe. You don't worry about lost packets. You worry about blocking (reading too slowly, writing too fast), disconnection (`recv == 0`), and buffer sizes.
- **UDP** (`SOCK_DGRAM`) = message in a bottle. Each `sendto` is one message. `recvfrom` reads a whole message (or truncates). No connection setup needed (`connect` is optional). No guarantee of order or delivery.

### C. Byte Order = Translation
- Your computer uses Little Endian. The network uses Big Endian (Network Byte Order).
- **Always translate ports and multi-byte integers**: `htons(port)` before sending, `ntohs()` when reading.
- **This applies beyond sockets**: Any protocol header (TCP header, IP header) uses Network Byte Order. Tomorrow's code may ask you to build a simple packet/header — the same concept applies.

### D. Memory & Pointers = Reading Raw Data
- `recv()` gives you a `char[]`. If you need to read multi-byte values (like a length field or checksum) from the stream, treat the buffer as `unsigned char*` and use pointer arithmetic or `memcpy()` with `ntohs()`.
- Example: If a 2-byte length field arrives in network order:
```c
unsigned short len = (buff[0] << 8) | buff[1]; // manual
// or
unsigned short len; memcpy(&len, buff, 2); len = ntohs(len);
```

### E. Modular Thinking (Split Main)
Even if the repo puts everything in `main()`, tomorrow you should split:
```c
void server_setup();
void handle_client(int client_fd);
void send_file(int fd, const char* filename);
```
- This makes debugging faster and shows the professor you understand the logic.

### F. Error Handling Pattern (Always Repeatable)
```c
if (function(...) == -1) {
    perror("function failed"); // perror prints errno message
    close(fd);
    exit(EXIT_FAILURE);
}
```
- `perror()` is more informative than `printf()` for system errors.

---

## 10. Absent / Upcoming Topics (For User to Add Later)

> The user mentioned some topics from absence. Once you tell me those, I'll add sections here. Typical missing topics for this course:

- [ ] Stop-and-Wait / Go-Back-N / Selective Repeat (Flow Control)
- [ ] Sliding Window Protocol
- [ ] CRC / Checksum / Parity (Error Detection)
- [ ] Framing (byte stuffing / bit stuffing)
- [ ] Multi-client chat server (full version)
- [ ] HTTP Client / Server basics
- [ ] DNS Lookup simulation
- [ ] Routing Algorithms
- [ ] Congestion Control (Slow Start, Congestion Avoidance)
- [ ] Packet Analysis with `tcpdump` / `wireshark`

---

## 11. File-Specific Quick Notes

### `basics/endianness_byte_swap.c`
- Extracts bytes of `258` with `& 255` and `>>`
- Compares with `(unsigned char*)&num` pointer method
- Swaps bytes manually to simulate Big Endian
- **Takeaway**: Bitmask + shift = universal byte extraction.

### `tcp/client/basic_tcp_client.c`
- Standard TCP client flow
- **Bug check**: None major — clean template.

### `tcp/client/chat_interactive_client.c`
- `while(1)` + `fgets()` loop
- Exit on `"exit"`
- **Pattern**: This is the exact template for interactive TCP clients.

### `tcp/client/simple_client_socket.c`
- Minimal socket → connect → send → recv
- **Takeaway**: Smallest working TCP client.

### `tcp/server/basic_tcp_server.c`
- `accept()` loop inside `while(1)`
- `recv()` loop inside inner `while(1)`
- Handles disconnect (`recv == 0` or `< 0`) with `break`
- **Pattern**: The standard sequential TCP server.

### `tcp/server/chat_interactive_server.c`
- Interactive server with user input (`fgets` from stdin)
- Uses `newsockfd` correctly for send/recv (not `ret`)
- **Note**: Fixed version of earlier broken server patterns in repo.

### `tcp/server/specific_ip_server.c`
- Uses `inet_aton()` instead of `htonl(INADDR_ANY)`
- **Takeaway**: When you need to bind to a specific interface/IP.

### `tcp/server/threaded_multiclient_server.c`
- `pthread_create()` per accepted client
- Passes fd via `malloc(sizeof(int))`
- **Note**: `pthread_join()` is called immediately in this version, which means clients are handled sequentially (one at a time), not concurrently. For true concurrency, you need `pthread_detach()` or manage threads separately.
- **Takeaway**: Always understand whether the design is concurrent or sequential. In this file, it's sequential due to immediate `join`.

### `udp/client/udp_sender.c`
- Binds first (`bind`), then `sendto`, then `recvfrom`
- **Pattern**: Client that needs to receive reply on known port.

### `udp/client/udp_simple_sender.c`
- No `bind` — just `sendto` → `recvfrom`
- **Pattern**: Simplest UDP client.

### `udp/server/udp_receiver.c`
- Standard UDP server: bind → recvfrom → sendto → close
- **Pattern**: Basic UDP server.

### `udp/server/udp_receiver_port_print.c`
- Same + `printf("Port: %d\n", ntohs(client_addr.sin_port));`
- **Takeaway**: Use `ntohs()` to display ports correctly.

### `file_transfer/client/file_receiver_client.c`
- **Broken source**: See Bug Table (Section 5). Does not define server address, uses wrong fd for recv, opens file in wrong mode.
- **What it tries to do**: Receive TCP stream, write to `result.txt`
- **Correct logic**: `recv(ret, ...)` loop → `fopen("result.txt", "w")` → `fwrite()` → `fclose()`.

### `file_transfer/server/file_sender_server.c`
- Uses `system("ls > result.txt")` to generate file
- `fopen()` → `fread()` chunks → `send()` loop
- **Note**: Uses `strlen(buff)` for `send()` length. Works for text files from `system()` command.
- **Pattern**: File server that reads from disk and streams over TCP.

---

*End of notes. Add your absent topics here after telling me. Keep this file open during tomorrow's lab — the patterns in Section 4 are your cheat sheet.*

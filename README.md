# 🔌 TCP/IP Network System – Custom Client–Server Protocol in C

A low-level networking project implementing a custom TCP-based client–server system using **ANSI C** and **BSD sockets**. The project focuses on reliable communication, concurrency, and protocol design, closely aligned with real-world systems programming and distributed systems fundamentals.

Developed as part of the **Computer Networks** course (Year 2) at the **Faculty of Computer Science, "Alexandru Ioan Cuza" University of Iași**.

---

## 🧠 Project Overview



This project applies core computer networking concepts through a hands-on implementation of a custom application-layer protocol over TCP. The system consists of a concurrent server and multiple clients, emphasizing:

* **Explicit protocol design:** Request/response cycles with ACK/NACK semantics.
* **Robust error handling:** Managing broken pipes and socket timeouts.
* **Concurrent connection management:** Handling multiple users simultaneously.
* **OS-level primitives:** Direct use of Linux/POSIX networking APIs.

The goal was to bridge networking theory (OSI / TCP-IP) with production-style systems programming.


---

## 🔧 Technologies & Concepts

### Language & Environment
* **C (ANSI C)**
* **Linux / POSIX environment**

### Networking & Systems Concepts
* TCP/IP stack and OSI model mapping
* Client–server architecture
* Application-layer protocol design
* Socket lifecycle management (Bind, Listen, Accept, Connect)

### Concurrency & Performance

* **Handling multiple clients via:**
    * `fork()` for process-based concurrency.
    * `select()` for I/O multiplexing.
* Blocking vs. non-blocking I/O considerations.

### Tooling & Engineering Practices
* **BSD Socket API:** `sys/socket.h`, `netinet/in.h`
* **Build System:** Makefile for reproducible builds.
* **Defensive Programming:** Strict input validation and memory safety.

---

## 🗂️ Repository Structure

```text
rc_p1/
├── client.c        # TCP client implementation
├── server.c        # Concurrent TCP server
├── protocol.h      # Custom protocol definitions (Request/Response codes)
├── utils.c/.h      # Shared utilities and helper functions
├── Makefile        # Build automation
└── README.md       # Project documentation
```

---

## ▶️ How to Build & Run

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/cristian-roman/rc_p1.git](https://github.com/cristian-roman/rc_p1.git)
   cd rc_p1
   ```

2. **Build the project:**
   ```bash
   make
   ```

3. **Start the server:**
   ```bash
   ./server
   ```

4. **Run a client (in a separate terminal):**
   ```bash
   ./client
   ```

---

## ✨ Key Highlights

* **Custom Protocol:** Designed a TCP-based protocol with explicit acknowledgment handling to ensure data integrity.
* **High Concurrency:** Implemented support for multiple simultaneous clients using OS-level primitives.
* **Pure C:** Built with **zero external libraries**, relying entirely on standard POSIX APIs.
* **Systems Debugging:** Practiced low-level debugging, memory management, and error recovery in a networked environment.

---

## 🎓 Academic Context
* **University:** "Alexandru Ioan Cuza" University of Iași
* **Faculty:** Computer Science
* **Course:** Computer Networks (Year 2)

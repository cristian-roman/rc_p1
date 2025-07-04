# Computer Networks Project – Second Year, Informatics Faculty, UAIC

This repository contains the code and resources for a project developed as part of the **Computer Networks** course at the **Faculty of Computer Science**, **"Alexandru Ioan Cuza" University of Iași**.

## Project Overview

The project was developed during the second year of study and is intended to apply core concepts from the computer networks curriculum. It focuses on both **theoretical understanding** and **practical implementation** of networking principles using modern programming techniques.

## 🔧 Technologies & Concepts Used

- **Programming Language**:  
  - C (ANSI C standard)

- **Computer Networks Theory**:  
  - OSI Model  
  - TCP/IP Stack  
  - Client-Server Architecture  
  - Reliable and Unreliable Transmission Protocols  
  - Socket Programming (with emphasis on TCP sockets)  
  - Connection handling and basic communication protocol design

- **Techniques & Tools**:  
  - **Socket API** (`sys/socket.h`, `netinet/in.h`, etc.)  
  - **Concurrency** (handling multiple connections using `fork()` or `select()`)  
  - **Makefile** for automated builds  
  - **Modular Programming** in C  
  - **Error Handling & Robust Communication** techniques

## Repository Structure

The repository includes the following:

- Source code written in C with clear modular separation
- `Makefile` for compilation
- Sample scripts or instructions for running and testing the applications

## How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/rc_p1-main.git
   cd rc_p1-main
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the server:
   ```bash
   ./server
   ```

4. Run the client (in a separate terminal):
   ```bash
   ./client
   ```

## Acknowledgements

This project was developed under the guidance of the Computer Networks course instructors at UAIC. It serves both as an academic assignment and a practical learning tool for students interested in **network protocol design**, **systems programming**, and **inter-process communication**.

---
**University**: "Alexandru Ioan Cuza" University of Iași  
**Faculty**: Informatics  
**Course**: Computer Networks (Year 2)

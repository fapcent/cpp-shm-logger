# 💾 Zero-Copy Shared Memory Logger

An ultra-low latency logging system using Inter-Process Communication (IPC) to avoid I/O bottlenecks in HFT systems.

## 🚀 Key Features
* **Lock-Free Architecture:** Uses atomic operations to manage concurrency without Mutexes.
* **Ring Buffer:** Circular buffer implementation in POSIX Shared Memory.
* **Decoupled Architecture:** The Producer (Trader) is never blocked by the Consumer (Disk Writer).

## 🛠 Stack
* **Language:** C++17
* **System:** POSIX Shared Memory, mmap

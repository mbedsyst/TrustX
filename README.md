# 🔐 TrustX – Embedded Hardware Security Module over USB

**TrustX** is a custom-built Hardware Security Module (HSM) designed for secure, offline cryptographic operations. Built around the STM32H5 microcontroller series, it supports Symmetric encryption, HMAC, Hashing, Key Management, Random Number Generation, and communication over USB using the CDC class.

This repository contains:
- Embedded firmware for STM32H563ZI (development version)
- Python-based GUI for host interaction
- PCB design assets (in progress)
- Documentation and security implementation details

---

## 🧭 Project Overview

**TrustX** is built with the goal of providing:
- **Secure key generation and storage** tied to the specific hardware instance
- **Hardware-accelerated cryptographic operations** that never expose key material to the host
- A clean, **USB CDC-based communication protocol** with a platform-independent Python GUI
- Optional tamper response and persistent configuration features

TrustX is suitable for experimentation, cryptographic offloading, or use as a secure peripheral in broader security-critical systems.

---

## ⚙️ Features
|------------------------------|--------------------------------------------------------------|
| Feature                      | Description                                                  |
|------------------------------|--------------------------------------------------------------|
| Encryption/Decryption        | Software based Symmetric Encryption and Decryption           |
| Hashing (SHA2)               | On-device SHA-224, SHA-256, SHA-384, SHA-512                 |
| HMAC (SHA-224/SHA-256)       | Secure message authentication                                |
| Random Number Generation     | True Random Number Generation (TRNG-based)                   |
| Key Derivation               | UID + Salt-based Runtime Key Derivation                      |
| Key Management               | Add / Read / Delete Key entries, encrypted in external Flash |
| OTP Stream Generator         | For One-Time Symmetric Secure Exchanges                      |
| USB CDC Interface            | Secure protocol between Host and HSM                         |
| Flash Binding                | External Flash encrypted using MCU-derived key               |
|------------------------------|--------------------------------------------------------------|

---

PCB design and enclosure are in progress. Images will be added when design is completed.

---

## 🧱 Architecture Overview

                          ┌──────────────────────────┐
                          │        Host PC GUI       │
                          │ (Python, USB Serial GUI) │
                          └────────────┬─────────────┘
                                       │ USB CDC
                          ┌────────────▼─────────────┐
                          │      TrustX Firmware     │
                          │      (STM32H563ZI)       │
                          └──────┬──────────┬────────┘
                                 │          │
                    ┌────────────▼───┐ ┌────▼────────────────┐
                    │ Key Derivation │ │   Command Handler   │
                    │ UID + Salt KDF │ │ (Dispatching Logic) │
                    └────────┬───────┘ └────┬────────────────┘
                             │              │
               ┌─────────────▼──────┐  ┌────▼──────────────────────────┐
               │  Master Key (RAM)  │  │        Operation Units        │
               │    (Not stored)    │  │ Encryption, Decryption, Hash  │
               └────────┬───────────┘  │   HMAC, RNG, Key Management   │
                        │              └────────┬──────────────────────┘
     ┌──────────────────▼────────────┐          │
     │ Encrypted External Flash      │◄─────────┘
     │ Key Store (Bound to UID)      │
     └───────────────────────────────┘


---

## 🖥️ Host Interface (Python GUI)

- Written in Python 3 with Tkinter
- Simple landing screen with separate operation pages
- Serial port detection, command packaging, and response parsing
- Supports all features provided by the HSM firmware
- Copy/download output support for generated or decrypted content

Run with:

```bash
pip install pyserial
python gui/main.py
# TrustX

**TrustX** is a custom-built Hardware Security Module designed for secure, offline cryptographic operations using the STM32H563ZI microcontroller series. It supports Symmetric Encryption, HMAC, SHA2 based Hashing, Secure Key Generation & Management, Random Number Generation, and communicates with a host system over USB using the CDC class.

TrustX was built with the goal of tying cryptographic operations and key storage to the unique identity of the hardware itself, ensuring that sensitive data never leaves the device in plaintext. It provides a simple interface for interacting with the HSM through a USB serial connection, along with optional tamper detection and persistent configuration capabilities. While still under development, TrustX is intended as a practical platform for Security Experimentation, Cryptographic Offloading, and Embedded Security learning.

The project includes firmware for the STM32H563ZI (used during development), a Python-based GUI for interacting with the device from a PC, and PCB design assets that are currently in progress. 

## Features

| Feature                      | Description                                                  |
|------------------------------|--------------------------------------------------------------|
| Encryption/Decryption        | Software based Symmetric Cryptography (Intel tinycrypt)      |
| Hashing                      | FIPS-compliant hardware HASH Engine with MD5/SHA1/SHA2       |
| HMAC (SHA-224/SHA-256)       | Message Authentication using HASH Engine                     |
| Random Number Generation     | NIST SP 800-90B compliant TRNG entropy source                |
| Key Derivation               | 96-bit UID + Runtime Salt for Key Derivation                 |
| Key Management               | Add/Delete Key entries, encrypted in external Flash          |
| OTP Stream Generator         | For One-Time Symmetric Secure Exchanges                      |
| USB CDC Interface            | Communication Middleware between Host and HSM                |
| Flash Binding                | External Flash encrypted using MCU-derived key               |

## Architecture Overview

		                          ┌──────────────────────────┐
		                          │        Host PC GUI       │
		                          │ (Python, USB Serial GUI) │
		                          └────────────┬─────────────┘
		                                       │ USB CDC
		                          ┌────────────▼────────────┐
		                          │     TrustX Firmware     │
		                          │      (STM32H563ZI)      │
		                          └──────┬──────────┬───────┘
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

## A Note on Security

While TrustX is designed with security in mind, I want to be clear that this is a learning project and may contain vulnerabilities — both known and unknown. I'm still exploring embedded cryptography and system design, and there’s a lot more to learn.

If you’re someone who’s into Firmware, Cryptography, or Embedded Systems and happen to notice a Security gap, Logic flaw, or Improvement, I’d really appreciate it if you raise it. I’m open to feedback and happy to work on fixing or improving it.

TrustX is an experiment.
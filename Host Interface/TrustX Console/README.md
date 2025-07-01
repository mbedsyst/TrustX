# TrustX Console

**TrustX Console** is a custom host interface designed for interacting with the TrustX Hardware Security Module (HSM) over USB. It supports a range of cryptographic operations including:

- Encryption / Decryption
- Hashing (SHA family)
- HMAC Generation
- Random Number Generation
- One-Time Pad Generation
- Key Generation / Storage / Deletion

The interface is built with a modern, dark-themed UI and includes integrated animations, a command terminal, and support for serial communication using `pyserial`.

---

## Technologies Used

- [PySide6](https://doc.qt.io/qtforpython/) – Qt-based GUI framework
- Python 3.x
- Qt WebEngine (for 3D animation)
- Serial Communication (USB CDC / UART)
- HTML + Three.js (for 3D graph animation in landing page)

---

## 📦 Installation

```bash
pip install -r requirements.txt
python main.py
```
---

## Code Generation Notice

ⓘ This GUI was fully generated using OpenAI's ChatGPT, with real-time direction, feedback, and architectural decisions provided. I did not write any of the GUI code manually, but played the role of architect, tester, and integrator, ensuring the final application meets the standards for usability, structure, and design.
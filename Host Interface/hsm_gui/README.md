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

## Project Structure
<pre> ``` hsm_gui/ ├── main.py # Entry point for launching the GUI ├── README.md # Project overview and documentation ├── requirements.txt # Python dependencies ├── debug_log.txt # Optional debug log output │ ├── app/ │ ├── core/ # Core communication & protocol logic │ │ ├── device_comm.py │ │ ├── iv_store.py │ │ ├── logger.py │ │ ├── packet_builder.py │ │ └── packet_parser.py │ │ ├── resources/ # UI resources │ │ ├── style.qss │ │ ├── fonts/ │ │ │ └── Orbitron-Regular.ttf │ │ ├── icons/ │ │ │ ├── copy.png │ │ │ ├── download.png │ │ │ ├── monogram_logo.png │ │ │ ├── trustx_icon.png │ │ │ └── upload.png │ │ ├── images/ │ │ │ ├── blogger.png │ │ │ ├── github.png │ │ │ ├── gmail.png │ │ │ ├── instagram.png │ │ │ └── linkedin.png │ │ ├── animation/ │ │ │ ├── graph.html │ │ │ └── libs/ │ │ │ ├── 3d-force-graph.min.js │ │ │ └── three.min.js │ │ └── splash/ │ │ └── splash_screen.py │ │ ├── ui/ # Main UI pages │ │ ├── about_page.py │ │ ├── decrypt_page.py │ │ ├── encrypt_page.py │ │ ├── hashing_page.py │ │ ├── hmac_page.py │ │ ├── keydelete_page.py │ │ ├── keygen_page.py │ │ ├── keystore_page.py │ │ ├── landing_page.py │ │ ├── main_window.py │ │ ├── otp_page.py │ │ ├── rng_page.py │ │ └── sidebar.py │ │ │ │ └── widgets/ # Custom UI components │ │ ├── clock.py │ │ ├── debug_terminal.py │ │ ├── graph_widget.py │ │ └── toast.py │ │ └── utils/ # Utility functions │ ├── clipboard_utils.py │ └── file_utils.py ``` </pre>

## Code Generation Notice

ⓘ This GUI was fully generated using OpenAI's ChatGPT, with real-time direction, feedback, and architectural decisions provided. I did not write any of the GUI code manually, but played the role of architect, tester, and integrator, ensuring the final application meets the standards for usability, structure, and design.
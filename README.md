# Hardware Security Module over USB-CDC Class Device
A Hardware Security Module (HSM) that supports AES, RSA, and ECC encryption and decryption, along with hashing using SHA-224, SHA-256, SHA-384, and SHA-512 algorithms. It includes HMAC verification using SHA-224 or SHA-256 and features secure random number generation of various sizes. Secure key management is implemented by storing keys in internal flash memory in an encrypted format, derived from the processor’s Hardware Unique Key (HUK), with an encrypted backup copy stored externally on flash memory.  

To ensure persistent configuration storage across resets, a serial EEPROM is incorporated. Hardware security measures include readout protection for internal flash memory and restricted access to the Secure World from Non-Secure code using the GTZC feature of the STM32. Tamper protection is also implemented, monitoring both internal tamper signals and a predefined external GPIO pin for enclosure breaches, triggering erasure protocols for both internal and external flash memory if necessary.  

Currently, the PCB is being designed with the STM32H573RIV6 MCU at the core, featuring a USB-C receptacle as the sole interface. SWD pins are exposed for potential future flashing and debugging needs, and UART terminal pins are included for use as a serial data logger during debugging.


## PCB Design (Work in Progress)

### 🧷 2D Views
**Top View:**

![2D Top View](PCB/40.%20PCB%20View%202D%20Top%20Silkscreen.png)

**Bottom View:**

![2D Bottom View](PCB/42.%20PCB%20View%202D%20Bottom%20Silkscreen.png)

### 🧷 3D Views
**Top View:**

![Gerber Top View](PCB/45.%20PCB%20Gerber%20View%20Top.png)

**Right View:**

![Gerber Right View](PCB/46.%20PCB%20Gerber%20View%20Bottom.png)

### 🧷 Routed Views
**Top Routed View:**

![2D Top Routed View](PCB/47.%20PCB%20View%202D%20Top%20Routed.png)

**Bottom Routed View:**

![2D Bottom Routed View](PCB/48.%20PCB%20View%202D%20Bottom%20Routed.png)

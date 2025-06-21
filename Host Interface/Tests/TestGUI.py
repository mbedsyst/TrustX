import tkinter as tk
from tkinter import scrolledtext, messagebox
import serial
import threading

class HSMTestInterface:
    def __init__(self, root):
        self.root = root
        self.root.title("HSM USB CDC Test Interface")

        self.port_name = "COM4"
        self.baud_rate = 115200
        self.serial_port = None

        # ✅ Fill these with your hex strings
        hex_packets = [
            "F1 D0 F1 D0 01 01 00 32 F1 53 65 63 72 65 74 4B 65 79 31 32 33 34 35 36 37 E1 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 1
            "F1 D0 F1 D0 01 01 00 32 F1 53 65 63 72 65 74 4B 65 79 31 32 33 34 35 36 37 E2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 2
            ###"F1 D0 F1 D0 01 01 00 32 F2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 E1 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 3
            ###"F1 D0 F1 D0 01 01 00 32 F2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 E2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 4
            "F1 D0 F1 D0 01 01 00 32 F3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 E1 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 5
            "F1 D0 F1 D0 01 01 00 32 F3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 E2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 6
            "F1 D0 F1 D0 02 11 00 32 D1 53 65 63 72 65 74 4B 65 79 31 32 33 34 35 36 37 C1 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 8B AE 29 AC 0B 70 C3 E0 38 85 57 27 06 E3 7E F1 DE AD BE EF",  # Packet 7
            "F1 D0 F1 D0 02 11 00 32 D1 53 65 63 72 65 74 4B 65 79 31 32 33 34 35 36 37 C2 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 8B AE 29 AC 0B 70 C3 E0 38 85 57 27 06 E3 7E F1 DE AD BE EF",  # Packet 8
            ###"F1 D0 F1 D0 02 11 00 32 D2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 C1 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 8B AE 29 AC 0B 70 C3 E0 38 85 57 27 06 E3 7E F1 DE AD BE EF",  # Packet 9
            ###"F1 D0 F1 D0 02 11 00 32 D2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 C2 49 6E 69 74 69 61 6C 69 7A 61 74 69 6F 6E 56 56 8B AE 29 AC 0B 70 C3 E0 38 85 57 27 06 E3 7E F1 DE AD BE EF",  # Packet 10
            "F1 D0 F1 D0 04 31 00 31 B1 48 4D 41 43 4B 65 79 5F 46 6F 72 5F 41 75 74 68 65 6E 74 69 63 61 74 69 6F 6E 31 32 33 34 35 36 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 11
            "F1 D0 F1 D0 04 32 00 31 B1 48 4D 41 43 4B 65 79 5F 46 6F 72 5F 41 75 74 68 65 6E 74 69 63 61 74 69 6F 6E 31 32 33 34 35 36 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 12
            ###"F1 D0 F1 D0 04 31 00 31 B2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 13
            ###"F1 D0 F1 D0 04 32 00 31 B2 XX XX XX XX 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 14
            "F1 D0 F1 D0 04 31 00 31 B3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 15
            "F1 D0 F1 D0 04 32 00 31 B3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 45 6E 63 72 79 70 74 54 68 69 73 4E 6F 77 21 21 DE AD BE EF",  # Packet 16
            "F1 D0 F1 D0 07 51 00 00 00 00 DE AD BE EF",  # Packet 17
            ###"F1 D0 F1 D0 07 52 00 11 XX 53 65 63 72 65 74 4B 65 79 31 32 33 34 35 36 37 | DE AD BE EF",  # Packet 18
            ###"F1 D0 F1 D0 07 53 00 04 XX XX XX XX  DE AD BE EF",  # Packet 19
        ]
        self.packet_data = [self.hex_to_bytes(p) for p in hex_packets]

        self.build_gui()

    def build_gui(self):
        self.connect_btn = tk.Button(self.root, text="Connect", width=15, command=self.connect)
        self.connect_btn.grid(row=0, column=0, padx=10, pady=10)

        # Layout: 5 columns per row
        for i in range(19):
            btn = tk.Button(self.root, text=f"Send Packet {i+1}", width=15,
                            command=lambda i=i: self.send_packet(i))
            row = (i // 5) + 1
            col = i % 5
            btn.grid(row=row, column=col, padx=5, pady=5)

        # Terminal viewer spans across 5 columns
        self.terminal = scrolledtext.ScrolledText(self.root, width=80, height=20, state='disabled')
        self.terminal.grid(row=5, column=0, columnspan=5, padx=10, pady=10)

    def connect(self):
        try:
            self.serial_port = serial.Serial(self.port_name, self.baud_rate, timeout=0.5)
            self.print_terminal(f"[INFO] Connected to {self.port_name}")
            threading.Thread(target=self.read_from_serial, daemon=True).start()
        except Exception as e:
            messagebox.showerror("Connection Failed", str(e))

    def read_from_serial(self):
        while self.serial_port and self.serial_port.is_open:
            try:
                data = self.serial_port.readline()
                if data:
                    ascii_part = data.decode(errors='ignore').strip()
                    self.print_terminal(f"[RX] {data.hex()} | {ascii_part}")
            except Exception as e:
                self.print_terminal(f"[ERROR] {e}")

    def send_packet(self, index):
        if not self.serial_port or not self.serial_port.is_open:
            messagebox.showwarning("Not Connected", "Connect to the device first.")
            return

        data = self.packet_data[index]
        if not data:
            self.print_terminal(f"[WARN] Packet {index+1} is empty.")
            return

        try:
            self.serial_port.write(data)
            self.print_terminal(f"[TX] {data.hex()}")
        except Exception as e:
            self.print_terminal(f"[ERROR] Failed to send packet {index+1}: {e}")

    def print_terminal(self, message):
        self.terminal.config(state='normal')
        self.terminal.insert(tk.END, message + "\n")
        self.terminal.see(tk.END)
        self.terminal.config(state='disabled')

    def hex_to_bytes(self, hex_str):
        try:
            return bytes.fromhex(hex_str)
        except ValueError:
            return b''

if __name__ == "__main__":
    root = tk.Tk()
    app = HSMTestInterface(root)
    root.mainloop()

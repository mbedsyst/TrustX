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
            "E5 87 4D C2 00 00 99",  # Packet 1
            "",  # Packet 2
            "",  # Packet 3
            "",  # Packet 4
            "",  # Packet 5
            "",  # Packet 6
            "",  # Packet 7
            "",  # Packet 8
            "",  # Packet 9
            "",  # Packet 10
        ]
        self.packet_data = [self.hex_to_bytes(p) for p in hex_packets]

        self.build_gui()

    def build_gui(self):
        self.connect_btn = tk.Button(self.root, text="Connect", width=15, command=self.connect)
        self.connect_btn.grid(row=0, column=0, padx=10, pady=10)

        for i in range(10):
            btn = tk.Button(self.root, text=f"Send Packet {i+1}", width=15,
                            command=lambda i=i: self.send_packet(i))
            btn.grid(row=(i//5)+1, column=i%5, padx=5, pady=5)

        self.terminal = scrolledtext.ScrolledText(self.root, width=80, height=20, state='disabled')
        self.terminal.grid(row=3, column=0, columnspan=5, padx=10, pady=10)

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

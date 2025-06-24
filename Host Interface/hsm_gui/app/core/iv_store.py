# app/core/iv_store.py
import hashlib
import json
import os

STORE_FILE = "iv_store.json"

class IVStore:
    def __init__(self):
        self.entries = []
        self.load()

    def _hash_ciphertext(self, ciphertext: bytes) -> str:
        return hashlib.sha256(ciphertext).hexdigest()

    def add_entry(self, key_id: bytes, iv: bytes, ciphertext: bytes):
        entry = {
            "key_id": key_id.hex(),
            "iv": iv.hex(),
            "digest": self._hash_ciphertext(ciphertext)
        }
        self.entries.append(entry)
        self.save()

    def find_iv(self, key_id: bytes, ciphertext: bytes) -> bytes | None:
        digest = self._hash_ciphertext(ciphertext)
        for entry in self.entries:
            if entry["key_id"] == key_id.hex() and entry["digest"] == digest:
                return bytes.fromhex(entry["iv"])
        return None

    def save(self):
        with open(STORE_FILE, "w") as f:
            json.dump(self.entries, f)

    def load(self):
        if os.path.exists(STORE_FILE):
            with open(STORE_FILE, "r") as f:
                self.entries = json.load(f)

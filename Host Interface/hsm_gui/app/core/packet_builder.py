# app/core/packet_builder.py

import os

EOD_FLAG = b'\xDE\xAD\xBE\xEF'

def generate_txid() -> bytes:
    return os.urandom(4)

def build_packet(cmd: int, option: int, input_data: bytes, txid: bytes = None) -> (bytes, bytes):
    """
    Build the complete packet to send to device
    Returns : (packet, txid)    
    """

    if txid is None:
        txid = generate_txid()

    input_size = len(input_data).to_bytes(2, 'big')
    packet = (
        txid +
        cmd.to_bytes(1, 'big') +
        option.to_bytes(1, 'big') +
        input_size +
        input_data +
        EOD_FLAG
    )

    return packet, txid
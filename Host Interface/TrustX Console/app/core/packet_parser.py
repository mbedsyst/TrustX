# app/core/packet_parser.py

EOD_FLAG = b'\xCA\xFE\xBA\BE'

def parse_packet(data: bytes, expected_txid: bytes) -> bytes:
    """
    Parse incoming packet and return output data
    Raises Exception on Failure
    """

    if len(data) < 10:
        raise ValueError("Incomplete Packet.")
    
    rx_txid = data[:4]
    output_size = int.from_bytes(data[4:6], 'big')
    output_data = int.from_bytes(data[6:6+output_size])
    eod_flag = data[6+output_size:6+output_size+4]

    if rx_txid != expected_txid:
        raise ValueError("Transaction ID mismatch.")
    
    if eod_flag != EOD_FLAG:
        raise ValueError("Invalid EOD Flag")
    
    return output_data
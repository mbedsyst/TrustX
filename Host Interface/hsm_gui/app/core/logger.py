# app/core/logger.py

log_callback = None

def set_gui_log_callback(callback):
    global log_callback
    log_callback = callback

def log(message: str):
    from datetime import datetime
    timestamp = datetime.now().strftime("%H:%M:%S")

    formatted = f"[{timestamp}] {message}"

    # Write to file
    with open("debug_log.txt", "a") as f:
        f.write(formatted + "\n")

    # Write to GUI (if set)
    if log_callback:
        log_callback(formatted)
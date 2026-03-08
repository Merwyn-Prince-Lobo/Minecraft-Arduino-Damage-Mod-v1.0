"""
arduino_bridge.py
-----------------
Listens for damage signals from the Minecraft mod on localhost:25575,
then sends a trigger byte over serial to the Arduino.

Requirements:
    pip install pyserial

Usage:
    python arduino_bridge.py
    
    Edit SERIAL_PORT below to match your Arduino:
      Windows:  "COM3"  (check Device Manager)
      Linux:    "/dev/ttyUSB0"  or  "/dev/ttyACM0"
      macOS:    "/dev/cu.usbmodem..."
"""

import socket
import serial
import time

# ── CONFIG ────────────────────────────────────────────────────────────────────
SERIAL_PORT   = "/dev/ttyUSB0"       # <-- change this to your Arduino port
BAUD_RATE     = 9600
LISTEN_PORT   = 25575        # must match the port in ArduinoDamageModClient.java
TRIGGER_MS    = 10          # how long the relay stays on (ms) — controlled by Arduino
# ─────────────────────────────────────────────────────────────────────────────

def connect_arduino():
    while True:
        try:
            ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
            time.sleep(2)  # wait for Arduino bootloader
            print(f"[OK] Arduino connected on {SERIAL_PORT}")
            return ser
        except serial.SerialException as e:
            print(f"[WAIT] Arduino not found on {SERIAL_PORT}: {e}")
            print("       Check your port and try again in 3s...")
            time.sleep(3)

def main():
    arduino = connect_arduino()

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", LISTEN_PORT))
    server.listen(5)
    server.settimeout(1.0)
    print(f"[OK] Listening for Minecraft on port {LISTEN_PORT}...")
    print("      Start Minecraft now and load a world.\n")

    while True:
        try:
            conn, addr = server.accept()
            with conn:
                data = conn.recv(1024)
                if data and b"DAMAGE" in data:
                    print("[HIT] Damage detected — triggering Arduino!")
                    try:
                        arduino.write(b'1')
                    except serial.SerialException:
                        print("[WARN] Arduino disconnected, reconnecting...")
                        arduino = connect_arduino()
                        arduino.write(b'1')
        except socket.timeout:
            continue  # no connection this second, just loop
        except KeyboardInterrupt:
            print("\nStopping bridge.")
            break

    arduino.close()
    server.close()

if __name__ == "__main__":
    main()

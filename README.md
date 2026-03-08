# Arduino Damage Feedback — Minecraft Mod(JAVA 1.21.11)


Get real physical feedback when you take damage in Minecraft via a buzzer triggered by a relay.

## What it does
Every time your player takes damage in Minecraft, the mod sends a signal through a Python bridge to an Arduino Uno, which triggers a relay that buzzes a buzzer for 500ms.

## Requirements
- Minecraft 1.21.11 (Java Edition)
- Fabric Loader 0.18.2+
- Fabric API for 1.21.11
- Arduino Uno with CH340 USB chip
- 1-channel relay module
- 5V buzzer
- Python 3 + pyserial (`pip install pyserial`)
- Linux (tested on Ubuntu)

## Wiring
```
Arduino 5V   →  Relay VCC
Arduino GND  →  Relay GND
Arduino Pin7 →  Relay IN
Arduino 5V   →  Relay COM
```

## Setup

### 1. Minecraft Mod
- Build with `./gradlew build`
- Copy `build/libs/arduinodamage-1.0.0.jar` to `~/.minecraft/mods/`
- Install Fabric API for 1.21.11 into mods folder

### 2. Arduino
- Open `arduino_damage_feedback.ino` in Arduino IDE
- Select Board: Arduino Uno
- Select Port: `/dev/ttyUSB0`
- Upload the sketch
- **Close Arduino IDE completely** — this is important! Arduino IDE holds the serial port open and will block the Python bridge from connecting

### 3. Python Bridge
- Edit `arduino_bridge.py` and set `SERIAL_PORT = "/dev/ttyUSB0"`
- Open a terminal and run:
```bash
cd ~/Storage/arduino-damage-mod
python3 arduino_bridge.py
```
- Wait until you see:
```
[OK] Arduino connected on /dev/ttyUSB0
[OK] Listening for Minecraft on port 25575...
```
- Keep this terminal open while playing

### 4. Play
- Launch Minecraft with the Fabric profile
- Load a singleplayer world
- Take damage → relay clicks → buzzer beeps!

## Troubleshooting
- **Port not found:** Run `sudo apt remove brltty -y` — BRLTTY hijacks CH340 devices on Ubuntu
- **Permission denied on port:** Run `sudo usermod -a -G dialout $USER` then log out and back in
- **Port busy / can't connect:** Make sure Arduino IDE is fully closed before running the Python bridge
- **Mod not loading:** Make sure Fabric API jar is also in your mods folder
- **No relay click:** Check VCC/GND wiring on relay module — the relay LED should light up as soon as Arduino is powered


(I made it for a 40kv step up connected in series )

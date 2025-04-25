# XGIMI BLE Advertiser for ESPHome

This ESPHome project broadcasts a custom BLE advertisement that emulates the power-on command of a Bluetooth remote used by XGIMI projectors (e.g., Horizon, MoGo Pro, etc.).

It serves as a **fully wireless alternative** to a USB Bluetooth dongle passed through to Home Assistant — ideal for setups where you need to place the Bluetooth signal near the projector without running long USB cables or relying on unstable passthrough.

---

## 🚀 Features

- Emulates XGIMI Bluetooth remote "power on" behavior
- Sends BLE advertisement with configurable manufacturer token
- Broadcasts:
  - Manufacturer ID: `0x0046`
  - Service UUID: `0x1812` (HID)
  - Appearance: `961` (Remote Control)
  - Device name: `"Bluetooth 4.0 RC"`
- Triggered by a **Home Assistant switch entity**
- Fully ESPHome-compatible and OTA-updatable

---

## ⚡ Quick Start (1-Line Install via ESPHome `packages`)

Create a new ESPHome device and paste the following:

```yaml
packages:
  xgimi_ble: github://bchevreau/xgimi-ble-advertiser/esphome/xgimi_ble_advertiser.yaml@main

substitutions:
  name: xgimi-ble-advertiser
  friendly_name: XGIMI BLE Advertiser
  board: esp32dev  # or nodemcu-32s, lolin32, etc.
```

🔐 Required secrets.yaml entries
yaml
Copy
Edit
wifi_ssid: "YourWiFi"
wifi_password: "YourWiFiPassword"
xgimi_token: 5EEBCF58395438FFFFFFFF3043524B544D  # Replace with your own token
📌 The token is the 16-byte hex string used by your XGIMI projector's Bluetooth remote.

🧠 How to Use
After flashing, Home Assistant will discover a switch named:

cpp
Copy
Edit
switch.xgimi_ble_advertiser
Turning it ON will:

Broadcast your token via BLE for 5 seconds

Automatically turn the switch OFF after the broadcast

Place the ESP32 physically near your projector to ensure reliable signal.

🧰 Advanced
Written in C++ as a custom component

Can be adapted to broadcast other tokens for similar BLE wake-up behaviors

Fully compatible with ESPHome’s packages: feature

🙌 Credits
Based on Xgimi-4-Home-Assistant

Created by @bchevreau

🪪 License
MIT — use freely, contribute, or fork.

yaml
Copy
Edit

---

## ✅ Next Step

1. Copy this into your `README.md` (replacing the current file on GitHub)
2. Commit it with a message like:

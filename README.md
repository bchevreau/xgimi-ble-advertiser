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
- Triggered by a **Home Assistant button entity**
- Fully ESPHome-compatible and OTA-updatable
- Configurable:
  - 🔁 How long to broadcast
  - 🔁 How often to repeat advertisement during that time

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

---

## 🔐 Required `secrets.yaml` entries

```yaml
wifi_ssid: "YourWiFi"
wifi_password: "YourWiFiPassword"
xgimi_token: 5EEBCF58395438FFFFFFFF3043524B544D  # Replace with your own token
```

📌 The token is the 16-byte hex string used by your XGIMI projector's Bluetooth remote.

---

## 🔘 Home Assistant Entities Created

- `button.xgimi_ble_advertiser` → triggers the BLE broadcast
- `number.ble_broadcast_duration` → how long to broadcast (in seconds)
- `number.ble_rebroadcast_interval` → how often to repeat the broadcast (in seconds)

All values are configurable directly from the HA dashboard — no firmware flashing needed. (pending changes)

---

## 🧠 How It Works

- When the button is pressed:
  - It begins broadcasting your BLE token every X seconds
  - It stops after Y seconds
- Place your ESP32 close to the projector for best results
- Ideal for wall-mounted ESP32s or when avoiding USB passthrough issues

---

## 🧰 Advanced

- Written in C++ as a [custom component](esphome/xgimi_ble_advertiser.h)
- Can be adapted to broadcast other tokens for similar BLE wake-up behaviors
- Fully compatible with ESPHome’s `packages:` feature
- BLE advertisement includes service UUID and appearance for XGIMI compatibility

---

## 🙌 Credits

- Based on [Xgimi-4-Home-Assistant](https://github.com/manymuch/Xgimi-4-Home-Assistant)
- BLE protocol analysis confirmed via Bluetooth logs
- Created by [@bchevreau](https://github.com/bchevreau)

---

## 🪪 License

MIT — use freely, contribute, or fork.

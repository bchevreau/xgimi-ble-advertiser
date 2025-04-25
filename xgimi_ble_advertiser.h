#include "esphome.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

class XgimiBleAdvertiser : public Component {
 public:
  void start_advertising(const char* hex_token) {
    ESP_LOGI("XgimiBLE", "Starting BLE advertisement with token: %s", hex_token);

    uint8_t token_bytes[16];
    if (!parse_token(hex_token, token_bytes)) {
      ESP_LOGW("XgimiBLE", "Invalid token format. Token must be 32 hex characters.");
      return;
    }

    // Set the BLE device name
    esp_ble_gap_set_device_name("Bluetooth 4.0 RC");

    // Create the full advertisement data
    uint8_t raw_adv_data[3 + 2 + 16 + 3 + 3] = {
      0x02, 0x01, 0x06,                     // Flags
      0x11, 0xFF, 0x46, 0x00,               // Manufacturer Data: len=0x11, type=0xFF, company ID=0x0046
      // token_bytes will go here [16 bytes]
      0x03, 0x03, 0x12, 0x18,               // Complete List of 16-bit Service UUIDs: 0x1812
      0x03, 0x19, 0xC1, 0x03                // Appearance: 961 (0x03C1 little endian)
    };

    memcpy(&raw_adv_data[7], token_bytes, 16);

    // Stop previous advertisements
    esp_ble_gap_stop_advertising();

    // Configure raw advertisement
    esp_err_t err = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
    if (err != ESP_OK) {
      ESP_LOGW("XgimiBLE", "Failed to configure advertisement data. Error code: %d", err);
      return;
    }

    delay(100);  // Let the BLE stack process config

    // Set advertisement parameters
    esp_ble_adv_params_t adv_params = {};
    adv_params.adv_int_min = 0x20;
    adv_params.adv_int_max = 0x40;
    adv_params.adv_type = ADV_TYPE_IND;
    adv_params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
    adv_params.channel_map = ADV_CHNL_ALL;
    adv_params.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;

    err = esp_ble_gap_start_advertising(&adv_params);
    if (err != ESP_OK) {
      ESP_LOGW("XgimiBLE", "Failed to start advertising. Error code: %d", err);
    }
  }

  void stop_advertising() {
    ESP_LOGI("XgimiBLE", "Stopping BLE advertising.");
    esp_ble_gap_stop_advertising();
  }

 private:
  bool parse_token(const char* hex, uint8_t* out_bytes) {
    if (strlen(hex) != 32) return false;
    for (int i = 0; i < 16; i++) {
      char byte_str[3] = { hex[i * 2], hex[i * 2 + 1], '\0' };
      out_bytes[i] = (uint8_t)strtol(byte_str, nullptr, 16);
    }
    return true;
  }
};

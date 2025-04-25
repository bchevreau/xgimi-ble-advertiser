#include "esphome.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"

class XgimiBleAdvertiser : public Component {
 public:
  void start_advertising(const char* hex_token) {
    ESP_LOGI("XgimiBLE", "Starting BLE advertising with token: %s", hex_token);

    uint8_t token_bytes[16];
    if (!parse_token(hex_token, token_bytes)) {
      ESP_LOGW("XgimiBLE", "Invalid token format.");
      return;
    }

    uint8_t raw_adv_data[] = {
      0x02, 0x01, 0x06,  // Flags
      0x11, 0xFF,        // Length + Manufacturer Specific
      0x46, 0x00,        // Manufacturer ID (0x0046 LE)
      // Followed by 16-byte token
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    memcpy(&raw_adv_data[7], token_bytes, 16);

    esp_ble_gap_stop_advertising();  // Reset any active session
    esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
    delay(50);  // Wait for config

    esp_ble_adv_params_t adv_params = {};
    adv_params.adv_int_min = 0x20;
    adv_params.adv_int_max = 0x40;
    adv_params.adv_type = ADV_TYPE_IND;
    adv_params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
    adv_params.channel_map = ADV_CHNL_ALL;
    adv_params.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;

    esp_ble_gap_start_advertising(&adv_params);
  }

  void stop_advertising() {
    ESP_LOGI("XgimiBLE", "Stopping BLE advertising.");
    esp_ble_gap_stop_advertising();
  }

 private:
  bool parse_token(const char* hex, uint8_t* out_bytes) {
    // Expect exactly 32 hex characters (16 bytes)
    if (strlen(hex) != 32) return false;
    for (int i = 0; i < 16; i++) {
      char byte_str[3] = { hex[i * 2], hex[i * 2 + 1], '\0' };
      out_bytes[i] = (uint8_t)strtol(byte_str, nullptr, 16);
    }
    return true;
  }
};

#include "esphome.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"

class XgimiBleAdvertiser : public Component {
 public:
  void start_advertising() {
    esp_bluedroid_status_t bt_status = esp_bluedroid_get_status();
    if (bt_status != ESP_BLUEDROID_STATUS_ENABLED) {
      ESP_LOGW("XgimiBLE", "Bluetooth not enabled.");
      return;
    }

    // Stop existing advertisement
    esp_ble_gap_stop_advertising();

    // Set raw advertisement data
    uint8_t raw_adv_data[] = {
      0x02, 0x01, 0x06,  // Flags
      0x11, 0xFF,        // Length + Manufacturer Specific (0xFF)
      0x46, 0x00,        // Manufacturer ID (0x0046, little endian)
      0x5E, 0xEB, 0xCF, 0x58, 0x39, 0x54, 0x38,
      0xFF, 0xFF, 0xFF, 0x30, 0x43, 0x52, 0x4B, 0x54, 0x4D
    };

    esp_ble_adv_data_t adv_data = {};
    adv_data.set_scan_rsp = false;
    adv_data.include_name = false;
    adv_data.include_txpower = false;
    adv_data.min_interval = 0x20;
    adv_data.max_interval = 0x40;
    adv_data.appearance = 0x00;
    adv_data.manufacturer_len = sizeof(raw_adv_data) - 3;
    adv_data.p_manufacturer_data = &raw_adv_data[3];
    adv_data.service_data_len = 0;
    adv_data.p_service_data = nullptr;
    adv_data.service_uuid_len = 0;
    adv_data.p_service_uuid = nullptr;
    adv_data.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

    esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));

    // Wait briefly to ensure config goes through
    delay(50);

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
    esp_ble_gap_stop_advertising();
  }
};

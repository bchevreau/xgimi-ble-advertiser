#include "esphome.h"
#include "esp_gap_ble_api.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_log.h"

namespace esphome {
namespace xgimi_ble_advertiser {

static const char *const TAG = "xgimi_ble_advertiser";

class XgimiBleAdvertiser : public Component {
 public:
  void setup() override {
    ESP_LOGI(TAG, "Setting up XGIMI BLE advertiser...");

    // Initialize BLE stack if not already started
    if (!btStarted()) {
      ESP_LOGI(TAG, "Starting Bluetooth controller...");
      esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
      esp_bt_controller_init(&bt_cfg);
      esp_bt_controller_enable(ESP_BT_MODE_BLE);
      esp_bluedroid_init();
      esp_bluedroid_enable();
    } else {
      ESP_LOGI(TAG, "Bluetooth controller already started.");
    }

    this->advertising_active_ = false;
    this->last_rebroadcast_time_ = 0;
  }

  void loop() override {
    if (!this->advertising_active_)
      return;

    uint32_t now = millis();

    // Stop advertising after the configured duration
    if (now - this->advertising_start_time_ > (get_advertisement_duration() * 1000UL)) {
      stop_advertising();
      this->last_rebroadcast_time_ = now;
    }

    // Check if we need to rebroadcast after interval
    if (!this->advertising_active_ && get_advertisement_interval() > 0) {
      if (now - this->last_rebroadcast_time_ > (get_advertisement_interval() * 1000UL)) {
        start_advertising();
      }
    }
  }

  void start_advertising() {
    if (this->advertising_active_) {
      ESP_LOGW(TAG, "Already advertising, skipping...");
      return;
    }

    ESP_LOGI(TAG, "Starting BLE advertising...");

    uint8_t adv_data[16] = {
        0x5E, 0xEB, 0xCF, 0x58, 0x39, 0x54, 0x38, 0xFF,
        0xFF, 0xFF, 0x30, 0x43, 0x52, 0x4B, 0x54, 0x4D};

    esp_ble_gap_config_adv_data_raw(adv_data, sizeof(adv_data));

    esp_ble_adv_params_t adv_params = {
        .adv_int_min = 0x20,
        .adv_int_max = 0x40,
        .adv_type = ADV_TYPE_NONCONN_IND,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .channel_map = ADV_CHNL_ALL,
        .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    };

    esp_ble_gap_start_advertising(&adv_params);

    this->advertising_start_time_ = millis();
    this->advertising_active_ = true;
  }

  void stop_advertising() {
    if (!this->advertising_active_) {
      ESP_LOGW(TAG, "Not advertising, nothing to stop...");
      return;
    }

    ESP_LOGI(TAG, "Stopping BLE advertising...");
    esp_ble_gap_stop_advertising();
    this->advertising_active_ = false;
  }

  // Helper methods to get settings from Home Assistant
  float get_advertisement_duration() {
    if (id(ble_advertisement_duration).has_state()) {
      return id(ble_advertisement_duration).state;
    }
    return 5.0;  // default 5s if not set
  }

  float get_advertisement_interval() {
    if (id(ble_advertisement_interval).has_state()) {
      return id(ble_advertisement_interval).state;
    }
    return 0.0;  // 0 = no rebroadcast by default
  }

 protected:
  bool advertising_active_;
  uint32_t advertising_start_time_;
  uint32_t last_rebroadcast_time_;
};

}  // namespace xgimi_ble_advertiser
}  // namespace esphome

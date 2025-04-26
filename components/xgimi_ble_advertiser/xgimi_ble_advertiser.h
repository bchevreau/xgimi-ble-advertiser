#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esp_gap_ble_api.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_log.h"
#include "esp_timer.h"

namespace esphome {
namespace xgimi_ble_advertiser {

static const char *const TAG = "xgimi_ble_advertiser";

class XgimiBleAdvertiser : public Component {
 public:
  void set_ble_token_sensor(text_sensor::TextSensor *sensor) {
    this->ble_token_sensor_ = sensor;
  }

  void setup() override {
    ESP_LOGI(TAG, "Setting up XGIMI BLE advertiser...");

    ESP_LOGI(TAG, "Starting Bluetooth controller...");
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();
  }

  void loop() override {
    if (!this->advertising_active_)
      return;

    uint32_t now = esp_timer_get_time() / 1000;

    if (now - this->advertising_start_time_ > (this->advertisement_duration_ * 1000UL)) {
      stop_advertising();
    }
  }

  void start_advertising(float duration_seconds = 5.0f) {
    if (this->advertising_active_) {
      ESP_LOGW(TAG, "Already advertising, skipping...");
      return;
    }

    if (this->ble_token_sensor_ == nullptr) {
      ESP_LOGE(TAG, "BLE token sensor not set!");
      return;
    }

    std::string token = this->ble_token_sensor_->state;
    if (token.empty() || token.length() != 32) {
      ESP_LOGE(TAG, "Invalid BLE token format!");
      return;
    }

    uint8_t adv_data[16];
    for (int i = 0; i < 16; i++) {
      std::string byte_str = token.substr(i * 2, 2);
      adv_data[i] = strtol(byte_str.c_str(), nullptr, 16);
    }

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

    this->advertising_start_time_ = esp_timer_get_time() / 1000;
    this->advertising_active_ = true;
    this->advertisement_duration_ = duration_seconds;
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

 protected:
  bool advertising_active_{false};
  uint32_t advertising_start_time_{0};
  float advertisement_duration_{5.0};
  text_sensor::TextSensor *ble_token_sensor_{nullptr};
};

}  // namespace xgimi_ble_advertiser
}  // namespace esphome

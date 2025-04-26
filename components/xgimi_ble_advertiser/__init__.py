import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID

CONF_BLE_TOKEN_SENSOR = "ble_token_sensor"

xgimi_ble_advertiser_ns = cg.esphome_ns.namespace('xgimi_ble_advertiser')
XgimiBleAdvertiser = xgimi_ble_advertiser_ns.class_('XgimiBleAdvertiser', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(XgimiBleAdvertiser),
    cv.Required(CONF_BLE_TOKEN_SENSOR): cv.use_id(cg.TextSensor),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    token_sensor = await cg.get_variable(config[CONF_BLE_TOKEN_SENSOR])
    cg.add(var.set_ble_token_sensor(token_sensor))

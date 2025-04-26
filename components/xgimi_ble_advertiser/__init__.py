import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID

xgimi_ble_advertiser_ns = cg.esphome_ns.namespace('xgimi_ble_advertiser')
XgimiBleAdvertiser = xgimi_ble_advertiser_ns.class_('XgimiBleAdvertiser', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(XgimiBleAdvertiser),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

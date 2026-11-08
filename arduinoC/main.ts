//% color="#FF6347" iconWidth=40 iconHeight=40
namespace max30102 {

    function ensureDriver() {
        Generator.addInclude('max30102_include', '#include "DFRobot_BloodOxygen_S.h"');
        Generator.addObject('MAX30102', 'DFRobot_BloodOxygen_S_I2C', 'MAX30102(&Wire, 0x57);');
        Generator.addObject('max30102_ok', 'bool', 'max30102_ok;');
        Generator.addSetup('max30102_begin', 'max30102_ok = MAX30102.begin();');
        Generator.addSetup('max30102_start', 'MAX30102.sensorStartCollect();');
    }

    //% block="初始化 MAX30102" blockType="command"
    export function init(parameter: any, block: any) {
        ensureDriver();
    }

    //% block="---"
    export function noteSep1() {
    }

    //% block="传感器连接成功？" blockType="boolean"
    export function isReady(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode(['max30102_ok', Generator.ORDER_UNARY_POSTFIX]);
    }

    //% block="更新心率血氧数据" blockType="command"
    export function update(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode('MAX30102.getHeartbeatSPO2();');
    }

    //% block="获取心率 (BPM)" blockType="reporter"
    export function getHR(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode(['MAX30102._sHeartbeatSPO2.Heartbeat', Generator.ORDER_UNARY_POSTFIX]);
    }

    //% block="获取血氧 (SPO2)" blockType="reporter"
    export function getSPO2(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode(['MAX30102._sHeartbeatSPO2.SPO2', Generator.ORDER_UNARY_POSTFIX]);
    }

    //% block="获取传感器温度 (℃)" blockType="reporter"
    export function getTemp(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode(['MAX30102.getTemperature_C()', Generator.ORDER_UNARY_POSTFIX]);
    }

    //% block="---"
    export function noteSep2() {
    }

    //% block="停止采集" blockType="command"
    export function stop(parameter: any, block: any) {
        ensureDriver();
        Generator.addCode('MAX30102.sensorEndCollect();');
    }
}

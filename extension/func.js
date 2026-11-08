class Func {
    constructor(runtime, extensionId) {
        this.runtime = runtime;
        this.extensionId = extensionId;
    }

    _ensure(generator) {
        generator.addInclude('DFRobot_BloodOxygen_S.h', 'DFRobot_BloodOxygen_S.h');
        generator.addObject('MAX30102', 'DFRobot_BloodOxygen_S_I2C MAX30102(&Wire, 0x57);');
        generator.addObject('max30102_ok', 'bool max30102_ok;');
        generator.addSetup('max30102_begin', 'max30102_ok = MAX30102.begin();', 0, true);
        generator.addSetup('max30102_start', 'MAX30102.sensorStartCollect();', 0, true);
    }

    init(generator, block, parameter) {
        this._ensure(generator);
        return '';
    }

    isReady(generator, block, parameter) {
        this._ensure(generator);
        return ['max30102_ok', generator.ORDER_ATOMIC];
    }

    update(generator, block, parameter) {
        this._ensure(generator);
        return 'MAX30102.getHeartbeatSPO2();';
    }

    getHR(generator, block, parameter) {
        this._ensure(generator);
        return ['MAX30102._sHeartbeatSPO2.Heartbeat', generator.ORDER_ATOMIC];
    }

    getSPO2(generator, block, parameter) {
        this._ensure(generator);
        return ['MAX30102._sHeartbeatSPO2.SPO2', generator.ORDER_ATOMIC];
    }

    getTemp(generator, block, parameter) {
        this._ensure(generator);
        return ['MAX30102.getTemperature_C()', generator.ORDER_ATOMIC];
    }

    stop(generator, block, parameter) {
        this._ensure(generator);
        return 'MAX30102.sensorEndCollect();';
    }
}

export default Func;

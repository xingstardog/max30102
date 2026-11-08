# MAX30102 心率血氧传感器模块 V2

Mind+ 用户库扩展，用于通过 I2C 读取 MAX30102 心率血氧传感器模块（DFRobot SEN0344，硬件 V2）的心率、血氧饱和度和芯片温度。

> 本模块集成了心率血氧算法与微控制器，直接输出数值，主控不需要做 PPG 波形处理。

## 支持主控

| 主控 | board | 说明 |
|------|-------|------|
| micro:bit | `microbit` | 已适配 |
| 掌控板 | `esp32` | 已适配 |

仅支持 **Windows 桌面版 Mind+**（`platform` 为 `win`）的上传模式。

## 技术规格

| 项目 | 参数 |
|------|------|
| 供电电压 | 3.3V |
| 工作电流 | < 15mA |
| 通信方式 | 模块支持 I2C / UART，**本扩展只实现 I2C** |
| I2C 地址 | 0x57 |
| 工作温度范围 | -40℃ ~ 85℃ |
| 模块尺寸 | 18 × 22 mm |

## 接线

模块引脚丝印与功能：

| 引脚 | 丝印 | 功能 |
|------|------|------|
| 1 | 3V3 | 电源正极 |
| 2 | GND | 电源负极 |
| 3 | SCL | I2C 时钟线 |
| 4 | SDA | I2C 数据线 |
| 5 | RST | 复位 |
| 9 | RX | 串口接收（本扩展不使用） |
| 10 | TX | 串口发送（本扩展不使用） |
| 11 | INT | 数据更新时输出脉冲（本扩展不使用） |

本扩展只用到前 4 个引脚：

| 模块 | micro:bit | 掌控板 |
|------|-----------|--------|
| 3V3 | 3V3 | 3V3 |
| GND | GND | GND |
| SCL | P19 | P22 |
| SDA | P20 | P21 |

## 积木说明

| 积木 | 作用 |
|------|------|
| 初始化 MAX30102 | 创建传感器对象，在 `setup()` 中调用 `begin()` 与 `sensorStartCollect()`。**建议放在程序最开头** |
| 传感器连接成功？ | 返回 `begin()` 的初始化结果，未接线或接触不良时为「假」 |
| 更新心率血氧数据 | 读取一次传感器，把结果存进对象。**取值前必须先执行它** |
| 获取心率 (BPM) | 最近一次更新得到的心率 |
| 获取血氧 (SPO2) | 最近一次更新得到的血氧饱和度 |
| 获取传感器温度 (℃) | **芯片板载**温度，不是人体体温 |
| 停止采集 | 调用 `sensorEndCollect()`，停止采集以降低功耗 |

## 两个必须知道的特性

### 1. 数据每 4 秒才更新一次

传感器内部算法每 **4 秒**刷新一次结果。这 4 秒内反复读取，拿到的都是同一个值。所以循环里不要快速空转，建议每次读取后等待 4 秒：

```
初始化 MAX30102
如果 传感器连接成功？ 那么
    重复执行：
        更新心率血氧数据
        显示 获取心率 (BPM)
        显示 获取血氧 (SPO2)
        等待 4 秒
否则
    显示 "传感器未连接"
```

### 2. 三个「获取」积木不会自己去读传感器

它们只取上一次「更新心率血氧数据」存下来的值。只放「获取心率」而不放「更新」，读数会一直是 0。

刚启动时传感器需要几秒预热，前几次读数可能是 0 或异常值，属于正常现象。

## 佩戴建议

- **用绑带把手指固定在模块上**，不要用手指直接按压。按压力度变化会改变血流，导致数值跳动。
- 指腹贴合光学窗口，测量过程中保持位置不动。
- 避免在强光直射下测量。

> ⚠️ 本模块**不是专业医疗仪器**，不能用于诊断或治疗，仅适合教学与创客项目。

## 生成的代码示例

上面那段积木会生成类似这样的 Arduino C 代码：

```cpp
#include "DFRobot_BloodOxygen_S.h"

DFRobot_BloodOxygen_S_I2C MAX30102(&Wire, 0x57);
bool max30102_ok;

void setup() {
    max30102_ok = MAX30102.begin();
    MAX30102.sensorStartCollect();
}

void loop() {
    if (max30102_ok) {
        MAX30102.getHeartbeatSPO2();
        // 心率：MAX30102._sHeartbeatSPO2.Heartbeat
        // 血氧：MAX30102._sHeartbeatSPO2.SPO2
        // 温度：MAX30102.getTemperature_C()
        delay(4000);
    }
}
```

## 版本记录

| 版本 | 说明 |
|------|------|
| 0.0.4 | 驱动精简为纯 I2C：移除 `DFRobot_BloodOxygen_S` 中的 UART 类（SoftWare/HardWareUart）及实现、移除 `DFRobot_RTU.h/.cpp` 依赖、清理 8 个 UART 专用宏与 CRC16 校验函数。驱动体积 46.0 KB → 11.2 KB（-76%） |
| 0.0.3 | 保存 `begin()` 返回值，新增「传感器连接成功？」布尔积木；文档补充 4 秒更新周期、绑带佩戴建议、技术规格 |
| 0.0.2 | 修正 namespace 与 id 不一致导致的输入框失效；圆形积木补运算符优先级；对象声明补分号；补齐驱动 .cpp；关闭驱动调试输出；「获取体温」更正为「获取传感器温度」；新增「停止采集」积木；封面图与图标规范化 |
| 0.0.1 | 初始骨架 |

## 依赖

`arduinoC/libraries/DFRobot_BloodOxygen_S.h/.cpp` 来自 DFRobot 官方开源库 **V1.1.0**：
https://github.com/DFRobot/DFRobot_BloodOxygen_S

MIT 协议，详见 `LICENSE`。

### 相对官方原版的本地修改

本扩展只支持 I2C，因此驱动做了三处裁剪，改动处均已用 `BEGIN/END Mind+ local modification` 注释块标出：

1. **注释掉 `ENABLE_DBG`** —— 避免驱动往串口打印调试信息。Mind+ 生成的程序不会调用 `Serial.begin()`，开着只会拖慢采集。
2. **删除 UART 类与实现** —— `DFRobot_BloodOxygen_S_SoftWareUart`、`DFRobot_BloodOxygen_S_HardWareUart` 及其 `begin/writeReg/readReg` 实现全部移除。
3. **删除 `DFRobot_RTU.h/.cpp`** —— 这两个文件是 Modbus RTU 实现，只被上面的 UART 类使用。同时移除了 `DFRobot_RTU.h`、`SoftwareSerial.h`、`HardwareSerial.h` 三行 include 和 8 个 UART 专用宏（`MAX_SIZE`、`DEVICE_ADDRESS`、`BROADCAST_ADDRESS`、`RTU_READ_REG_CMD`、`RTU_WRITE_REG_CMD`、`RTU_WRITE_MULTIPLE_REG_CMD`、`SERIAL_DATA_BUF_MAX_SIZE`、`DEV_ADDRESS`）。
4. **删除 `calculate_CRC()`** —— Modbus CRC16 校验函数，随 RTU 一起失去调用者。`static` 函数留着只会被编译器报「定义但未使用」。

> 如果要恢复 UART 支持，请从官方仓库重新取回 `.h` / `.cpp` 和 `DFRobot_RTU` 两个文件，不要在本仓库里手工加回——版本容易错配。

## 参考资料

- 产品资料与使用教程：https://wiki.dfrobot.com.cn/_SKU_SEN0344_Fermion_MAX30102_Heart_Rate_and_Oximeter_Sensor_V2
- Mind+ 用户库开发文档：https://mindplus.dfrobot.com.cn/ext-api

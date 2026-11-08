# MAX30102 心率血氧传感器 Mind+ 扩展（V2）

`MAX30102` 是一个适用于 Mind+ V2 的传感器扩展，通过 I2C 读取 MAX30102 心率血氧传感器模块（DFRobot SEN0344，硬件 V2）的心率、血氧饱和度和芯片温度。

模块内置心率血氧算法与微控制器，直接输出数值，主控不需要处理 PPG 波形。可用于可穿戴、家庭健康测量、长时间心率监测等教学与创客项目。

支持产品：Fermion MAX30102 心率血氧传感器 V2.0（`SEN0344`）

> 本仓库基于官方模板 [mindplus-ext2-builder](https://gitee.com/mind-plus/mindplus-ext2-builder) 开发。

## 环境要求

- **Mind+ V2.0.7 或以上**
- 仅支持**上传模式**
- 开发构建需要 Node.js（推荐 v16.20.2，v22 实测也可）与 Git

## 支持主控

| 主控 | deviceId | 最低版本 |
|------|----------|----------|
| micro:bit V2 | `dev-DFRobot-microbitV2` | ^0.0.4 |
| 掌控板 | `dev-DFRobot-handpyEsp32` | ^0.0.3 |

仅在这两个主板上测试通过。驱动本身基于 `Wire`，理论上其他带硬件 I2C 的主板也能用，但未经实测，因此未在 `supportDevices` 中声明。

## 接线

模块使用 I2C 通信，地址固定 **0x57**。

| 引脚 | 丝印 | 功能 |
|------|------|------|
| 1 | 3V3 | 电源正极 |
| 2 | GND | 电源负极 |
| 3 | SCL | I2C 时钟线 |
| 4 | SDA | I2C 数据线 |
| 5 | RST | 复位 |
| 9 / 10 | RX / TX | 串口（本扩展不使用） |
| 11 | INT | 数据更新脉冲（本扩展不使用） |

本扩展只用到前 4 个：

| 模块 | micro:bit | 掌控板 |
|------|-----------|--------|
| 3V3 | 3V3 | 3V3 |
| GND | GND | GND |
| SCL | P19 | P22 |
| SDA | P20 | P21 |

技术规格：供电 3.3V，工作电流 < 15mA，工作温度 -40℃ ~ 85℃，模块尺寸 18 × 22 mm。

## 积木

| 积木 | opcode | 说明 |
|------|--------|------|
| 初始化 MAX30102 | `init` | 创建传感器对象，在 `setup()` 中调用 `begin()` 与 `sensorStartCollect()` |
| 传感器连接成功？ | `isReady` | 返回 `begin()` 的结果，未接线或接触不良时为「假」 |
| 更新心率血氧数据 | `update` | 读取一次传感器，结果存入对象。**取值前必须先执行它** |
| 获取心率 (BPM) | `getHR` | 最近一次更新得到的心率 |
| 获取血氧 (SPO2) | `getSPO2` | 最近一次更新得到的血氧饱和度 |
| 获取传感器温度 (℃) | `getTemp` | **芯片板载**温度，不是人体体温 |
| 停止采集 | `stop` | 调用 `sensorEndCollect()`，降低功耗 |

## 示例程序

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

### 两个必须知道的特性

**1. 数据每 4 秒才更新一次。** 传感器内部算法每 4 秒刷新一次结果，这期间反复读取拿到的都是同一个值。循环里不要快速空转，建议每次读取后等待 4 秒。

**2. 三个「获取」积木不会自己去读传感器。** 它们只取上一次「更新心率血氧数据」存下来的值。只放「获取心率」而不放「更新」，读数会一直是 0。

刚启动时传感器需要几秒预热，前几次读数可能是 0 或异常值，属正常现象。

## 佩戴建议

- **用绑带把手指固定在模块上**，不要用手指直接按压。按压力度变化会改变血流，导致数值跳动。
- 指腹贴合光学窗口，测量过程中保持位置不动。
- 避免在强光直射下测量。

> ⚠️ 本模块**不是专业医疗仪器**，不能用于诊断或治疗，仅适合教学与创客项目。

## 生成的代码

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
    // 心率 MAX30102._sHeartbeatSPO2.Heartbeat
    // 血氧 MAX30102._sHeartbeatSPO2.SPO2
    // 温度 MAX30102.getTemperature_C()
    delay(4000);
  }
}
```

## 开发与构建

```bash
npm install      # 首次
npm run build    # 产出 build/ext-xingstar-max30102@<version>/
```

测试方式：Mind+ V2 打开上传模式 → 右上角设置 → 扩展 → 左下角「加载测试扩展」→ 选择 `build/ext-xingstar-max30102@<version>/config.json`。

> 开发阶段的扩展**不能保存项目文件**，只能测试。

### 目录结构

```
extension/
├── index.js                  积木外观与参数定义
├── func.js                   积木对应的代码生成
├── icon/
│   ├── blockIcon.svg         积木上的图标（白色）
│   └── menuIcon.svg          左侧分类菜单图标（积木色）
├── locales/
│   ├── zh-cn.json
│   ├── en.json
│   └── index.js              构建时自动生成
└── public/
    ├── config.json           扩展元信息
    ├── cover.png             封面 600×372
    └── libraries/
        └── DFRobot_BloodOxygen_S/   Arduino 驱动（纯 I2C）
```

## 与 V1 版本的关系

`config.json` 中的 `compatibilityId` 为 `xingstar-max30102-thirdex`，对应 V1 用户库 `Xingstar/max30102`。7 个积木的 `opcode` 与 V1 `main.ts` 中的函数名完全一致（`init` / `isReady` / `update` / `getHR` / `getSPO2` / `getTemp` / `stop`），因此 V1 版本保存的项目文件可以在 Mind+ V2 中正常打开。

> V1 版本中的两个 `---` 分隔积木没有移植。V2 没有对应的分隔块机制，强行保留会变成两个内容为 `---` 的实体积木。

## 版本记录

| 版本 | 说明 |
|------|------|
| 0.1.0 | 首个 Mind+ V2 版本。7 个积木从 V1 0.0.4 移植，opcode 保持一致并声明 `compatibilityId`，旧的 V1 项目可继续打开 |

## 依赖与修改说明

`extension/public/libraries/DFRobot_BloodOxygen_S/` 来自 DFRobot 官方开源库 **V1.1.0**：
https://github.com/DFRobot/DFRobot_BloodOxygen_S

MIT 协议，见 `LICENSE`。相对官方原版的修改（源码中以 `BEGIN/END Mind+ local modification` 注释块标出）：

1. 注释掉 `ENABLE_DBG`，不向串口输出调试信息；
2. 删除 UART 类 `DFRobot_BloodOxygen_S_SoftWareUart` / `_HardWareUart` 及其实现；
3. 删除 `DFRobot_RTU` 依赖、`SoftwareSerial`/`HardwareSerial` 头文件与 8 个 UART 专用宏；
4. 删除仅服务于 Modbus 的 `calculate_CRC()`。

驱动体积由 46.0 KB 降至 11.2 KB。如需恢复 UART 支持，请从官方仓库重新取回文件，不要在本仓库手工加回。

## 参考资料

- 产品资料与使用教程：https://wiki.dfrobot.com.cn/_SKU_SEN0344_Fermion_MAX30102_Heart_Rate_and_Oximeter_Sensor_V2
- Mind+ V2 扩展开发总览：https://mindplus.dfrobot.com.cn/mp2/Extensions/ExtDevelopmentDocs/extension-development-overview/
- Mind+ V2 上传模式开发手册：https://mindplus.dfrobot.com.cn/mp2/Extensions/ExtDevelopmentDocs/upload-mode-extension-development-guide/

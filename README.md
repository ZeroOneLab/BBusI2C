# BBusI2C

![Image](https://p9-flow-imagex-sign.byteimg.com/tos-cn-i-a9rns2rl98/rc/online_import/05e53c13acbc4549a5dd17d6841c6312~tplv-noop.jpeg?rk3s=49177a0b&x-expires=1772245517&x-signature=tmIZc%2BTVYHm%2FPAkHosflgirmY%2Fo%3D&resource_key=7053ea1f-32c0-4cda-aedb-68f744661b00&resource_key=7053ea1f-32c0-4cda-aedb-68f744661b00)

![Image](https://p9-flow-imagex-sign.byteimg.com/tos-cn-i-a9rns2rl98/rc/online_import/9a6cfbc80b4444ac809c5b16b0d45127~tplv-noop.jpeg?rk3s=49177a0b&x-expires=1772245517&x-signature=kQHHkYTNpCsf7H5ZjoNCQQ93EAs%3D&resource_key=10bda3d3-347b-4bb4-8119-1a0389894db0&resource_key=10bda3d3-347b-4bb4-8119-1a0389894db0)

![Image](https://p9-flow-imagex-sign.byteimg.com/tos-cn-i-a9rns2rl98/rc/online_import/bbac07da52a14ae88dfb95ab5553f1c4~tplv-noop.jpeg?rk3s=49177a0b&x-expires=1772245517&x-signature=%2FHXEtBjeTdQd1ull6Kkak89JD%2B8%3D&resource_key=c9e81d62-fd79-4f1c-85e9-f25eba8b6078&resource_key=c9e81d62-fd79-4f1c-85e9-f25eba8b6078)

一款**高通用性、分层解耦**的软件I2C驱动框架，专为嵌入式MCU设计，解决硬件I2C资源不足、传统软件I2C代码冗余、跨平台移植困难等问题，支持多路总线并行、独立速度配置，适配STM32等主流MCU平台。

## 关键词
soft_i2c, software i2c, bitbang i2c, bbus_i2c, 软件I2C, 位爆破I2C

## 命名说明
本项目命名为`bbus_i2c`，其中：
- `bbus` = Bit-Bang Bus（软件iic），是软件模拟I2C时序的核心实现方式；
- `i2c` = 两线式串行总线（Inter-Integrated Circuit）；
因此`bbus_i2c`本质是**纯软件模拟的I2C驱动框架**，区别于硬件I2C（hw_i2c/hard_i2c）。

## 🌟 核心特性

✅ **分层解耦设计**：核心逻辑与硬件操作完全分离，移植仅需修改硬件抽象层，无需改动核心代码

✅ **多总线支持**：通过LUN（逻辑单元号）标识多路I2C总线，每条总线对应独立GPIO引脚

✅ **独立速度配置**：为每条总线单独设置微秒级延时，适配100kHz（标准）/400kHz（快速）等通信速率

✅ **鲁棒性保护**：内置ACK应答超时检测、死等防护，支持RTOS临界区保护

✅ **双读写模式**：支持**带寄存器地址**的常规读写、**无寄存器地址**的直接字节序列读取，覆盖99% I2C设备场景

✅ **易调试**：内置错误日志打印，通信失败时精准输出错误原因（地址/寄存器/数据ACK失败）

✅ **轻量无依赖**：静态内存分配，无动态内存申请，资源占用低，适合小型嵌入式系统

## 📂 文件结构

驱动仅包含4个核心文件，结构清晰，易于集成到任意嵌入式工程中：

```c
BBusI2C/
├── bbus_i2c.c      # 核心驱动层：I2C时序、数据收发、ACK处理等通用逻辑
├── bbus_i2c.h      # 核心驱动头文件：对外暴露所有API接口
├── bbus_i2c_port.c # 硬件抽象层：GPIO操作、延时、系统时钟等硬件相关实现（需用户适配）
└── bbus_i2c_port.h # 硬件抽象层头文件：宏定义、硬件层函数声明
```

## 🏗️ 系统架构

采用**两层架构**实现**硬件无关性**，核心逻辑跨平台复用，移植成本极低：

```c
应用层（传感器/外设驱动）
    ↕️
核心驱动层（bbus_i2c.c/h）：通用I2C协议逻辑（时序、收发、ACK/NAK、超时）
    ↕️
硬件抽象层（bbus_i2c_port.c/h）：平台相关硬件操作（GPIO、延时、Tick、临界区）
```

## 🚀 快速开始

### 步骤1：硬件抽象层适配（仅需修改`bbus_i2c_port.c/h`）

这是唯一需要用户修改的部分，根据自身MCU平台实现硬件相关函数，以STM32为例：

1. **修改宏定义**（`bbus_i2c_port.h`）

    - `BBUS_I2C_BUS_NUM`：设置实际使用的软件I2C总线数量（如2路则定义为2）

    - `BBUS_I2C_LOG`：开启日志打印（默认注释，改为`printf(__VA_ARGS__)`即可）

2. **实现基础函数**（`bbus_i2c_port.c`）
    - **先说明LUN含义**：LUN（逻辑单元号）是用于区分多路软件I2C总线的标识，**一个LUN对应一组独立的SDA/SCL引脚**（即一条I2C总线）；一个LUN（一条总线）可挂载多个I2C从设备，只要各设备地址不冲突（I2C总线本身支持多主从架构，靠从设备地址区分不同设备）。

    - `bbus_i2c_port_delay_us`：对接平台微秒级延时函数（如`delay_us`）

    - `bbus_i2c_port_tick_get`：对接系统毫秒级时钟（如STM32的`HAL_GetTick`）

    - `bbus_i2c_port_init`：初始化SDA/SCL引脚为**开漏输出+上拉**，初始电平置高

    - `bbus_i2c_port_sda/scl_set`：实现SDA/SCL引脚电平控制

    - `bbus_i2c_port_sda_get`：实现SDA引脚电平读取

    - `bbus_i2c_port_sda_set_in/out`：实现SDA引脚输入/输出模式切换

    - （可选）`bbus_i2c_port_enter/exit_critical`：RTOS下实现临界区保护，裸机可留空、
**举例**：
```C
/**
 * @brief   设置I2C SDA引脚电平
 * @param   lun: I2C总线号
 * @param   level: SDA引脚电平，1：高电平，0：低电平
 * @retval  无
 */
void bbus_i2c_port_sda_set(uint8_t lun, uint8_t level)
{
    switch (lun)
    {
    case 0://0号iic总线
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    case 1://1号iic总线
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}
```

3. **I2C硬件要求**：SDA/SCL引脚必须**外接4.7kΩ~10kΩ上拉电阻**（开漏输出特性要求）

### 步骤2：工程集成

1. 将4个核心文件添加到工程中

2. 在编译器中添加BBusI2C文件夹的头文件路径

3. （若开启日志）重定向`printf`到串口（如STM32重定向`fputc`），并勾选编译器微库（Use MicroLIB）

### 步骤3：驱动初始化

在工程主函数中初始化驱动，示例：

```C

#include "bbus_i2c.h"

int main(void)
{
    // 平台初始化（时钟、GPIO、延时、串口等）
    HAL_Init();
    SystemClock_Config();
    delay_init(72); // 微秒延时初始化
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    // 初始化BBusI2C驱动
    bbus_i2c_init();
    // 为0号总线设置延时100us（调整通信速率）
    bbus_i2c_set_delay_time(0, 100);
    
    while(1)
    {
        // 业务逻辑
    }
}
```

## 📚 核心API

驱动对外暴露**基础时序函数**和**核心通信函数**，基础函数可自定义通信流程，核心函数直接满足常规使用需求。

### 基础时序函数（自定义通信流程使用）

用于手动实现I2C时序，适用于特殊通信协议的设备：

```C

void bbus_i2c_start(uint8_t lun);        // 产生起始信号
void bbus_i2c_stop(uint8_t lun);         // 产生停止信号
uint8_t bbus_i2c_wait_ack(uint8_t lun, uint32_t timeout); // 等待ACK应答
void bbus_i2c_ack(uint8_t lun);          // 发送ACK应答
void bbus_i2c_nack(uint8_t lun);         // 发送NAK应答
void bbus_i2c_send_byte(uint8_t lun, const uint8_t data); // 发送1字节
uint8_t bbus_i2c_read_byte(uint8_t lun, uint8_t ack);     // 读取1字节
```

### 核心通信函数（常规使用推荐）

封装好的连续读写函数，直接调用即可，覆盖绝大多数I2C设备场景：

|函数|功能|适用场景|
|---|---|---|
|`bbus_i2c_check_address`|检测I2C设备地址是否存在|总线设备扫描、设备连接检测|
|`bbus_i2c_write_data`|带寄存器地址的连续写|向传感器/外设指定寄存器写入数据（如配置参数）|
|`bbus_i2c_read_data`|带寄存器地址的连续读|从传感器/外设指定寄存器读取数据（如读取温湿度）|
|`bbus_i2c_read_seq`|无寄存器地址的直接读|从无寄存器地址的设备读取字节序列（如部分EEPROM/简单ADC）|
## 💻 使用示例

### 示例1：I2C总线设备扫描

检测0号总线上挂载的所有I2C设备，打印设备地址：

```C

#include "bbus_i2c.h"
#include <stdio.h>

void i2c_scan(uint8_t lun)
{
    printf("Scanning I2C bus %d...\r\n", lun);
    for (uint8_t addr = 0x01; addr <= 0x7F; addr++)
    {
        if (bbus_i2c_check_address(lun, addr, 20) == 0)
        {
            printf("Found I2C device: 0x%02X\r\n", addr);
        }
    }
    printf("Scan done!\r\n");
}

// 主函数中调用
i2c_scan(0);
```

### 示例2：带寄存器地址的连续读写（核心示例）

以**传感器寄存器操作**为例，向0x40设备的0x01寄存器写入2字节数据，再从该寄存器读取3字节数据：

```C

#include "bbus_i2c.h"

#define I2C_LUN        0       // 使用0号I2C总线
#define SLAVE_ADDR     0x40    // 从设备地址
#define REG_ADDR       0x01    // 寄存器地址
#define TIMEOUT_MS     100     // 超时时间

int main(void)
{
    // 平台+驱动初始化...
    bbus_i2c_init();
    bbus_i2c_set_delay_time(I2C_LUN, 100);

    uint8_t write_buf[2] = {0x12, 0x34};
    uint8_t read_buf[3] = {0};

    // 向指定寄存器写入2字节数据
    if (bbus_i2c_write_data(I2C_LUN, SLAVE_ADDR, REG_ADDR, write_buf, 2, TIMEOUT_MS) == 0)
    {
        printf("Write data success!\r\n");
    }
    else
    {
        printf("Write data failed!\r\n");
    }

    // 从指定寄存器读取3字节数据
    if (bbus_i2c_read_data(I2C_LUN, SLAVE_ADDR, REG_ADDR, read_buf, 3, TIMEOUT_MS) == 0)
    {
        printf("Read data: 0x%02X 0x%02X 0x%02X\r\n", read_buf[0], read_buf[1], read_buf[2]);
    }
    else
    {
        printf("Read data failed!\r\n");
    }

    while(1);
}
```

### 示例3：无寄存器地址的直接读取（如AHT30温湿度传感器）

```C

#include "bbus_i2c.h"

#define I2C_LUN        0       // 使用0号I2C总线
#define AHT30_ADDR     0x38
#define AHT30_BUF_LEN  6

uint8_t aht30_read_data(uint8_t *data)
{
    bbus_i2c_set_delay_time(I2C_LUN, 100);
    return bbus_i2c_read_seq(I2C_LUN, AHT30_ADDR, data, AHT30_BUF_LEN, 100);
}

// 主函数中调用
uint8_t aht30_buf[AHT30_BUF_LEN] = {0};
if (aht30_read_data(aht30_buf) == 0)
{
    // 解析温湿度数据...
}
```

## 🚨 故障排除

驱动使用中常见问题及解决方法，按优先级排查：

|问题现象|可能原因|解决方法|
|---|---|---|
|设备扫描无结果、读写返回失败|延时时间不合理|调整`bbus_i2c_set_delay_time`的延时值，建议50~200us，速率过快会导致通信失败|
|部分总线通信正常，部分失败|LUN总线号错误|检查函数入参的LUN是否与硬件适配层的GPIO配置一一对应|
|日志打印GPIO相关错误|硬件适配函数未实现|检查`bbus_i2c_port.c`中GPIO的读写、模式切换函数是否正确实现|
|总线始终无应答|缺少上拉电阻|SDA/SCL引脚必须外接4.7kΩ~10kΩ上拉电阻，开漏输出无拉电阻无法输出高电平|
|无错误日志打印|日志未开启|在`bbus_i2c_port.h`中把`BBUS_I2C_LOG`定义为`printf(__VA_ARGS__)`，并确保串口重定向成功|
|RTOS下通信乱码/失败|无临界区保护|在`bbus_i2c_port.c`中实现临界区函数，保护I2C总线操作不被任务打断|
## 📝 调试方法

驱动内置**精准的错误日志**，开启后通信失败时会打印具体错误信息，快速定位问题：

1. 开启日志：`bbus_i2c_port.h`中修改`#define BBUS_I2C_LOG(...) printf(__VA_ARGS__)`

2. 错误日志示例：

    - 设备地址ACK失败：`[I2C Read][ERROR]: Wait ACK failed for address 0x40 in read mode`

    - 寄存器ACK失败：`[I2C Write][ERROR]: Wait ACK failed for register 0x01`

    - 数据ACK失败：`[I2C Write][ERROR]: Wait ACK failed for data 0x12`

日志直接指出**错误类型、失败的地址/寄存器/数据**，大幅降低调试难度。

## 📄 许可证

本项目基于**MIT开源协议**发布，详见 LICENSE 文件。

## 🔗 相关链接

- 详细使用教程：[BBusI2C：高通用性的软件 I2C 驱动框架](https://www.yuque.com/u54102073/rs06a5/ugd9qgtyf86gd668?singleDoc#)

- 嵌入式技术交流群：181921938

## 如果这个项目对你有帮助，请给它一个 ⭐ ！

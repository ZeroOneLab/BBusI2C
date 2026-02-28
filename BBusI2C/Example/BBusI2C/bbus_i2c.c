/**
 * @file    bbus_i2c.c
 * @version v1.0
 * @date    2026-02-28
 * @author  ZeroOneLab
 * @website https://github.com/ZeroOneLab/BBusI2C.git
 *
 * @license MIT License
 * Copyright (c) 2026 ZeroOneLab
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bbus_i2c.h"

static uint8_t delay_time[BBUS_I2C_BUS_NUM];

#define SDA_OUT(lun)            bbus_i2c_port_sda_set_out(lun)
#define SDA_IN(lun)             bbus_i2c_port_sda_set_in(lun)
#define SDA_SET(lun, level)     bbus_i2c_port_sda_set(lun, level)
#define SDA_GET(lun)            bbus_i2c_port_sda_get(lun)
#define SCL_SET(lun, level)     bbus_i2c_port_scl_set(lun, level)
#define DELAY_US(delay_time)    bbus_i2c_port_delay_us(delay_time)
#define ENTER_CRITICAL(lun)     bbus_i2c_port_enter_critical(lun)
#define EXIT_CRITICAL(lun)      bbus_i2c_port_exit_critical(lun)

/**
 * @brief   初始化软件I2C
 * @param   无
 * @retval  无
 */
void bbus_i2c_init(void)
{
    for (uint8_t i = 0; i < BBUS_I2C_BUS_NUM; i++)
    {
        bbus_i2c_port_init(i);
        delay_time[i] = 0;
    }
}

/**
 * @brief   设置I2C延时时间
 * @param   lun: I2C总线号
 * @param   xus: 延时时间 (单位: us)
 * @retval  无
 */
void bbus_i2c_set_delay_time(uint8_t lun, uint32_t xus)
{
    delay_time[lun] = xus;
}

/**
 * @brief   产生I2C起始信号
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_start(uint8_t lun)
{
    SDA_OUT(lun);
    SDA_SET(lun, 1);
    SCL_SET(lun, 1);
    DELAY_US(delay_time[lun]);
    SDA_SET(lun, 0); /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 0); /* 钳住I2C总线，准备发送或接收数据 */
}

/**
 * @brief       产生I2C停止信号
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_stop(uint8_t lun)
{
    SDA_OUT(lun);
    SDA_SET(lun, 0); /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    SCL_SET(lun, 0); /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 1);
    SDA_SET(lun, 1); /* 发送I2C总线结束信号 */
    DELAY_US(delay_time[lun]);
}

/**
 * @brief       等待应答信号到来
 * @param       lun: I2C总线号
 * @param       timeout: 超时时间ms
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
uint8_t bbus_i2c_wait_ack(uint8_t lun, uint32_t timeout)
{
    uint32_t wait_time = bbus_i2c_port_tick_get();

    SDA_IN(lun);     /* 设置SDA为输入模式 */
    SDA_SET(lun, 1); /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 1); /* SCL=1, 此时从机可以返回ACK */
    DELAY_US(delay_time[lun]);
    while (SDA_GET(lun)) /* 等待应答 */
    {
        if ((bbus_i2c_port_tick_get() - wait_time) >= timeout)
        {
            bbus_i2c_stop(lun);
            return 1;
        }
    }
    SCL_SET(lun, 0); /* SCL=0, 结束ACK检查 */
    return 0;
}

/**
 * @brief       产生ACK应答
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_ack(uint8_t lun)
{
    SCL_SET(lun, 0); /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    SDA_OUT(lun);
    DELAY_US(delay_time[lun]);
    SDA_SET(lun, 0); /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 1); /* 产生一个时钟 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 0);
}

/**
 * @brief       不产生ACK应答
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_nack(uint8_t lun)
{
    SCL_SET(lun, 0); /* 产生一个时钟 */
    SDA_OUT(lun);
    SDA_SET(lun, 1); /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 1); /* 产生一个时钟 */
    DELAY_US(delay_time[lun]);
    SCL_SET(lun, 0);
}

/**
 * @brief       I2C发送一个字节
 * @param       lun: I2C总线号
 * @param       data: 要发送的数据
 * @retval      无
 */
void bbus_i2c_send_byte(uint8_t lun, const uint8_t data)
{
    SDA_OUT(lun);
    SCL_SET(lun, 0); /* 产生一个时钟 */
    for (uint8_t i = 0; i < 8; i++)
    {
        SDA_SET(lun, (((data << i) & 0x80) >> 7));

        DELAY_US(delay_time[lun]);
        SCL_SET(lun, 1);
        DELAY_US(delay_time[lun]);
        SCL_SET(lun, 0);
    }
}

/**
 * @brief       I2C读取一个字节
 * @param       lun: I2C总线号
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t bbus_i2c_read_byte(uint8_t lun, uint8_t ack)
{
    uint8_t i, receive = 0;
    SDA_SET(lun, 1);
    SDA_IN(lun);            /* 设置SDA为输入模式 */
    for (i = 0; i < 8; i++) /* 接收1个字节数据 */
    {

        SCL_SET(lun, 0);
        DELAY_US(delay_time[lun]);
        SCL_SET(lun, 1);
        receive <<= 1; /* 高位先输出,所以先收到的数据位要左移 */

        if (SDA_GET(lun))
        {
            receive++;
        }
        DELAY_US(delay_time[lun]);
    }
    if (!ack)
    {
        bbus_i2c_nack(lun); /* 发送nACK */
    }
    else
    {
        bbus_i2c_ack(lun); /* 发送ACK */
    }

    return receive;
}

/**
 * @brief       检查从设备地址是否正确
 * @param       lun: I2C总线号
 * @param       slave_addr: 从设备地址
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_check_address(uint8_t lun, uint8_t slave_addr, uint32_t timeout)
{
    ENTER_CRITICAL(lun);

    bbus_i2c_start(lun);
    bbus_i2c_send_byte(lun, slave_addr & 0xFE);

    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Check][ERROR]: Wait ACK failed for address 0x%02X\n", slave_addr);
        return 1; // 接收应答失败
    }

    bbus_i2c_stop(lun);

    EXIT_CRITICAL(lun);
    return 0;
}

/**
 * @brief       软件I2C连续写数据
 * @param       lun: I2C总线号
 * @param       salve_adress: 从设备地址
 * @param       reg_address: 寄存器地址
 * @param       data: 存储读取数据的缓冲区
 * @param       len: 要读取的数据长度
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_write_data(uint8_t lun, uint8_t slave_addr, uint8_t reg_address, const uint8_t *data, uint8_t len, uint32_t timeout)
{
    ENTER_CRITICAL(lun);
    // 产生起始信号
    bbus_i2c_start(lun);

    // 发送从设备地址 + 写命令
    bbus_i2c_send_byte(lun, slave_addr & 0xFE);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Write][ERROR]: Wait ACK failed for address 0x%02X\n", slave_addr);
        return 1; // 接收应答失败
    }

    // 发送寄存器地址
    bbus_i2c_send_byte(lun, reg_address);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Write][ERROR]: Wait ACK failed for register 0x%02X\n", reg_address);
        return 1; // 接收应答失败
    }

    // 发送数据
    for (uint8_t i = 0; i < len; i++)
    {
        bbus_i2c_send_byte(lun, data[i]);
        if (bbus_i2c_wait_ack(lun, timeout))
        {
            bbus_i2c_stop(lun);
            BBUS_I2C_LOG("[I2C Write][ERROR]: Wait ACK failed for data 0x%02X\n", data[i]);
            return 1; // 接收应答失败
        }
    }

    // 产生停止信号
    bbus_i2c_stop(lun);
    EXIT_CRITICAL(lun);

    return 0;
}

/**
 * @brief       软件I2C连续读数据
 * @param       lun: I2C总线号
 * @param       salve_adress: 从设备地址
 * @param       reg_address: 寄存器地址
 * @param       data: 存储读取数据的缓冲区
 * @param       len: 要读取的数据长度
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_read_data(uint8_t lun, uint8_t slave_addr, uint8_t reg_address, uint8_t *data, uint8_t len, uint32_t timeout)
{
    ENTER_CRITICAL(lun);
    // 产生起始信号
    bbus_i2c_start(lun);

    // 发送从设备地址 + 写命令
    bbus_i2c_send_byte(lun, slave_addr & 0xFE);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Read][ERROR]: Wait ACK failed for address 0x%02X\n", slave_addr);
        return 1; // 接收应答失败
    }
    // 发送寄存器地址
    bbus_i2c_send_byte(lun, reg_address);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Read][ERROR]: Wait ACK failed for register 0x%02X\n", reg_address);
        return 1; // 接收应答失败
    }
    // 产生起始信号
    bbus_i2c_start(lun);
    // 发送从设备地址 + 读命令
    bbus_i2c_send_byte(lun, slave_addr | 0x01);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Read][ERROR]: Wait ACK failed for address 0x%02X in read mode\n", slave_addr);
        return 1; // 接收应答失败
    }

    // 读取数据
    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = bbus_i2c_read_byte(lun, i < len - 1 ? 1 : 0);
    }

    // 产生停止信号
    bbus_i2c_stop(lun);
    EXIT_CRITICAL(lun);

    return 0; // 读取成功
}

/**
 * @brief       直接读 N 字节序列（无寄存器地址阶段）
 * @param       lun: I2C总线号
 * @param       salve_adress: 从设备地址
 * @param       data: 存储读取数据的缓冲区
 * @param       len: 要读取的数据长度
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_read_seq(uint8_t lun, uint8_t slave_addr, uint8_t *data, uint8_t len, uint32_t timeout)
{
    ENTER_CRITICAL(lun);
    // 产生起始信号
    bbus_i2c_start(lun);
    // 发送从设备地址 + 读命令
    bbus_i2c_send_byte(lun, slave_addr | 0x01);
    if (bbus_i2c_wait_ack(lun, timeout))
    {
        bbus_i2c_stop(lun);
        BBUS_I2C_LOG("[I2C Read][ERROR]: Wait ACK failed for address 0x%02X in read mode\n", slave_addr);
        return 1; // 接收应答失败
    }
    // 读取数据
    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = bbus_i2c_read_byte(lun, i < len - 1 ? 1 : 0);
    }

    // 产生停止信号
    bbus_i2c_stop(lun);
    EXIT_CRITICAL(lun);

    return 0; // 读取成功
}

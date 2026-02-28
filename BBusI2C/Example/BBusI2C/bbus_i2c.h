/**
 * @file    bbus_i2c.h
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

#ifndef BBUS_I2C_H
#define BBUS_I2C_H

#include "bbus_i2c_port.h"
#include <stdint.h>

/**
 * @brief   初始化软件I2C
 * @param   无
 * @retval  无
 */
void bbus_i2c_init(void);

/**
 * @brief   设置I2C延时时间
 * @param   lun: I2C总线号
 * @param   xus: 延时时间 (单位: us)
 * @retval  无
 */
void bbus_i2c_set_delay_time(uint8_t lun, uint32_t xus);

/**
 * @brief   产生I2C起始信号
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_start(uint8_t lun);

/**
 * @brief       产生I2C停止信号
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_stop(uint8_t lun);

/**
 * @brief       等待应答信号到来
 * @param       lun: I2C总线号
 * @param       timeout: 超时时间ms
 * @retval      1，接收应答失败, 0，接收应答成功
 */
uint8_t bbus_i2c_wait_ack(uint8_t lun, uint32_t timeout);

/**
 * @brief       产生ACK应答
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_ack(uint8_t lun);

/**
 * @brief       不产生ACK应答
 * @param       lun: I2C总线号
 * @retval      无
 */
void bbus_i2c_nack(uint8_t lun);

/**
 * @brief       I2C发送一个字节
 * @param       lun: I2C总线号
 * @param       data: 要发送的数据
 * @retval      无
 */
void bbus_i2c_send_byte(uint8_t lun, const uint8_t data);

/**
 * @brief       I2C读取一个字节
 * @param       lun: I2C总线号
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t bbus_i2c_read_byte(uint8_t lun, uint8_t ack);

/**
 * @brief       检查从设备地址是否正确
 * @param       lun: I2C总线号
 * @param       slave_addr: 从设备地址
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_check_address(uint8_t lun, uint8_t slave_addr, uint32_t timeout);

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
uint8_t bbus_i2c_write_data(uint8_t lun, uint8_t slave_addr, uint8_t reg_address, const uint8_t *data, uint8_t len, uint32_t timeout);

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
uint8_t bbus_i2c_read_data(uint8_t lun, uint8_t slave_addr, uint8_t reg_address, uint8_t *data, uint8_t len, uint32_t timeout);

/**
 * @brief       直接读 N 字节序列（无寄存器地址阶段）
 * @param       lun: I2C总线号
 * @param       salve_adress: 从设备地址
 * @param       data: 存储读取数据的缓冲区
 * @param       len: 要读取的数据长度
 * @param       timeout: 超时时间ms
 * @retval      0，读取成功；1，读取失败
 */
uint8_t bbus_i2c_read_seq(uint8_t lun, uint8_t slave_addr, uint8_t *data, uint8_t len, uint32_t timeout);

#endif

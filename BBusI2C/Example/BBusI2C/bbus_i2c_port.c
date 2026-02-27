/*
 * Copyright (c) 2026 零壹实验室-ZoreOneLab
 * Repository: https://github.com/ZeroOneLab/BBusI2C.git
 * Tutorial: https://www.yuque.com/u54102073/rs06a5/ugd9qgtyf86gd668?singleDoc#
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

#include "bbus_i2c_port.h"

#include "main.h"
#include "delay.h"

/**
 * @brief   软件I2C延时函数
 * @param   xus: 延时时间，单位us
 * @retval  无
 */
void bbus_i2c_port_delay_us(uint32_t xus)
{
    delay_us(xus);
}

/**
 * @brief   获取当前系统时间，单位ms
 * @param   无
 * @retval  当前系统时间，单位ms
 */
uint32_t bbus_i2c_port_tick_get(void)
{
    return HAL_GetTick();
}

/**
 * @brief   软件I2C端口初始化
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_port_init(uint8_t lun)
{
    switch (lun)
    {
    case 0:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
        break;
    default:
        break;
    }
}

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
    case 0:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}

/**
 * @brief   设置I2C SCL引脚电平
 * @param   lun: I2C总线号
 * @param   level: SCL引脚电平，1：高电平，0：低电平
 * @retval  无
 */
void bbus_i2c_port_scl_set(uint8_t lun, uint8_t level)
{
    switch (lun)
    {
    case 0:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, (level == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}

/**
 * @brief   获取I2C SDA引脚电平
 * @param   lun: I2C总线号
 * @retval  SDA引脚电平，1：高电平，0：低电平
 */
uint8_t bbus_i2c_port_sda_get(uint8_t lun)
{
    uint8_t ret = 0;
    switch (lun)
    {
    case 0:
        ret = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) ? 1 : 0;
        break;
    case 1:
        ret = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) ? 1 : 0;
        break;
    default:
        break;
    }
    return ret;
}

/**
 * @brief   设置I2C SDA引脚为输出模式
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_port_sda_set_out(uint8_t lun)
{
    switch (lun)
    {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

/**
 * @brief   设置I2C SDA引脚为输入模式
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_port_sda_set_in(uint8_t lun)
{
    switch (lun)
    {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

/**
 * @brief   进入临界区
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_port_enter_critical(uint8_t lun)
{
    switch (lun)
    {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

/**
 * @brief   退出临界区
 * @param   lun: I2C总线号
 * @retval  无
 */
void bbus_i2c_port_exit_critical(uint8_t lun)
{
    switch (lun)
    {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

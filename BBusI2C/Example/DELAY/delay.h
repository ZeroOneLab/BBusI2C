#ifndef __DELAY_H_
#define __DELAY_H_

/* =========================== 用户配置 =========================== */
#include "main.h"

static inline void INIT(uint16_t sys_clk) /* 系统时钟抵滴答初始化 */
{
    // SysTick_Config(sys_clk * 1000); /* 1ms中断一次 */
}

/* =========================== 外部声明 =========================== */

#include "stdint.h"

/**
 * @brief     初始化延迟函数
 * @param     sysclk: 系统时钟频率, 即CPU频率(HCLK), 72Mhz
 * @retval    无
 */
void delay_init(uint16_t sysclk);

/**
 * @brief     延时nus
 * @note      无论是否使用OS, 都是用时钟摘取法来做us延时
 * @param     nus: 要延时的us数
 * @note      nus取值范围: 0 ~ (2^32 / fac_us)
 * @retval    无
 */
void delay_us(uint32_t nus);

/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= (2^32 / fac_us / 1000))
 * @retval    无
 */
void delay_ms(uint16_t nms);

#endif 

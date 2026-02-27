#include "delay.h"

static uint32_t fac_us = 0; /* us延时倍乘数 */

/**
 * @brief     初始化延迟函数
 * @param     sys_clk: 系统时钟频率, 即CPU频率(HCLK), 72Mhz
 * @retval    无
 */
void delay_init(uint16_t sys_clk)
{
    /* HAL库此处不用配置，如没有提前配置Systick寄存器，则需要配置*/
    // SysTick->CTRL |= (1 << 2);  /* SYSTICK使用内部时钟源,频率为HCLK*/
    // SysTick->CTRL |= 1 << 0;    /* 使能Systick */
    // SysTick->LOAD = 0X0FFFFFFF; /* 注意systick计数器24位，所以这里设置最大重装载值 */
    // INIT(sys_clk);              /* HAL不用配置 */

    fac_us = sys_clk;
}

/**
 * @brief     延时nus
 * @param     nus: 要延时的us数
 * @note      nus取值范围: 0 ~ (2^32 / fac_us)
 * @retval    无
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks = nus * fac_us; // 目标周期数 = 微秒数 × 每微秒周期数
    uint32_t time_old, time_now, time_cnt = 0;
    uint32_t reload = SysTick->LOAD; // 获取最大重载值（通常为0x00FFFFFF）

    time_old = SysTick->VAL; // 获取初始快照
    for (;;)
    {
        time_now = SysTick->VAL;
        if (time_now != time_old) // 计数器发生递减
        {
            // 处理24位计数器溢出回绕
            if (time_now < time_old)
                time_cnt += time_old - time_now;
            else
                time_cnt += reload - time_now + time_old;

            time_old = time_now;

            if (time_cnt >= ticks) // 达到目标周期数，退出
                break;
        }
    }
}
/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= (2^32 / fac_us / 1000))
 * @retval    无
 */
void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000)); /* 普通方式延时 */
}

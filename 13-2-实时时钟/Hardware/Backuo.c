#include "stm32f10x.h"

/**
 * @brief 备份寄存器初始化
 * @param None
 * @retval None
 */
void Backup_Init()
{
    /** 开启 PWR 和 BKP 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    /** 备份寄存器访问使能 */
    PWR_BackupAccessCmd(ENABLE);
}


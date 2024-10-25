#include "stm32f10x.h"
#include <time.h>
#include "Backup.h"

uint16_t MyRTC_Time[] = {2024, 10, 16, 23, 59, 50}; ///< 定义全局的时间数组，数组内容分别为年、月、日、时、分、秒
void MyRTC_SetTime(void);

/**
 * @brief RTC 初始化
 * @param None
 * @retval None
 */
void MyRTC_Init()
{
    /** Backup 初始化 */
    Backup_Init(); 

    if (BKP_ReadBackupRegister(BKP_DR1) != 0x01) // 通过写入备份寄存器的标志位，判断 RTC 是否是第一次配置,这个值可以随意
    {
        RCC_LSEConfig(RCC_LSE_ON); ///< 开启 LSE 时钟
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET); ///< 等待 LSE 准备就绪
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); ///< 选择 RTCCLK 来源为 LSE
        RCC_RTCCLKCmd(ENABLE); ///< RTCCLK 使能
        RTC_WaitForSynchro(); ///< 等待同步
        RTC_WaitForLastTask(); ///< 等待上一次操作完成
        RTC_SetPrescaler(32768 - 1); ///< 设置 RTC 预分频器，预分频后的计数频率为 1Hz
        RTC_WaitForLastTask(); ///< 等待上一次操作完成

        MyRTC_SetTime(); ///< 设置时间，调用此函数，全局数组里时间值刷新到 RTC 硬件电路
        BKP_WriteBackupRegister(BKP_DR1, 0x01); ///< 在备份寄存器写入自己规定的标志位，用于判断 RTC 是不是第一次执行配置
    } 
    else
    {
        RTC_WaitForSynchro(); ///< 等待同步
        RTC_WaitForLastTask(); ///< 等待上一次操作完成        
    }
}

/**
 * @brief RTC 初始化（备用 LSI）
 * @param None
 * @retval None
 * @attention
 * 如果LSE无法起振导致程序卡死在初始化函数中，
 * 可将初始化函数替换为下述代码，使用 LSI 当作 RTCCLK，
 * LSI 无法由备用电源供电，故主电源掉电时，RTC 走时会暂停 
 */
void MyRTC_Init_Standby()
{
    /** Backup 初始化 */
    Backup_Init(); 

    if (BKP_ReadBackupRegister(BKP_DR1) != 0x01) // 通过写入备份寄存器的标志位，判断 RTC 是否是第一次配置,这个值可以随意
    {
        RCC_LSICmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET); ///< 等待 LSE 准备就绪
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); ///< 选择 RTCCLK 来源为 LSI
        RCC_RTCCLKCmd(ENABLE); ///< RTCCLK 使能
        RTC_WaitForSynchro(); ///< 等待同步
        RTC_WaitForLastTask(); ///< 等待上一次操作完成
        RTC_SetPrescaler(32768 - 1); ///< 设置 RTC 预分频器，预分频后的计数频率为 1Hz
        RTC_WaitForLastTask(); ///< 等待上一次操作完成

        MyRTC_SetTime(); ///< 设置时间，调用此函数，全局数组里时间值刷新到 RTC 硬件电路
        BKP_WriteBackupRegister(BKP_DR1, 0x01); ///< 在备份寄存器写入自己规定的标志位，用于判断 RTC 是不是第一次执行配置
    } 
    else
    {
        RCC_LSICmd(ENABLE); ///< 即使不是第一次配置，也需要再次开启 LSI 时钟
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
                
        RTC_WaitForSynchro(); ///< 等待同步
        RTC_WaitForLastTask(); ///< 等待上一次操作完成        
    }
}    

/**
 * @brief RTC 设置时间
 * @param None
 * @retval None
 * @note 调用此函数后，全局数组里时间值将刷新到 RTC 硬件电路
 */
void MyRTC_SetTime()
{
    time_t time_count;
    struct tm time_date; ///< 定义日期时间数据类型

    time_date.tm_year = MyRTC_Time[0] - 1900;
    time_date.tm_mon = MyRTC_Time[1] - 1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];

    time_count = mktime(&time_date) - 8 * 60 * 60; ///< 调用 mktime 函数，将日期时间转换为秒计数器格式，其中 - 8 * 60 * 60 为东八区的时区调整
    RTC_SetCounter(time_count); ///< 将秒计数器写入到 RTC 的 CNT 中
    RTC_WaitForLastTask(); ///< 等待上一次操作完成     
}

/**
 * @brief RTC 读取时间
 * @param None
 * @retval None
 * @note 调用此函数后，全局数组里时间值将刷新到 RTC 硬件电路
 */
void MyRTC_ReadTime()
{
    time_t time_count;
    struct tm time_date; ///< 定义日期时间数据类型

    time_count = RTC_GetCounter() + 80 * 60 * 60; ///< 读取 RTC 的 CNT，获取当前的秒计数器；+ 8 * 60 * 60 为东八区的时区调整
    time_date = *localtime(&time_count); ///< 使用 localtime 函数，将秒计数器转换为日期时间格式

	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

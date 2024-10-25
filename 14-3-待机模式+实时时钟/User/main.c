#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "MyRTC.h"

int main()
{
	/** 模块初始化 */
	OLED_Init();
	MyRTC_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "CNT :");
	OLED_ShowString(2, 1, "ALR :");
	OLED_ShowString(3, 1, "ALRF:");

	/* 使能 WKUP 引脚 */
	PWR_WakeUpPinCmd(ENABLE); // 使能位于 PA0 的 WKUP 引脚，WKUP 引脚上升沿唤醒待机模式

	/* 设置闹钟,这里要多使用一个变量的来存储计数的值，因为 ALR 寄存器不可读 */
	uint32_t Alarm = RTC_GetCounter() + 30; // 闹钟为唤醒后当前时间的后30s
	RTC_SetAlarm(Alarm); // 写入闹钟值到 RTC 的 ALR 寄存器

	while (1)
	{
		OLED_ShowNum(1, 6, RTC_GetCounter(), 10); // 显示 32 位的秒计数器
		OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1); // 显示闹钟标志位	

		/* OLED 闪烁 Running，指示当前主循环正在运行 */
		OLED_ShowString(4, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);

		/* OLED 闪烁 STANDBY，指示即将进入待机模式 */
		OLED_ShowString(4, 9, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString(4, 9, "       ");
		Delay_ms(100);

		/* OLED 清屏，模拟关闭外部所有的耗电设备，以达到极度省电，同时建议使用待机模式的时候，关闭所有外设才能达到良好的省电 */
		OLED_Clear();

		PWR_EnterSTANDBYMode(); // STM32 进入停止模式，并等待指定的唤醒事件（WKUP 上升沿或 RTC 闹钟）

		/* 待机模式唤醒后，程序会重头开始运行 */
	}
}

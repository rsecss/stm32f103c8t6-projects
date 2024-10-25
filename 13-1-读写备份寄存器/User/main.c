#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Backup.h"

uint8_t KeyNumber; // 定义用于接收按键键码的变量
uint16_t WriteData[] = {0x000, 0x888}; // 定义要写入数据的测试数组
uint16_t ReadData[2]; // 定义要读取数据的测试数组

int main()
{
	/** 模块初始化 */
	OLED_Init();
	Key_Init();
	Backup_Init(); 

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");

	while (1)
	{
		KeyNumber = key_GetNumber(); // 获取按键键码

		WriteData[0]++;
		WriteData[1]++;

		/* 写入测试数据到备份寄存器 */
		BKP_WriteBackupRegister(BKP_DR1, WriteData[0]);
		BKP_WriteBackupRegister(BKP_DR2, WriteData[1]);

		/* 显示写入的测试数据 */
		OLED_ShowHexNum(1, 3, WriteData[0], 4);
		OLED_ShowHexNum(1, 8, WriteData[1], 4);

		/* 读取备份寄存器 */
		BKP_ReadBackupRegister(BKP_DR1);
		BKP_ReadBackupRegister(BKP_DR2);

		/* 显示读取的备份寄存器的数据 */
		OLED_ShowHexNum(2, 3, ReadData[0], 4);
		OLED_ShowHexNum(2, 8, ReadData[1], 4);
	}
}

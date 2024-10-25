#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID; // 定义用于存放 MID 号的变量
uint16_t DID; // 定义用于存放 DID 号的变量
uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04}; // 定义要写入数据的测试数组
uint8_t ArrayRead[4]; // 定义要读取数据的测试数组
int main()
{
	/* 模块初始化 */
	OLED_Init();
	W25Q64_Init();

	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");	

	/* 显示 ID 号 */
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);

	/* W25Q64 功能函数测试 */
	W25Q64_SectorErase(0x00000000); // 扇区擦除
	W25Q64_PageProgram(0x00000000, ArrayWrite, 4); // 将写入数据的测试数组写入到 W25Q64 中
	W25Q64_ReadData(0x00000000, ArrayRead, 4); // 读取刚写入的测试数据到读取数据的测试数组中

	/* 显示数据 */
	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2); // 显示写入数据的测试数组
	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
	
	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);	// 显示读取数据的测试数组
	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);

	while (1)
	{
		
	}
}

#include "stm32f10x.h"
#include "Flash.h"

#define STORE_STRAT_ADDRESS 0x0800FC00 // 存储的起始地址
#define STORE_COUNT 512 // 存储数据的个数

uint16_t Store_Data[STORE_COUNT]; // 定义 SRAM 数组

/**
 * @brief 参数存储模块初始化
 * @param None
 * @retval None
 */
void Store_Init()
{
    /** 判断是不是第一次使用 */
    if (MyFlash_ReadHalfWord(STORE_STRAT_ADDRESS) != 0xA5A5) ///< 读取第一个半字的标志位，if 成立，则执行第一次使用的初始化
    {
        MyFlash_ErasePage(STORE_STRAT_ADDRESS); ///< 擦除指定页
        MyFlash_ProgramHalfWord(STORE_STRAT_ADDRESS, 0xA5A5); ///< 在第一个半字写入自己规定的标志位，用于判断是不是第一次使用

        for (uint16_t i = 1; i < STORE_COUNT; i++)
        {
            MyFlash_ProgramHalfWord(STORE_STRAT_ADDRESS + i * 2, 0x0000); ///< 除了标志位的有效数据全部清 0
        }
    }

    /** 上电时，将闪存数据加载回 SRAM 数组，实现 SRAM 数组的掉电不丢失 */
    for (uint16_t i = 0; i < STORE_COUNT; i++)
    {
        Store_Data[i] = MyFlash_ReadHalfWord(STORE_STRAT_ADDRESS + i * 2);
    }
}

/**
 * @brief 参数存储模块保存数据到闪存
 * @param None
 * @retval None
 */
void Store_Save()
{
    MyFlash_ErasePage(STORE_STRAT_ADDRESS);

    for (uint16_t i = 0; i < STORE_COUNT; i++) ///< 循环 STORE_COUNT 次，包括第一个标志位
    {
        MyFlash_ProgramHalfWord(STORE_STRAT_ADDRESS + i * 2, Store_Data[i]); ///< 将 SRAM 数组的数据备份保存到闪存
    }
}

/**
 * @brief 参数存储模块将所有有效数据清 0
 * @param None
 * @retval None
 */
void Store_Clear()
{
    for (uint16_t i = 1; i < STORE_COUNT; i++) ///< 循环STORE_COUNT次，除了第一个标志位
    {
        Store_Data[i] = 0x0000;
    }

    Store_Save(); ///< 保存数据到闪存
}

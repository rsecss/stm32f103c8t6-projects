#include "stm32f10x.h"

/**
 * @brief FLASH 读取一个 32 位的字
 * @param Address: 要读取数据的字地址
 * @retval None
 */
uint32_t MyFlash_ReadWord(uint32_t Address)
{
    return *((__IO uint32_t *)(Address));
}

/**
 * @brief FLASH 读取一个 16 位的字
 * @param Address: 要读取数据的字地址
 * @retval None
 */
uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
    return *((__IO uint16_t *)(Address));
}

/**
 * @brief FLASH 读取一个 8 位的字
 * @param Address: 要读取数据的字地址
 * @retval None
 */
uint8_t MyFlash_ReadByte(uint32_t Address)
{
    return *((__IO uint8_t *)(Address));
}

/**
 * @brief FLASH 全擦除
 * @param None
 * @retval None
 * @attention 
    调用此函数后，FLASH 的所有页都会被擦除，包括程序文件本身，擦除后，程序将不复存在
 */
void MyFlash_EraseAllPages()
{
    FLASH_Unlock(); // 解锁
    FLASH_EraseAllPages(); // 全檫除
    FLASH_Lock(); // 上锁
}

/**
 * @brief FLASH 页擦除
 * @param PageAddress 要擦除页的页地址
 * @retval None
 */
void MyFlash_ErasePage(uint32_t PageAddress)
{
    FLASH_Unlock(); // 解锁
    FLASH_ErasePage(PageAddress); // 页擦除
    FLASH_Lock(); // 上锁
}

/**
 * @brief FLASH 编程字
 * @param Address: 要写入数据的字地址
 * @param Data: 要写入的 32 位数据
 * @retval None
 */
void MyFlash_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock(); // 解锁
    FLASH_ProgramWord(Address, Data); // 编程字
    FLASH_Lock(); // 上锁
}

/**
 * @brief FLASH 编程半字
 * @param Address: 要写入数据的字地址
 * @param Data: 要写入的 16 位数据
 * @retval None
 */
void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock(); // 解锁
    FLASH_ProgramHalfWord(Address, Data); // 编程半字
    FLASH_Lock(); // 上锁
}

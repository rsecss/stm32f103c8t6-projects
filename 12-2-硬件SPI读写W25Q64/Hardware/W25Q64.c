#include "stm32f10x.h"
#include "MySPI.h"
#include "W25Q64_Ins.h"

/**
 * @brief W25Q64 初始化
 * @param None
 * @retval None
 */
void W25Q64_Init()
{
    MySPI_Init();
}

/**
 * @brief MPU6050 读取 ID 号
 * @param MID: 工厂ID，使用输出参数的形式返回
 * @param DID: 设备ID，使用输出参数的形式返回
 * @retval None
 */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID) 
{
    MySPI_Start(); ///< 开始时序

    MySPI_SwapByte(W25Q64_JEDEC_ID); ///< 交换发送读取ID的指令
    *MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE); ///< 交换接收 MID，通过输出参数返回
    *DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE); ///< 交换接收 DID 高 8 位
    *DID <<= 8; ///< 高 8 位移到高位
    *DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE); ///< 或上交换接收 DID 的低 8 位，通过输出参数返回

    MySPI_Stop(); ///< SPI 终止
}

/**
 * @brief W25Q64 写使能
 * @param None
 * @retval None
 */
void W25Q64_WriteEnable()
{
    MySPI_Start();

    /** 交换发送写使能的指令 */
    MySPI_SwapByte(W25Q64_DUMMY_BYTE); 

    MySPI_Stop();
}

/**
 * @brief W25Q64 等待忙（超时等待）
 * @param None
 * @retval None
 */
void W25Q64_WaitBusy()
{
    uint32_t TimeOut = 100000;

    MySPI_Start();

    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1); ///< 交换发送读状态寄存器 1 的指令

    while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) ///< 循环等待忙标志位
    {
        TimeOut--;
        if (TimeOut == 0) 
        {
            break;
        }
    }

    MySPI_Stop();
}

/**
 * @brief W25Q64 页编程
 * @param Address: 页编程的起始地址，范围：0x000000 ~ 0x7FFFFF
 * @param DataArray: 用于写入数据的数组
 * @param Count: 要写入数据的数量，范围：0~256
 * @retval None
 * @attention 写入的地址范围不能跨页
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t* DataArray, uint16_t Count)
{
    W25Q64_WriteEnable(); ///< 写使能

    MySPI_Start();

    /** SPI 起始 */
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM); ///< 交换发送页编程的指令
    MySPI_SwapByte(Address >> 16); ///< 交换发送地址 23~16 位
    MySPI_SwapByte(Address >> 8); ///< 交换发送地址 15~8 位
    MySPI_SwapByte(Address ); ///< 交换发送地址 7~0 位

    for (uint16_t i = 0; i < Count; i++) 
    {
        MySPI_SwapByte(DataArray[i]); ///< 依次在起始地址后写入数据
    }

    MySPI_Stop();
    W25Q64_WaitBusy(); ///< 等待忙
}

/**
 * @brief W25Q64 扇区擦除（4KB）
 * @param Address: 指定扇区的地址，范围：0x000000~0x7FFFFF
 * @retval None
 */
void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable();
    MySPI_Start();

    /** SPI 起始 */
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB); ///< 交换发送扇区擦除的指令
    MySPI_SwapByte(Address >> 16); ///< 交换发送地址 23~16 位
    MySPI_SwapByte(Address >> 8); ///< 交换发送地址 15~8 位
    MySPI_SwapByte(Address); ///< 交换发送地址 7~0 位

    MySPI_Stop();
    W25Q64_WaitBusy();
}

/**
 * @brief W25Q64 读取数据
 * @param Address: 读取数据的起始地址，范围：0x000000 ~ 0x7FFFFF
 * @param DataArray: 用于接收读取数据的数组，通过输出参数返回
 * @param Count: 要读取数据的数量，范围：0~0x800000
 * @retval None
 */
void W25Q64_ReadData(uint32_t Address, uint8_t* DataArray, uint32_t Count)
{
    MySPI_Start();

    MySPI_SwapByte(W25Q64_READ_DATA); ///< 交换发送读取数据的指令
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM); ///< 交换发送页编程的指令
    MySPI_SwapByte(Address >> 16); ///< 交换发送地址 23~16 位
    MySPI_SwapByte(Address >> 8); ///< 交换发送地址 15~8 位
    MySPI_SwapByte(Address ); ///< 交换发送地址 7~0 位
    for (uint32_t i = 0; i < Count; i++)
    {
        DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE); ///< 依次在起始地址后读取数据
    }

    MySPI_Stop();
}

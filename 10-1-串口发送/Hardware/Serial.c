#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * @brief 串口初始化
 * @param None
 * @retval None
 */
void Serial_Init()
{
    /** 开启 USART1 和 GPIOA 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /** GPIO 初始化：将 PA9 引脚初始化为复用推挽输出 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /** USART 初始化 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600; ///< 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; ///< 硬件控制流， 不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx; ///< 模式，选择为发送模式
    USART_InitStructure.USART_Parity = USART_Parity_No; ///< 奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1; ///< 停止位，选择 1 位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; ///< 字长，选择 8 位
    USART_Init(USART1, &USART_InitStructure);

    /** USART 使能 */
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief 串口发送一个字节
 * @param Byte: 要发送的一个字节
 * @retval None
 * @attention 下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位
 */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte); ///< 将字节数据写入数据寄存器，写入后 USART 自动生成时序波形
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); ///< 等待发送完成
}
/**
 * @brief 串口发送一个数组
 * @param Array: 要发送数组的首地址
 * @param Length: 要发送数组的长度
 * @retval None
 */
void Serial_SendArray(uint8_t* Array, uint16_t Length)
{
    for (uint16_t i = 0; i < Length; i++) 
    {
        Serial_SendByte(Array[i]); ///< 依次调用 Serial_SendByte 发送每个字节数据
    }
}

/**
 * @brief 串口发送一个字符串
 * @param String: 要发送字符串的首地址
 * @retval None
 */
void Serial_SendString(int8_t* String)
{
    uint8_t i = 0;
    while (String[i] != '\0') ///< 遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        Serial_SendByte(String[i]); ///< 依次调用 Serial_SendByte 发送每个字节数据
        i++;
    }
}

/**
 * @brief 次方函数
 * @note 该函数仅内部使用
 * @param X: 底
 * @param Y: 次方
 * @return Result 返回值等于 X 的 Y 次方
 */
uint16_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }

    return Result;
}

/**
 * @brief 串口发送数字
 * @param Number: 要发送的数字，范围：0~4294967295
 * @param Length: 要发送数字的长度，范围：0~10
 * @retval None
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length) 
{
    for (uint8_t i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0'); ///< 依次调用 Serial_SendByte 发送每位数字
    }
}

/**
 * @brief 使用 printf 需要重定向的底层函数
 * @param 保持原始格式即可，无需变动
 * @return 保持原始格式即可，无需变动
 * @attention 
 * 这里涉及到可变参数函数的使用，如有不懂可在网上查找资料学习
 */
int fput(int ch, FILE* f)
{
    Serial_SendByte(ch); ///< 将 printf 的底层重定向到自己的发送字节函数

    return ch;
}

/**
 * @brief 自己封装 prinf 函数
 * @param format: 格式化字符串
 * @param ...: 可变参数列表
 * @retval None
 * @attention 
 * 这里涉及到可变参数函数的使用，如有不懂可在网上查找资料学习
 */
void Serial_Printf(int8_t* format, ...)
{
    int8_t String[100];
    va_list arg; ///< 定义可变参数列表数据类型的变量 arg
    va_start(arg, format); ///< 从 format 开始，接收参数列表到 arg 变量
    vsprintf(String, format, arg); ///< 使用 vsprintf 打印格式化字符串和参数列表到字符数组中
    va_end(arg); ///< 结束变量arg
    Serial_SendString(String);
}

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxPacket[100];///< 定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t Serial_RxFlag; ///< 定义串口接收的标志位变量

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

    /** GPIO 初始化：将 PA10 引脚初始化为上拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /** USART 初始化 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600; ///< 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; ///< 硬件控制流， 不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; ///< 模式，选择为发送和接受模式
    USART_InitStructure.USART_Parity = USART_Parity_No; ///< 奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1; ///< 停止位，选择 1 位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; ///< 字长，选择 8 位
    USART_Init(USART1, &USART_InitStructure);

    /* 中断输出配置 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); ///< 开启串口接收数据的中断

    /* NVIC 中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); ///< 配置 NVIC 为分组 2

    /* NVIC 配置 */
	NVIC_InitTypeDef NVIC_InitStructure; ///< 定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; ///< 选择配置 NVIC 的 USART1 线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	///< 指定 NVIC 线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; ///< 指定 NVIC 线路的抢占优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; ///< 指定 NVIC 线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure); ///< 将结构体变量交给 NVIC_Init，配置 NVIC 外设    

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
void Serial_SendString(char* String)
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
 * @brief USART1 中断函数
 * @param None
 * @retval None
 * @attention
 * 此函数为中断函数，无需调用，中断触发后自动执行；
 * 函数名为预留的指定名称，可以从启动文件复制；
 * 请确保函数名正确，不能有任何差异，否则中断函数将不能进入；
 */
void USART1_IRQHandler()
{
    static uint8_t RxState = 0; ///< 定义表示当前状态机状态的静态变量
    static uint8_t pRxPacket = 0; ///< 定义表示当前接收数据位置的静态变量

    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1); ///< 读取数据寄存器，存放在接收的数据变量
        /** 使用操作系统中的状态机思路，依次处理数据包不同部分 */

        /** 当前状态：0，接收数据包包头 */
        if (RxState == 0) 
        {
            if (RxData == '@' && Serial_RxFlag == 0) ///< 如果数据是包头
            {
                RxState = 1; ///< 置下一个状态
                pRxPacket = 0; ///< 数据包的位置归零
            }
        }
        /* 当前状态：1，接收数据包数据 */
        else if (RxState == 1)
        {
            while (RxData != '\r')
            {
                Serial_RxPacket[pRxPacket] = RxState; ///< 将数据存入数据包数组的指定位置
                pRxPacket++;
            }
            RxState = 2; ///< 置下一个状态
        }
        /* 当前状态：2，接收数据包尾 */
        else if (RxState == 2)
        {
            if (RxData== '\n') ///< 如果数据确实是包尾部
            {
                RxData = 0; ///< 状态归 0
                Serial_RxPacket[pRxPacket] = '\0'; ///< 将收到的字符数据包添加一个字符串结束标志
                Serial_RxFlag = 1; ///< 接收数据包标志位置 1，成功接收一个数据包
            }
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE); ///< 清除 USART1 的 RXNE 标志位，读取数据寄存器会自动清除此标志位，如果已经读取了数据寄存器，也可以不执行此代码
    } 
}

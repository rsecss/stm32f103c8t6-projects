#include "stm32f10x.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0 ///< MPU6050 的 I2C 从机地址

/**
 * @brief  MPU6050 等待事件
 * @param  同I2C_CheckEvent
 * @retval  None
 */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
    uint32_t TimeOut = 10000; ///< 给定超时计数时间
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        TimeOut--;
        if (TimeOut == 0) 
        {
            break; ///< 跳出等待
        }
    }
}

/**
 * @brief  MPU6050 写寄存器
 * @note 
 * 在指定位置写的步骤（7位主发送）：
 * 1. 先开启硬件 I2C 通道
 * 2. 等待 EV5
 * 3. 硬件发送从机地址
 * 4. 等待 EV6
 * 5. 硬件发送寄存器地址
 * 6. 等待 EV8
 * 7. 发送要写入寄存器的数据
 * 8. 等待 EV8_2
 * 9. 发送完后，硬件生成终止条件
 * @param  RegAddress: 寄存器地址，范围：参考 MPU6050 手册的寄存器描述 
 * @param  Data: 要写入寄存器的数据，范围：0x00~0xFF
 * @retval  None
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    I2C_GenerateSTART(I2C2, ENABLE); ///< 硬件 I2C 生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); ///< 等待 EV5
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); ///< 硬件 I2C 发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); ///< 等待 EV6
    I2C_SendData(I2C2, RegAddress); ///< 硬件 I2C 发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); ///< 等待 EV8
    I2C_SendData(I2C2, Data); ///< 硬件 I2C 发送数据
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); ///< 等待 EV8_2
    I2C_GenerateSTOP(I2C2, ENABLE); ///< 硬件 I2C 生成终止条件
}

/**
 * @brief  MPU6050 读寄存器
 * @note 
 * 在指定位置写的步骤（7位主接收）：
 * 1. 先开启硬件 I2C 通道
 * 2. 等待 EV5
 * 3. 硬件发送从机地址，方向为发送
 * 4. 等待 EV6
 * 5. 硬件发送寄存器地址
 * 6. 等待 EV8_2
 * 7. 硬件 I2C 重复开启
 * 8. 等待 EV5
 * 9. 硬件发送从机地址，方向为接受
 * 10. 等待 EV6，同时将应答失能和申请停止条件的产生
 * 11. 等待 EV7
 * 12. 利用变量接受数据
 * 13. 恢复应答使能
 * @param  RegAddress: 寄存器地址，范围：参考 MPU6050 手册的寄存器描述 
 * @retval  Data: 读取寄存器的数据，范围：0x00~0xFF
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    I2C_GenerateSTART(I2C2, ENABLE); ///< 硬件 I2C 生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); ///< 等待 EV5
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); ///< 硬件 I2C 发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); ///< 等待 EV6
    I2C_SendData(I2C2, RegAddress); ///< 硬件 I2C 发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); ///< 等待 EV8_2

    I2C_GenerateSTART(I2C2, ENABLE); ///< 硬件 I2C 生成重复起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); ///< 等待 EV5
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver); ///< 硬件 I2C 发送从机地址，方向为接收    
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); ///< 等待 EV6
    I2C_AcknowledgeConfig(I2C2, DISABLE); ///< 在接收最后一个字节之前提前将应答失能
    I2C_GenerateSTOP(I2C2, ENABLE); ///< 在接收最后一个字节之前提前申请停止条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);  ///< 等待 EV7
    Data = I2C_ReceiveData(I2C2); ///< 接受数据寄存器
    I2C_AcknowledgeConfig(I2C2, ENABLE); ///< 将应答恢复为使能，为了不影响后续可能产生的读取多字节操作

    return Data;
}

/**
 * @brief I2C 初始化
 * @param None
 * @retval None
 */
void MyI2C_Init()
{
    /** 开启 I2C 和 GPIOB 的时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /** GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); ///< 将 PB10 和 PB11 引脚初始化为复用开漏输出

    /** I2C 初始化 */
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; ///< 模式，选择为 I2C 模式
    I2C_InitStructure.I2C_ClockSpeed = 50000; ///< 时钟速度，选择为 50KHz
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; ///< 时钟占空比，选择 Tlow/Thigh = 2
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; ///< 应答，选择使能
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; ///< 应答地址，选择7位，从机模式下才有效
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; ///< 自身地址，从机模式下才有效
    I2C_Init(I2C2, &I2C_InitStructure);

    /** I2C 使能 */
    I2C_Cmd(I2C2, ENABLE);
}

/**
 * @brief  MPU6050 初始化
 * @param  None
 * @retval  None
 */
void MPU6050_Init()
{
    MyI2C_Init(); ///< 初始化底层的 I2C

    /* MPU6050 寄存器初始化，需要对照 MPU6050 手册的寄存器描述配置，此处仅配置了部分重要的寄存器 */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); ///< 电源管理寄存器 1，取消睡眠模式，选择时钟源为 X 轴陀螺仪
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); ///< 电源管理寄存器 2，保持默认值 0，所有轴均不待机
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); ///< 采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);	///< 配置寄存器，配置 DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); ///< 陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); ///< 加速度计配置寄存器，选择满量程为±16g    
}

/**
 * @brief  MPU6050 获取 ID 号
 * @param  None
 * @retval  MPU6050 的 ID 号
 */
uint8_t MPU6050_ID()
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I); ///< 返回 WHO_AM_I 寄存器的值
}

/**
 * @brief MPU6050 获取数据
 * @param AccX: 加速度计 X 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @param AccY: 加速度计 Y 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @param ACCZ: 加速度计 Z 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @param GyroX: 陀螺仪 X 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @param GyroY: 陀螺仪 Y 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @param GyroZ: 陀螺仪 Z 轴的数据，使用输出参数的形式返回，范围：-32768~32767
 * @retval None
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
					 int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint16_t DataHigh, DataLow;

    /** 分别读取加速度计 X,Y,Z 轴的高八位和低八位的数据 */
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (DataHigh << 8) | DataLow;
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataHigh << 8) | DataLow;
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataHigh = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataHigh << 8) | DataLow;

    /** 分别读取陀螺仪计 X,Y,Z 轴的高八位和低八位的数据 */
    DataHigh = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataLow = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataHigh << 8) | DataLow;
    DataHigh = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataLow = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataHigh << 8) | DataLow;
    DataHigh = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataLow = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataHigh << 8) | DataLow;
}

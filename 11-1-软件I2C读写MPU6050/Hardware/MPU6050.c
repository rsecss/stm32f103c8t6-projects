#include "stm32f10x.h"
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0 ///< MPU6050 的 I2C 从机地址

/**
 * @brief MPU6050 写寄存器
 * @note 
 * 在指定位置写的步骤：
 * 1. 先开启 I2C 通道
 * 2. 发送从机地址，并接受应答
 * 3. 发送寄存器地址，并接受应答
 * 4. 发送要写入寄存器的数据，并接受应答（如果要发送更多数据，应该在此处使用循环）
 * 5. 发送完后，应该关闭 I2C 通道 
 * @param RegAddress: 寄存器地址，范围：参考 MPU6050 手册的寄存器描述 
 * @param Data: 要写入寄存器的数据，范围：0x00~0xFF
 * @retval None
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS); ///< 发送从机地址，读写位为 0，表示即将写入
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress); ///< 发送寄存器地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data); ///< 发送要写入寄存器的数据
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

/**
 * @brief MPU6050 读寄存器
 * @note 
 * 在指定位置写的步骤：
 * 1. 先开启 I2C 通道
 * 2. 发送从机地址，并接受应答
 * 3. 发送寄存器地址，并接受应答
 * 4. I2C 重复起始打开
 * 5. 发送从机地址（读写位为 1），并接受应答
 * 6. 定义一个变量来接受指定寄存器的数据
 * 7. 如果要继续接受同样使用循环，如果不继续接受则应该发送给从机非应答，终止从机输出
 * 8. 终止从机接受数据输出后，应该关闭 I2C 通道 
 * @param RegAddress: 寄存器地址，范围：参考 MPU6050 手册的寄存器描述 
 * @retval Data: 读取寄存器的数据，范围：0x00~0xFF
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS); ///< 发送从机地址，读写位为 0，表示即将写入
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress); ///< 发送寄存器地址
    MyI2C_ReceiveAck();
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01); ///< 发送从机地址，读写位为 1，表示即将读取
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte(); ///< 接收指定寄存器的数据
    MyI2C_SendAck(1); ///< 发送应答，给从机非应答，终止从机的数据输出
    MyI2C_Stop();

    return Data;
}

/**
 * @brief MPU6050 初始化
 * @param None
 * @retval None
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
 * @brief MPU6050 获取 ID 号
 * @param None
 * @retval MPU6050 的 ID 号
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

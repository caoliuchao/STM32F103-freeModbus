**想要详细了freeModbus工作模式和移植方法等，可以访问下面网址**
**freeModbus对应的网址：**<https://www.embedded-solutions.at/en/freemodbus/>

**本工程的主控芯片是STM32F103ZET6,在UCOSII的任务中移植了freeModbus，**
**如果添加到你的过程中可能USART接口不太一致，需要在PORT.h文件中修改。**

**修改为你需要的usart和timer即可**，**portserial.c和porttimer.c修改源码。**

```c
//初始化freemodbus 设置RTU模式和ID等
eStatus = eMBInit(MB_RTU, 0x0A,2, 38400, MB_PAR_NONE);
if (MB_ENOERR != eStatus)
{
	printf("freemodbus 设置RTU或模式失败\r\n");
      return false;
}
```

```c
 /* 激活协议栈以及串口和定时器而已。*/ 
 eStatus = eMBEnable();
 if (MB_ENOERR != eStatus)
 {
	printf("使能ModbusProtocolStack失败\r\n");
      return false;
 }
```

**需一定时间间隔调用下面函数，建议5ms调用一次。**

```c
//一定时间间隔的轮训函数
 eMBPoll();
```


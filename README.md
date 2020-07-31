# stm_cpp_test_c8t6
测试单片机上运行C++（由于单片机工程默认生成的主程序是main.c，所以只能采用C/C++混编的形式）

## 文件说明

- `main.c`（`main.h`）：主程序，IDE自动生成的，只能是`.c`。
- `usart.c`（`usart.h`）：串口操作的相关库，定义了串口发送，串口接收的缓冲区及其中断响应函数。
- `algomain.cpp`（`algomain.h`）：用C++写的一个简单的类，可以存储若干字符串并记录重复次数，支持查询重复次数最多字符串的重复次数。（内容很简单，重点在测试C++语法写的程序能不能运行）
- `bridge.cpp`（`bridge.h`）：Bridging-Header头文件，将C++的函数封装为C可调用的接口。

## 关于串口的说明

串口发送没啥好说的，用`uart_fputs`函数即可。

关于串口接收，需要改两个地方，首先是`stm32f1xx_it.c`中的`USART1_IRQHandler`函数体中，在`USER CODE USART1_IRQn 0`注释块中添加自己的响应函数。

```c
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	call_on_USART_IRQHandler(&huart1, &USART1_Buffer);
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
```

然后在`stm32f1xx_hal_msp.c`的`HAL_UART_MspInit`函数的`USER CODE USART1_MspInit 1`注释块中添加一句代码（`__HAL_UART_ENABLE_IT`），启动该串口的中断。

> 这个地方我最开始忘加了，然后导致的现象是发送正常但是无法响应接收，即使`USART1_IRQHandler`是正确的。

```c
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE); // This line is necessary.
  /* USER CODE END USART1_MspInit 1 */
  }

}
```

另一个需要注意的点是，不要在串口接收的响应函数中使用`HAL_Delay`或者调用其他用到了`HAL_Delay`的函数（我的主函数的无限循环中有闪烁LED的代码，初步猜测在中断处闪灯调用了Delay会破坏掉主函数中正在运行的Delay）
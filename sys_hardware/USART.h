#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <string.h>//strlen
#include <stdio.h>//微库
#include <stddef.h>//size_t类型

//选择使用哪个USART,usart1和usart2所挂载的总线不同
#define usartx     2     //1表示开启usart1   0表示开启usart2

//接收缓存区大小(x-1个字节)
#define usart_rx_buff_size 32
#define usart_rx_baud      115200
#define TIM3CLK            72000000

#if (usartx == 1)
void usart1_init(void);
void usart1_send_Char(uint8_t character);
void usart1_send_str(char *str);
#elif (usartx ==2)
void usart2_init(void);
void usart2_send_Char(uint8_t character);
void usart2_send_str(char *str);
#endif

void rx_data_proc(void);





//void test(void);

#endif // !__USART_H


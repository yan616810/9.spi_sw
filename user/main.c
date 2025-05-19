/**
 * @file main.c
 * @author YLAD (you@domain.com)
 * @brief 软件spi，1.每次开机在读取flash中，上次设备断电前存储的实时UTC时间 2.向flash写入UTC的时区信息
 * @version 0.1
 * @date 2025-04-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f10x.h"                  // Device header
// #include "smg.h"
#include "Delay.h"
#include "key.h"
// #include "OLED.h"
// #include "BKP.h"
#include "RTC.h"
#include "USART.h"
#include <stdio.h>
#include "iic.h"
// #include "mpu6050.h"
#include "u8g2_monochrome_display.h"
#include <string.h>//memset
#include "hw_iic.h"

volatile uint16_t second_cnt=1000;
volatile uint8_t key_cnt=10,key_task_cnt=10;
volatile uint8_t ms_cnt=100;
volatile uint8_t ms_25_cnt=25;
/*RTC*/
Type_Struct_Timezone_and_UTCxTime RTC_Init_And_Adjustment,RTC_read_RTCStruct;

//u8g2
u8g2_t u8g2;
char u8g2_buf[18];

uint16_t second;

uint8_t ii=1;

uint8_t smg_ui_root;

// short gx,gy,gz,ax,ay,az;

void timer2_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		
		TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
		
		TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period=1000-1;
		TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	}
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	{
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=15;//最低优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
		NVIC_Init(&NVIC_InitStruct);
	}
	TIM_Cmd(TIM2,ENABLE);
	
}


/**
 * @brief 按键处理任务
 * 
 */
void key_task()
{
	if(key_value=='L')//长按B13清屏
	{
		key_value=0;
	}
	else if (key_value==1)//B13
	{
		key_value=0;

	}
	else if (key_value==2)//B12
	{
		key_value=0;

	}
}
 
void task_proc(void)
{
	if(second_cnt == 1000)//1s
	{
		second_cnt=0;
		usart_send_str("[--YLAD--]\r\n");
				
	}
	if(key_cnt==10)//10ms
	{
		key_cnt=0;
		get_key();

		key_task_cnt++;//每200ms处理一下按键任务
		if(key_task_cnt==20)
		{
			key_task_cnt=0;
			key_task();
		}
	}
	if(ms_cnt==100)//100ms
	{
		ms_cnt=0;		
		// //将上一次画布缓冲区角速度旧值区域填充空Box
		// u8g2_SetDrawColor(&u8g2,0);
		// u8g2_DrawBox(&u8g2,2*7,0*10,6*7,10);
		// u8g2_DrawBox(&u8g2,2*7,1*10,6*7,10);
		// u8g2_DrawBox(&u8g2,2*7,2*10,6*7,10);
		// //在缓冲区空Box处写入角速度数据
		// u8g2_SetDrawColor(&u8g2,1);
		// memset(u8g2_buf, 0, sizeof(u8g2_buf));  // 将数组所有字节设为0
		// sprintf(u8g2_buf,"%+6d",gx);
		// u8g2_DrawStr(&u8g2,2*7,0*10,u8g2_buf);
		// memset(u8g2_buf,0,sizeof(u8g2_buf));
		// sprintf(u8g2_buf,"%+6d",gy);
		// u8g2_DrawStr(&u8g2,2*7,1*10,u8g2_buf);
		// memset(u8g2_buf,0,sizeof(u8g2_buf));
		// sprintf(u8g2_buf,"%+6d",gz);
		// u8g2_DrawStr(&u8g2,2*7,2*10,u8g2_buf);
		// //将上一次画布缓冲区重力加速度旧值区域填充空Box
		// u8g2_SetDrawColor(&u8g2,0);
		// u8g2_DrawBox(&u8g2,11*7,0*10,6*7,10);
		// u8g2_DrawBox(&u8g2,11*7,1*10,6*7,10);
		// u8g2_DrawBox(&u8g2,11*7,2*10,6*7,10);
		// //在缓冲区空Box处写入重力加速度数据
		// u8g2_SetDrawColor(&u8g2,1);
		// memset(u8g2_buf, 0, sizeof(u8g2_buf));  // 将数组所有字节设为0
		// sprintf(u8g2_buf,"%+6d",ax);
		// u8g2_DrawStr(&u8g2,11*7,0*10,u8g2_buf);
		// memset(u8g2_buf,0,sizeof(u8g2_buf));
		// sprintf(u8g2_buf,"%+6d",ay);
		// u8g2_DrawStr(&u8g2,11*7,1*10,u8g2_buf);
		// memset(u8g2_buf,0,sizeof(u8g2_buf));
		// sprintf(u8g2_buf,"%+6d",az);
		// u8g2_DrawStr(&u8g2,11*7,2*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%4d",RTC_read_RTCStruct.UTCxTime.tm_year);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,0*10,4*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,0*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%2d",RTC_read_RTCStruct.UTCxTime.tm_mon);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,1*10,2*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,1*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%2d",RTC_read_RTCStruct.UTCxTime.tm_mday);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,2*10,2*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,2*10,u8g2_buf);		

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%02d",RTC_read_RTCStruct.UTCxTime.tm_hour);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,3*10,2*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,3*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%02d",RTC_read_RTCStruct.UTCxTime.tm_min);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,4*10,2*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,4*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%02d",RTC_read_RTCStruct.UTCxTime.tm_sec);
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,5*7,5*10,2*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,5*7,5*10,u8g2_buf);

		memset(u8g2_buf, 0, sizeof(u8g2_buf));
		sprintf(u8g2_buf,"%10lu",RTC_read_Timestamp());
		u8g2_SetDrawColor(&u8g2,0);
		u8g2_DrawBox(&u8g2,8*7,5*10,10*7,10);
		u8g2_SetDrawColor(&u8g2,1);
		u8g2_DrawStr(&u8g2,8*7,5*10,u8g2_buf);

		u8g2_SendBuffer(&u8g2);
	}
	if(ms_25_cnt==25)
	{
		ms_25_cnt=0;
		// MPU_Get_Gyroscope(&gx,&gy,&gz);
		// MPU_Get_Accelerometer(&ax,&ay,&az);

		RTC_get_DataStruct(&RTC_read_RTCStruct,&RTC_Init_And_Adjustment);
	}
}

int main(void)
{
/*按键初始化*/
	key_init();
/*串口初始化*/
	usart1_init();
/*IIC协议端口初始化 && 以极低的协议速度搜索iic设备并通过串口打印地址信息*/
	IIC_InitPins_or_ChangePins(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_8,RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_9);
	IIC_Set_speed(10);//防止iic协议速度过快，搜索不到低速设备
	IIC_Search_all_devices_printf_example();
	IIC_Set_speed(1);
/*oled初始化*/
	u8g2_oled_init(&u8g2);
	// u8g2_oled_play_Animation(&u8g2);
	u8g2_SetFont(&u8g2,u8g2_font_courB08_tr);//w=7  h=10
	u8g2_SetFontPosTop(&u8g2);
	u8g2_SetFontMode(&u8g2,0);//显示字体的背景，不透明
	u8g2_SetDrawColor(&u8g2,1);
	// u8g2_DrawStr(&u8g2,0,0*10,"X=");
	// u8g2_DrawStr(&u8g2,0,1*10,"Y=");
	// u8g2_DrawStr(&u8g2,0,2*10,"Z=");
	// u8g2_DrawStr(&u8g2,0,3*10,"Gyro");
	// u8g2_DrawStr(&u8g2,9*7,0*10,"X=");
	// u8g2_DrawStr(&u8g2,9*7,1*10,"Y=");
	// u8g2_DrawStr(&u8g2,9*7,2*10,"Z=");
	// u8g2_DrawStr(&u8g2,9*7,3*10,"Acce");

	u8g2_DrawStr(&u8g2,0,0,"RTC initing...");

	u8g2_SendBuffer(&u8g2);
/*片上RTC时钟初始化*/
	RTC_Filling_DataStruct(&RTC_Init_And_Adjustment,8,2025,5,19,15,58,0,-1);
	RTC_init(&RTC_Init_And_Adjustment);
	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawStr(&u8g2,0,0,"year->");
	u8g2_DrawStr(&u8g2,0,1*10,"mon ->");
	u8g2_DrawStr(&u8g2,0,2*10,"day ->");
	u8g2_DrawStr(&u8g2,0,3*10,"hour->");
	u8g2_DrawStr(&u8g2,0,4*10,"min ->");
	u8g2_DrawStr(&u8g2,0,5*10,"sec ->");
	u8g2_SendBuffer(&u8g2);
/*mpu6050初始化*/
	// while(MPU_Init()){printf("mpu6050 init error!\r\n");Delay_ms(500);}
/*任务滴答*/
	timer2_init();
	usart_send_str("system init success!!!\r\n");
	while(1)
	{
		task_proc();
		rx_data_proc();
	}
}

void TIM2_IRQHandler(void)//1ms
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		if(second_cnt<1000)second_cnt++;
		if(ms_cnt<100)ms_cnt++;
		if(key_cnt<10)key_cnt++;
		if(ms_25_cnt<25)ms_25_cnt++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//用函数指针替代
void rx_data_procCallback(uint16_t cmd_num)//Starting from 0
{
	switch(cmd_num)
	{
		case 0://cmd1
			printf("rx_buff=cmd1 -> Ctrl_1\r\n");
			break;
		case 1://cmd2
			printf("rx_buff=cmd2 -> Ctrl_2\r\n");
			break;
		case 2://cmd3
			printf("rx_buff=cmd3 -> Ctrl_3\r\n");
			break;
		case 3://help
			printf("rx_buff=help -> I will help you!\r\n");
			break;
		case 4://YLAD
			printf("\
[===============YLAD=============]\r\n\
[================================]\r\n\
[*****欢迎来到无人机租赁公司*******]\r\n\
[1.关于公司。。。                 ]\r\n\
[2.立刻注册免费租一架试玩10min!    ]\r\n\
[3.更多功能敬请期待！             ]\r\n\
");
			break;
		default:break;
	}
}




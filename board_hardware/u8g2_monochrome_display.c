#include "u8g2_monochrome_display.h"

//软件iic，u8x8_gpio_and_delay_for_sw_iic()函数中用于初始化软件iic引脚
void oled_pin_init(void)//SCL=PB8     SDA=PB9
{
	RCC_APB2PeriphClockCmd(u8g2_iic_sw_scl_RCC_APB2Periph_GPIOx|u8g2_iic_sw_sda_RCC_APB2Periph_GPIOx,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=u8g2_iic_sw_scl_pin;//SCL
	GPIO_Init(u8g2_iic_sw_scl_port,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=u8g2_iic_sw_sda_pin;//SDA
	GPIO_Init(u8g2_iic_sw_sda_port,&GPIO_InitStruct);
}

// 软件iic使用的回调
uint8_t u8x8_gpio_and_delay_for_sw_iic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint32_t delay_us;
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
		oled_pin_init();//SCL和SDA引脚初始化
      break;							// can be used to setup pins
    // case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
		// 
    //   break;    
    // case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
		// __NOP();
    //   break;
    // case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
		// Delay_us(10*arg_int);
    //   break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
		Delay_ms(1*arg_int);//OLED上电复位开机顺序，等待100ms才可写寄存器
      break;
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
		delay_us = 5000 / arg_int; // 单位：ns
		delay_us /= 1000; // 转换为μs
		Delay_us(delay_us);//默认是1us，400KHZ
		// Delay_us(5);
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    // case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
    // case U8X8_MSG_GPIO_SPI_CLOCK:
    //   break;
    // case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
    // case U8X8_MSG_GPIO_SPI_DATA:
    //   break;
    // case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
    //   break;
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
		//在这里我们不使用重置引脚，貌似我的显示屏也没有？？
		//iic协议中有对这个引脚进行操作(具体在u8g2_InitDisplay(u8g2);函数中U8X8_MSG_DISPLAY_INIT条件下的u8x8_d_helper_display_init();的第3)步会进行操作)，1->delay_ms(100)->0->delay_ms(100)->1->delay_ms(100);
      break;
    // case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
    //   break;
    // case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
    //   break;
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
		arg_int ? GPIO_SetBits(u8g2_iic_sw_scl_port,u8g2_iic_sw_scl_pin) : GPIO_ResetBits(u8g2_iic_sw_scl_port,u8g2_iic_sw_scl_pin);
      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
		arg_int ? GPIO_SetBits(u8g2_iic_sw_sda_port,u8g2_iic_sw_sda_pin) : GPIO_ResetBits(u8g2_iic_sw_sda_port,u8g2_iic_sw_sda_pin);
      break;							// arg_int=1: Input dir with pullup high for I2C data pin
    // case U8X8_MSG_GPIO_MENU_SELECT:
    //   u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_NEXT:
    //   u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_PREV:
    //   u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
    //   break;
    // case U8X8_MSG_GPIO_MENU_HOME:
    //   u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
    //   break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}

/***************************以下两个回调函数实现u8g2与硬件iic之间的接口**********************************/

//硬件iic使用的回调
uint8_t u8x8_gpio_and_delay_for_hw_iic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t delay_us;
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
      //hw_iic初始化
      hw_iic_init();
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
      Delay_ms(10*arg_int);
      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
      Delay_ms(arg_int);//屏幕上电复位会使用到，如ssd1306的100ms延时
      break;
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      delay_us = 5000 / arg_int; // 单位：ns
		  delay_us /= 1000; // 转换为μs
		  Delay_us(delay_us);//默认是1us，400KHZ
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
      break;							// arg_int=1: Input dir with pullup high for I2C data pin
    default:
      u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}

//仿照官方的软件iic接口u8x8_byte_sw_i2c()，仿写出硬件iic接口回调
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
  uint8_t timeout;
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while(arg_int > 0)
      {
        I2C_SendData(I2C1,*data);
        while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)//DR和数据移位寄存器都是空
        {
            timeout++;Delay_us(1);
            if(timeout==100)
            {
                I2C_GenerateSTOP(I2C1,ENABLE);
                return 0;
                // break;
            }
        }
        timeout=0;
        data++;
        arg_int--;
      }

      // data = (uint8_t *)arg_ptr;
      // while( arg_int > 0 )
      // {
	      // i2c_write_byte(u8x8, *data);
	      // data++;
	      // arg_int--;
      // }
      break; 
    case U8X8_MSG_BYTE_INIT:
      // i2c_init(u8x8);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
      {
          timeout++;Delay_us(1);
          if(timeout==100)
          {
              I2C_GenerateSTOP(I2C1,ENABLE);
              return 0;
              // break;
          }
      }
      timeout=0;   
      I2C_GenerateSTART(I2C1,ENABLE);
      while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
      {
          timeout++;Delay_us(1);
          if(timeout==100)
          {
              I2C_GenerateSTOP(I2C1,ENABLE);
              return 0;
              // break;
          }
      }
      timeout=0;
      I2C_Send7bitAddress(I2C1,u8x8_GetI2CAddress(u8x8),I2C_Direction_Transmitter);
      while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
      {
          timeout++;Delay_us(1);
          if(timeout==100)
          {
              I2C_GenerateSTOP(I2C1,ENABLE);
              return 0;
              // break;
          }
      }
      timeout=0;      
      
      // i2c_start(u8x8);
      // i2c_write_byte(u8x8, u8x8_GetI2CAddress(u8x8));
      // //i2c_write_byte(u8x8, 0x078);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      I2C_GenerateSTOP(I2C1,ENABLE);
      
      // i2c_stop(u8x8);
      break;
    default:
		return 0;
    }
  return 1;
}

/***************************初始化一个u8g2屏幕实例************************/

void u8g2_oled_init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_hw_i2c,u8x8_gpio_and_delay_for_hw_iic);
	  u8g2_InitDisplay(u8g2);//in sleep
	  u8g2_SetPowerSave(u8g2,0);//disable sleep
	  u8g2_ClearBuffer(u8g2);
}

/************************************以下是非必要函数，都是u8g2的上层使用**************************************/

void u8g2_oled_play_Animation(u8g2_t *u8g2)
{
//	u8g2_DrawBox(u8g2,0,0,128,128);
//	u8g2_SetFont(u8g2,u8g2_font_5x7_tr);
//	u8g2_SetFontMode(u8g2,0);//0：填充  1：透明  
//	u8g2_SetDrawColor(u8g2,0);//字符由像素点亮表示   2：异或
//	u8g2_SetFontPosTop(u8g2);
//	u8g2_DrawStr(u8g2,0,0,"[==hello,world==]");

	u8g2_SetFont(u8g2,u8g2_font_unifont_t_76);//u8g2_font_unifont_t_cards  32-117
	u8g2_SetFontPosTop(u8g2);
	u8g2_SetFontMode(u8g2,1);
	u8g2_SetDrawColor(u8g2,1);
	u8g2_DrawGlyphX2(u8g2,30,20,9762);//核警告
	u8g2_DrawGlyphX2(u8g2,70,20,9763);//生化警告
	
	// 9728-9856
	u8g2_SetFont(u8g2,u8g2_font_koleeko_tr);
	u8g2_DrawStr(u8g2,20,2,">-WARNING-<");
	u8g2_DrawRFrame(u8g2,0,0,128,64,5);
	u8g2_DrawHLine(u8g2,3,1,122);
	u8g2_DrawHLine(u8g2,0,16,127);
	u8g2_DrawBox(u8g2,0,13,128,3);
	u8g2_SendBuffer(u8g2);
	
	Delay_ms(500);
	u8g2_ClearBuffer(u8g2);
	u8g2_DrawStr(u8g2,20,2,">-WARNING-<");
	u8g2_DrawRFrame(u8g2,0,0,128,64,5);
	u8g2_DrawHLine(u8g2,3,1,122);
	u8g2_DrawHLine(u8g2,0,16,127);
	u8g2_DrawBox(u8g2,0,13,128,3);
{
	for(uint16_t i=32;i<=118;i++)
	{
		u8g2_SetFont(u8g2,u8g2_font_unifont_t_cards);
		u8g2_DrawGlyphX2(u8g2,50-u8g2_GetXOffsetGlyph(u8g2,i),25+2*13,i);//x,y都有偏移,50,20才是真正坐标
		u8g2_SendBuffer(u8g2);
		u8g2_SetFont(u8g2,u8g2_font_koleeko_tr);
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawStr(u8g2,20,2,">-WARNING-<");
		u8g2_DrawRFrame(u8g2,0,0,128,64,5);
		u8g2_DrawHLine(u8g2,3,1,122);
		u8g2_DrawHLine(u8g2,0,16,127);
		u8g2_DrawBox(u8g2,0,13,128,3);
//		Delay_ms(1);
	}
}
  u8g2_ClearDisplay(u8g2);

}

//空间中画一个长方体，
void u8g2_oled_draw_cube(void)
{

}
static void creat_matrix(uint16_t w,uint16_t h)
{

}

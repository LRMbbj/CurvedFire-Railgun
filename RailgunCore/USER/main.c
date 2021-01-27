#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "lcd.h"
#include "ur.h"

int main(void)
{
	//Init
	u8 lcd_id[12];
	delay_init(168);
	uart_init(115200);
	PWM_Init();
	LCD_Init();
	
	
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将 LCD ID打印到 lcd_id数组。
	
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	
	while(1)
	{
		LCD_Clear(WHITE);
		
		LCD_ShowString( 30, 40, 7*12, 24, 24, (u8 *)"RAILGUN CONTROL PANEL");
		LCD_ShowString( 30, 70, 10*12, 24, 24, (u8 *)"ROTATE:");
		LCD_ShowString( 150, 70, 10*12, 24, 24, (u8 *)"+ 0 deg");
		LCD_ShowString( 30, 110, 10*12, 24, 24, (u8 *)"PITCH:");
		LCD_ShowString( 150, 110, 10*12, 24, 24, (u8 *)"+ 0 deg");
		
		delay_ms(2000);
		
		//测试LCD
	}
	
	//return 0;
	
}

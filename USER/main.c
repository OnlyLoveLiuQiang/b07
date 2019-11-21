#include "stm32f10x.h"
#include "key.h"
#include "exti.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "sd.h"
#include "ff.h"
#include "SIM868.h"
#include "spi.h"
#include "mpu9250.h"
#include "bmp280.h"

typedef struct{
	long temp;
	unsigned long press;
	unsigned char accel[6];
	unsigned char gyro[6];
	unsigned char mag[6];
	unsigned char whoami;
	unsigned char id;
}TEST;

//1������MPU9250��BMP280
//2������SIM868��GSM��GPS
int main()
{
	unsigned int m = 0;
	TEST test;
	key_init();
	led_init();
	key_on();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart1_init(115200);
	uart2_init(115200);
	uart3_init(115200);
	spi2_init();
	delay_ms(50);
	MPU9250_init();
	delay_ms(50);
	bmp280_init();
	delay_ms(50);
	test.whoami = MPU9250_Read_Reg(WHO_AM_I);
	test.id = bmp280_read(ID);
	OPEN_SIM868();
	printf("+++");
	GPS_init();
	GSM_init();
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	while(1){
		READ_MPU9250_ACCEL(test.accel);
		READ_MPU9250_GYRO(test.gyro);
		READ_MPU9250_MAG(test.mag);
		test.temp = bmp280_gettemperature();
		test.press = bmp280_getcompensate();
		if(m<60){
			delay_ms(500);
			Usart_SendStr_length(USART3,(unsigned char *)(&test),28);
			delay_ms(500);
			if(m == 59){
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			}
		}else{
			delay_ms(500);
			Usart_SendStr_length(USART1,(unsigned char *)(&test),28);
			delay_ms(500);
		}
		m++;
	}
}

/*
//���Դ���2
//printf�Ӵ���2�������
int main()
{
	key_init();
	led_init();
	key_on();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart3_init(115200);
	printf("hello world 5u-clound");
	while(1){
		delay_ms(1000);
	}
}
*/

/*
//����SIM868оƬ
//printf�򴮿�1���
int main()
{
	key_init();
	led_init();
	key_on();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart1_init(115200);
	uart2_init(115200);
	uart3_init(115200);
	OPEN_SIM868();
//	SIM868_GNSS_UART_OFF;
//	printf("+++");
	GPS_init();
//	GSM_init();
	while(1){
//		printf("hello world 5u-clound");
		delay_ms(1000);
	}
}
*/

/*
//����MPU9250
int main()
{
	unsigned char i = 0;
	unsigned char whoami = 0;
	unsigned char accel[6] = "";
	unsigned char gyro[6] = "";
	unsigned char mag[6] = "";
	key_init();
	led_init();
	key_on();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart3_init(115200);
	spi2_init();
	MPU9250_init();
	whoami = MPU9250_Read_Reg(WHO_AM_I);
	delay_ms(1000);
	delay_ms(1000);
	USART_SendData(USART3,whoami);
	while(1){
		READ_MPU9250_ACCEL(accel);
		READ_MPU9250_GYRO(gyro);
		READ_MPU9250_MAG(mag);
		delay_ms(1000);
		for(i=0;i<6;i++){
//			USART_SendData(USART3,accel[i]);
//			USART_SendData(USART3,gyro[i]);
			USART_SendData(USART3,mag[i]);
			delay_ms(50);
		}	
	}
}
*/

/*
//����BMP280
int main()
{
	long temp = 0;
	unsigned long press = 0;
	unsigned char id = 0;
	key_init();
	led_init();
	key_on();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart3_init(115200);
	spi2_init();
	bmp280_init();
	id = bmp280_read(ID);
	delay_ms(1000);
	USART_SendData(USART3,id);
	delay_ms(1000);
	while(1){
		temp = bmp280_gettemperature();
		press = bmp280_getcompensate();
		Usart_SendStr_length(USART3,(unsigned char *)(&temp),4);
		Usart_SendStr_length(USART3,(unsigned char *)(&press),4);
//		printf("temp = %ld press = %ld",temp,press);
		delay_ms(1000);
		delay_ms(1000);
	}
}
*/

/*
//����FatFs�ļ�ϵͳ
//printf�򴮿�3���
int main(){
	unsigned int res;
	FATFS fs;
	FIL fil;
	UINT wbw = 0;
	UINT rbw = 0;
	unsigned char config[11] = "config.txt";
	unsigned char wbuff[32] = "beijing 5u-clound";
	unsigned char rbuff[32] = "";
	key_init();
	led_init();
//	led_on(1);
	key_on();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti0_init();
	uart3_init(115200);
	delay_init();
	while(sd_init());
	//========����������========
	res = f_mount(&fs,"0:",1);
	printf("f_mount:res = %d\n",res);
	//=============================
	//========��/�����ļ�========
	res = f_open(&fil,(const char*)config,FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	f_sync(&fil);
	printf("f_open config:res = %d\n",res);
	//==============================
	//========��SD����д����========
	res = f_write(&fil,(const void*)wbuff,32,&wbw);
	f_sync(&fil);
	printf("f_write config:res = %d wbw = %d\n",res,wbw);
	//==============================
	res = f_lseek(&fil,0);
	//========��SD���ж�ȡ����======
	res = f_read(&fil,(void *)rbuff,32,&rbw);
	printf("f_read hello.txt: res = %d rbuff = %s\n",res,rbuff);
	//==============================
	//========�رմ򿪵��ļ�========
	res = f_close(&fil);
	printf("f_close config:res = %d\n",res);
	//==============================
	//=========�رվ�����=========
	res = f_mount(0,"0:",1);
	printf("f_mount:res = %d\n",res);
	//==============================
	while(1){
		
	}
}
*/

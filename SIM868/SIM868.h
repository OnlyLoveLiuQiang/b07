#ifndef __SIM868_H__
#define __SIM868_H__	

#define CT_SIM868_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define CT_SIM868_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SIM868_GNSS_UART_ON   GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define SIM868_GNSS_UART_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_0)

extern unsigned char RxBuffer1[256];

void OPEN_SIM868(void);
void  GSM_init(void);
void  GPS_init(void);
void BT_init(void);
void BT_connect(void);
#endif

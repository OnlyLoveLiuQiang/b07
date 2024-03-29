#include"SIM868.h"
#include"usart.h"
#include"delay.h"
#include"sys.h"
#include"string.h"
#include"only_strstr.h"

char *Readystrx;
unsigned char getdata[100];
extern unsigned char RxCounter1;

static void pa5_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

static void pb0_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

static void pb1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
}

void OPEN_SIM868(void)
{
	char *strx = NULL;
	pa5_init();
	pb0_init();
	pb1_init();
	printf("AT\r\n"); 
	delay_ms(300);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	printf("AT\r\n"); 
	delay_ms(300);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	if(strx==NULL)
	{
		CT_SIM868_ON;//拉高
		delay_ms(300);
		delay_ms(300);
		CT_SIM868_OFF;//拉低正常开机
		delay_ms(1000);
		delay_ms(1000);
//		CT_SIM868_ON;
	}
	printf("AT\r\n"); 
	delay_ms(300);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	while(strx==NULL)
	{	
			printf("AT\r\n"); 
			delay_ms(300);
			strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	}
	Clear_Buffer1();
//	printf("AT+CBC\r\n");
//	delay_ms(300);
//	Clear_Buffer1();
}

void  GSM_init(void)
{
//	unsigned char i = 0;
	char *strx = NULL;
	printf("AT\r\n"); 
	delay_ms(500);
	printf("AT\r\n"); 
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	while(strx==NULL)
	{
		Clear_Buffer1();	
		printf("AT\r\n"); 
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//返回OK
	}
	printf("ATE0\r\n"); //打开回显
	delay_ms(500);
//	for(i=0;i<255;i++){
//	Clear_Buffer1();	
//	printf("AT+CSQ\r\n"); //检查CSQ，可以利用手机APP查看CSQ值
//	delay_ms(500);
//	}
	/////////////////////////////////
	printf("AT+CPIN?\r\n");//检查SIM卡是否在位
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"+CPIN: READY");//查看是否返回ready
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CPIN?\r\n");
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
	}
	Clear_Buffer1();
	printf("AT+CCID\r\n");
	Clear_Buffer1();
	///////////////////////////////////
	printf(" AT+CREG?\r\n");//查看是否注册GSM网络
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"+CREG: 0,5");//返回正常
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf(" AT+CREG?\r\n");//查看是否注册GSM网络
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"+CREG: 0,1");//返回正常
	}
	Clear_Buffer1();
	/////////////////////////////////////
	printf(" AT+CGREG?\r\n");//查看是否注册GPRS网络
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"+CGREG: 0,5");//，这里重要，只有注册成功，才可以进行GPRS数据传输。
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf(" AT+CGREG?\r\n");//查看是否注册GPRS网络
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"+CGREG: 0,1");//，这里重要，只有注册成功，才可以进行GPRS数据传输。
	}
	Clear_Buffer1();
	printf("AT+CIPCLOSE=1\r\n");//关闭TCP连接，防止前面的连接没有断开
	delay_ms(500);
	printf("AT+CIPSHUT\r\n");//关闭移动场景
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"SHUT OK");//关闭成功
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CIPSHUT\r\n");//关闭移动场景
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"SHUT OK");//关闭成功
	}
	Clear_Buffer1();
	printf("AT+CGCLASS=\"B\"\r\n");//设置GPRS移动台类别为B,支持包交换和数据交换 
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//开启成功
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CGCLASS=\"B\"\r\n");//设置GPRS移动台类别为B,支持包交换和数据交换 
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");////开启成功
	}
	Clear_Buffer1();
	printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//设置PDP上下文,互联网接协议,接入点等信息 APN
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置OK
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//设置PDP上下文,互联网接协议,接入点等信息 APN
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//开启成功
	}
	Clear_Buffer1();
	printf("AT+CGATT=1\r\n");//附着GPRS业务
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//开启成功
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CGATT=1\r\n");//附着GPRS业务
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//开启成功
	}		
	Clear_Buffer1();
	
	printf("AT+CIPCSGP=1,\"CMNET\"\r\n");//设置为GPRS连接模式
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CIPCSGP=1,\"CMNET\"\r\n");
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	}
	Clear_Buffer1();
	printf("AT+CIPMUX=0\r\n");//单路传输
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CIPMUX=0\r\n");
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	}
	Clear_Buffer1();	
	printf("AT+CIPMODE=1\r\n");//打开透传功能
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	while(strx==NULL)
	{
		Clear_Buffer1();
		printf("AT+CIPMODE=1\r\n");//打开透传功能
		delay_ms(500);
		strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//设置ok
	}
	Clear_Buffer1();
	printf("AT+CIPSTART=\"TCP\",\"242e119n19.qicp.vip\",17523\r\n");//这里是需要登陆的IP号码，输入公网的IP以及端口号就可以了。
//	printf("AT+CIPSTART=\"TCP\",\"223.100.24.135\",13000\r\n");
//	printf("AT+CIPSTART=\"TCP\",\"47.92.81.218\",1194\r\n");
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"OK");//检查是否登陆成功
	while(strx==NULL&&Readystrx==NULL)
	{
		strx=only_strstr((const char*)RxBuffer1,(const char*)"CONNECT OK");//判断连接状态
		Readystrx=only_strstr((const char*)RxBuffer1,(const char*)"ALREADY CONNECT"); //已经连接状态
		delay_ms(100);
	}
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	Clear_Buffer1();
}

void  GPS_init(void)
{	
	char *strx = NULL;
	Clear_Buffer1();
	printf("AT+CGPSPWR?\r\n"); //查看是否上电了
	delay_ms(500);
	strx=only_strstr((const char*)RxBuffer1,(const char*)"+CGPSPWR: 0");//返回0，需要重新上电
	if(strx)
	{
		printf("AT+CGPSPWR=1\r\n"); //对模块进行上电
		delay_ms(500);
	}
	Clear_Buffer1();	
	printf("AT+CGPSSTATUS?\r\n"); //查看定位状态
	delay_ms(500);
	Clear_Buffer1();	
	printf("AT+CGPSOUT=255\r\n");//开启数据获取
}	

void BT_init(void)
{
	printf("AT+BTPOWER=1\r\n");
	delay_ms(1000);
	Clear_Buffer1();
	printf("AT+BTHOST=5uCloud\r\n");
	delay_ms(1000);
	Clear_Buffer1();
	printf("AT+BTHOST?\r\n");
	delay_ms(1000);
	Clear_Buffer1();
	printf("AT+BTVIS=1\r\n");
	delay_ms(1000);
	Clear_Buffer1();
}

void BT_connect(void)
{
	char i,j,k;
	char *strx = NULL;
	strx=strstr((const char*)RxBuffer1,(const char*)"+BTPAIRING:");//获取到有连接对象
	if(strx)
	{
		printf("AT+BTPAIR=1,1\r\n");//允许连接
		delay_ms(500);
		Clear_Buffer1();	
    }
	strx=strstr((const char*)RxBuffer1,(const char*)"+BTCONNECTING:");//密码配对ok
	if(strx)
	{
		printf("AT+BTACPT=1\r\n");//正式连接
		delay_ms(500);
		Clear_Buffer1();	
    }
	strx=strstr((const char*)RxBuffer1,(const char*)"+BTCONNECT:");//连接成功返回
	if(strx) RxCounter1 = 0;
	strx=strstr((const char*)RxBuffer1,(const char*)"+BTSPPDATA:");//读取数据
	delay_ms(100);
	if(strx)
	{
		j=0;
		for(i=0;i<RxCounter1;i++)
		{
			if(RxBuffer1[i]==',')
			j++;
			if(j==2)//第二个逗号了，后面跟的是数据
			 {
				k=i;
				break;
			 }
		}
		for(i=k+1;i<RxCounter1;i++)
		getdata[i-k-1]=RxBuffer1[i];
		getdata[i-k-1]=0;
		printf("AT+BTSPPSEND\r\n");//
		delay_ms(100);
		printf((const char *)getdata);
		delay_ms(100);
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = (u8) 0x1a;   
		delay_ms(100);
		Clear_Buffer1();
	}
}

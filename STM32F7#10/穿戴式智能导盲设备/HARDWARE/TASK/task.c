#include "task.h"
#include "picture.h"
#include "gps.h"
#include "string.h"
#include "math.h"
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//串口1,发送缓存区
nmea_msg gpsx; 											//GPS信息
__align(4) u8 dtbuf[50];   								//打印缓存器
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode字符串 
static float g[10] = {0};
void InitTask(void)
{
	static u8 flag[3] = {0},i = 0;
	static u8 buf[10] = {0};
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz
	delay_init(216);                //延时初始化
	uart_init(115200);
	usart3_init(38400);  						//初始化串口3波特率为38400
	uart4_init(9600);  
	uart5_init(921600);
	wt931Init();
	PowerInit();
	LCD_Init();                     //初始化LCD
	LED_Init();
	EXTI_Init();
	MY_ADC_Init();
	my_spi_init();
	XFS_Init();
  POINT_COLOR=BLACK; 
	LCD_ShowString(10,40,300,32,32,"JingMing Technology"); 	
	LCD_ShowString(10,80,350,24,24,"Wearable Smart Guide Device");
	LCD_ShowString(10,110,240,16,16,"STM32F7"); 				
 // LCD_DrawPicture(400,80,496,197,(u16*)(gImage_picture+8));
	XFS_Speech("欢迎使用景明科技第%d代产品。%s",3,"景明，让生活更美好！");
	while(i<2) //尝试连接5次
	{
		flag[0] = Get_ImageMsg(3,buf,0);
		//flag[1] = Get_ImageMsg(3,buf,1);
		//flag[2] = Get_SoundMsg();
		if(flag[0])
			break;
		i++;
	}
	if(i>=2)
		XFS_Speech("初始化失败");
	else
		XFS_Speech("初始化成功");
	LCD_ShowString(10,130,240,16,16,"Complete");
}

u8 Get_ImageMsg(u8 flag, u8* buf, u8 openmvaddr) //flag 0房间编号识别,1图像识别,2人脸识别
{
	u16 i=0,rxlen,j=0,k=0;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);	//PA4置0
	delay_ms(100);
	while(i<2) //最多重发2次
	{
		if(openmvaddr == 0)
			printf("%d%d\r\n",OPENMVCODE1,flag);   
		if(openmvaddr == 1)
			printf("%d%d\r\n",OPENMVCODE2,flag);
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); //等待发送结束
		delay_ms(100);
		while(!(USART_RX_STA&0X8000) && j<10) //最多等待一秒
		{delay_ms(100);j++;}
		if(USART_RX_STA&0X8000)
		{
			rxlen=USART_RX_STA&0X3FFF;	//得到数据长度
			for(j=0;j<rxlen;j++)
				if(USART_RX_BUF[j]!='[' && USART_RX_BUF[j]!=']')
				{
					buf[k]=USART_RX_BUF[j];
					k++;
				}
			for(j=0;j<k;j++)
				buf[j] -= 48;	
			//判断接受数据是否正确
			if(buf[0] >= 10)
			{
				buf[0] = 0;
				return 0;
			}
			USART_RX_STA = 0;	
			break; //接受完数据，退出此循环
		}
		i++;j=0;
	}
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);	//PA4置1
	if(i>=2)
		return 0;//若十次重发均无接受，贼get数据失败
	if(openmvaddr == 0)
		switch(flag)
	{
		case 0 :LCD_ShowString(10,150,260,24,24,"Room number:");LCD_ShowxNum(160,150,buf[0],1,24,0);break;
		case 1 :LCD_ShowString(10,150,260,24,24,"Product number:");LCD_ShowxNum(160,150,buf[0],1,24,0);break;
		case 2 :LCD_ShowString(10,150,260,24,24,"Face number:");LCD_ShowxNum(160,150,buf[0],1,24,0);break;
		case 3 :break;
	}
	else
		LCD_ShowString(10,140,260,24,24,"Obstacle detection:");
	USART_RX_STA=0;		   	//启动下一次接收
	return 1;
}

void VoiceTask(u16 voice_msg_code,u8 message) //0：房间编号；1：图像编号；2：人脸编号；
{
	switch(voice_msg_code)
	{
		case 0 :XFS_Speech("房间编号为%d",message);break;
		case 1 :XFS_Speech("图像编号为%d",message);break;
		case 2 :XFS_Speech("人脸编号为%d",message);break;
		case 3 :XFS_Speech("前方有障碍物");break;
		case 4 :XFS_Speech("左方有障碍物");break;
		case 5 :XFS_Speech("右方有障碍物");break;
		case 6 :XFS_Speech("下方有障碍物");break;
		case 7 :XFS_Speech("下方有凹陷");break;
		//case 8 :XFS_Speech("无障碍");break;
	}
}
void Gps_Msg_Show(void)
{
 	float tp;		   
	POINT_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//得到经度字符串
 	LCD_ShowString(10,120,200,16,16,dtbuf);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//得到纬度字符串
 	LCD_ShowString(10,140,200,16,16,dtbuf);	 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//得到高度字符串
 	LCD_ShowString(10,160,200,16,16,dtbuf);	 			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//得到速度字符串	 
 	LCD_ShowString(10,180,200,16,16,dtbuf);	 				    
	if(gpsx.fixmode<=3)														//定位状态
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	  LCD_ShowString(10,200,200,16,16,dtbuf);			   
	}	 	    
	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//用于定位的GPS卫星数
 	LCD_ShowString(10,220,200,16,16,dtbuf);	    
	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//可见GPS卫星数
 	LCD_ShowString(10,240,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//可见北斗卫星数
 	LCD_ShowString(10,260,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
	LCD_ShowString(10,280,200,16,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
  LCD_ShowString(10,300,200,16,16,dtbuf);		  
}
void GpsTask(void)
{
	u16 i,rxlen;
	u8 key=0XFF,times = 0;
	if(SkyTra_Cfg_Rate(5)!=0)//设置定位信息更新速度为5Hz,顺便判断GPS模块是否在位. 
	{
		LCD_ShowString(10,220,200,16,16,"SkyTraF8-BD Setting...");
		do
		{
			usart3_init(9600);			//初始化串口3波特率为9600
			SkyTra_Cfg_Prt(3);			//重新设置模块的波特率为38400
			usart3_init(38400);			//初始化串口3波特率为38400
			key=SkyTra_Cfg_Tp(100000);	//脉冲宽度为100ms
			times++;
		}while(SkyTra_Cfg_Rate(5)!=0&&key!=0&&times<10);//配置SkyTraF8-BD的更新速率为5Hz
	}
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{
		rxlen=USART3_RX_STA&0X7FFF;	//得到数据长度
		for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
 		USART3_RX_STA=0;		   	//启动下一次接收
		USART1_TX_BUF[i]=0;			//自动添加结束符
		GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//分析字符串
		Gps_Msg_Show();				//显示信息	
 	}	
}
float fix_g(void)
{
	static double f_x = 0,f_y = 0,f_z = 0;
	static float real_g = 0;
	f_y = wt931_msg[1]*sin(wt931_msg[9]/360.f*2*3.1415);
	f_x = wt931_msg[0]*cos(wt931_msg[8]/360.f*2*3.1415)*cos(wt931_msg[9]/360.f*2*3.1415);
	f_z = wt931_msg[2]*sin(wt931_msg[8]/360.f*2*3.1415);
	real_g = -f_y+f_x+f_z;
	return real_g;
}
void SoundTask(void)
{
	u16 i=0,rxlen,j=0;
	u8 sound_buf[4] = {0},msg_buf[10] = {0};
	wt931_read(); //得到wt931数据，存储于全集变量中
	
	if(wt931_msg[13])
	{
		for(i=0;i<9;i++)
			g[i]=g[i+1];
		g[9] = fix_g(); //更新重力加速度G的数值
		LCD_ShowString(10,190,260,24,24,"WT931 OK");
		LCD_ShowNum(10,220,wt931_msg[0]*10000,10,24);
		LCD_ShowNum(10,250,g[9]*10000,10,24);
	}else{
		LCD_ShowString(10,190,260,24,24,"WT931 NO");
		}
	if(g[9])
	{
		printf("%d\r\n",STM32F1CODE);
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); //等待发送结束
		delay_ms(500);
		while(!(USART_RX_STA&0X8000) && i<20) //等待F1返回数据，最多等待1s
		{LCD_ShowString(200,280,360,24,24,"Running obstacle avoidance algorithm...");i++;delay_ms(100);}
		if(USART_RX_STA&0X8000)
		{
			LCD_ShowString(200,310,360,24,24,"Obstacle avoidance complete");
			rxlen=USART_RX_STA&0X3FFF;	//得到数据长度
			for(i=0;i<rxlen;i++)
				sound_buf[i]=USART_RX_BUF[i];
			USART_RX_STA = 0;				
		}
		else
			LCD_ShowString(200,310,260,24,24,"Obstacle avoidance failed");
	}
	//Get_ImageMsg(0, msg_buf, 1);
	if(!(XFS_isBusy()))
	{
			//if(sound_buf[0] == 0x30 && sound_buf[1] == 0x30 && sound_buf[2] == 0x30 && sound_buf[3] == 0x30) 
			//	VoiceTask(8,0);
			if(sound_buf[0] == 0x31)
				VoiceTask(3,0);
			if(sound_buf[1] == 0x31)
				VoiceTask(4,0);
			if(sound_buf[2] == 0x31)
				VoiceTask(5,0);
			if(sound_buf[3] == 0x31 || msg_buf[0] == 0x31)
				VoiceTask(6,0);
			if(sound_buf[3] == 0x32 || msg_buf[0] == 0x32)
				VoiceTask(7,0);
	}
}
u8 GetVoiceMsg(void)
{
	u8 buf;
	if(UART4_RX_STA&0x8000)
	{
		buf = UART4_RX_BUF[0];
		UART4_RX_STA = 0;
	}
	if(buf == 49) //房间编号
		return 1;
	if(buf == 50) //图像识别
		return 2;
	if(buf == 51) //人脸识别
		return 3;
	if(buf == 52) //卫星定位
		return 4;
	return 0;
}
void ChargeTask(void)
{
	
}

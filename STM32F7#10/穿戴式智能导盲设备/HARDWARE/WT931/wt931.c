#include "wt931.h"
#include "usart.h"

float wt931_msg[14];//0-2:ax ay az 3:t 4-6:wx wy wz 7-9:Roll Pitch Yaw 10-12:hx hy hz

void wt931Init(void)
{
	u8 i;
	for(i=0;i<14;i++)
		wt931_msg[i] = 0;
}

void wt931_read(void)
{
	if(UART5_RX_STA&0x8000)
	{
		
		u16 AxL = UART5_RX_BUF[2];
		u16 AxH = UART5_RX_BUF[3];
		u16 AyL = UART5_RX_BUF[4];
		u16 AyH = UART5_RX_BUF[5];
		u16 AzL = UART5_RX_BUF[6];
		u16 AzH = UART5_RX_BUF[7];
		u16 TL = UART5_RX_BUF[8];
		u16 TH = UART5_RX_BUF[9];
		
		//x方向加速度
		if(AxH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[0]=((AxH<<8)|AxL)/32768.f*16.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Ax = (AxH<<8)|AxL;
			u16 msk = 1;
			for(int i = 0;i < 16;i++)
			{
				Ax = Ax^msk;
				msk <<= 1;
			}
			Ax += 1;
			//Ax = -Ax;
			wt931_msg[0]=Ax/32768.f*16.f;
		}
			
		//y方向加速度
		if(AyH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[1]=((AyH<<8)|AyL)/32768.f*16.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Ay = (AyH<<8)|AyL;
			u16 msk = 1;
			for(int i = 0;i < 16;i++)
			{
				Ay = Ay^msk;
				msk <<= 1;
			}
			Ay += 1;
			//Ay = -Ay;
			wt931_msg[1]=Ay/32768.f*16.f;
		}
		
	//z方向加速度
		if(AzH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[2]=((AzH<<8)|AzL)/32768.f*16.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Az = (AzH<<8)|AzL;
			u16 msk = 1;
			for(int i = 0;i < 16;i++)
			{
				Az = Az^msk;
				msk <<= 1;
			}
			Az += 1;
			//Az = -Az;
			wt931_msg[2]=Az/32768.f*16.f;
		}
		
		//温度值
		wt931_msg[3]=((TH<<8)|TL)/100.f; 
		
		
		u16 WxL = UART5_RX_BUF[13];
		u16 WxH = UART5_RX_BUF[14];
		u16 WyL = UART5_RX_BUF[15];
		u16 WyH = UART5_RX_BUF[16];
		u16 WzL = UART5_RX_BUF[17];
		u16 WzH = UART5_RX_BUF[18];
		
		//x方向角速度
		if(WxH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[4]=((WxH<<8)|WxL)/32768.f*2000.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Wx = (WxH<<8)|WxL;
			Wx -= 1;
			u16 msk = 1;
			for(int i = 0;i < 15;i++)
			{
				Wx = Wx^msk;
				msk <<= 1;
			}
			Wx = -Wx;
			wt931_msg[4]=Wx/32768.f*2000.f;
		}
		
		//y方向角速度
		if(WyH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[5]=((WyH<<8)|WyL)/32768.f*2000.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Wy = (WyH<<8)|WyL;
			Wy -= 1;
			u16 msk = 1;
			for(int i = 0;i < 15;i++)
			{
				Wy = Wy^msk;
				msk <<= 1;
			}
			Wy = -Wy;
			wt931_msg[5]=Wy/32768.f*2000.f;
		}
		
		//z方向角速度
		if(WzH < 128)//最高位不为1,代表其为正数，原码与补码一样
			wt931_msg[6]=((WzH<<8)|WzL)/32768.f*2000.f;
		else//最高位为1，代表其为负数，做补码转原码的转换
		{
			int Wz = (WzH<<8)|WzL;
			Wz -= 1;
			u16 msk = 1;
			for(int i = 0;i < 15;i++)
			{
				Wz = Wz^msk;
				msk <<= 1;
			}
			Wz = -Wz;
			wt931_msg[6]=Wz/32768.f*2000.f;
		}
	
		u16 RollL = UART5_RX_BUF[24];
		u16 RollH = UART5_RX_BUF[25];
		u16 PitchL = UART5_RX_BUF[26];
		u16 PitchH = UART5_RX_BUF[27];
		u16 YawL = UART5_RX_BUF[28];
		u16 YawH = UART5_RX_BUF[29];
		wt931_msg[7]=((RollH<<8)|RollL)/32768.f*180.f;
		wt931_msg[8]=((PitchH<<8)|PitchL)/32768.f*180.f;
		wt931_msg[9]=((YawH<<8)|YawL)/32768.f*180.f;

		
		u16 HxL = UART5_RX_BUF[35];
		u16 HxH = UART5_RX_BUF[36];
		u16 HyL = UART5_RX_BUF[37];
		u16 HyH = UART5_RX_BUF[38];
		u16 HzL = UART5_RX_BUF[39];
		u16 HzH = UART5_RX_BUF[40];
		wt931_msg[10]=((HxH<<8)|HxL);
		wt931_msg[11]=((HyH<<8)|HyL);
		wt931_msg[12]=((HzH<<8)|HzL);

		
		UART5_RX_STA = 0;
		wt931_msg[13] = 1;//数据接受完成
	//memset(UART5_RX_BUF,'\0',10);	
	}
}

#include "task.h"
int main(void)
{
	u8 flag = 0; //功能选择 0无功能,1房间编号识别,2图像识别,3人脸识别,4Gps
	//u8 ApirlTag_msg = 0;
	//u8 position_msg = 0;
	u8 image_msg = 0;
	u8 databuf[10] = {0};
	InitTask();
	while(1)
	{
		flag = GetVoiceMsg(); //获取语音指令，用于功能选择 0为AprilTag识别，1为图像识别，2为人脸识别，3为惯性导航
		SoundTask(); //超声波避障任务
		if(!(XFS_isBusy())) //若正在语音合成，则不执行此任务
			switch(flag)
			{
				case 0:break;
				case 1:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //AprilTag图像识别，img_msg存储房间编号，databuf存储距离角度信息
				case 2:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //图像识别
				case 3:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //人脸识别
				case 4:GpsTask();break; //惯性导航
			}
		ChargeTask(); //电源管理任务
		//ApirlTag_msg = 0;
		//position_msg = 0;
		image_msg = 0;
		databuf[0] = 0;
		wt931_msg[13] = 0;
	}
}

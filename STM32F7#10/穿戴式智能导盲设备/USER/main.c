#include "task.h"
int main(void)
{
	u8 flag = 0; //����ѡ�� 0�޹���,1������ʶ��,2ͼ��ʶ��,3����ʶ��,4Gps
	//u8 ApirlTag_msg = 0;
	//u8 position_msg = 0;
	u8 image_msg = 0;
	u8 databuf[10] = {0};
	InitTask();
	while(1)
	{
		flag = GetVoiceMsg(); //��ȡ����ָ����ڹ���ѡ�� 0ΪAprilTagʶ��1Ϊͼ��ʶ��2Ϊ����ʶ��3Ϊ���Ե���
		SoundTask(); //��������������
		if(!(XFS_isBusy())) //�����������ϳɣ���ִ�д�����
			switch(flag)
			{
				case 0:break;
				case 1:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //AprilTagͼ��ʶ��img_msg�洢�����ţ�databuf�洢����Ƕ���Ϣ
				case 2:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //ͼ��ʶ��
				case 3:Get_ImageMsg(flag-1, databuf, 0);image_msg = databuf[0];VoiceTask(flag-1,image_msg);flag = 0;break; //����ʶ��
				case 4:GpsTask();break; //���Ե���
			}
		ChargeTask(); //��Դ��������
		//ApirlTag_msg = 0;
		//position_msg = 0;
		image_msg = 0;
		databuf[0] = 0;
		wt931_msg[13] = 0;
	}
}

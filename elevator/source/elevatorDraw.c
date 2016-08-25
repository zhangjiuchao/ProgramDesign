#include"test.h"
#include "egg.h" 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500

HEGG heggs[30];
HEGG doorEgg[9];

void DrawFace()
{
	HEGG hegg;
	int	i;
	//贴背景图 
	EggStart(WINDOW_WIDTH,WINDOW_HEIGHT);
	DrawBitmap("face_01.bmp");
	DrawBitmap("face_02.bmp");
	DrawBitmap("face_03.bmp");
	
	//贴，内部请求的指示灯 
	heggs[1]=LayEgg();
	MovePen(560,330);
	DrawBitmap("in_1.bmp");
	ShowEgg(0);
	heggs[2]=LayEgg();
	MovePen(630,330);
	DrawBitmap("in_2.bmp");
	ShowEgg(0);
	heggs[3]=LayEgg();
	MovePen(700,330);
	DrawBitmap("in_3.bmp");
	ShowEgg(0);
	heggs[4]=LayEgg();
	MovePen(560,260);
	DrawBitmap("in_4.bmp");
	ShowEgg(0);
	heggs[5]=LayEgg();
	MovePen(630,260);
	DrawBitmap("in_5.bmp");
	ShowEgg(0);
	heggs[6]=LayEgg();
	MovePen(700,260);
	DrawBitmap("in_6.bmp");
	ShowEgg(0);
	heggs[7]=LayEgg();
	MovePen(560,190);
	DrawBitmap("in_7.bmp");
	ShowEgg(0);
	heggs[8]=LayEgg();
	MovePen(630,190);
	DrawBitmap("in_8.bmp");
	ShowEgg(0);
	heggs[9]=LayEgg();
	MovePen(700,190);
	DrawBitmap("in_9.bmp");
	ShowEgg(0);
	heggs[0]=LayEgg();
	MovePen(40,75);
	DrawBitmap("elevator.bmp");

	//贴外部向上呼叫的指示灯 
	heggs[11]=LayEgg();
	MovePen(130,65);
	DrawBitmap("up_arrow_1.bmp");
	ShowEgg(0);
	for(i=13;i<=18;i++)
	{
		heggs[i]=LayEgg();
		MovePen(110,65+(i-11)*45);
		DrawBitmap("up_arrow.bmp");
		ShowEgg(0);
	}
	heggs[12]=LayEgg();
	MovePen(110,110);
	DrawBitmap("up_arrow_2.bmp");
	ShowEgg(0);
	heggs[22]=LayEgg();
	MovePen(150,110);
	
	//贴外部向下的指示灯 
	DrawBitmap("down_arrow_2.bmp");
	ShowEgg(0);	
	heggs[29]=LayEgg();
	MovePen(130,425);
	DrawBitmap("down_arrow_9.bmp");
	ShowEgg(0);
	for(i=23;i<=28;i++)
	{
		heggs[i]=LayEgg();
		MovePen(150,65+(i-21)*45);
		DrawBitmap("down_arrow.bmp");
		ShowEgg(0);
	}
	
	//贴策略的指示灯 
	heggs[10]=LayEgg();
	MovePen(570,450);
	DrawBitmap("byway_light.bmp");
	
	heggs[19]=LayEgg();
	MovePen(680,450);
	DrawBitmap("bytime_light.bmp");
	ShowEgg(0);
	
	heggs[20]=LayEgg();
	MovePen(570,105);
	DrawBitmap("open_light.bmp");
	ShowEgg(0);
	
	heggs[21]=LayEgg();
	MovePen(675,105);
	DrawBitmap("close_light.bmp");
	ShowEgg(0);
	
    //贴电梯门的图片 
	doorEgg[1]=LayEgg();
	MovePen(250,395);	
	DrawBitmap("left_1.bmp");
	doorEgg[2]=LayEgg();
	MovePen(280,395);
	DrawBitmap("left_2.bmp");
	doorEgg[3]=LayEgg();
	MovePen(310,395);
	DrawBitmap("left_3.bmp");
	doorEgg[4]=LayEgg();
	MovePen(340,395);
	DrawBitmap("left_4.bmp");
	doorEgg[5]=LayEgg();
	MovePen(370,395); 
	DrawBitmap("right_4.bmp");
	doorEgg[6]=LayEgg();
	MovePen(400,395);
	DrawBitmap("right_3.bmp");
	doorEgg[7]=LayEgg();
	MovePen(430,395);
	DrawBitmap("right_2.bmp");
	doorEgg[8]=LayEgg();
	MovePen(460,395);
	DrawBitmap("right_1.bmp");
}


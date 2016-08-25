#include"test.h" 

int InsideCmd[10];
int NoResponseFloor_up[10];
int NoResponseFloor_down[10];
double duration;
FILE *cfPtr;
struct elevator Elevator;
/////
//打印开始界面与使用说明
void Hello_Face(void) 
{
	printf("Welcome to use LZL group's similate elevator ~^0^~\n\n");
	printf("Our elevator ues the service of by the way, hope you have a time.\n\n");
	printf("|--------------------------------------------------------------------------|\n");
	printf("| Followings are the instructions：                                        |\n"); 
	printf("|                                                                          |\n");
	printf("| Numbers 1~9 represent the interior request floor 1~9.                    |\n");
	printf("| Q、W、E、R、T、Y、U、I represent the exterior Upward request floor 1~8.  |\n");
	printf("| A、S、D、F、G、H、J、K represent the exterior Downward request floor 2~9.|\n");
	printf("| P.S. You can ues the lowercases replace the majuscules. ^-^              |\n"); 
	printf("|                                                                          |\n");
	printf("| Enter Z represents Open-door, enter X represents Close-door.             |\n");
	printf("| Please enter the key Enter after input.                                  |\n");
	printf("|                                                                          |\n");
	printf("| Status display example:                                                  |\n");
	printf("| 10.0s Stop 2.0 6 5.0s <1> [3 7] <8>                                      |\n"); 
	printf("|                                                                          |\n");
	printf("| 10.0s:run time          Stop:current stat                                |\n");
	printf("| 2.0:current floor       6:goal floor            5.0s:stop time           |\n");
	printf("| <1>:no response upward request                                           |\n");
	printf("| [3 7]:no response downward request                                       |\n");
	printf("| <8>:no response interior request                                         |\n");
	printf("|--------------------------------------------------------------------------|\n\n"); 
	
		
	if((cfPtr=fopen("clients.txt","a"))==NULL)//打开文件失败 
		printf("File could not be opened!\n");
	else
	{
		fprintf(cfPtr,"Welcome to use LZL group's similate elevator ~^0^~\n\n");
		fprintf(cfPtr,"Our elevator ues the service of by the way, hope you have a time.\n\n");
		fprintf(cfPtr,"|--------------------------------------------------------------------------|\n");
		fprintf(cfPtr,"| Followings are the instructions：                                        |\n"); 
		fprintf(cfPtr,"|                                                                          |\n");
		fprintf(cfPtr,"| Numbers 1~9 represent the interior request floor 1~9.                    |\n");
		fprintf(cfPtr,"| Q、W、E、R、T、Y、U、I represent the exterior Upward request floor 1~8.  |\n");
		fprintf(cfPtr,"| A、S、D、F、G、H、J、K represent the exterior Downward request floor 2~9.|\n");
		fprintf(cfPtr,"| P.S. You can ues the lowercases replace the majuscules. ^-^              |\n"); 
		fprintf(cfPtr,"|                                                                          |\n");
		fprintf(cfPtr,"| Enter Z represents Open-door, enter X represents Close-door.             |\n");
		fprintf(cfPtr,"| Please enter the key Enter after input.                                  |\n");
		fprintf(cfPtr,"|                                                                          |\n");
		fprintf(cfPtr,"| Status display example:                                                  |\n");
		fprintf(cfPtr,"| 10.0s Stop 2.0 6 5.0s <1> [3 7] <8>                                      |\n"); 
		fprintf(cfPtr,"|                                                                          |\n");
		fprintf(cfPtr,"| 10.0s:run time          Stop:current stat                                |\n");
		fprintf(cfPtr,"| 2.0:current floor       6:goal floor            5.0s:stop time           |\n");
		fprintf(cfPtr,"| <1>:no response upward request                                           |\n");
		fprintf(cfPtr,"| [3 7]:no response downward request                                       |\n");
		fprintf(cfPtr,"| <8>:no response interior request                                         |\n");
		fprintf(cfPtr,"|--------------------------------------------------------------------------|\n\n"); 
	
		fclose(cfPtr);//关闭文件 
	}
}


void outputArray(int Array[])
{
	int n = 1, num = 0, x;
	
	while(n <= 9){
		if(Array[n] == 1)//判断该楼层是否有呼叫 
			num = num + 1;
		
		n = n + 1;	
	}
	
	n = 1;
	x = num;//存放需要呼叫的个数 
	num = 1; 
	
	//打印呼叫楼层 
	while(n <= 9){ 
		if(Array[n] == 1 && num == x)
			printf("%d", n);				
		else if(Array[n] == 1 && num != x){
			printf("%d ", n);
			num = num + 1;	
		}			
		n = n + 1;			 
	}			
}

//将呼叫楼层写入文件 
void foutputArray(int Array[])
{
	int n = 1, num = 0, x;
	
	while(n <= 9){
		if(Array[n] == 1)//判断该楼层是否有呼叫
			num = num + 1;
		
		n = n + 1;	
	}
	
	n = 1;
	x = num;//存放需要呼叫的个数
	num = 1; 

	//打印呼叫楼层 
	while(n <= 9){
		if(Array[n] == 1 && num == x)
		{
			fprintf(cfPtr,"%d", n);
		}					
		else if(Array[n] == 1 && num != x){
			fprintf(cfPtr,"%d ", n);
			num = num + 1;	
		}			
		n = n + 1;			 
	}			
}

//打印电梯状态并写入文件 
void outPrint()
{
	double now_time;

	now_time=(double)(clock()-start_time)/CLOCKS_PER_SEC;//计算当前时间 
	
	if((cfPtr=fopen("clients.txt","a"))==NULL)//打开文件失败 
		printf("File could not be opened！！\n");
	else
	{
		//将电梯状态写入文件 
		fprintf(cfPtr,"%.1fs\t",now_time);//打印运行时间 
		switch(Elevator.state)//根据运行状态打印结果 
		{
			case 1:
				fprintf(cfPtr,"Up↑↑\t");
				break;
			case -1:
				fprintf(cfPtr,"Down↓↓\t");
				break;
			case 2:
				fprintf(cfPtr,"Stop--\t");
				break;
			case 0:
				fprintf(cfPtr,"Free～\t");
				break;
		}
		
		fprintf(cfPtr,"%.1f\t",Elevator.floor);//打印当前楼层 
		
		if(Elevator.state!=0) 
		{
			fprintf(cfPtr,"%d  ",GoalFloor);//电梯非空闲打印目标楼层 
			
			if(Elevator.state==2)
				fprintf(cfPtr,"0  ");
			else
				fprintf(cfPtr,"   ");
			fprintf(cfPtr,"<");
 			foutputArray(NoResponseFloor_up);//打印未响应向上呼叫 
 			fprintf(cfPtr,">\t\t[");
			foutputArray(NoResponseFloor_down);//打印未响应向下呼叫
			fprintf(cfPtr,"]\t\t(");
			foutputArray(InsideCmd);//打印未响应外部呼叫
			fprintf(cfPtr,")\n"); 
		}
		else
			fprintf(cfPtr,"\n");
		
		fclose(cfPtr);//关闭文件	
	}
	
	
	//打印电梯状态 
 	printf("%.1fs\t",now_time);//打印运行时间	
	switch(Elevator.state){//根据运行状态打印结果 
		case 1:
			printf("Up↑↑\t");
			break;
		case -1:
			printf("Down↓↓\t");
			break;
		case 2:
			printf("Stop--\t");
			break;
		case 0:
			printf("Free～\t");
			break;	
	}
	
	printf("%.1f  ",Elevator.floor);//打印当前楼层 
	
	if(Elevator.state!=0)
	{
		printf("%d  ",GoalFloor);//电梯非空闲打印目标楼层
	
		if(Elevator.state==2)
			printf("0  ");
		else
			printf("   ");
			
	printf("<");
 	outputArray(NoResponseFloor_up);//打印未响应向上呼叫	
 	printf(">\t\t[");
	outputArray(NoResponseFloor_down);//打印未响应向下呼叫
	printf("]\t\t(");
	outputArray(InsideCmd);//打印未响应外部呼叫
	printf(")\n");	 
	}
	else
		printf("\n");
}

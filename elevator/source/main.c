#include"test.h"

DWORD WINAPI RUN(LPVOID lpParameter);//声明电梯运行的线程 

DWORD WINAPI inputOder(LPVOID lpParameter);//声明接收用户输入指令的线程 

clock_t start_time;//程序开始运行的时间 

int InsideCmd[10]={0};//定义存放内部请求的数组 
int NoResponseFloor_up[10]={0};//定义存放外部向上呼叫的数组 
int NoResponseFloor_down[10]={0};//定义存放外部向下呼叫的数组 
int openDoor=0;//开门的指令 ，当它为1时，表示有开门的指令 
int closeDoor=0;//关门的指令，当它为1时，表示有关门的指令 
int strategy=1;
struct elevator Elevator;//定义电梯的结构 
FILE *cfPtr;//定义存放运行结果的文件 

main()
{	
	Hello_Face();//打印运行界面及使用说明 
	DrawFace();	
	
	start_time=clock();//计算当前时间 
	//初始化电梯的状态 
	Elevator.state=0;
	Elevator.floor=1;
	
//	outPrint();//打印电梯的开始状态 
	printf("\n");
	
	HANDLE hThread1,hThread2;
	
	hThread1=CreateThread(NULL,0,RUN,NULL,0,NULL);//创建电梯运行的线程 
	hThread2=CreateThread(NULL,0,inputOder,NULL,0,NULL);//创建接收用户输入指令的线程 
	
	CloseHandle(hThread1);
	CloseHandle(hThread2);

	Sleep(10000000);
	return 0;
}

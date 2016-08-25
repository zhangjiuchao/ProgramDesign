#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"egg.h"

extern HEGG heggs[30];
extern HEGG doorEgg[9];

extern int InsideCmd[10];//定义存放内部请求的数组
extern int NoResponseFloor_up[10];//定义存放外部向上呼叫的数组
extern int NoResponseFloor_down[10];//定义存放外部向下呼叫的数组 
extern int GoalFloor;//
extern FILE *cfPtr;
//定义电梯的结构 
struct elevator{
	int state;
	double floor;
};


struct listNode
{
	int require;
	struct listNode * nextPtr;
};

typedef struct listNode LISTNODE;
typedef LISTNODE * LISTNODEPTR;

LISTNODEPTR headPtr; 
struct elevator Elevator;

extern int openDoor;//开门的指令 ，当它为1时，表示有开门的指令
extern int closeDoor;//关门的指令，当它为1时，表示有关门的指令 
extern clock_t start_time;//程序开始运行的时间 
extern int strategy;
DWORD WINAPI RUN(LPVOID lpParameter);//声明电梯运行的线程

DWORD WINAPI inputOder(LPVOID lpParameter);//声明接收用户输入指令的线程


#include"test.h"
#include"time.h"

double duration=0;
int InsideCmd[10];
int NoResponseFloor_up[10];
int NoResponseFloor_down[10];
int GoalFloor;
int change;

HEGG heggs[30];
HEGG doorEgg[9];

LISTNODEPTR headPtr=NULL;

int strategy;
int closeFloor_up(int Array[],int Floor)//检测上行中最近的目标楼层的函数 
{
	int i;
	i=Floor+1;	
	while(i<=9)
	{
		if(Array[i]==1)
			return i;
		i++;
	}
	if(i==10)
	return 0;
}
int closeFloor_down(int Array1[],int Floor)//检测下行中的最近的目标楼层的函数 
{
	int i;
	i=Floor-1;
	while(i>=1)
	{
		if(Array1[i]==1)
	 	return i;
		i--;
	}
	if(i==0) 
	return 0;
}
int farmostFloor_down(int insideCmd[],int NoResponseFloor_down[],int Floor)//检测最远的向下呼叫的函数 
{
	int i=9,j=9;
	while(i>Floor&& NoResponseFloor_down[i]!=1)
	{
		i--;
	}
	while(j>Floor&&insideCmd[j]!=1)
	{
		j--;
	}
	if(i>j)  
		return i;
	else if(j>i) 
		return j;
	else if(i==j&&i!=Floor) 
		return i;
	else 
		return 0;
}
int farmostFloor_up(int insideCmd[],int NoResponseFloor_up[],int Floor)//检测最远的向上呼叫 
{
	int i=1,j=1;
	while(i<Floor&& NoResponseFloor_up[i]!=1)
	{
		i++;  
	}
	while(j<Floor&&insideCmd[j]!=1)
	{
		j++;
	}
	if(i<j)  
		return i;
	else if(j<i) 
		return j;
	else if(i==j&&i!=Floor) 
		return i;
	else 
		return 0;
}
void addition(int floor)
{
	LISTNODEPTR endPtr;	
	LISTNODEPTR movePtr;
	
	movePtr=headPtr;
	while(headPtr!=NULL&&movePtr->nextPtr!=NULL)
		movePtr=movePtr->nextPtr;	
	endPtr=(LISTNODEPTR)malloc(sizeof(LISTNODEPTR));
	
	if(endPtr!=NULL)
	{
		endPtr->require=floor;
		endPtr->nextPtr=NULL;
		if(headPtr==NULL)
			headPtr=endPtr;
		else
			movePtr->nextPtr=endPtr;
	}
//	printList(headPtr);
}


 void cancelRequire(int floor)
{
	LISTNODEPTR movePtr;                                                           
	LISTNODEPTR lastPtr; 
	movePtr=headPtr;
	lastPtr=headPtr;
	while(movePtr!=NULL)
	{
		if(movePtr->require==floor)
		{
			if(movePtr==headPtr)
			{
				headPtr=headPtr->nextPtr;
				movePtr=headPtr;
				lastPtr=headPtr;
			}	
			else
			{
				lastPtr->nextPtr=movePtr->nextPtr;
				movePtr=movePtr->nextPtr;
			}			
		}
		else
		{
			lastPtr=movePtr;
			movePtr=movePtr->nextPtr;
		}
	}
}

void cancelGoal(int floor,int state)
{
	LISTNODEPTR movePtr;                                                           
	LISTNODEPTR lastPtr; 
	movePtr=headPtr;
	lastPtr=headPtr;
	while(movePtr!=NULL)
	{
		if(state==1)
		{
			if(movePtr->require==floor||(movePtr->require-10)==floor)
			{
				if(movePtr==headPtr)
				{
					headPtr=headPtr->nextPtr;
					movePtr=NULL;
					lastPtr=NULL;
				}	
				else
				{
					lastPtr->nextPtr=movePtr->nextPtr;
					movePtr=movePtr->nextPtr;
				}		
			}
			else
			{
				lastPtr=movePtr;
				movePtr=movePtr->nextPtr;
			}
			
		}
		if(state==-1)
		{
			if(movePtr->require==floor||(movePtr->require-20)==floor)
			{
				if(movePtr==headPtr)
				{
					headPtr=headPtr->nextPtr;
					movePtr=NULL;
					lastPtr=NULL;
				}
				else
				{
					lastPtr->nextPtr=movePtr->nextPtr;	
					movePtr=movePtr->nextPtr;
				}	
			}
			else
			{
				lastPtr=movePtr;
				movePtr=movePtr->nextPtr;
			}
		
		}
	}
	
}
int checkGoal_up(int insideCmd[],int NoResponseFloor_up [] ,int NoResponseFloor_down[], int Floor,LISTNODEPTR startPtr)
{
	int closeFloor1,closeFloor2,fastistFloor,i;
	
	if(strategy==1)
	{
		closeFloor1=closeFloor_up(insideCmd,Floor);
		closeFloor2=closeFloor_up(NoResponseFloor_up,Floor);
 		if(closeFloor1!=0||closeFloor2!=0)
 		{
		 	if(closeFloor1!=0&&closeFloor2==0)
	 			return closeFloor1;
 			else if(closeFloor1==0&&closeFloor2!=0)
 				return closeFloor2;
	 		else
			 	return (closeFloor1>=closeFloor2)?closeFloor2:closeFloor1;
		 }
	 		
 		else
 		{
			fastistFloor=farmostFloor_down(insideCmd,NoResponseFloor_down,Floor);
			if(fastistFloor!=0)
				return fastistFloor;
			else
			{
				closeFloor1=closeFloor_down(insideCmd,Floor);
				closeFloor2=closeFloor_down(NoResponseFloor_down,Floor);
				if(closeFloor1!=0||closeFloor2!=0) 
		 			return (closeFloor1>=closeFloor2)?closeFloor1:closeFloor2;
				else
				{
					for(i=1;i<=Floor;i++)
					{
						if(NoResponseFloor_up[i]==1)
							return i;
					}
					return 0;
				}
			}
 		} 		
	}
	else
	{
		if(startPtr==NULL)
			return 0;	
		else
			return (startPtr->require)%10;
	 		
	}
}
int checkGoal_down(int insideCmd[],int NoResponseFloor_down[],int NoResponseFloor_up[],int Floor,LISTNODEPTR startPtr)
{ 
	int closeFloor1,closeFloor2,fastistFloor,i;

	if(strategy==1)
	{
		closeFloor1=closeFloor_down(insideCmd,Floor);
		closeFloor2=closeFloor_down(NoResponseFloor_down,Floor);
		if(closeFloor1!=0||closeFloor2!=0)
 		{
	 		if(closeFloor1!=0&&closeFloor2==0)
	 			return closeFloor1;
 			else if(closeFloor1==0&&closeFloor2!=0)
 				return closeFloor2;
	 		else
		 		return (closeFloor1>=closeFloor2)?closeFloor1:closeFloor2;
		 }
 		else
 		{
			fastistFloor=farmostFloor_up(insideCmd,NoResponseFloor_up,Floor);
			if(fastistFloor!=0)
				return fastistFloor;
			else
			{
				closeFloor1=closeFloor_up(insideCmd,Floor);
				closeFloor2=closeFloor_up(NoResponseFloor_up,Floor);
				if(closeFloor1!=0||closeFloor2!=0) 
		 			return (closeFloor1>=closeFloor2)?closeFloor2:closeFloor1;
				else
				{
					for(i=9;i<=Floor;i--)
					{
						if(NoResponseFloor_up[i]==1)
							return i;
					}
					return 0;
				}
			}
	 	}
	}
	else
	{
		if(startPtr==NULL)
			return 0;
		else
	 		return (startPtr->require)%10;
	}
}


//控制电梯向上运行的函数 ，运行一层的时间为5秒 
void up_run()
{
	int count=1;
	while(count<=5)
	{
		SetActiveEgg(heggs[0]);
		OffsetEgg(0,9);
		count++;
		
		Sleep(1000);
	}
	Elevator.floor+=1;
}
//控制电梯向下运行的函数 ，运行一层的时间为5秒 
void down_run()
{
	
	int count=1;
	while(count<=5)
	{
		SetActiveEgg(heggs[0]);
		OffsetEgg(0,-9);
		count++;
		
		Sleep(1000);
	}
	Elevator.floor-=1;
}
//控制电梯停靠的函数 
void elevatorstop()
{
	clock_t start,finish;
	double duration,time=5.0;
	start = clock();
	duration=0.0;
	//循环5秒 
	while(duration<time)
	{ 
     	finish = clock();
    	duration = (double)(finish - start) / CLOCKS_PER_SEC;
   		
		if(openDoor==1)//当遇到开门指令时 
		{
			duration=0;
			start=clock();
			SetActiveEgg(doorEgg[4]);
			ShowEgg(0);
			SetActiveEgg(doorEgg[5]);
			ShowEgg(0);
			Sleep(330);
			SetActiveEgg(doorEgg[3]);
			ShowEgg(0);		
			SetActiveEgg(doorEgg[6]);
			ShowEgg(0);
			Sleep(330);
			SetActiveEgg(doorEgg[2]);
			ShowEgg(0);
			SetActiveEgg(doorEgg[7]);
			ShowEgg(0);
			Sleep(330);
			SetActiveEgg(doorEgg[1]);
			ShowEgg(0);
			SetActiveEgg(doorEgg[8]);
			ShowEgg(0);
			openDoor=0;
			SetActiveEgg(heggs[20]);
			ShowEgg(0);
			printf("The door is openning. ← →\n");
		}
		if(duration>=4.0)
   		 	closeDoor=1;
 		if(closeDoor==1)//当遇到关门指令时 
		{	
			printf("The door is closing.  → ←\n");
			closeDoor=0;
			time=1.0;
			duration=0.0; 
			start=clock();
			SetActiveEgg(doorEgg[1]);
			ShowEgg(1);
			SetActiveEgg(doorEgg[8]);
			ShowEgg(1);
			Sleep(330);
			SetActiveEgg(doorEgg[2]);
			ShowEgg(1);	
			SetActiveEgg(doorEgg[7]);
			ShowEgg(1);
			Sleep(330);
			SetActiveEgg(doorEgg[3]);
			ShowEgg(1);
			SetActiveEgg(doorEgg[6]);
			ShowEgg(1);
			Sleep(330);
			SetActiveEgg(doorEgg[4]);
			ShowEgg(1);
			SetActiveEgg(doorEgg[5]);
			ShowEgg(1);
			SetActiveEgg(heggs[21]);
			ShowEgg(0);
			duration=5.0;
		}
	}	
}

	/*控制电梯运行的线程
	电梯有上行，下行，停靠，空闲四个状态，检测电梯上一时刻的状态来判断电梯的目标楼层，
	决定电梯的下一时刻的状态，电梯的运行方式为：每运行一层检测一次电梯的目标楼层 
	*/ 
DWORD WINAPI RUN(LPVOID lpParameter)
{		
	int previous_state;
	previous_state=Elevator.state;
	
	while(1)
	{
		
		switch(previous_state)
		{
			case 1://上一时刻电梯是向上运行时 
			
				GoalFloor=checkGoal_up(InsideCmd,NoResponseFloor_up,NoResponseFloor_down,Elevator.floor,headPtr);

				if(GoalFloor==0)//没有呼叫时，电梯转为空闲状态
				{
					Elevator.state=0;
					previous_state=0;
					outPrint();
				}
				else
				{
					if(GoalFloor>Elevator.floor)//目标楼层高于当前楼层。电梯继续上行 
					{
						outPrint(); 
						up_run();
						
						if(GoalFloor==Elevator.floor)//到达目标楼层时，电梯状态变为停靠 
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							NoResponseFloor_down[GoalFloor]=0;
							cancelGoal(GoalFloor,1);
							cancelGoal(GoalFloor,-1);
							outPrint();
							openDoor=1;
							closeDoor=0;
							elevatorstop();	
						}
						
					}	
					else//目标楼层低于当前楼层，电梯转为下行 
					{
						Elevator.state=-1;
						previous_state=-1;
						outPrint();
						down_run();
						if(GoalFloor==Elevator.floor)//到达目标楼层时，电梯状态变为停靠 
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}	
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_down[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							cancelGoal(GoalFloor,-1);
							cancelGoal(GoalFloor,1);
							outPrint();
							openDoor=1;
							closeDoor=0;
							elevatorstop();	
							
						}
					}
				}break;
			case -1://上一时刻电梯是向下运行时
				
				GoalFloor=checkGoal_down(InsideCmd,NoResponseFloor_down,NoResponseFloor_up,Elevator.floor,headPtr);

				if(GoalFloor==0)//没有呼叫时，电梯转为空闲状态 
				{
					Elevator.state=0;
					previous_state=0;
					outPrint();
				}
				else
				{
					if(GoalFloor<Elevator.floor)//目标楼层低于当前楼层。电梯继续下行 
					{
					
						outPrint();
						down_run();
						if(GoalFloor==Elevator.floor)//到达目标楼层时，电梯状态变为停靠
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_down[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							cancelGoal(GoalFloor,-1);
							cancelGoal(GoalFloor,1);
							outPrint();
							openDoor=1;
							SetActiveEgg(heggs[20]);
							ShowEgg(1);
							closeDoor=0;
							SetActiveEgg(heggs[21]);
							ShowEgg(0);
							elevatorstop();	
						}
					}
					else//目标楼层高于当前楼层，电梯转为上行
					{
						Elevator.state=1;
						
						previous_state=1;
						outPrint();
						up_run();
					
						if(GoalFloor==Elevator.floor)	//到达目标楼层时，电梯状态变为停靠
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							NoResponseFloor_down[GoalFloor]=0;
							cancelGoal(GoalFloor,1);
							cancelGoal(GoalFloor,-1);
							outPrint();
							openDoor=1;
							SetActiveEgg(heggs[20]);
							ShowEgg(1);
							closeDoor=0;
							SetActiveEgg(heggs[21]);
							ShowEgg(0);
							elevatorstop();	
						}
					}
				}break;
			case 0://上一时刻电梯处于空闲状态时
			 
				GoalFloor=checkGoal_up(InsideCmd,NoResponseFloor_up,NoResponseFloor_down,Elevator.floor,headPtr); 
	
				if(GoalFloor!=0)//有呼叫请求时 
				{
					if(GoalFloor>Elevator.floor)//目标楼层高于当前楼层，电梯转为上行
					{
						Elevator.state=1;
						previous_state=1;
						outPrint();
						up_run();
						
						if(GoalFloor==Elevator.floor)//到达目标楼层时，电梯状态变为停靠
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							NoResponseFloor_down[GoalFloor]=0;
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							cancelGoal(GoalFloor,1);
							cancelGoal(GoalFloor,-1);
							outPrint();
							openDoor=1;
							SetActiveEgg(heggs[20]);
							ShowEgg(1);
							closeDoor=0;
							SetActiveEgg(heggs[21]);
							ShowEgg(0);
							elevatorstop();	
						}
					}
					else
					{
						if(GoalFloor==Elevator.floor)
						{
							Elevator.state=2;
							SetActiveEgg(heggs[GoalFloor]);
							ShowEgg(0);
							if(GoalFloor!=9)
							{
								SetActiveEgg(heggs[GoalFloor+10]);
								ShowEgg(0);
							}
							SetActiveEgg(heggs[GoalFloor+20]);
							ShowEgg(0);
							InsideCmd[GoalFloor]=0;
							NoResponseFloor_up[GoalFloor]=0;
							NoResponseFloor_down[GoalFloor]=0;
							cancelGoal(GoalFloor,1);
							cancelGoal(GoalFloor,-1);
							outPrint();
							openDoor=1;
							SetActiveEgg(heggs[20]);
							ShowEgg(1);
							closeDoor=0;
							SetActiveEgg(heggs[21]);
							ShowEgg(0);
							elevatorstop();	
						}
						else//目标楼层低于当前楼层，电梯转为下行
						{
							Elevator.state=-1;
							previous_state=-1;
							outPrint();
							down_run();
						
							if(GoalFloor==Elevator.floor)//到达目标楼层时，电梯状态变为停靠
							{
								Elevator.state=2;
								SetActiveEgg(heggs[GoalFloor]);
								ShowEgg(0);
								SetActiveEgg(heggs[GoalFloor+20]);
								ShowEgg(0);
								if(GoalFloor!=9)
								{
									SetActiveEgg(heggs[GoalFloor+10]);
									ShowEgg(0);
								}
								InsideCmd[GoalFloor]=0;
								NoResponseFloor_down[GoalFloor]=0;
								NoResponseFloor_up[GoalFloor]=0;
								cancelGoal(GoalFloor,-1);
								cancelGoal(GoalFloor,1);
								outPrint();
								openDoor=1;
								SetActiveEgg(heggs[20]);
								ShowEgg(1);
								closeDoor=0;
								SetActiveEgg(heggs[21]);
								ShowEgg(0);
								elevatorstop();	
							}
						}
					}
				}
				break;
		}	
					
	}
	return 0;
}

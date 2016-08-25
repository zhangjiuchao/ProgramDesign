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
int closeFloor_up(int Array[],int Floor)//��������������Ŀ��¥��ĺ��� 
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
int closeFloor_down(int Array1[],int Floor)//��������е������Ŀ��¥��ĺ��� 
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
int farmostFloor_down(int insideCmd[],int NoResponseFloor_down[],int Floor)//�����Զ�����º��еĺ��� 
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
int farmostFloor_up(int insideCmd[],int NoResponseFloor_up[],int Floor)//�����Զ�����Ϻ��� 
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


//���Ƶ����������еĺ��� ������һ���ʱ��Ϊ5�� 
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
//���Ƶ����������еĺ��� ������һ���ʱ��Ϊ5�� 
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
//���Ƶ���ͣ���ĺ��� 
void elevatorstop()
{
	clock_t start,finish;
	double duration,time=5.0;
	start = clock();
	duration=0.0;
	//ѭ��5�� 
	while(duration<time)
	{ 
     	finish = clock();
    	duration = (double)(finish - start) / CLOCKS_PER_SEC;
   		
		if(openDoor==1)//����������ָ��ʱ 
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
			printf("The door is openning. �� ��\n");
		}
		if(duration>=4.0)
   		 	closeDoor=1;
 		if(closeDoor==1)//����������ָ��ʱ 
		{	
			printf("The door is closing.  �� ��\n");
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

	/*���Ƶ������е��߳�
	���������У����У�ͣ���������ĸ�״̬����������һʱ�̵�״̬���жϵ��ݵ�Ŀ��¥�㣬
	�������ݵ���һʱ�̵�״̬�����ݵ����з�ʽΪ��ÿ����һ����һ�ε��ݵ�Ŀ��¥�� 
	*/ 
DWORD WINAPI RUN(LPVOID lpParameter)
{		
	int previous_state;
	previous_state=Elevator.state;
	
	while(1)
	{
		
		switch(previous_state)
		{
			case 1://��һʱ�̵�������������ʱ 
			
				GoalFloor=checkGoal_up(InsideCmd,NoResponseFloor_up,NoResponseFloor_down,Elevator.floor,headPtr);

				if(GoalFloor==0)//û�к���ʱ������תΪ����״̬
				{
					Elevator.state=0;
					previous_state=0;
					outPrint();
				}
				else
				{
					if(GoalFloor>Elevator.floor)//Ŀ��¥����ڵ�ǰ¥�㡣���ݼ������� 
					{
						outPrint(); 
						up_run();
						
						if(GoalFloor==Elevator.floor)//����Ŀ��¥��ʱ������״̬��Ϊͣ�� 
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
					else//Ŀ��¥����ڵ�ǰ¥�㣬����תΪ���� 
					{
						Elevator.state=-1;
						previous_state=-1;
						outPrint();
						down_run();
						if(GoalFloor==Elevator.floor)//����Ŀ��¥��ʱ������״̬��Ϊͣ�� 
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
			case -1://��һʱ�̵�������������ʱ
				
				GoalFloor=checkGoal_down(InsideCmd,NoResponseFloor_down,NoResponseFloor_up,Elevator.floor,headPtr);

				if(GoalFloor==0)//û�к���ʱ������תΪ����״̬ 
				{
					Elevator.state=0;
					previous_state=0;
					outPrint();
				}
				else
				{
					if(GoalFloor<Elevator.floor)//Ŀ��¥����ڵ�ǰ¥�㡣���ݼ������� 
					{
					
						outPrint();
						down_run();
						if(GoalFloor==Elevator.floor)//����Ŀ��¥��ʱ������״̬��Ϊͣ��
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
					else//Ŀ��¥����ڵ�ǰ¥�㣬����תΪ����
					{
						Elevator.state=1;
						
						previous_state=1;
						outPrint();
						up_run();
					
						if(GoalFloor==Elevator.floor)	//����Ŀ��¥��ʱ������״̬��Ϊͣ��
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
			case 0://��һʱ�̵��ݴ��ڿ���״̬ʱ
			 
				GoalFloor=checkGoal_up(InsideCmd,NoResponseFloor_up,NoResponseFloor_down,Elevator.floor,headPtr); 
	
				if(GoalFloor!=0)//�к�������ʱ 
				{
					if(GoalFloor>Elevator.floor)//Ŀ��¥����ڵ�ǰ¥�㣬����תΪ����
					{
						Elevator.state=1;
						previous_state=1;
						outPrint();
						up_run();
						
						if(GoalFloor==Elevator.floor)//����Ŀ��¥��ʱ������״̬��Ϊͣ��
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
						else//Ŀ��¥����ڵ�ǰ¥�㣬����תΪ����
						{
							Elevator.state=-1;
							previous_state=-1;
							outPrint();
							down_run();
						
							if(GoalFloor==Elevator.floor)//����Ŀ��¥��ʱ������״̬��Ϊͣ��
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

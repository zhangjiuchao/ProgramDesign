#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"egg.h"

extern HEGG heggs[30];
extern HEGG doorEgg[9];

extern int InsideCmd[10];//�������ڲ����������
extern int NoResponseFloor_up[10];//�������ⲿ���Ϻ��е�����
extern int NoResponseFloor_down[10];//�������ⲿ���º��е����� 
extern int GoalFloor;//
extern FILE *cfPtr;
//������ݵĽṹ 
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

extern int openDoor;//���ŵ�ָ�� ������Ϊ1ʱ����ʾ�п��ŵ�ָ��
extern int closeDoor;//���ŵ�ָ�����Ϊ1ʱ����ʾ�й��ŵ�ָ�� 
extern clock_t start_time;//����ʼ���е�ʱ�� 
extern int strategy;
DWORD WINAPI RUN(LPVOID lpParameter);//�����������е��߳�

DWORD WINAPI inputOder(LPVOID lpParameter);//���������û�����ָ����߳�


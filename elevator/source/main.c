#include"test.h"

DWORD WINAPI RUN(LPVOID lpParameter);//�����������е��߳� 

DWORD WINAPI inputOder(LPVOID lpParameter);//���������û�����ָ����߳� 

clock_t start_time;//����ʼ���е�ʱ�� 

int InsideCmd[10]={0};//�������ڲ���������� 
int NoResponseFloor_up[10]={0};//�������ⲿ���Ϻ��е����� 
int NoResponseFloor_down[10]={0};//�������ⲿ���º��е����� 
int openDoor=0;//���ŵ�ָ�� ������Ϊ1ʱ����ʾ�п��ŵ�ָ�� 
int closeDoor=0;//���ŵ�ָ�����Ϊ1ʱ����ʾ�й��ŵ�ָ�� 
int strategy=1;
struct elevator Elevator;//������ݵĽṹ 
FILE *cfPtr;//���������н�����ļ� 

main()
{	
	Hello_Face();//��ӡ���н��漰ʹ��˵�� 
	DrawFace();	
	
	start_time=clock();//���㵱ǰʱ�� 
	//��ʼ�����ݵ�״̬ 
	Elevator.state=0;
	Elevator.floor=1;
	
//	outPrint();//��ӡ���ݵĿ�ʼ״̬ 
	printf("\n");
	
	HANDLE hThread1,hThread2;
	
	hThread1=CreateThread(NULL,0,RUN,NULL,0,NULL);//�����������е��߳� 
	hThread2=CreateThread(NULL,0,inputOder,NULL,0,NULL);//���������û�����ָ����߳� 
	
	CloseHandle(hThread1);
	CloseHandle(hThread2);

	Sleep(10000000);
	return 0;
}

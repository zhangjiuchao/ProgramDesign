#include"test.h" 

int InsideCmd[10];
int NoResponseFloor_up[10];
int NoResponseFloor_down[10];
double duration;
FILE *cfPtr;
struct elevator Elevator;
/////
//��ӡ��ʼ������ʹ��˵��
void Hello_Face(void) 
{
	printf("Welcome to use LZL group's similate elevator ~^0^~\n\n");
	printf("Our elevator ues the service of by the way, hope you have a time.\n\n");
	printf("|--------------------------------------------------------------------------|\n");
	printf("| Followings are the instructions��                                        |\n"); 
	printf("|                                                                          |\n");
	printf("| Numbers 1~9 represent the interior request floor 1~9.                    |\n");
	printf("| Q��W��E��R��T��Y��U��I represent the exterior Upward request floor 1~8.  |\n");
	printf("| A��S��D��F��G��H��J��K represent the exterior Downward request floor 2~9.|\n");
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
	
		
	if((cfPtr=fopen("clients.txt","a"))==NULL)//���ļ�ʧ�� 
		printf("File could not be opened!\n");
	else
	{
		fprintf(cfPtr,"Welcome to use LZL group's similate elevator ~^0^~\n\n");
		fprintf(cfPtr,"Our elevator ues the service of by the way, hope you have a time.\n\n");
		fprintf(cfPtr,"|--------------------------------------------------------------------------|\n");
		fprintf(cfPtr,"| Followings are the instructions��                                        |\n"); 
		fprintf(cfPtr,"|                                                                          |\n");
		fprintf(cfPtr,"| Numbers 1~9 represent the interior request floor 1~9.                    |\n");
		fprintf(cfPtr,"| Q��W��E��R��T��Y��U��I represent the exterior Upward request floor 1~8.  |\n");
		fprintf(cfPtr,"| A��S��D��F��G��H��J��K represent the exterior Downward request floor 2~9.|\n");
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
	
		fclose(cfPtr);//�ر��ļ� 
	}
}


void outputArray(int Array[])
{
	int n = 1, num = 0, x;
	
	while(n <= 9){
		if(Array[n] == 1)//�жϸ�¥���Ƿ��к��� 
			num = num + 1;
		
		n = n + 1;	
	}
	
	n = 1;
	x = num;//�����Ҫ���еĸ��� 
	num = 1; 
	
	//��ӡ����¥�� 
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

//������¥��д���ļ� 
void foutputArray(int Array[])
{
	int n = 1, num = 0, x;
	
	while(n <= 9){
		if(Array[n] == 1)//�жϸ�¥���Ƿ��к���
			num = num + 1;
		
		n = n + 1;	
	}
	
	n = 1;
	x = num;//�����Ҫ���еĸ���
	num = 1; 

	//��ӡ����¥�� 
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

//��ӡ����״̬��д���ļ� 
void outPrint()
{
	double now_time;

	now_time=(double)(clock()-start_time)/CLOCKS_PER_SEC;//���㵱ǰʱ�� 
	
	if((cfPtr=fopen("clients.txt","a"))==NULL)//���ļ�ʧ�� 
		printf("File could not be opened����\n");
	else
	{
		//������״̬д���ļ� 
		fprintf(cfPtr,"%.1fs\t",now_time);//��ӡ����ʱ�� 
		switch(Elevator.state)//��������״̬��ӡ��� 
		{
			case 1:
				fprintf(cfPtr,"Up����\t");
				break;
			case -1:
				fprintf(cfPtr,"Down����\t");
				break;
			case 2:
				fprintf(cfPtr,"Stop--\t");
				break;
			case 0:
				fprintf(cfPtr,"Free��\t");
				break;
		}
		
		fprintf(cfPtr,"%.1f\t",Elevator.floor);//��ӡ��ǰ¥�� 
		
		if(Elevator.state!=0) 
		{
			fprintf(cfPtr,"%d  ",GoalFloor);//���ݷǿ��д�ӡĿ��¥�� 
			
			if(Elevator.state==2)
				fprintf(cfPtr,"0  ");
			else
				fprintf(cfPtr,"   ");
			fprintf(cfPtr,"<");
 			foutputArray(NoResponseFloor_up);//��ӡδ��Ӧ���Ϻ��� 
 			fprintf(cfPtr,">\t\t[");
			foutputArray(NoResponseFloor_down);//��ӡδ��Ӧ���º���
			fprintf(cfPtr,"]\t\t(");
			foutputArray(InsideCmd);//��ӡδ��Ӧ�ⲿ����
			fprintf(cfPtr,")\n"); 
		}
		else
			fprintf(cfPtr,"\n");
		
		fclose(cfPtr);//�ر��ļ�	
	}
	
	
	//��ӡ����״̬ 
 	printf("%.1fs\t",now_time);//��ӡ����ʱ��	
	switch(Elevator.state){//��������״̬��ӡ��� 
		case 1:
			printf("Up����\t");
			break;
		case -1:
			printf("Down����\t");
			break;
		case 2:
			printf("Stop--\t");
			break;
		case 0:
			printf("Free��\t");
			break;	
	}
	
	printf("%.1f  ",Elevator.floor);//��ӡ��ǰ¥�� 
	
	if(Elevator.state!=0)
	{
		printf("%d  ",GoalFloor);//���ݷǿ��д�ӡĿ��¥��
	
		if(Elevator.state==2)
			printf("0  ");
		else
			printf("   ");
			
	printf("<");
 	outputArray(NoResponseFloor_up);//��ӡδ��Ӧ���Ϻ���	
 	printf(">\t\t[");
	outputArray(NoResponseFloor_down);//��ӡδ��Ӧ���º���
	printf("]\t\t(");
	outputArray(InsideCmd);//��ӡδ��Ӧ�ⲿ����
	printf(")\n");	 
	}
	else
		printf("\n");
}

#include<stdio.h>
#include "read_txt.h"
#include<time.h> 
#include<pthread.h>
#include "mystruct.h"
#include "selecting.h"
#include<string.h>
int main(int argc,char * argv[])
{
	
	int i,j,ret;
	int num;
	int limit;
	data_1 d;
	
	pthread_t id;
	char order_date[15];
	char ship_date[10];
	char mktsegment[20];
	
	pthread_t rcus_id,rord_id,ritem_id; 
	int start_time,end_time;
	txt  r_txt;  //�ṹ��txt���зֱ�ָ�� customer��orders��lineitem�ṹ�����͵�ָ�룬�ֱ�����ָ��ͻ���������Ͳ�Ʒ�� 
	printf("�������̸߳�����");
	scanf ("%d",&(d.num_of_thread)); 
	start_time  = clock();
	
	//���̶߳�ȡ�ļ� 
	pthread_create(&rcus_id,NULL,read_customer_txt,&r_txt);  //������ȡcustomer.txt���̣߳�����ͻ��� 
	pthread_create(&rord_id,NULL,read_orders_txt,&r_txt);    //������ȡorders.txt���̣߳�����ͻ��� 
	pthread_create(&ritem_id,NULL,read_lineitem_txt,&r_txt); //������ȡlineitem.txt���̣߳�����ͻ��� 
	pthread_join(rcus_id,NULL);
	pthread_join(rord_id,NULL);                              //pthread_join���������� ������ǰ����
	pthread_join(ritem_id,NULL);                             //������ý��̻�û������ִ����һ�����³������ 
	
	
	d.r_txt = &r_txt;
	
	num = change_argv_to_number(argv[4]);//�ܹ�����Ĵ���
	
	for(i=0;num>0;num--,i=i+4)
	{
		
		strcpy(d.sc1,argv[5+i]);
		strcpy(d.sc2,argv[6+i]);
		d.sc2[10]=0;
		strcpy(d.sc3,argv[7+i]);
		limit = change_argv_to_number(argv[8+i]);
		ret = pthread_create(&id,NULL,select_final1,&d);
		ret = pthread_join(id,NULL);
		//printf("ret = %d,main\n");
		printf("l_orderkey|o_orderdate|revenue\n");
		for(j=0;j<limit;j++)
		{
			if(d.result[j].l_extendedprice==0)
				printf("null      |null       |null   \n");
			else
				printf("%-10d|%-11s|%-20.2lf\n",d.result[j].l_orderkey,d.result[j].o_orderdate,d.result[j].l_extendedprice);
		}
		
		free(d.result);
		d.result = NULL;
		//printf("print ok ,%d\n",num);
	}
	//printf("this!ok\n"); 
	end_time  = clock();
	printf("\n\n��ѯ�����ˣ�%dms\n", end_time-start_time);
	
	return 0;	
}

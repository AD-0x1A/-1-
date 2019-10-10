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
	txt  r_txt;  //结构体txt内有分别指向 customer，orders和lineitem结构体类型的指针，分别用于指向客户表、订单表和产品表 
	printf("请输入线程个数：");
	scanf ("%d",&(d.num_of_thread)); 
	start_time  = clock();
	
	//多线程读取文件 
	pthread_create(&rcus_id,NULL,read_customer_txt,&r_txt);  //创建读取customer.txt的线程，导入客户表 
	pthread_create(&rord_id,NULL,read_orders_txt,&r_txt);    //创建读取orders.txt的线程，导入客户表 
	pthread_create(&ritem_id,NULL,read_lineitem_txt,&r_txt); //创建读取lineitem.txt的线程，导入客户表 
	pthread_join(rcus_id,NULL);
	pthread_join(rord_id,NULL);                              //pthread_join的作用在于 阻塞当前进程
	pthread_join(ritem_id,NULL);                             //避免调用进程还没结束就执行下一步导致程序出错 
	
	
	d.r_txt = &r_txt;
	
	num = change_argv_to_number(argv[4]);//总共计算的次数
	
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
	printf("\n\n查询花费了：%dms\n", end_time-start_time);
	
	return 0;	
}

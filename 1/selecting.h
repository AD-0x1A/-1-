#ifndef SELECTING_H
#define SELECTING_H
#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include"mystruct.h"
#include<pthread.h>
#include<semaphore.h>
void * select_customers(void * scus)          //根据customer的筛选条件 ，对客户表先进行筛选 
{
	s_cus * a = (s_cus *)scus;   //多线程传参的类型为void * 类型，所以需要进行强制类型转换 
	customer * old_cus = a->old_data->cus;     //获取从txt中read的未进行筛选的客户表 
	customer * new_cus = (customer *)malloc(100*sizeof(customer));//动态申请新客户表，存放筛选出来的数据 
	char select_condition[20];
	strcpy(select_condition,a->selection_condition); //获取筛选条件 
	int i,j=0;
	for(i=0;i<100;i++) //进行筛选 
	{
		new_cus[i].c_custkey = -1;//初始化 
		if(strcmp(select_condition,old_cus[i].c_mkgsegment)==0)
		{
			new_cus[j].c_custkey = old_cus[i].c_custkey;
			//printf("%d\n",new_cus[j].c_custkey);
			strcpy(new_cus[j].c_mkgsegment,old_cus[i].c_mkgsegment);
			j++;
		}
	}
	a->new_cus = new_cus; //放回新客户表 
	//printf("this selecet-cus!ok\n");
	//printf("%d\n",a->new_cus);
	return NULL;
}


void * select_orders(void * sord)            //根据orders定货时间的筛选条件 ，对订单表先进行筛选
{
	s_ord * a = (s_ord *)sord;  //强制类型转换 
	orders * old_ord = a->old_data->ord;  //获取从txt中read的未进行筛选的订单表
	orders * new_ord = (orders *)malloc(4000*sizeof(orders));//动态申请新订单表，存放筛选出来的数据
	char select_condition[10];
	strcpy(select_condition,a->selection_condition);//获取筛选条件 
	int i,j=0;
	//printf("%s\n",select_condition);
	for(i=0;i<4000;i++) //进行筛选 
	{
		new_ord[i].o_orderkey=-1;
		if(strcmp(old_ord[i].o_orderdate,select_condition)<0)
		{
			new_ord[j].o_orderkey = old_ord[i].o_orderkey;
			new_ord[j].o_custkey = old_ord[i].o_custkey;
			strcpy(new_ord[j].o_orderdate,old_ord[i].o_orderdate);
			j++;
		}
	}
	a->new_ord = new_ord;//返回新订单表
	a->num = j;//返回表长 
	//printf("this selecet-ord!ok\n");
	return NULL;
}

void * select_lineitems(void * sitem)        //根据lineitem发货时间的筛选条件 ，对产品表先进行筛选
{
	s_item * a = (s_item *)sitem;  //强制类型转换
	lineitem * old_item = a->old_data->item;//获取从txt中read的未进行筛选的产品表
	lineitem * new_item = (lineitem *)malloc(1000*sizeof(lineitem));//动态申请新产品表，存放筛选出来的数据
	char select_condition[10];
	strcpy(select_condition,a->selection_condition);//获取筛选条件
	int i,j=0;
	//printf("%s\n",a->selection_condition);
	for(i=0;i<1000;i++)//进行筛选 
	{
		new_item[i].l_orderkey=-1;
		if(strcmp(old_item[i].l_shipdate,select_condition)>0)
		{
			new_item[j].l_orderkey = old_item[i].l_orderkey;
			new_item[j].l_extendedprice = old_item[i].l_extendedprice;
			strcpy(new_item[j].l_shipdate,old_item[i].l_shipdate);
			j++;
		}
	}
	//printf("j = %d",j);
	a->new_item = new_item;//放回新订单表 
	//printf("this - selecet item!ok\n");
	return NULL;
}

void * sum_operation(void * arg)
{
	//printf("123456\n");
	data_2 * d2 = (data_2 *)arg;
	select_result * r = d2->result2;
	int i,m;
	for(m=0;r[m].l_extendedprice!=0;m++);
	if(m!=0)m--;
	//printf("%d\n",m);
	//printf("123456\n");
	for(i=0;i<(d2->l);i++)
	{
		//printf("%d\n",i);
		if(i==0&&r[m].l_extendedprice==0)
		{
			r[m].l_orderkey = d2->result1[i].l_orderkey;
			strcpy(r[m].o_orderdate,d2->result1[i].o_orderdate);
			r[m].l_extendedprice = d2->result1[i].l_extendedprice;
			continue;
		}
		if(d2->result1[i].l_orderkey==d2->result1[i-1].l_orderkey)
		{
			r[m].l_extendedprice = r[m].l_extendedprice + d2->result1[i].l_extendedprice;
			
		}
		else
		{
			
			m=m+1;
			r[m].l_orderkey = d2->result1[i].l_orderkey;
			strcpy(r[m].o_orderdate,d2->result1[i].o_orderdate);
			r[m].l_extendedprice = d2->result1[i].l_extendedprice;
			
		}
	}
	//for(m=0;d2->result2[m].l_extendedprice!=0;m++)
	//printf("%lf\n",d2->result2[m].l_extendedprice);
	//printf("add ok!\n");
	return NULL;
}

void * connect_operation(void * arg)//连接操作 
{
	//printf("77777\n\n\n");
	
	data_2 * d2 = (data_2 *)arg;
	orders * new_ord = d2->new_ord;
	customer * new_cus = d2->new_cus;
	lineitem * new_item =d2->new_item;
	int start = d2->start;
	int end = d2->end;
	select_result * result1 = NULL; 
	result1 = (select_result *)malloc(1001*sizeof(select_result));
	//printf("start = %d,end = %d\n",start,end);
	int i,j,k,l=0,m=0;
	for(i=0;new_cus[i].c_custkey!=-1&&i<100;i++)
	{
		//printf("%d\n",new_cus[i].c_custkey);
		for(j=start;new_ord[j].o_orderkey!=-1&&j<end;j++)
		{
			
			
			for(k=0;new_item[k].l_orderkey!=-1&&k<1000;k++)
			{   
				//if(new_item[k+1].l_orderkey==-1&&i == 0)
			
				//printf("%d\n%d",new_item[k].l_orderkey,j);
			
				if(new_cus[i].c_custkey==new_ord[j].o_custkey&&new_ord[j].o_orderkey==new_item[k].l_orderkey)
				{
					//printf("%d,%d,%lf\n",new_cus[i].c_custkey,new_ord[j].o_orderkey,new_item[k].l_extendedprice);
					result1[l].l_orderkey=new_item[k].l_orderkey;
					strcpy(result1[l].o_orderdate,new_ord[j].o_orderdate);
					result1[l].l_extendedprice=new_item[k].l_extendedprice;
					//printf("%d,%s,%lf\n",result1[l].l_orderkey,result1[l].o_orderdate,result1[l].l_extendedprice);
					l++;//printf("l=%d\n",l);
				}
			}
		}	
	}
	//printf("connect ok!\n");
	int ret;
	pthread_t id;
	if(l!=0)
	{	
		sem_wait(d2->sem_addr);
		d2->l = l;
		d2->result1 = result1;
		pthread_create(&id,NULL,sum_operation,d2);
		ret=pthread_join(id,NULL);
		//printf("ret=%d,add\n");
		sem_post(d2->sem_addr);
	}
	free(result1);
	return NULL;
}



void * select_final1(void * data1)
{
	void * rr;
	int ret;                       //pthread_create的返回值 
	data_1 * d1 = (data_1 *)data1; //强制类型转换 
	s_cus s_new_cus;			   //创建线程 select_customers传参的结构体参数 
	s_ord s_new_ord;			   //创建线程 select_orders传参的结构体参数
	s_item s_new_item;			   //创建线程 select_lineitems传参的结构体参数
	//对结构体参数进行初始化赋值 
	s_new_cus.old_data = d1->r_txt;
	s_new_ord.old_data = d1->r_txt;
	s_new_item.old_data = d1->r_txt;
	strcpy(s_new_cus.selection_condition,d1->sc1);
	strcpy(s_new_ord.selection_condition,d1->sc2);
	strcpy(s_new_item.selection_condition,d1->sc3);
	//实现筛选三个表的多线程，提高筛选速度 
	   //创建3个线程 
	pthread_t s_new_cus_id;
	pthread_t s_new_ord_id;
	pthread_t s_new_item_id;
	ret = pthread_create(&s_new_cus_id,NULL,select_customers,&s_new_cus);	   
	ret = pthread_create(&s_new_ord_id,NULL,select_orders,&s_new_ord);
	ret = pthread_create(&s_new_item_id,NULL,select_lineitems,&s_new_item);
	   //将当前线程阻塞，防止筛选未结束执行下一步操作 
	ret = pthread_join(s_new_cus_id,&rr);
	//printf("ret = %d,筛选客户表\n",ret); 
	ret = pthread_join(s_new_ord_id,&rr);
	//printf("ret = %d,筛选订单表\n",ret);
	ret = pthread_join(s_new_item_id,&rr);
	//printf("ret = %d,筛选产品表\n",ret);

	select_result * result2=NULL;
	int pthread_num = d1->num_of_thread,pp;
	pthread_t  ids[200]={0};
	pthread_t  id;
	sem_t sem;
	result2 = (select_result *)malloc(1001*sizeof(select_result));
	for(ret = 0;ret<1001;ret++)
	{
		result2[ret].l_extendedprice = 0;
	}
	//printf("result2=%d\n",result2);
	data_2 *data2=NULL;
	int ord_len = s_new_ord.num;
	int boundary = ord_len/pthread_num;
	int start,end;
	//printf("77777");
	sem_init(&sem,0,1);
	//printf("pthread_num = %d,ord_len = %d,boundary = %d\n",pthread_num,ord_len,boundary);
	for(pp=0;pp<pthread_num;pp++)
	{
		//printf("id = %d",ids[pp]);
		start = pp*boundary;
		if((start+boundary)>ord_len)
			end = ord_len+1;
		else 
			end = start+boundary;
		//printf("\n\n\n\nstart=%d,end=%d\n",start,end);
		data2 = (data_2 *)malloc(1*sizeof(data_2));
		//printf("data2=%d\n",data2);
		
		data2->result2 = result2;
		data2->new_cus = s_new_cus.new_cus;
		data2->new_item = s_new_item.new_item;
		data2->new_ord  = s_new_ord.new_ord;
		data2->sem_addr = &sem;
		data2->start = start;
		data2->end  = end;
		pthread_create(&ids[pp],NULL,connect_operation,data2);
		data2 = NULL;
		
	}
	
	for(pp=0;pp<pthread_num;pp++)
	{
		
		ret=pthread_join(ids[pp],NULL);
		//printf("ret=%d,join=%d\n",ret,ids[pp]);
	}
	
	free(s_new_cus.new_cus);
	free(s_new_item.new_item);
	free(s_new_ord.new_ord);

	int i,j,m;
	select_result temp;
	for(m=0;result2[m].l_extendedprice!=0;m++)
	//printf("m=%d\n",m);
	for(i=0;i<m-1;i++)//冒泡排序 ，将选择结果排为降序 
	{
		for(j=0;j<m-1-i;j++)
		{
			//printf("%lf->%lf\n",result2[j].l_extendedprice,result2[j+1].l_extendedprice);
			if(result2[j].l_extendedprice<result2[j+1].l_extendedprice)
			{
				//printf("123");
				temp.l_extendedprice=result2[j].l_extendedprice;
				temp.l_orderkey=result2[j].l_orderkey;
				strcpy(temp.o_orderdate,result2[j].o_orderdate);
				result2[j].l_extendedprice=result2[j+1].l_extendedprice;
				result2[j].l_orderkey=result2[j+1].l_orderkey;
				strcpy(result2[j].o_orderdate,result2[j+1].o_orderdate);
				result2[j+1].l_extendedprice=temp.l_extendedprice;
				result2[j+1].l_orderkey=temp.l_orderkey;
				strcpy(result2[j+1].o_orderdate,temp.o_orderdate);
			}
		}
	}
	//printf("123\n");
	//free(result1);
	//printf("select all ok!\n");
	d1->result = result2;
	return NULL;
}

int change_argv_to_number(char s[])//将命令行里读入的数字字符串转化为整形数字 
{
	int i=0;
	int number=0;
	while(s[i]!='\0')
	{
		if(i==0)
			number = (s[i]-48);
		else
			number = number*10 + (s[i]-48);
		//printf("%d,%d\n",i,number);
		i++;
	}
	return number;
}


#endif 

#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include<semaphore.h>
typedef struct
{
	int c_custkey;    	   //顾客编号
	char c_mkgsegment[20]; //对应的某个市场部门
}customer;				   //顾客结构体 

typedef struct
{
	int o_orderkey;    	 //订单号 
	int o_custkey;    	 //顾客编号
	char o_orderdate[10];//订货日期 
}orders;				 //订单

typedef struct
{
	int l_orderkey;//订单号
	double l_extendedprice;//额外价格
	char l_shipdate[10];//发货日期 
}lineitem; //商品信息 

typedef struct
{
	int l_orderkey;//订单号
	char o_orderdate[10];//订货日期 
	double l_extendedprice;//额外价格
}select_result;

typedef struct
{
	customer * cus;  //指向客户表
	orders * ord;    //指向订单表
	lineitem * item;//指向产品表
}txt;    //结构体txt内有分别指向 customer，orders和lineitem结构体类型的指针，分别用于指向客户表、订单表和产品表

typedef struct
{
	txt * old_data;   //指向旧的客户表 
	char selection_condition[20];//市场部门，用于选择 
	customer * new_cus;   //指向新客户表 
}s_cus;   //结构体s_cus内有用于选择判断条件的市场部门，并且将选择出的新客户表放置于new_cus ，为方便多线程传参而设置 

typedef struct
{
	txt * old_data;  //指向旧的订单表 
	char selection_condition[10];//订单日期，用于选择 
	orders * new_ord;  //指向新的客户表 
	int num;//选择出来的个数 
}s_ord;  //结构体s_ord内有用于选择判断条件的订单日期，并且将选择出的新的订单表放置于new_ord，为方便多线程传参而设置 

typedef struct
{
	txt * old_data;  //指向旧的产品表 
	char selection_condition[10];//发货日期，用于选择 
	lineitem * new_item;  //指向新的产品表 
}s_item;  //结构体s_item内有用于选择判断条件的发货日期，并且将选择出的新的订单表放置于new_item，为方便多线程传参而设置 

typedef struct
{
	txt * r_txt;      //指向从txt中读取的客户表、订单表和产品表 
	char sc1[20];     //筛选客户表的选择条件 
	char sc2[11];     //筛选订单表的选择条件 
	char sc3[10];     //筛选产品表的选择条件 
	int num_of_thread;  //线程处理的个数 
	select_result * result; //储存选择结果返回 
}data_1;

typedef struct
{
	sem_t * sem_addr; //信号量，因为要多线程要共享资源result2，所以为避免并发访问共享资源需要信号控制 
	//txt * selected_txt; //指向已进行筛选过的客户表、订单表和产品表 
	customer * new_cus;
	orders * new_ord;
	lineitem * new_item;
	int start; //表的开始位置 
	int end;   //表的结束位置
	int l;    //连接结果的个数，即为result1的个数
	select_result * result2;//指向求和后的结果 
	select_result * result1;
}data_2;
#endif

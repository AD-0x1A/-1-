#ifndef MYSTRUCT_H
#define MYSTRUCT_H
#include<semaphore.h>
typedef struct
{
	int c_custkey;    	   //�˿ͱ��
	char c_mkgsegment[20]; //��Ӧ��ĳ���г�����
}customer;				   //�˿ͽṹ�� 

typedef struct
{
	int o_orderkey;    	 //������ 
	int o_custkey;    	 //�˿ͱ��
	char o_orderdate[10];//�������� 
}orders;				 //����

typedef struct
{
	int l_orderkey;//������
	double l_extendedprice;//����۸�
	char l_shipdate[10];//�������� 
}lineitem; //��Ʒ��Ϣ 

typedef struct
{
	int l_orderkey;//������
	char o_orderdate[10];//�������� 
	double l_extendedprice;//����۸�
}select_result;

typedef struct
{
	customer * cus;  //ָ��ͻ���
	orders * ord;    //ָ�򶩵���
	lineitem * item;//ָ���Ʒ��
}txt;    //�ṹ��txt���зֱ�ָ�� customer��orders��lineitem�ṹ�����͵�ָ�룬�ֱ�����ָ��ͻ���������Ͳ�Ʒ��

typedef struct
{
	txt * old_data;   //ָ��ɵĿͻ��� 
	char selection_condition[20];//�г����ţ�����ѡ�� 
	customer * new_cus;   //ָ���¿ͻ��� 
}s_cus;   //�ṹ��s_cus��������ѡ���ж��������г����ţ����ҽ�ѡ������¿ͻ��������new_cus ��Ϊ������̴߳��ζ����� 

typedef struct
{
	txt * old_data;  //ָ��ɵĶ����� 
	char selection_condition[10];//�������ڣ�����ѡ�� 
	orders * new_ord;  //ָ���µĿͻ��� 
	int num;//ѡ������ĸ��� 
}s_ord;  //�ṹ��s_ord��������ѡ���ж������Ķ������ڣ����ҽ�ѡ������µĶ����������new_ord��Ϊ������̴߳��ζ����� 

typedef struct
{
	txt * old_data;  //ָ��ɵĲ�Ʒ�� 
	char selection_condition[10];//�������ڣ�����ѡ�� 
	lineitem * new_item;  //ָ���µĲ�Ʒ�� 
}s_item;  //�ṹ��s_item��������ѡ���ж������ķ������ڣ����ҽ�ѡ������µĶ����������new_item��Ϊ������̴߳��ζ����� 

typedef struct
{
	txt * r_txt;      //ָ���txt�ж�ȡ�Ŀͻ���������Ͳ�Ʒ�� 
	char sc1[20];     //ɸѡ�ͻ����ѡ������ 
	char sc2[11];     //ɸѡ�������ѡ������ 
	char sc3[10];     //ɸѡ��Ʒ���ѡ������ 
	int num_of_thread;  //�̴߳���ĸ��� 
	select_result * result; //����ѡ�������� 
}data_1;

typedef struct
{
	sem_t * sem_addr; //�ź�������ΪҪ���߳�Ҫ������Դresult2������Ϊ���Ⲣ�����ʹ�����Դ��Ҫ�źſ��� 
	//txt * selected_txt; //ָ���ѽ���ɸѡ���Ŀͻ���������Ͳ�Ʒ�� 
	customer * new_cus;
	orders * new_ord;
	lineitem * new_item;
	int start; //��Ŀ�ʼλ�� 
	int end;   //��Ľ���λ��
	int l;    //���ӽ���ĸ�������Ϊresult1�ĸ���
	select_result * result2;//ָ����ͺ�Ľ�� 
	select_result * result1;
}data_2;
#endif

#ifndef READ_TXT_H
#define READ_TXT_H
#include<stdio.h>
#include<stdlib.h>
#include "mystruct.h"
void * read_customer_txt(void * r_txt) //读取customer。txt内容 
{
	FILE * fp;
	customer *a=NULL;
	txt * p = (txt *)r_txt;    //应为传过来的参数为void*类型，所以需要强制类型转换 
	a = (customer *)malloc(101*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("customer.txt","r"); 
	if(NULL==fp)
	{
		printf("cannot open customer.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		//printf("%d  ",i);
		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);
		//printf("%d%c%s\n",a[i].c_custkey,b,a[i].c_mkgsegment);
		i++;
	}
	fclose(fp);
	//return a;
	p->cus = a;   //返回 
	return NULL;
}
void * read_orders_txt(void * r_txt)//读取orders.txt内容 
{
	int i =0; 
	orders * a=NULL;
	txt * p = (txt *)r_txt;    //应为传过来的参数为void*类型，所以需要强制类型转换
	a = (orders * )malloc(4000*sizeof(orders));
	char b,c;
	long d;
	FILE *fp;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("cannot open orders.txt!");
		return NULL;
	}
	while(!feof(fp))
	{	
		//printf("%d!",i);
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;
		//printf("%d	%c	%ld	%c	%s\n",a[i].o_orderkey,b,a[i].o_custkey,c,a[i].o_orderdate);
		i++;
	}
	fclose(fp);
	//return a;
	p->ord = a;   //返回
	return NULL;
}

void * read_lineitem_txt(void * r_txt)//读取lineitem.txt内容
{
	FILE * fp;
	lineitem * l=NULL;
	txt * p = (txt *)r_txt;    //应为传过来的参数为void*类型，所以需要强制类型转换
	l = (lineitem *)malloc(1001*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("cannot open lineitem.txt!");
		return NULL;
	}
	while(!feof(fp))
	{
		//printf("%d!",i);
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);
		//printf("%d,%lf,%s\n",l[i].l_orderkey,l[i].l_extendedprice,l[i].l_shipdate);
		i++;
	}
	fclose(fp);
	p->item = l; //返回
	return NULL;
}
#endif

#ifndef SQLEXECUTE_H
#define SQLEXECUTE_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>
#include <Windows.h>
#include <iostream>
using namespace std;

class SqlExecute
{
private:
	string user_name;
	string passwd;
	string host;
	string database;
	unsigned int port;
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
public:
	SqlExecute(string user_name,string passwd,string host,string database,unsigned int port)
	{
		this->user_name=user_name;
		this->passwd=passwd;
		this->host=host;
		this->database=database;
		this->port=port;
	}
	bool openConnect()
	{
		mysql_init(&myCont);
		return mysql_real_connect(&myCont,host.c_str(),user_name.c_str(),passwd.c_str(),database.c_str(),port,NULL,0);
	}
	vector<string>* exec(string sql)
	{
		vector<string>* values=new vector<string>();
		char column[32][32];
		mysql_query(&myCont, "SET NAMES GBK");
		int res=mysql_query(&myCont,sql.c_str());
		if(!res)
		{
			result=mysql_store_result(&myCont);//保存查询到的数据到result
			if(result)
			{
				int i,j;
				cout<<"number of result: "<<(unsigned long)mysql_num_rows(result)<<endl;
				for(i=0;fd=mysql_fetch_field(result);i++)//获取列名
				{
					strcpy(column[i],fd->name);
				}
				j=mysql_num_fields(result);
				for(i=0;i<j;i++)
				{
					printf("%s\t",column[i]);
				}
				printf("\n");
				while(sql_row=mysql_fetch_row(result))//获取具体的数据
				{
					for(i=0;i<j;i++)
					{
						values->push_back(sql_row[i]);
						printf("%s\t",sql_row[i]);
					}
					printf("\n");
				}
			}
		}
		else
		{
			cout<<"query sql failed!"<<endl;
		}
		return values;
	}
	void closedConnect()
	{
		if(result!=NULL) mysql_free_result(result);//释放结果资源
		mysql_close(&myCont);//断开连接
	}
};
#endif
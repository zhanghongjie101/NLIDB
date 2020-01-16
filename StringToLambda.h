#ifndef STRINGTOLAMBDA_H
#define STRINGTOLAMBDA_H
#include <string>
#include <stack>
#include <map>
#include <vector>
#include "LambdaExpression.h"
using namespace std;

class StringToLambda
{
private:
	map<string,bool> domain;
	vector<string>* split(string str)
	{
		vector<string>* tmpV=new vector<string>();
		stack<char> tmpS;
		string tmpStr="";
		for(int i=0;i<str.length();++i)
		{
			if(str[i]=='(')
			{
				tmpS.push(str[i]);
				tmpStr+=str[i];
			}
			else if(str[i]==')')
			{
				tmpS.pop();
				tmpStr+=str[i];
				if(tmpS.empty())
				{
					tmpV->push_back(tmpStr);
					tmpStr="";
					++i;
				}
			}
			else tmpStr+=str[i];
		}
		return tmpV;
	}
public:
	string test(string str)
	{
		str=str.substr(str.find(' ',0)+1,str.length()-(str.find(' ',0)+1));
		vector<string>* v=split(str);
		return "";
	}
	void initDomain()
	{
		domain.insert(map<string,bool>::value_type("AND",true));
		domain.insert(map<string,bool>::value_type("OR",true));
		domain.insert(map<string,bool>::value_type("TOP",true));
		domain.insert(map<string,bool>::value_type("BOTTOM",true));
		domain.insert(map<string,bool>::value_type("NOTTOP",true));
		domain.insert(map<string,bool>::value_type("NOTBOTTOM",true));
		domain.insert(map<string,bool>::value_type("EXISTS",true));
		domain.insert(map<string,bool>::value_type("NOTEXISTS",true));
		domain.insert(map<string,bool>::value_type("=",true));
		domain.insert(map<string,bool>::value_type("<>",true));
		domain.insert(map<string,bool>::value_type("<",true));
		domain.insert(map<string,bool>::value_type(">",true));
		domain.insert(map<string,bool>::value_type("IN",true));
		domain.insert(map<string,bool>::value_type("NOTIN",true));
		domain.insert(map<string,bool>::value_type("LIKE",true));
		domain.insert(map<string,bool>::value_type("NOTLIKE",true));
		domain.insert(map<string,bool>::value_type("ATLEAST",true));
		domain.insert(map<string,bool>::value_type("ATMOST",true));
	}
	LambdaExpression* toLambda(string sem)
	{
		if(sem[0]=='(')
		{
			sem=sem.substr(1,sem.length()-2);
			string tmpStr=sem.substr(0,sem.find(' ',0));
			map<string ,bool >::iterator l_it;
			l_it=domain.find(tmpStr);
			if(l_it==domain.end())
			{
				if(sem.find(' ',0)==string::npos) return new LambdaAtom(tmpStr);
				string paras=sem.substr(sem.find(' ',0)+1,sem.length()-(sem.find(' ',0)+1));
				return new CompoundLambda(new LambdaAtom(tmpStr),new LambdaAtom(paras));
			}
			else
			{
				sem=sem.substr(sem.find(' ',0)+1,sem.length()-(sem.find(' ',0)+1));
				vector<string>* paras=split(sem);
				int size=paras->size();
				vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
				for(int i=0;i<size;++i)
				{
					string para=paras->at(i);
					newParas->push_back(toLambda(para));
				}
				return new CompoundLambda(new LambdaAtom(tmpStr),new LambdaParas(newParas));
			}
		}
		else return new LambdaAtom(sem);
	}
};
#endif
#ifndef DOMAIN_H
#define DOMAIN_H
#include <map>
#include <string>
#include "Tag.h"
using namespace std;

class Domain
{
private:
	map<string,bool> *domain;
protected:
	void split(string& s, string& delim,vector< string >* ret)
	{
		size_t last = 0;
		size_t index=s.find_first_of(delim,last);
		while (index!=string::npos)
		{
			ret->push_back(s.substr(last,index-last));
			last=index+1;
			index=s.find_first_of(delim,last);
		}
		if (index-last>0)
		{
			ret->push_back(s.substr(last,index-last));
		}
	}
	bool isDigit(string word)
	{
		int size=word.length();
		for(int i=0;i<size;++i)
		{
			if(word[i]>'9'||word[i]<'0')
				return false;
		}
		return true;
	}
	bool isQuot(string word)
	{
		return word[0]=='\'';
	}
public:
	Domain()
	{
		domain=new map<string,bool>();
		initDomain();
	}
	void initDomain()
	{
		domain->insert(map<string,bool>::value_type("list the",true));
		domain->insert(map<string,bool>::value_type("list",true));
		domain->insert(map<string,bool>::value_type("the",true));
		domain->insert(map<string,bool>::value_type("big",true));
		domain->insert(map<string,bool>::value_type("biggest",true));
		domain->insert(map<string,bool>::value_type("cities",true));
		domain->insert(map<string,bool>::value_type("in the",true));
		domain->insert(map<string,bool>::value_type("in",true));
		domain->insert(map<string,bool>::value_type("the",true));
		domain->insert(map<string,bool>::value_type("mid-western",true));
		domain->insert(map<string,bool>::value_type("largest",true));
		domain->insert(map<string,bool>::value_type("state",true));
		domain->insert(map<string,bool>::value_type("in",true));
	}
	void addWord(string word)
	{
		domain->insert(map<string,bool>::value_type(word,true));
	}
	bool inDomain(string word)
	{
		map<string ,bool >::iterator l_it;
		l_it=domain->find(word);
		if(l_it==domain->end())
			return false;
		else return true;
	}
	vector<string>* splitStr(string query)
	{
		vector<string>* words=new vector<string>();
		vector<string>* wordTag=new vector<string>();
		string trim=" ";
		split(query,trim,words);
		int size=words->size();
		for(int i=0;i<size;++i)
		{
			string inStr=words->at(i);
			if(inStr[0]=='\'')
			{
				string quot="";
				while(inStr[inStr.length()-1]!='\'')
				{
					if(quot!="") quot+=" ";
					quot+=inStr;
					++i;
					inStr=words->at(i);
				}
				if(quot!="") quot+=" ";
				quot+=inStr;
				wordTag->push_back(quot);
			}
			else if(isDigit(inStr))
			{
				wordTag->push_back(inStr);
			}
			else
			{
				string olgStr="";
				while(inDomain(inStr)&&(++i)<size)
				{
					olgStr=inStr;
					inStr+=" ";
					inStr+=words->at(i);
				}
				if(i==size)
					wordTag->push_back(inStr);
				else
				{
					wordTag->push_back(olgStr);
					--i;
				}
			}
		}
		delete words;
		return wordTag;
	}
	vector<Tag*>* tagsStr(vector<string>* words)
	{
		vector<Tag*>* tag=new vector<Tag*>();
		int size=words->size();
		for(int i=0;i<size;++i)
		{
			if(isDigit(words->at(i)))
			{
				tag->push_back(new Tag("NVAL",words->at(i),words->at(i)));
			}
			else if(isQuot(words->at(i)))
			{
				tag->push_back(new Tag("QVAL",words->at(i),words->at(i)));
			}
			else
			{
				tag->push_back(new Tag("SVAL",words->at(i),words->at(i)));
			}
		}
		return tag;
	}
};
#endif
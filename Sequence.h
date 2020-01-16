#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>
using namespace std;

class Sequence
{
private:
	string* seqs;
	int size;
	int index;
public:
	Sequence(int size)
	{
		this->size=size;
		this->seqs=new string[size];
		this->index=0;
	}
	void append(string str)
	{
		if(index<size)
			this->seqs[index++]=str;
	}
	string getStr(int idx)
	{
		return this->seqs[idx];
	}
	void subst(string oldStr,string newStr)
	{
		for(int i=0;i<size;++i)
		{
			if(seqs[i]==oldStr)
				seqs[i]=newStr;
		}
	}
	int getSize()
	{
		return size;
	}
};
#endif
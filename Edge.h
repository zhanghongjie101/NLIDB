#ifndef EDGE_H
#define EDGE_H
#include "Symbol.h"
#include <vector>
#include <iostream>
using namespace std;

class Edge
{
private:
	int start;
	int end;
	Symbol lhs;
	Symbol* rhs;
	LambdaExpression* sem;
	int found;
	vector<Edge*>* tree;
public:
	Edge(int start,int end,Symbol lhs,Symbol* rhs,int found,LambdaExpression* sem)
	{
		this->start=start;
		this->end=end;
		this->lhs=lhs;
		this->rhs=rhs;
		this->sem=sem;
		this->found=found;
		this->tree=new vector<Edge*>();
	}
	vector<Edge*>* getTree() const
	{
		return tree;
	}
	void setTree(vector<Edge*>* tree)
	{
		int size=tree->size();
		this->tree->clear();
		for(int i=0;i<size;++i)
		{
			this->tree->push_back(tree->at(i));
		}
	}
	void addSubTree(Edge* edge)
	{
		tree->push_back(edge);
	}
	int getStart() const
	{
		return start;
	}
	int getEnd() const
	{
		return end;
	}
	Symbol getLhs() const
	{
		return lhs;
	}
	Symbol* getRhs() const
	{
		return rhs;
	}
	int getFound() const
	{
		return found;
	}
	int setFound(int found)
	{
		this->found=found;
	}
	bool isComplete() const
	{
		return rhs[found].equal(Nil);
	}
	Symbol expect() const
	{
		return rhs[found];
	}
	bool equal(const Edge* another)
	{
		if(this->lhs.equal(another->getLhs())&&
			this->rhs==another->getRhs()&&
			this->found==another->getFound()&&
			this->start==another->getStart()&&
			this->end==another->getEnd())
			return true;
		return false;
	}
	void print() const
	{
		if(lhs.getType()==Type::Terminal) return;
		cout<<lhs.getValue()<<"->";
		for(int i=0;!rhs[i].equal(Nil);++i)
		{
			cout<<rhs[i].getValue()<<" ";
		}
		cout<<endl;
		if(!tree->empty())
		{
			int size=tree->size();
			for(int i=0;i<size;++i)
			{
				tree->at(i)->print();
			}
		}
	}
	LambdaExpression* getSem() const
	{
		return sem;
	}
	void setSem(LambdaExpression* arg)
	{
		sem=sem->lambdaApply(arg);
	}
};
#endif
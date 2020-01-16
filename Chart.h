#ifndef CHART_H
#define CHART_H
#include "Edge.h"
#include "Grammer.h"
#include <vector>

class Chart
{
private:
	vector<Edge*>* ends_at;
	vector<string> words;
	Grammer grammer;
protected:
	void completer(Edge* edge)
	{
		int size=ends_at[edge->getStart()].size();
		for(int i=0;i<size;++i)
		{
			Edge* old=ends_at[edge->getStart()][i];
			if(old->expect().equal(edge->getLhs()))
			{
				LambdaExpression* oldLambda=old->getSem()->deep_copy();
				LambdaExpression* arg=edge->getSem()==NULL? NULL:edge->getSem()->deep_copy();
				Edge* temp=new Edge(old->getStart(),edge->getEnd(),old->getLhs(),old->getRhs(),old->getFound()+1,oldLambda->lambdaApply(arg));
//				cout<<temp->getSem()->getLambdaExpression()<<endl;
				temp->setTree(old->getTree());
				temp->addSubTree(edge);
				add_edge(temp);
			}
		}
	}
	void predictor(Edge* edge)
	{
		vector<Rule*>* rhs=grammer.getRhs(edge->expect());
		if(rhs==NULL) return;
		int size=rhs->size();
		for(int i=0;i<size;++i)
		{
			Symbol* slist=rhs->at(i)->getRhs();
			LambdaExpression* nLambda=rhs->at(i)->getSem()==NULL? NULL:rhs->at(i)->getSem()->deep_copy();
			Edge* temp=new Edge(edge->getEnd(),edge->getEnd(),edge->expect(),slist,0,nLambda);
			add_edge(temp);
		}
	}
	void scanner(int index)
	{
		Symbol* slist=new Symbol[2];
		slist[0].initor(Type::Terminal,words[index]);
		slist[1].initor(Type::NIL,"nil");
		Edge* temp=new Edge(index,index+1,Symbol(Type::Terminal,words[index]),slist,1,NULL);
		add_edge(temp);
	}
	bool isHere(Edge* edge)
	{
		int size=ends_at[edge->getEnd()].size();
		for(int i=0;i<size;++i)
		{
			if(ends_at[edge->getEnd()][i]->equal(edge))
				return true;
		}
		return false;
	}
	void add_edge(Edge* edge)
	{
		if(isHere(edge)) return;
		ends_at[edge->getEnd()].push_back(edge);
		if(edge->isComplete())
		{
			completer(edge);
		}
		else
		{
			predictor(edge);
		}
	}
public:
	Chart(vector<string>& words,Grammer& grammer)
	{
		this->words=words;
		this->grammer=grammer;
		ends_at=new vector<Edge*>[words.size()+1];
	}
	void chart_parse()
	{
		Symbol* slist=new Symbol[2];
		slist[0].initor(Type::NonTerminal,grammer.getStart()->getValue());
		slist[1].initor(Type::NIL,"nil");
		Edge* temp=new Edge(0,0,Symbol(Type::NonTerminal,"S*"),slist,0,
			new LambdaAbs("F",new CompoundLambda(new LambdaAtom("F"),new LambdaAtom("x"))));
		add_edge(temp);
		int size=words.size();
		for(int i=0;i<size;++i)
		{
			scanner(i);
		}
	}
	Edge* getTree() const
	{
		vector<Edge*>& temp=ends_at[words.size()];
		int size=temp.size();
		Edge* root=NULL;
		for(int i=0;i<size;++i)
		{
			if(temp[i]->getLhs().equal(Symbol(Type::NonTerminal,"S*")))
			{
				root=temp[i];
				break;
			}
		}
		return root;
	}
};
#endif
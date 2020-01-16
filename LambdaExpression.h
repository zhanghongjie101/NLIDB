#ifndef LAMBDAEXPRESSION_H
#define LAMBDAEXPRESSION_H
#include <string>
#include <map>
#include <vector>
#include "Element.h"

using namespace std;

class LambdaAbs;
class LambdaAtom;
class CompoundLambda;
class LambdaParas;

class LambdaExpression
{
public:
	virtual string getLambdaExpression() const=0;
	virtual LambdaAbs* getLambdaAbs()
	{
		return NULL;
	}
	virtual LambdaAtom* getLambdaAtom()
	{
		return NULL;
	}
	virtual CompoundLambda* getCompoundLambda()
	{
		return NULL;
	}
	virtual LambdaParas* getLambdaParas()
	{
		return NULL;
	}
	virtual void getFreeVariable(map<string,bool>* bound,map<string,bool>* freeVariable)=0;
	virtual LambdaExpression* subsVariable(string var,LambdaExpression* arg)=0;
	virtual LambdaExpression* eval()
	{
		return this;
	}
	virtual string getConstraintVariable() const
	{
		return "";
	}
	virtual LambdaExpression* lambdaApply(LambdaExpression* arg)=0;
	virtual LambdaExpression* deep_copy()=0;
	virtual bool lambdaEqual(LambdaExpression* arg)=0;
	virtual LambdaExpression* andFlat()=0;
	virtual LambdaExpression* notAndFlat()
	{
		return this;
	}
	virtual LambdaExpression* lambdaToQuery()
	{
		return NULL;
	}
};

class CompoundLambda : public LambdaExpression
{
private:
	LambdaExpression* first;
	LambdaExpression* rest;
public:
	bool isAnd() const
	{
		if(first->getLambdaAtom()!=NULL&&first->getLambdaExpression()=="AND")
			return true;
		else return false;
	}
	virtual LambdaExpression* andFlat()
	{
//		rest=rest->andFlat();
		if(!isAnd())
		{
			rest=rest->notAndFlat();
			return this;
		}
		rest=rest->andFlat();
//		if(rest->getLambdaParas()==NULL)
//			return rest;
		return this;
	}
	LambdaExpression* getFirst()
	{
		return first;
	}
	LambdaExpression* getRest()
	{
		return rest;
	}
	virtual bool lambdaEqual(LambdaExpression* arg)
	{
		if(arg->getCompoundLambda()==NULL) return false;
		return first->lambdaEqual(((CompoundLambda*)arg)->getFirst())&&rest->lambdaEqual(((CompoundLambda*)arg)->getRest());
	}
	virtual LambdaExpression* eval()
	{
		first=first->eval();
		rest=rest->eval();
		if(first->getLambdaAbs()!=NULL)
		{
			first=first->lambdaApply(rest);
			return first;
		}
		else
		{
			return this;
		}
	}

	CompoundLambda(LambdaExpression* first,LambdaExpression* rest)
	{
		this->first=first;
		this->rest=rest;
	}
	virtual string getLambdaExpression() const
	{
		string tmp="(";
		tmp+=first->getLambdaExpression()+" ";
		tmp+=rest->getLambdaExpression();
		tmp+=")";
		return tmp;
	}
	virtual void getFreeVariable(map<string,bool>* bound,map<string,bool>* freeVariable)
	{
		map<string,bool> tmpBound;
		map<string,bool>::iterator m1_Iter;
		for (m1_Iter = bound->begin(); m1_Iter != bound->end(); m1_Iter++)
			tmpBound[m1_Iter->first]=true;
		first->getFreeVariable(&tmpBound,freeVariable);
		rest->getFreeVariable(bound,freeVariable);
	}
	virtual LambdaExpression* subsVariable(string var,LambdaExpression* arg)
	{
//		first=first->subsVariable(var,arg);
//		rest=rest->subsVariable(var,arg);
		return new CompoundLambda(first->subsVariable(var,arg),rest->subsVariable(var,arg));
	}
	virtual CompoundLambda* getCompoundLambda()
	{
		return this;
	}
	virtual LambdaExpression* lambdaApply(LambdaExpression* arg)
	{
		if(arg==NULL) return this->deep_copy();
		LambdaExpression* tmp=new CompoundLambda(this,arg);
		tmp=tmp->eval();
		return tmp;
	}
	virtual LambdaExpression* deep_copy()
	{
		LambdaExpression* cfirst=first->deep_copy();
		LambdaExpression* crest=rest->deep_copy();
		return new CompoundLambda(cfirst,crest);
	}
};

class LambdaAtom : public LambdaExpression
{
private:
	string predictor;
public:
	virtual LambdaExpression* andFlat()
	{
		return this;
	}
	virtual bool lambdaEqual(LambdaExpression* arg)
	{
		if(arg->getLambdaAtom()==NULL) return false;
		return predictor==arg->getLambdaExpression();
	}
	LambdaAtom(string predictor)
	{
		this->predictor=predictor;
	}
	virtual string getLambdaExpression() const
	{
		return predictor;
	}
	virtual void getFreeVariable(map<string,bool>* bound,map<string,bool>* freeVariable)
	{
		map<string,bool>::iterator l_it=bound->find(predictor);
		if(l_it==bound->end())
			freeVariable->insert(map<string,bool>::value_type(predictor,true));
	}
	virtual LambdaExpression* subsVariable(string var,LambdaExpression* arg)
	{
		if(predictor==var)
		{
			return arg->deep_copy();
		}
		return this;
	}
	virtual LambdaAtom* getLambdaAtom()
	{
		return this;
	}
	virtual LambdaExpression* lambdaApply(LambdaExpression* arg)
	{
		if(arg==NULL) return this;
		return new CompoundLambda(this,arg->deep_copy());
	}
	virtual LambdaExpression* deep_copy()
	{
		return new LambdaAtom(predictor);
	}
};

class LambdaAbs : public LambdaExpression
{
private:
	string constraintVariable;
	LambdaExpression* expression;
protected:
	bool isFree(string var,LambdaExpression* arg)
	{
		map<string,bool>* bound=new map<string,bool>();
		map<string,bool>* freeVariable=new map<string,bool>();
		arg->getFreeVariable(bound,freeVariable);
		map<string,bool>::iterator l_it=freeVariable->find(var);
		if(l_it==freeVariable->end())
			return false;
		return true;
	}
public:
	virtual LambdaExpression* andFlat()
	{
		expression=expression->andFlat();
		return this;
	}
	LambdaExpression* getExp()
	{
		return expression;
	}
	virtual bool lambdaEqual(LambdaExpression* arg)
	{
		if(arg->getLambdaAbs()==NULL) return false;
		return constraintVariable==arg->getConstraintVariable()&&expression->lambdaEqual(((LambdaAbs*)arg)->getExp());
	}
	LambdaAbs(string constraintVariable,LambdaExpression* expression)
	{
		this->constraintVariable=constraintVariable;
		this->expression=expression;
	}
	virtual LambdaAbs* getLambdaAbs()
	{
		return this;
	}
	virtual string getLambdaExpression() const
	{
		string tmp="(L "+constraintVariable;
		if(expression->getLambdaAbs()==NULL)
			tmp+="(";
		tmp+=expression->getLambdaExpression();
		tmp+=")";
		if(expression->getLambdaAbs()==NULL)
			tmp+=")";
		return tmp;
	}
	virtual void getFreeVariable(map<string,bool>* bound,map<string,bool>* freeVariable)
	{
		bound->insert(map<string,bool>::value_type(constraintVariable,true));
		expression->getFreeVariable(bound,freeVariable);
	}
	virtual LambdaExpression* subsVariable(string var,LambdaExpression* arg)
	{
		if(constraintVariable==var)
		{
			return this;
		}
		return new LambdaAbs(constraintVariable,expression->subsVariable(var,arg));
	}
	virtual string getConstraintVariable() const
	{
		return constraintVariable;
	}
	virtual LambdaExpression* lambdaApply(LambdaExpression* arg)
	{
		if(arg==NULL) return this;
		if(isFree(constraintVariable,arg))
		{
			string freshVar=Element::getFreshVar(constraintVariable);
			expression=expression->subsVariable(constraintVariable,new LambdaAtom(freshVar));
			this->constraintVariable=freshVar;
		}
		LambdaExpression* tmp=expression->subsVariable(constraintVariable,arg);
//		cout<<tmp->getLambdaExpression()<<endl;
		tmp=tmp->eval();
//		cout<<tmp->getLambdaExpression()<<endl;
		return tmp;
	}
	virtual LambdaExpression* eval()
	{
		return new LambdaAbs(constraintVariable,expression->eval());
	}
	virtual LambdaExpression* deep_copy()
	{
		LambdaExpression* cexpression=expression->deep_copy();
		return new LambdaAbs(constraintVariable,cexpression);
	}
};

class LambdaParas : public LambdaExpression
{
private:
	vector<LambdaExpression*>* paras;
public:
	vector<LambdaExpression*>* getParas()
	{
		return paras;
	}
	LambdaParas(vector<LambdaExpression*>* paras)
	{
		this->paras=paras;
	}
	virtual string getLambdaExpression() const
	{
		string tmp="";
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			tmp+=paras->at(i)->getLambdaExpression();
			if(i!=size-1)
				tmp+=" ";
		}
		return tmp;
	}
	virtual LambdaParas* getLambdaParas()
	{
		return this;
	}
	virtual void getFreeVariable(map<string,bool>* bound,map<string,bool>* freeVariable)
	{
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			map<string,bool> tmpBound;
			map<string,bool>::iterator m1_Iter;
			for (m1_Iter = bound->begin(); m1_Iter != bound->end(); m1_Iter++)
				tmpBound[m1_Iter->first]=true;
			paras->at(i)->getFreeVariable(&tmpBound,freeVariable);
		}
	}
	virtual LambdaExpression* subsVariable(string var,LambdaExpression* arg)
	{
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			paras->at(i)=paras->at(i)->subsVariable(var,arg);
		}
		return this;
	}
	virtual LambdaExpression* eval()
	{
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			paras->at(i)=paras->at(i)->eval();
		}
		return this;
	}
	virtual LambdaExpression* lambdaApply(LambdaExpression* arg)
	{
		return NULL;
	}
	virtual LambdaExpression* deep_copy()
	{
		vector<LambdaExpression*>* cparas=new vector<LambdaExpression*>();
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			cparas->push_back(paras->at(i)->deep_copy());
		}
		return new LambdaParas(cparas);
	}
	virtual bool lambdaEqual(LambdaExpression* arg)
	{
		if(arg->getLambdaParas()==NULL) return false;
		vector<LambdaExpression*>* argParas=((LambdaParas*)arg)->getParas();
		int size=paras->size();
		if(size!=argParas->size()) return false;
		for(int i=0;i<size;++i)
		{
			if(!paras->at(i)->lambdaEqual(argParas->at(i)))
				return false;
		}
		return true;
	}
	virtual LambdaExpression* andFlat()
	{
		vector<LambdaExpression*>* tmp=new vector<LambdaExpression*>();
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			paras->at(i)=paras->at(i)->andFlat();
			if(paras->at(i)->getLambdaParas()!=NULL)
			{
				LambdaParas* la=(LambdaParas*)(paras->at(i));
				vector<LambdaExpression*>* loc=la->getParas();
				int k=loc->size();
				for(int j=0;j<k;++j)
				{
					tmp->push_back(loc->at(j)->deep_copy());
				}
			}
			else if(paras->at(i)->getCompoundLambda()!=NULL)
			{
				CompoundLambda* la=(CompoundLambda*)(paras->at(i));
				if(la->isAnd())
				{
					vector<LambdaExpression*>* loc=((LambdaParas*)(la->getRest()))->getParas();
					int k=loc->size();
					for(int j=0;j<k;++j)
					{
						tmp->push_back(loc->at(j)->deep_copy());
					}
				}
				else
				{
					tmp->push_back(paras->at(i)->deep_copy());
				}
			}
			else
			{
				tmp->push_back(paras->at(i)->deep_copy());
			}
		}
		vector<LambdaExpression*>* tmp2=new vector<LambdaExpression*>();
		size=tmp->size();
		for(int i=0;i<size;++i)
		{
			int bsize=tmp2->size();
			bool flag=false;
			for(int j=0;j<bsize;++j)
			{
				if(tmp->at(i)->lambdaEqual(tmp2->at(j)))
				{
					flag=true;
					break;
				}
			}
			if(!flag)
			{
				tmp2->push_back(tmp->at(i)->deep_copy());
			}
		}
		/*if(tmp2->size()==1)
		{
			return tmp2->at(0);
		}*/
		return new LambdaParas(tmp2);
	}
	virtual LambdaExpression* notAndFlat()
	{
		int size=paras->size();
		for(int i=0;i<size;++i)
		{
			paras->at(i)=paras->at(i)->andFlat();
		}
		return this;
	}
};

#endif
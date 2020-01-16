#ifndef QUERYTRANSFORMERS_H
#define QUERYTRANSFORMERS_H
#include "LambdaExpression.h"
#include "LambdaFunction.h"
#include "QueryType.h"

class QueryTransformers
{
public:
	static LambdaExpression* dropNotExists(LambdaExpression* query)
	{
		if(query->getCompoundLambda()==NULL) return NULL;
		CompoundLambda* tmpQuery=(CompoundLambda*)query;
		LambdaParas* condlist=(LambdaParas*)(tmpQuery->getRest());
		vector<LambdaExpression*>* newCondlist=new vector<LambdaExpression*>();
		int size=condlist->getParas()->size();
		for(int i=0;i<size;++i)
		{
			if(condlist->getParas()->at(i)->getCompoundLambda()==NULL)
			{
				newCondlist->push_back(condlist->getParas()->at(i)->deep_copy());
				continue;
			}
			CompoundLambda* tmpCond=(CompoundLambda*)(condlist->getParas()->at(i));
			if(tmpCond->getFirst()->getLambdaExpression()!="NOTEXISTS")
			{
				newCondlist->push_back(condlist->getParas()->at(i)->deep_copy());
				continue;
			}
		}
		return new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(newCondlist));
	}
};
#endif
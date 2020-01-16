#ifndef QUERYTYPE_H
#define QUERYTYPE_H
#include "LambdaExpression.h"

class QueryType
{
public:
	static bool isQquery(LambdaExpression* query)
	{
		if(query->getCompoundLambda()!=NULL)
		{
			CompoundLambda* comQuery=(CompoundLambda*)query;
			if(comQuery->getFirst()->getLambdaExpression()=="OR") return true;
			else return false;
		}
		return false;
	}
	static bool isLquery(LambdaExpression* query)
	{
		return !isQquery(query);
	}
	static bool isTupleQuery(LambdaExpression* query)
	{
		if(!isLquery(query)) return false;
		CompoundLambda* tmp=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmp->getRest());
		if(paras->getParas()->at(0)->getLambdaAtom()!=NULL) return true;
		else return false;
	}
	static bool isProjectionQuery(LambdaExpression* query)
	{
		if(!isLquery(query)) return false;
		CompoundLambda* tmp=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmp->getRest());
		if(paras->getParas()->at(0)->getCompoundLambda()!=NULL) return true;
		else return false;
	}
	/*add new type here*/
};
#endif
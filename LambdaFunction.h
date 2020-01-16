#ifndef LAMBDAFUNCTION_H
#define LAMBDAFUNCTION_H
#include "LambdaExpression.h"

class LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")=0;
};

class NotExists: public LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")
	{
		if(arg->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)arg;
		if(tmpCondition->getFirst()->getLambdaExpression()=="NOTEXISTS")
			return true;
		else return false;
	}
};

class RangeCondition: public LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")
	{
		return ConditionType::isRangeCondition(arg);
	}
};

class WhereCondition: public LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")
	{
		return ConditionType::isRegularCondition(arg)||
			   ConditionType::isJoinCondition(arg)||
			   ConditionType::isTupleEqualityCondition(arg);
	}
};

class SuperlativeCondition:public LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")
	{
		return ConditionType::isSuperlativeCondition(arg);
	}
};

class AtomicParametersCondition:public LambdaFunction
{
public:
	virtual bool meetFunction(LambdaExpression* arg,string var="")
	{
		if(!ConditionType::isSimpleCondition(arg)) return false;
		CompoundLambda* tmp=(CompoundLambda*)arg;
		LambdaParas* paras=(LambdaParas*)(tmp->getRest());
		string str=paras->getParas()->at(2)->getLambdaExpression();
		return str[0]=='$';
	}
};
#endif
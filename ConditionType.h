#ifndef CONDITIONTYPE_H
#define CONDITIONTYPE_H
#include "LambdaExpression.h"

class ConditionType
{
public:
	static bool isComponentCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()=="EXISTS"||
			tmpCondition->getFirst()->getLambdaExpression()=="NOTEXISTS")
			return true;
		else return false;
	}
	static bool isCardinalityCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()=="ATLEAST"||
			tmpCondition->getFirst()->getLambdaExpression()=="ATMOST")
			return true;
		else return false;
	}
	static bool isRangeCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaAtom()!=NULL&&
			tmpCondition->getRest()->getLambdaParas()==NULL)
			return true;
		else return false;
	}
	static bool isAndCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()=="AND")
			return true;
		else return false;
	}
	static bool isSimpleCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(!Element::isSimpleOp(tmpCondition->getFirst()->getLambdaExpression())) return false;
		LambdaParas* tmpParas=(LambdaParas*)(tmpCondition->getRest());
		return tmpParas->getParas()->size()==3;
	}
	static bool isSetCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(!Element::isSetOp(tmpCondition->getFirst()->getLambdaExpression())) return false;
		LambdaParas* tmpParas=(LambdaParas*)(tmpCondition->getRest());
		return tmpParas->getParas()->size()==3;
	}
	static bool isRegularCondition(LambdaExpression* condition)
	{
		return isSimpleCondition(condition)||isSetCondition(condition);
	}
	static bool isJoinCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(!Element::isSimpleOp(tmpCondition->getFirst()->getLambdaExpression())) return false;
		LambdaParas* tmpParas=(LambdaParas*)(tmpCondition->getRest());
		return tmpParas->getParas()->size()==4;
	}
	static bool isTupleEqualityCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()!="=") return false;
		LambdaParas* tmpParas=(LambdaParas*)(tmpCondition->getRest());
		return tmpParas->getParas()->size()==2;
	}
	static bool isSuperlativeCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()=="TOP"||
			tmpCondition->getFirst()->getLambdaExpression()=="BOTTOM"||
			tmpCondition->getFirst()->getLambdaExpression()=="NOTTOP"||
			tmpCondition->getFirst()->getLambdaExpression()=="NOTBOTTOM")
			return true;
		else return false;
	}
	static bool isLikeCondition(LambdaExpression* condition)
	{
		if(condition->getCompoundLambda()==NULL) return false;
		if(!isSimpleCondition(condition)) return false;
		CompoundLambda* tmpCondition=(CompoundLambda*)condition;
		if(tmpCondition->getFirst()->getLambdaExpression()=="LIKE"||
			tmpCondition->getFirst()->getLambdaExpression()=="NOTLIKE")
			return true;
		else return false;
	}
};
#endif
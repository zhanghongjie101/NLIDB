#ifndef RULE_H
#define RULE_H
#include "Symbol.h"
#include "LambdaExpression.h"

class Rule
{
private:
	Symbol lhs;
	Symbol* rhs;
	//Ìí¼ÓÓïÒå;
	LambdaExpression* sem;
public:
	Rule(Symbol lhs,Symbol* rhs,LambdaExpression* sem)
	{
		this->lhs=lhs;
		this->rhs=rhs;
		this->sem=sem;
	}
	Symbol getLhs() const
	{
		return lhs;
	}
	Symbol* getRhs() const
	{
		return rhs;
	}
	LambdaExpression* getSem() const
	{
		return sem;
	}
};
#endif
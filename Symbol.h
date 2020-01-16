#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include "LambdaExpression.h"
using namespace std;
enum Type {Terminal,NonTerminal,NIL};

class Symbol
{
private:
	string value;
	Type type;
public:
	Symbol(Type type=Type::NIL,string value="")
	{
		this->type=type;
		this->value=value;
	}
	Symbol(const Symbol& copy)
	{
		this->type=copy.getType();
		this->value=copy.getValue();
	}
	void initor(Type type,string value)
	{
		this->type=type;
		this->value=value;
	}
	void setType(Type type)
	{
		this->type=type;
	}
	void setValue(string value)
	{
		this->value=value;
	}
	Type getType() const
	{
		return type;
	}
	string getValue() const
	{
		return value;
	}
	bool equal(const Symbol& another)
	{
		return type==another.getType()&&value==another.getValue();
	}
};

const Symbol Nil(Type::NIL,"nil");
#endif
#ifndef PATTERN_H
#define PATTERN_H
#include "Sequence.h"
#include "LambdaExpression.h"

class Pattern
{
private:
	string node_name;
	string pattern_type;
	Sequence* sequence;
	string features;
	string inject_features;
	bool dropped;
public:
	Pattern(string node_name,string pattern_type,Sequence* sequence,string features,string inject_features,bool dropped)
	{
		this->node_name=node_name;
		this->pattern_type=pattern_type;
		this->sequence=sequence;
		this->features=features;
		this->inject_features=inject_features;
		this->dropped=dropped;
	}

};
#endif
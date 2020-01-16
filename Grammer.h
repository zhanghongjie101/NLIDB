#ifndef GRAMMER_H
#define GRAMMER_H
#include <vector>
#include <string>
#include <hash_map>
#include "Rule.h"
#include "StringToLambda.h"
#include "Tag.h"

using namespace std;

class Grammer
{
private:
	hash_map<string,vector<Rule*>*> rewrites_for;
	Symbol* start;
	StringToLambda s2l;
public:
	Grammer()
	{
		s2l.initDomain();
		rewrites_for.clear();
	}
	vector<Rule*>* getRhs(Symbol& lhs)
	{
		return rewrites_for[lhs.getValue()];
	}
	Symbol* getStart() const
	{
		return start;
	}
	void initGrammer()
	{
		//QUERY->"list the" NP
		vector<Rule*>* rhs=new vector<Rule*>();
		Symbol* slist=new Symbol[3];
		slist[0].initor(Type::Terminal,"list the");
		slist[1].initor(Type::NonTerminal,"NP");
		slist[2].initor(Type::NIL,"nil");
		Rule* rule=new Rule(Symbol(Type::NonTerminal,"QUERY"),slist,new LambdaAbs("x",s2l.toLambda("x")));
		rhs->push_back(rule);
		slist=new Symbol[2];
		slist[0].initor(Type::NonTerminal,"NP");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"QUERY"),slist,new LambdaAbs("x",s2l.toLambda("x")));
		rhs->push_back(rule);
		rewrites_for["QUERY"]=rhs;
		//NP->PRE NP
		//NP->NP1
		rhs=new vector<Rule*>();
		slist=new Symbol[3];
		slist[0].initor(Type::NonTerminal,"PRE");
		slist[1].initor(Type::NonTerminal,"NP");
		slist[2].initor(Type::NIL,"nil");
		LambdaExpression* prenp=new LambdaAbs("f",new LambdaAbs("g",new LambdaAbs("x",
							s2l.toLambda("(AND (g x) (f x))"))));
		rule=new Rule(Symbol(Type::NonTerminal,"NP"),slist,prenp);
		rhs->push_back(rule);
		slist=new Symbol[2];
		slist[0].initor(Type::NonTerminal,"NP1");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"NP"),slist,new LambdaAbs("x",s2l.toLambda("x")));
		rhs->push_back(rule);
		rewrites_for["NP"]=rhs;
		//NP1->NP1 POST
		//NP1->HEAD
		rhs=new vector<Rule*>();
		slist=new Symbol[3];
		slist[0].initor(Type::NonTerminal,"NP1");
		slist[1].initor(Type::NonTerminal,"POST");
		slist[2].initor(Type::NIL,"nil");
		LambdaExpression* nppost=new LambdaAbs("f",new LambdaAbs("g",new LambdaAbs("x",
			s2l.toLambda("(AND (f x) (g x))"))));
		rule=new Rule(Symbol(Type::NonTerminal,"NP1"),slist,nppost);
		rhs->push_back(rule);
		slist=new Symbol[2];
		slist[0].initor(Type::NonTerminal,"HEAD");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"NP1"),slist,new LambdaAbs("x",s2l.toLambda("x")));
		rhs->push_back(rule);
		rewrites_for["NP1"]=rhs;
		//HEAD->"cities"
		//HEAD->"state"
		rhs=new vector<Rule*>();
		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"cities");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"HEAD"),slist,new LambdaAbs("x",s2l.toLambda("(City x)")));
		rhs->push_back(rule);
		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"state");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"HEAD"),slist,new LambdaAbs("x",s2l.toLambda("(State x)")));
		rhs->push_back(rule);
		rewrites_for["HEAD"]=rhs;
		//PRE->"big"
		//PRE->"largest"
		//PRE->"mid-western"

		rhs=new vector<Rule*>();
		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"big");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"PRE"),slist,new LambdaAbs("x",
			s2l.toLambda("(> (x) (population) (100000))")));
		rhs->push_back(rule);
		/*biggest*/
		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"biggest");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"PRE"),slist,new LambdaAbs("x",
			s2l.toLambda("(TOP (1) (population) (x))")));
		rhs->push_back(rule);

		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"largest");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"PRE"),slist,new LambdaAbs("x",
			s2l.toLambda("(TOP (1) (population) (x))")));
		rhs->push_back(rule);
		slist=new Symbol[2];
		slist[0].initor(Type::Terminal,"mid-western");
		slist[1].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"PRE"),slist,new LambdaAbs("x",
			s2l.toLambda("(IN (x) (name) (\"Indiana,...,Wisconsin\"))")));
		rhs->push_back(rule);
		rewrites_for["PRE"]=rhs;
		//POST->"in the"
		rhs=new vector<Rule*>();
		slist=new Symbol[3];
		slist[0].initor(Type::Terminal,"in the");
		slist[1].initor(Type::NonTerminal,"NP");
		slist[2].initor(Type::NIL,"nil");
		rule=new Rule(Symbol(Type::NonTerminal,"POST"),slist,new LambdaAbs("z",new LambdaAbs("x",
			s2l.toLambda("(AND (City x) (EXISTS (AND (y)) (AND (State y) (= (x) (state) (y) (name)) (z y))))"))));
		rhs->push_back(rule);
		rewrites_for["POST"]=rhs;
		rewrites_for["SVAL"]=new vector<Rule*>();
		rewrites_for["NVAL"]=new vector<Rule*>();
		rewrites_for["QVAL"]=new vector<Rule*>();
		start=new Symbol(Type::NonTerminal,"QUERY");
	}
	void buildRuleFromTags(vector<Tag*>* tags)
	{
		int size=tags->size();
		for(int i=0;i<size;++i)
		{
			Symbol* slist=new Symbol[2];
			slist[0].initor(Type::Terminal,tags->at(i)->getTagSpan());
			slist[1].initor(Type::NIL,"nil");
			Rule* rule=new Rule(Symbol(Type::NonTerminal,tags->at(i)->getTagType()),slist,s2l.toLambda(tags->at(i)->getTagValue()));
			rewrites_for[tags->at(i)->getTagType()]->push_back(rule);
		}
	}
	void buildRuleFromLexicon()/*meet domain model*/
	{
		vector<Rule*>* rhs=rewrites_for["PRE"];
		Symbol* slist=new Symbol[3];
		slist[0].initor(Type::NonTerminal,"NVAL");
		slist[1].initor(Type::Terminal,"largest");
		slist[2].initor(Type::NIL,"nil");
		LambdaExpression* concept=new LambdaAbs("$C5",new LambdaAbs("x",
			s2l.toLambda("(TOP ($C5) (AREA) (x))")));
		Rule* rule=new Rule(Symbol(Type::NonTerminal,"PRE"),slist,concept);
		rhs->push_back(rule);
	}
};
#endif
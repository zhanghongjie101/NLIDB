#include <iostream>
#include "Chart.h"
#include "Element.h"
#include "SqlDml.h"
#include "Domain.h"
#include "StringToLambda.h"
#include "SqlExecute.h"
using namespace std;

int main()
{
/*	StringToLambda ss2l;
	ss2l.initDomain();
	LambdaExpression* teststr=ss2l.toLambda("(AND (City x) (= (x) (name) ($C1)) (= (x) (name) (hefei)))");
	SqlDml test(teststr);
	cout<<test.query2sql()<<endl;*/
/*	SqlExecute sqlExec("root","mysql","localhost","test",3306);
	sqlExec.openConnect();
	sqlExec.exec("select * from samples");
	sqlExec.closedConnect();*/
	Grammer grammer;
	grammer.initGrammer();
	Domain domain;
	//string query="list the biggest cities in the mid-western state";
	//string query="5 largest state";
	string query="list the 3 largest cities in the mid-western state";
	//string query="cities in the mid-western state";
	vector<string>* words=domain.splitStr(query);
	vector<Tag*>* tags=domain.tagsStr(words);
	grammer.buildRuleFromTags(tags);
	grammer.buildRuleFromLexicon();
	cout<<"Ô­¾ä:"<<query<<endl;
	Chart chart(*words,grammer);
	chart.chart_parse();
	Edge* root=chart.getTree();
	if(root==NULL)
	{
		cout<<"can not parse"<<endl;
		system("pause");
		return 0;
	}
	root->print();
	LambdaExpression* newLambda=root->getSem()->andFlat();
	cout<<"ÓïÒå£º"<<newLambda->getLambdaExpression()<<endl;
/*	map<string,bool>* bound=new map<string,bool>();
	map<string,bool>* freeVariable=new map<string,bool>();
//	newLambda=new LambdaAbs("z",newLambda);
	newLambda->getFreeVariable(bound,freeVariable);
	map<string,bool>::iterator m1_Iter;
	cout<<"free variables:";
	for (m1_Iter = freeVariable->begin(); m1_Iter != freeVariable->end(); m1_Iter++)
		cout<<m1_Iter->first<<" ";
	cout<<endl;*/
	SqlDml sql(newLambda);
	cout<<endl<<sql.query2sql()<<endl;
/*	LambdaAbs* lambda=new LambdaAbs("f",new LambdaAbs("x",
									new CompoundLambda(new LambdaAtom("LargestByArea"),
									new CompoundLambda(new LambdaAtom("x"),
									new CompoundLambda(new LambdaAtom("and"),
									new CompoundLambda(new CompoundLambda(new LambdaAtom("f"),new LambdaAtom("x")),
									new CompoundLambda(new LambdaAtom("State"),new LambdaAtom("x"))))))));*/
/*	vector<LambdaExpression*>* paras=new vector<LambdaExpression*>();
	paras->push_back(new CompoundLambda(new LambdaAtom("f"),new LambdaAtom("x")));
	paras->push_back(new CompoundLambda(new LambdaAtom("State"),new LambdaAtom("x")));
	CompoundLambda* c1=new CompoundLambda(new LambdaAtom("and"),new LambdaParas(paras));
	paras=new vector<LambdaExpression*>();
	paras->push_back(new LambdaAtom("x"));
	paras->push_back(c1);
	LambdaAbs* lambda=new LambdaAbs("f",new LambdaAbs("x",
									new CompoundLambda(new LambdaAtom("LargestByArea"),
									new LambdaParas(paras))));
	cout<<lambda->getLambdaExpression()<<endl;
	LambdaExpression* newLambda=lambda->lambdaApply(new LambdaAbs("x",new CompoundLambda(new LambdaAtom("City"),new LambdaAtom("x"))));
	cout<<newLambda->getLambdaExpression()<<endl;
	newLambda=newLambda->lambdaApply(new LambdaAtom("x"));
	cout<<newLambda->getLambdaExpression()<<endl;

	map<string,bool>* bound=new map<string,bool>();
	map<string,bool>* freeVariable=new map<string,bool>();
	newLambda->getFreeVariable(bound,freeVariable);
	map<string,bool>::iterator m1_Iter;
	cout<<"free variables:";
	for (m1_Iter = freeVariable->begin(); m1_Iter != freeVariable->end(); m1_Iter++)
		cout<<m1_Iter->first<<" ";
	cout<<endl;*/
/*	LambdaAbs* l1=new LambdaAbs("x",new CompoundLambda(new LambdaAtom("x"),new LambdaAtom("y")));
	LambdaAbs* l2=new LambdaAbs("y",new CompoundLambda(new LambdaAtom("y"),new LambdaAtom("z")));
	LambdaExpression* l3=l2->lambdaApply(l1)->andFlat();
	cout<<l3->getLambdaExpression()<<endl;*/
	system("pause");
}
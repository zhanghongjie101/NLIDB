#ifndef SQLDML_H
#define SQLDML_H
#include "LambdaExpression.h"
#include "QueryType.h"
#include "ConditionType.h"
#include "LambdaFunction.h"

class SqlDml
{
private:
	LambdaExpression* query;
protected:
	vector<LambdaExpression*>* getQueryParameters()
	{
		AtomicParametersCondition atomicParametersCondition;
		return getWithinQuery(&atomicParametersCondition);
	}
	LambdaExpression* getXthPara(LambdaExpression* arg,int x)
	{
		if(arg->getCompoundLambda()==NULL) return NULL;
		CompoundLambda* tmpQuery=(CompoundLambda*)arg;
		if(tmpQuery->getRest()->getLambdaParas()==NULL) return NULL;
		LambdaParas* paras=(LambdaParas*)(tmpQuery->getRest());
		return paras->getParas()->at(x)->deep_copy();
	}
	LambdaExpression* l2query()
	{
		if(query->getCompoundLambda()==NULL)
			return NULL;
		CompoundLambda* compoundQuery=(CompoundLambda*)query;
		if(compoundQuery->getFirst()->getLambdaExpression()=="AND")
		{
			LambdaParas* paras=(LambdaParas*)compoundQuery->getRest();
			CompoundLambda* queryType=(CompoundLambda*)(paras->getParas()->at(0));
			CompoundLambda* tempType=(CompoundLambda*)(paras->getParas()->at(1));
			string type=queryType->getFirst()->getLambdaExpression();
			if(type=="x")
			{
				LambdaExpression* newQuery=query->deep_copy();
				CompoundLambda* compoundNewQuery=(CompoundLambda*)newQuery;
				LambdaParas* newParas=(LambdaParas*)compoundNewQuery->getRest();
				newParas->getParas()->at(1)=new LambdaAtom(tempType->getFirst()->getLambdaExpression());
				return newQuery;
			}
			else
			{
				vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
				newParas->push_back(new LambdaAtom("x"));
				newParas->push_back(new LambdaAtom(type));
				int size=paras->getParas()->size();
				for(int i=1;i<size;++i)
				{
					newParas->push_back(paras->getParas()->at(i)->deep_copy());
				}
				return new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(newParas));
			}
		}
		else
		{
			vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
			newParas->push_back(new LambdaAtom(compoundQuery->getRest()->getLambdaExpression()));
			newParas->push_back(new LambdaAtom(compoundQuery->getFirst()->getLambdaExpression()));
			return new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(newParas));
		}
	}
	LambdaExpression* removeCond(LambdaExpression* condlist,LambdaFunction *lFunction)
	{
		if(condlist==NULL) return NULL;
		LambdaParas* paras;
		if(condlist->getLambdaParas()!=NULL) paras=(LambdaParas*)(condlist);
		else
		{
			CompoundLambda* tmpCondlist=(CompoundLambda*)condlist;
			paras=(LambdaParas*)(tmpCondlist->getRest());
		}
		int size=paras->getParas()->size();
		vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
		for(int i=0;i<size;++i)
		{
			if(!lFunction->meetFunction(paras->getParas()->at(i)))
				newParas->push_back(paras->getParas()->at(i)->deep_copy());
		}
		return new LambdaParas(newParas);
	}
	string findRange(string e,LambdaExpression* rlist)
	{
		LambdaParas* paras=(LambdaParas*)(rlist);
		int size=paras->getParas()->size();
		for(int i=0;i<size;++i)
		{
			if(ConditionType::isRangeCondition(paras->getParas()->at(i)))
			{
				CompoundLambda* tmpCond=(CompoundLambda*)(paras->getParas()->at(i));
				if(tmpCond->getRest()->getLambdaExpression()==e)
					return tmpCond->getFirst()->getLambdaExpression();
			}
		}
		return "";
	}
	string existential2sql(LambdaExpression* condlist,LambdaExpression* query)
	{
		string exStr="";
		LambdaParas* paras=(LambdaParas*)condlist;
		CompoundLambda* listE=(CompoundLambda*)(paras->getParas()->at(0));
		LambdaParas* elist=(LambdaParas*)(listE->getRest());
		CompoundLambda* listR=(CompoundLambda*)(paras->getParas()->at(1));
		LambdaParas* rlist=(LambdaParas*)(listR->getRest());

		int esz=elist->getParas()->size();
		for(int k=0;k<esz;++k)
		{
			string tmpStr="";
			if(k>0) tmpStr+=",";
			vector<LambdaExpression*>* tmpConlist=new vector<LambdaExpression*>();
			SuperlativeCondition superlativeCondition;
			string fe=elist->getParas()->at(k)->getLambdaExpression();
			getCondFromList(rlist,&superlativeCondition,tmpConlist,fe);
			if(!tmpConlist->empty())
			{
				LambdaExpression* sup=tmpConlist->at(0);
				string range=findRange(fe,rlist);
				vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
				newParas->push_back(new LambdaAtom("x"));
				newParas->push_back(new LambdaAtom(range));
				LambdaExpression* tmpQuery=new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(newParas));
				tmpStr+=superlative2sql(sup,NULL,tmpQuery);
				tmpStr+=" AS ";
				tmpStr+=fe;
				exStr+=tmpStr;
			}
			else
			{
				string tmpStr="";
				string range=findRange(fe,rlist);
				tmpStr+=range;
				tmpStr+=" AS ";
				tmpStr+=fe;
				exStr+=tmpStr;
			}
		}
		return exStr;
	}
	string cardinality2sql(LambdaExpression* cond,LambdaExpression* argQuery)
	{
		string carStr="";
		/*go on*/
		return "";
	}
	string component2sql(LambdaExpression* cond,LambdaExpression* argQuery)
	{
		string comStr="";
		CompoundLambda* tmpCond=(CompoundLambda*)cond;
		LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
		SuperlativeCondition superlativeCondition;
		LambdaExpression* condlist=removeCond(tmpParas->getParas()->at(1),&superlativeCondition);
		comStr+=tmpCond->getFirst()->getLambdaExpression()+"(SELECT *\r\nFROM ";
		comStr+=existential2sql(tmpParas,argQuery);
		LambdaParas* newParas=(LambdaParas*)condlist;
		if(!newParas->getParas()->empty())
		{
			comStr+=" WHERE ";
			comStr+=condlist2sql(newParas,argQuery);
			comStr+="\r\n";
		}
		comStr+=" )";
		return comStr;
	}
	string condlist2sql(LambdaExpression* condlist,LambdaExpression* argQuery)
	{
		if(condlist==NULL||(condlist->getCompoundLambda()==NULL&&condlist->getLambdaParas()==NULL)) return "";
		string condStr="";
		RangeCondition rangeCondition;
		LambdaParas* newCondlist=(LambdaParas*)(removeCond(condlist,&rangeCondition));
		int size=newCondlist->getParas()->size();
		bool flag=false;
		for(int i=0;i<size;++i)
		{
			if(flag) condStr+=" AND ";
			if(ConditionType::isLikeCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				CompoundLambda* tmpCond=(CompoundLambda*)(newCondlist->getParas()->at(i));
				LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
				condStr+=tmpParas->getParas()->at(0)->getLambdaExpression()+".";
				condStr+=tmpParas->getParas()->at(1)->getLambdaExpression()+" ";
				condStr+=tmpCond->getFirst()->getLambdaExpression()+" ";
				condStr+="'%"+tmpParas->getParas()->at(2)->getLambdaExpression()+"%'";
				continue;
			}
			if(ConditionType::isSimpleCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				CompoundLambda* tmpCond=(CompoundLambda*)(newCondlist->getParas()->at(i));
				LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
				condStr+=tmpParas->getParas()->at(0)->getLambdaExpression()+".";
				condStr+=tmpParas->getParas()->at(1)->getLambdaExpression()+" ";
				if(tmpParas->getParas()->at(2)->getLambdaExpression()=="NULL"&&
					tmpCond->getFirst()->getLambdaExpression()=="=")
					condStr+=" IS NULL ";
				else if(tmpParas->getParas()->at(2)->getLambdaExpression()=="NULL"&&
					tmpCond->getFirst()->getLambdaExpression()=="<>")
					condStr+=" IS NOT NULL ";
				else
					condStr+=tmpCond->getFirst()->getLambdaExpression()+
					tmpParas->getParas()->at(2)->getLambdaExpression();
				continue;
			}
			if(ConditionType::isSetCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				CompoundLambda* tmpCond=(CompoundLambda*)(newCondlist->getParas()->at(i));
				LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
				condStr+=tmpParas->getParas()->at(0)->getLambdaExpression()+".";
				condStr+=tmpParas->getParas()->at(1)->getLambdaExpression()+" ";
				condStr+=tmpCond->getFirst()->getLambdaExpression()+" (";
				condStr+=tmpParas->getParas()->at(2)->getLambdaExpression()+")";
				continue;
			}
			if(ConditionType::isJoinCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				CompoundLambda* tmpCond=(CompoundLambda*)(newCondlist->getParas()->at(i));
				LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
				condStr+=tmpParas->getParas()->at(0)->getLambdaExpression()+".";
				condStr+=tmpParas->getParas()->at(1)->getLambdaExpression();
				condStr+=tmpCond->getFirst()->getLambdaExpression();
				condStr+=tmpParas->getParas()->at(2)->getLambdaExpression()+".";
				condStr+=tmpParas->getParas()->at(3)->getLambdaExpression();
				continue;
			}
			if(ConditionType::isTupleEqualityCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				CompoundLambda* tmpCond=(CompoundLambda*)(newCondlist->getParas()->at(i));
				LambdaParas* tmpParas=(LambdaParas*)(tmpCond->getRest());
				condStr+=tmpParas->getParas()->at(0)->getLambdaExpression()+"=";
				condStr+=tmpParas->getParas()->at(1)->getLambdaExpression();
				continue;
			}
			if(ConditionType::isCardinalityCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				condStr+=cardinality2sql(newCondlist->getParas()->at(i),argQuery);
				continue;
			}
			if(ConditionType::isComponentCondition(newCondlist->getParas()->at(i)))
			{
				flag=true;
				condStr+=component2sql(newCondlist->getParas()->at(i),argQuery);
				continue;
			}
		}
		return condStr;
	}
	LambdaExpression* getCondlist()
	{
		if(query->getCompoundLambda()==NULL) return NULL;
		CompoundLambda* tmpQuery=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmpQuery->getRest());
		vector<LambdaExpression*>* condlist=new vector<LambdaExpression*>();
		int size=paras->getParas()->size();
		for(int i=2;i<size;++i)
		{
			condlist->push_back(paras->getParas()->at(i)->deep_copy());
		}
		return new LambdaParas(condlist);
	}
	string superlative2sql(LambdaExpression* sup,LambdaExpression* condlist,LambdaExpression* argQuery)
	{
		CompoundLambda* supExp=(CompoundLambda*)sup;
		string supType=supExp->getFirst()->getLambdaExpression();
		string pred=getXthPara(argQuery,1)->getLambdaExpression();
		string num=getXthPara(sup,0)->getLambdaExpression();
		string attr=getXthPara(sup,1)->getLambdaExpression();
		string var=getXthPara(sup,2)->getLambdaExpression();
		SuperlativeCondition superlativeCondition;
		string whereStr=condlist2sql(removeCond(condlist,&superlativeCondition),argQuery);
		string supStr="";
		supStr+=" (SELECT * \r\nFROM ";
		supStr+=pred + " AS "+var;
		supStr+=" WHERE ";
		supStr+=var+"."+attr;
		supStr+=" IS NOT NULL ";
		if(whereStr!="")
		{
			supStr+="AND \r\n";
			supStr+=whereStr;
			supStr+="\r\n";
		}
		supStr+=" ORDER BY ";
		supStr+=attr;
		if(supType=="TOP")
		{
			supStr+=" DESC ";
		}
		supStr+=" LIMIT ";
		supStr+=num;
		supStr+=")";
		return supStr;
	}
	string getFreeVariable()
	{
		CompoundLambda* tmp=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmp->getRest());
		if(QueryType::isTupleQuery(query))
		{
			return paras->getParas()->at(0)->getLambdaExpression();
		}
		else if(QueryType::isProjectionQuery(query))
		{
			CompoundLambda* first=(CompoundLambda*)(paras->getParas()->at(0));
			return first->getFirst()->getLambdaExpression();
		}
		else return "";
	}
	vector<string>* getQueryExistentialVariables()
	{
		RangeCondition *rangeCondition=new RangeCondition();
		vector<LambdaExpression*>* exVarTuple=getWithinQuery(rangeCondition);
		vector<string>* exVar=new vector<string>();
		exVar->push_back(getFreeVariable());
		int size=exVarTuple->size();
		map<string,bool>* freeVariable=new map<string,bool>();
		for(int i=0;i<size;++i)
		{
			CompoundLambda* tmpTuple=(CompoundLambda*)(exVarTuple->at(i));
			freeVariable->insert(map<string,bool>::value_type(tmpTuple->getRest()->getLambdaExpression(),true));
		}
		map<string,bool>::iterator m1_Iter;
		for (m1_Iter = freeVariable->begin(); m1_Iter != freeVariable->end(); m1_Iter++)
		{
			exVar->push_back(m1_Iter->first);
		}
		delete freeVariable;
		return exVar;
	}
	LambdaExpression* dropParas()
	{
		AtomicParametersCondition atomicParametersCondition;
		CompoundLambda* drop=new CompoundLambda(new LambdaAtom("AND"),removeCond(query,&atomicParametersCondition));
		return drop;
	}
	void getWithinCondlist(LambdaExpression* condlist,LambdaFunction *lFunction,vector<LambdaExpression*>* meetCondlist)
	{
		CompoundLambda* tmpCondlist=(CompoundLambda*)condlist;
		LambdaParas* paras=(LambdaParas*)(tmpCondlist->getRest());
		vector<LambdaExpression*>* condContainer=paras->getParas();

		if(condContainer->empty()) return;
		int size=condContainer->size();
		for(int i=0;i<size;++i)
		{
			if(lFunction->meetFunction(condContainer->at(i)))
			{
				meetCondlist->push_back(condContainer->at(i)->deep_copy());
				continue;
			}
			if(ConditionType::isComponentCondition(condContainer->at(i)))/*component-condition*/
			{
				vector<LambdaExpression*>* tmpParas=new vector<LambdaExpression*>();
				CompoundLambda* tmpCond=(CompoundLambda*)(condContainer->at(i));
				LambdaParas* tmpCondParas=(LambdaParas*)(tmpCond->getRest());
				int tmpSize=tmpCondParas->getParas()->size();
				for(int j=1;j<tmpSize;++j)
				{
					tmpParas->push_back(tmpCondParas->getParas()->at(j)->deep_copy());
				}
				getWithinCondlist(new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(tmpParas)),lFunction,meetCondlist);
				continue;
			}
			if(ConditionType::isCardinalityCondition(condContainer->at(i)))/*cardinality-condition*/
			{
				vector<LambdaExpression*>* tmpParas=new vector<LambdaExpression*>();
				CompoundLambda* tmpCond=(CompoundLambda*)(condContainer->at(i));
				LambdaParas* tmpCondParas=(LambdaParas*)(tmpCond->getRest());
				int tmpSize=tmpCondParas->getParas()->size();
				for(int j=2;j<tmpSize;++j)
				{
					tmpParas->push_back(tmpCondParas->getParas()->at(j)->deep_copy());
				}
				getWithinCondlist(new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(tmpParas)),lFunction,meetCondlist);
				continue;
			}
			if(ConditionType::isAndCondition(condContainer->at(i)))
			{
				getWithinCondlist(condContainer->at(i)->deep_copy(),lFunction,meetCondlist);
				continue;
			}
		}
	}
	vector<LambdaExpression*>* getWithinQuery(LambdaFunction *lFunction)
	{
		CompoundLambda* tmp=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmp->getRest());
		vector<LambdaExpression*>* newParas=new vector<LambdaExpression*>();
		int size=paras->getParas()->size();
		for(int i=2;i<size;++i)
		{
			newParas->push_back(paras->getParas()->at(i)->deep_copy());
		}
		vector<LambdaExpression*>* meetCondlist=new vector<LambdaExpression*>();
		getWithinCondlist(new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(newParas)),lFunction,meetCondlist);
		return meetCondlist;
	}
	bool isOverVariable(LambdaExpression* condition,string var,string attr="")
	{
		if(var=="") return true;
		if(attr==""&&ConditionType::isJoinCondition(condition))
		{
			CompoundLambda* tmpCond=(CompoundLambda*)condition;
			LambdaParas* paras=(LambdaParas*)(tmpCond->getRest());
			if(paras->getParas()->at(0)->getLambdaExpression()==var||
				paras->getParas()->at(2)->getLambdaExpression()==var)
				return true;
			else return false;
		}
		if(ConditionType::isJoinCondition(condition))
		{
			CompoundLambda* tmpCond=(CompoundLambda*)condition;
			LambdaParas* paras=(LambdaParas*)(tmpCond->getRest());
			if((paras->getParas()->at(0)->getLambdaExpression()==var&&
				paras->getParas()->at(1)->getLambdaExpression()==attr)||
				(paras->getParas()->at(2)->getLambdaExpression()==var&&
				paras->getParas()->at(3)->getLambdaExpression()==attr))
				return true;
			else return false;
		}
		if(ConditionType::isSuperlativeCondition(condition))
		{
			CompoundLambda* tmpCond=(CompoundLambda*)condition;
			LambdaParas* paras=(LambdaParas*)(tmpCond->getRest());
			if(paras->getParas()->at(2)->getLambdaExpression()==var)
				return true;
			else return false;
		}
		if(attr=="")
		{
			CompoundLambda* tmpCond=(CompoundLambda*)condition;
			if(tmpCond->getRest()->getLambdaAtom()!=NULL)
				return tmpCond->getRest()->getLambdaExpression()==var;
			LambdaParas* paras=(LambdaParas*)(tmpCond->getRest());
			if(paras->getParas()->at(0)->getLambdaExpression()==var)
				return true;
			else return false;
		}
		CompoundLambda* tmpCond=(CompoundLambda*)condition;
		LambdaParas* paras=(LambdaParas*)(tmpCond->getRest());
		if(paras->getParas()->at(0)->getLambdaExpression()==var||
			paras->getParas()->at(1)->getLambdaExpression()==attr)
			return true;
		else return false;
	}
	void getCondFromList(LambdaExpression* condlist,LambdaFunction *lFunction,vector<LambdaExpression*>* meetCondlist,string var,string attr="")
	{
		if(!ConditionType::isAndCondition(condlist)) return;
		CompoundLambda* tmpQuery=(CompoundLambda*)condlist;
		LambdaParas* paras=(LambdaParas*)(tmpQuery->getRest());
		int size=paras->getParas()->size();
		for(int i=2;i<size;++i)
		{
			if(!lFunction->meetFunction(paras->getParas()->at(i))) continue;
			if(isOverVariable(paras->getParas()->at(i),var,attr))
			{
				meetCondlist->push_back(paras->getParas()->at(i)->deep_copy());
			}
		}
	}
	string getQueryVariableRelation(string var)
	{
		if(query->getCompoundLambda()==NULL) return "";
		CompoundLambda* tmpQuery=(CompoundLambda*)query;
		LambdaParas* paras=(LambdaParas*)(tmpQuery->getRest());
		if(getFreeVariable()==var)
		{
			return paras->getParas()->at(1)->getLambdaExpression();
		}
		RangeCondition rangeCondition;
		vector<LambdaExpression*>* meetCondlist=getWithinQuery(&rangeCondition);
		int size=meetCondlist->size();
		for(int i=0;i<size;++i)
		{
			CompoundLambda* tmpRange=(CompoundLambda*)(meetCondlist->at(i));
			if(tmpRange->getRest()->getLambdaExpression()==var)
				return tmpRange->getFirst()->getLambdaExpression();
		}
		return "";
	}
	string selectClause()
	{
		string selectStr="";
		if(QueryType::isTupleQuery(query)) selectStr="*";
		if(QueryType::isProjectionQuery(query))
		{
			CompoundLambda* tmpQuery=(CompoundLambda*)query;
			LambdaParas* paras=(LambdaParas*)(tmpQuery->getRest());
			CompoundLambda* clause=(CompoundLambda*)(paras->getParas()->at(0));
			selectStr=clause->getRest()->getLambdaExpression();
		}
		/*add other type*/
		vector<LambdaExpression*>* paras=getQueryParameters();
		int size=paras->size();
		if(size==0) return selectStr;
		selectStr="";
		for(int i=0;i<size;++i)
		{
			if(i!=0) selectStr+=",";
			CompoundLambda* tmpPara=(CompoundLambda*)(paras->at(i));
			LambdaParas* lparas=(LambdaParas*)(tmpPara->getRest());
			selectStr+=lparas->getParas()->at(0)->getLambdaExpression();
			selectStr+=".";
			selectStr+=lparas->getParas()->at(1)->getLambdaExpression();
		}
		this->query=dropParas();
		return selectStr;
	}
	string fromClause()
	{
		string fromString="";
		vector<string>* vars=getQueryExistentialVariables();
		int size=vars->size();
		for(int i=0;i<size;++i)
		{
			fromString+=getQueryVariableRelation(vars->at(i))+" AS "+vars->at(i);
			if(i<size-1)
				fromString+=",";
		}
		vector<LambdaExpression*>* meetCondlist=new vector<LambdaExpression*>();
		SuperlativeCondition superlativeCondition;
		getCondFromList(this->query,&superlativeCondition,meetCondlist,getFreeVariable());
		if(meetCondlist->empty()) return fromString;
		else
		{
			fromString="";
			LambdaExpression* sup=meetCondlist->at(0);
			fromString+=superlative2sql(sup,new CompoundLambda(new LambdaAtom("AND"),getCondlist()),this->query);
			fromString+=" AS x";
		}
		return fromString;
	}
	string whereClause()
	{
		string wheStr="";
		WhereCondition whereCondition;
		vector<LambdaExpression*>* whereCondlist=getWithinQuery(&whereCondition);
		NotExists notExists;
		vector<LambdaExpression*>* notExistsCondlist=getWithinQuery(&notExists);
		vector<LambdaExpression*>* condlist=new vector<LambdaExpression*>();
		int size=whereCondlist->size();
		for(int i=0;i<size;++i)
		{
			condlist->push_back(whereCondlist->at(i));
		}
		size=notExistsCondlist->size();
		for(int i=0;i<size;++i)
		{
			condlist->push_back(notExistsCondlist->at(i));
		}
		LambdaExpression* newCondlist=new CompoundLambda(new LambdaAtom("AND"),new LambdaParas(condlist));
		wheStr=condlist2sql(newCondlist,this->query);
		return wheStr;
	}
public:
	SqlDml(LambdaExpression* query)
	{
		this->query=query;
	}
	string query2sql()
	{
		this->query=l2query();
		cout<<this->query->getLambdaExpression()<<endl;
		string qSql="";
		vector<LambdaExpression*>* meetCondlist=new vector<LambdaExpression*>();
		SuperlativeCondition superlativeCondition;
		getCondFromList(this->query,&superlativeCondition,meetCondlist,getFreeVariable());
		qSql+="SELECT ";
		qSql+=selectClause();
		qSql+="\r\n";
		qSql+="FROM ";
		qSql+=fromClause();
		if(!meetCondlist->empty()) return qSql;
		string wheStr=whereClause();
		if(wheStr!="")
		{
			qSql+="\r\n";
			qSql+="WHERE ";
			qSql+=whereClause();
		}
		return qSql;
	}
};
#endif
#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include <sstream>
using namespace std;

class Element
{
private:
	static int id_iterator;
public:
	static string getFreshVar(string var)
	{
		id_iterator++;
		ostringstream ost;
		ost << id_iterator;
		string id_string(ost.str());
		string tmp=var.substr(0,1);
		tmp+=id_string;
		return tmp;
	}
	static bool isSimpleOp(string var)
	{
		return var==">"||var=="<"||
			   var==">="||var=="<="||
			   var=="="||var=="<>"||
			   var=="LIKE"||var=="NOTLIKE";
	}
	static bool isSetOp(string var)
	{
		return var=="IN"||var=="NOTIN";
	}
};
int Element::id_iterator=0;
#endif
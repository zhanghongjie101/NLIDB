#ifndef TAG_H
#define TAG_H

class Tag
{
private:
	string tagType;
	string tagValue;
	string tagSpan;
public:
	Tag(string tagType,string tagValue,string tagSpan)
	{
		this->tagType=tagType;
		this->tagValue=tagValue;
		this->tagSpan=tagSpan;
	}
	string getTagType()
	{
		return this->tagType;
	}
	string getTagValue()
	{
		return this->tagValue;
	}
	string getTagSpan()
	{
		return this->tagSpan;
	}
};
#endif
#ifndef TRANSFORMER_STR2INT_H
#define TRANSFORMER_STR2INT_H

#include "./data_transformer.h"

class StrToIntTransformer : DataTransformer
{
public:
	StrToIntTransformer(const char* attributeName);

	//if a value is not in the words set, then it will be morbed into -1
	virtual void transformField(DataSet& dataSet);
};

#endif
#ifndef TRANSFORMER_STR2FLOAT_H
#define TRANSFORMER_STR2FLOAT_H

#include "data_transformer.h"

class StrToFloatTransformer : DataTransformer
{
public:
	StrToFloatTransformer(const char* attributeName);

	//if a value is not in the words set, then it will be morbed into NaN
	virtual void transformField(DataSet& dataSet);
};

#endif
#ifndef TRANSFORMER_INTERVAL_H
#define TRANSFORMER_INTERVAL_H

#include "./data_transformer.h"
#include <vector>


//divides the dataset int intervalCount different subsets where each subset has the same number of rows
class IntervalTransformer : DataTransformer
{
private:
	int intervalCount;
public:
	IntervalTransformer(const char* attributeName, int intervalCount);

	//if a value is not in the words set, then it will be morbed into -1
	virtual void transformField(DataSet& dataSet);
};

#endif
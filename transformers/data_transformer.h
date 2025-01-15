#ifndef DATA_TRANSFORMER_H
#define DATA_TRANSFORMER_H

#include "../data_set.h"

class DataTransformer {
protected:
	char* attributeName;
public:
	DataTransformer(const char* attributeName);
	~DataTransformer();

	virtual void transformField(DataSet& dataSet) = 0;
};

#endif
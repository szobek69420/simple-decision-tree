#ifndef DATA_SET_H
#define DATA_SET_H

#include <vector>
#include "value_types/value_type.h"

class DataTransformer;

class DataSet {
private:
	static const int MAX_FILE_LINE_LENGTH = 10000;

	char* csvFilePath;
	std::vector<DataTransformer*> dataTransformers;
	std::vector<const char*> excludedAttributes;
	float testRatio;
	bool alreadyProcessed;

public:
	char* targetAttribute;
	std::vector<char*> attributeNames;
	std::vector<int> attributeValues; //how many different values does an attribute have
	std::vector<ValueTypeBase**> trainRows;
	std::vector<int> trainRowLabels;//what is the label of the row (a boolean value, 0 or 1)
	std::vector<ValueTypeBase**> testRows;
	std::vector<int> testRowLabels;

	std::vector<std::vector<ValueTypeBase*>> unprocessedRows; //the rows before they are splitted into train and test set (they contain the labels as well)


	DataSet(const char* csvFilePath, const char* targetAttribute, float testRatio, const std::vector<DataTransformer*>& dataTransformers, const std::vector<const char*> excludedAttributes);
	~DataSet();

	void processData();
};

#endif
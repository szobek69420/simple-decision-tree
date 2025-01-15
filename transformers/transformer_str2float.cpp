#define _CRT_SECURE_NO_WARNINGS

#include "transformer_str2float.h"


#include <vector>
#include <string.h>

StrToFloatTransformer::StrToFloatTransformer(const char* attributeName) :DataTransformer(attributeName) {}

void StrToFloatTransformer::transformField(DataSet& dataSet)
{
	//get the column index of the attribute
	int columnIndex = -1;
	for (int i = 0; i < dataSet.attributeNames.size(); i++)
	{
		if (strcmp(this->attributeName, dataSet.attributeNames[i]) == 0)
		{
			columnIndex = i;
			break;
		}
	}

	if (columnIndex == -1)
	{
		fprintf(stderr, "StrToFloatTransformer::transformField: there is no such field as \"%s\"\n", this->attributeName);
		return;
	}

	//transform the attributes
	for (int i = 0; i < dataSet.unprocessedRows.size(); i++)
	{
		unsigned int value = 0x7FFFFFFF;
		sscanf(((ValueTypeString*)dataSet.unprocessedRows[i][columnIndex])->value, "%f", &value);

		delete dataSet.unprocessedRows[i][columnIndex];
		dataSet.unprocessedRows[i][columnIndex] = (ValueTypeBase*)new ValueTypeFloat(*(float*)&value);
	}
}
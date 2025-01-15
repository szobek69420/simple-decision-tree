#include "transformer_interval.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

class SortHelper {
public:
	int index;
	ValueTypeBase* value;

	SortHelper() :index(-1), value(nullptr) {}
	SortHelper(int index, ValueTypeBase* value) : index(index), value(value) {}

	static int compare(const void* a, const void* b)
	{
		if (*(((SortHelper*)a)->value) == *(((SortHelper*)b)->value))
			return 0;
		if (*(((SortHelper*)a)->value) > *(((SortHelper*)b)->value))
			return 69;
		if (*(((SortHelper*)a)->value) < *(((SortHelper*)b)->value))
			return -69;
	}
};

IntervalTransformer::IntervalTransformer(const char* attributeName, int intervalCount) : DataTransformer(attributeName), intervalCount(intervalCount) {}

void IntervalTransformer::transformField(DataSet& dataSet)
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
		fprintf(stderr, "IntervalTransformer::transformField: there is no such field as \"%s\"\n", this->attributeName);
		return;
	}

	//get the bounds of the intervals
	SortHelper* values = new SortHelper[dataSet.unprocessedRows.size()];

	for (int i = 0; i < dataSet.unprocessedRows.size(); i++)
		values[i] = SortHelper(i, dataSet.unprocessedRows[i][columnIndex]);

	qsort(values, dataSet.unprocessedRows.size(), sizeof(SortHelper), SortHelper::compare);

	//transform cells
	for (int i = 0; i < this->intervalCount; i++)
	{
		for (int j = i * dataSet.unprocessedRows.size() / this->intervalCount; j < (i + 1) * dataSet.unprocessedRows.size() / this->intervalCount; j++)
		{
			delete dataSet.unprocessedRows[values[j].index][columnIndex];
			dataSet.unprocessedRows[values[j].index][columnIndex] = (ValueTypeBase*)new ValueTypeInt(i);
		}
	}

	//morb the attribute values
	dataSet.attributeValues[columnIndex] = this->intervalCount;

	delete[] values;
}
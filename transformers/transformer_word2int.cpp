#define _CRT_SECURE_NO_WARNINGS

#include "transformer_word2int.h"
#include "../value_types/value_type.h"

#include <vector>
#include <string.h>
#include <stdio.h>

WordToIntTransformer::WordToIntTransformer(const char* attributeName, const char** possibleWords, int wordCount) : DataTransformer(attributeName)
{
	this->words = std::vector<char*>();

	for (int i = 0; i < wordCount; i++)
	{
		char* word = new char[strlen(possibleWords[i]) + 1];
		strcpy(word, possibleWords[i]);
		this->words.push_back(word);
	}
}

WordToIntTransformer::~WordToIntTransformer()
{
	for (int i = 0; i < this->words.size(); i++)
		delete[] this->words[i];
}

void WordToIntTransformer::transformField(DataSet& dataSet)
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
		fprintf(stderr, "WordToIntTransformer::transformField: there is no such field as \"%s\"\n", this->attributeName);
		return;
	}

	//transform the attributes
	for (int i = 0; i < dataSet.unprocessedRows.size(); i++)
	{
		int newValue = -1;
		for (int j = 0; j < this->words.size(); j++)
		{
			if (strcmp(((ValueTypeString*)dataSet.unprocessedRows[i][columnIndex])->value, this->words[j]) == 0)
			{
				newValue = j;
				break;
			}
		}

		delete dataSet.unprocessedRows[i][columnIndex];
		dataSet.unprocessedRows[i][columnIndex] = (ValueTypeBase*)new ValueTypeInt(newValue);
	}

	//change the attribute value
	dataSet.attributeValues[columnIndex] = this->words.size();
}
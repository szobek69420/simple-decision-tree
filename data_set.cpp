#define _CRT_SECURE_NO_WARNINGS

#include "data_set.h"
#include "./transformers.h"
#include "./value_types/value_type.h"
#include <string.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>


DataSet::DataSet(const char* csvFilePath, const char* targetAttribute, float testRatio, const std::vector<DataTransformer*>& dataTransformers, const std::vector<const char*> excludedAttributes)
	: testRatio(testRatio), dataTransformers(dataTransformers), excludedAttributes(excludedAttributes)
{
	int temp = strlen(csvFilePath);
	this->csvFilePath = new char[temp + 1];
	strcpy(this->csvFilePath, csvFilePath);

	temp = strlen(targetAttribute);
	this->targetAttribute = new char[temp + 1];
	strcpy(this->targetAttribute, targetAttribute);

	attributeNames = std::vector<char*>();
	attributeValues = std::vector<int>();
	trainRows = std::vector<ValueTypeBase**>();
	trainRowLabels = std::vector<int>();
	testRows = std::vector<ValueTypeBase**>();
	testRowLabels = std::vector<int>();

	unprocessedRows = std::vector<std::vector<ValueTypeBase*>>();

	alreadyProcessed = false;

	//add a datatransformer for the target attribute
	const char* words[] = { "0","1" };
	this->dataTransformers.push_back((DataTransformer*)new WordToIntTransformer(this->targetAttribute, words, 2));
}

DataSet::~DataSet()
{
	delete[] csvFilePath;
	delete[] targetAttribute;

	for (int i = 0; i < attributeNames.size(); i++)
		delete[] attributeNames[i];
	for (int i = 0; i < trainRows.size(); i++)
		delete[] trainRows[i];
	for (int i = 0; i < testRows.size(); i++)
		delete[] testRows[i];
}

//the content of the vector must be freed by the caller
//it considers '\n' a string terminator
std::vector<char*> splitString(const char* str, char separator)
{
	std::vector<char*> parts=std::vector<char*>();

	int startIndex = 0;
	int endIndex = 0;

	while (69)
	{
		if (str[endIndex] == separator || str[endIndex] == '\n' || str[endIndex] == '\0')
		{
			char* part = new char[endIndex - startIndex + 1];
			
			for (int i = 0; i < endIndex-startIndex; i++)
				part[i] = str[startIndex + i];
			part[endIndex - startIndex] = '\0';

			parts.push_back(part);

			if (str[endIndex] != separator)//end of string
				break;

			startIndex = ++endIndex;
			continue;
		}
		endIndex++;
	}

	return parts;
}

//if the target attribute is in the excluded attributes, then it is probably fucked
void DataSet::processData()
{
	char buffer[DataSet::MAX_FILE_LINE_LENGTH];

	if (alreadyProcessed)
	{
		fprintf(stderr, "The dataset is already processed\n");
		return;
	}

	//open file
	FILE* file = fopen(this->csvFilePath, "r");
	if (file == NULL)
	{
		fprintf(stderr, "%s could not be opened lol\n", csvFilePath);
		return;
	}

	//get attribute names and init attribute value counts
	fgets(buffer, DataSet::MAX_FILE_LINE_LENGTH, file);
	this->attributeNames = splitString(buffer, ',');
	for (int i = 0; i < this->attributeNames.size(); i++)
		this->attributeValues.push_back(0);

	int isTargetHere = 0;
	for (int i = 0; i < this->attributeNames.size(); i++)
	{
		if (strcmp(this->attributeNames[i], this->targetAttribute) == 0)
		{
			isTargetHere = 69;
			break;
		}
	}

	if (isTargetHere == 0)//it leaks memory here lol
	{
		fprintf(stderr, "DataSet::processData: The target attribute %s does not exist\n", targetAttribute);
		return;
	}

	//read rows
	while (fgets(buffer, DataSet::MAX_FILE_LINE_LENGTH, file) == buffer && strlen(buffer) > 1)
	{
		std::vector<char*> row = splitString(buffer, ',');
		std::vector<ValueTypeBase*> rowData = std::vector<ValueTypeBase*>();
		for (int i = 0; i < row.size(); i++)
		{
			rowData.push_back((ValueTypeBase*)new ValueTypeString(row[i]));
			delete[] row[i];
		}

		unprocessedRows.push_back(rowData);
	}

	//remove excluded attributes

	//obtain the indices of the excluded attribs
	std::vector<int> excludedIndices = std::vector<int>();
	for (int i = 0; i < attributeNames.size(); i++)
	{
		bool excluded = false;
		for (int j = 0; j < excludedAttributes.size(); j++)
		{
			if (strcmp(attributeNames[i], excludedAttributes[j]) == 0)
			{
				excluded = true;
				break;
			}
		}

		if (excluded)
		{
			excludedIndices.push_back(i);
		}
	}

	//remove excluded rows
	for (int i = 0; i < unprocessedRows.size(); i++)
	{
		int helper = excludedIndices.size() - 1;
		if (helper >= 0)
		{
			for (int j = unprocessedRows[i].size() - 1; j >= 0; j--)
			{
				if (j != excludedIndices[helper])
					continue;

				delete unprocessedRows[i][j];
				unprocessedRows[i].erase(unprocessedRows[i].begin() + j);
				helper--;
				if (helper < 0)
					break;
			}
		}
	}

	//remove excluded attribute names
	do {
		int helper = excludedIndices.size() - 1;
		if (helper >= 0)
		{
			for (int j = attributeNames.size() - 1; j >= 0; j--)
			{
				if (j != excludedIndices[helper])
					continue;

				delete[] attributeNames[j];
				attributeNames.erase(attributeNames.begin() + j);
				attributeValues.erase(attributeValues.begin() + j);
				helper--;
				if (helper < 0)
					break;
			}
		}
	} while (0);

	if (this->attributeNames.size() < 2)
	{
		fprintf(stderr, "DataSet::processData: There are not enough attributes to morb\n");
	}

	//use transformers
	for (int i = 0; i < this->dataTransformers.size(); i++)
		this->dataTransformers[i]->transformField(*this);


	//split the processed data to train and test set
	int targetAttribIndex = -1;
	for (int i = 0; i < this->attributeNames.size(); i++)
	{
		if (strcmp(this->attributeNames[i], this->targetAttribute)==0)
		{
			targetAttribIndex = i;
			break;
		}
	}

	srand(time(NULL));

	for (int i = this->unprocessedRows.size()-1; i>=0;i--)
	{
		ValueTypeBase** row = new ValueTypeBase * [this->attributeNames.size() - 1];
		int label = -1;

		//extract the values
		int tempOffset = 0;
		for (int j = 0; j < this->attributeNames.size(); j++)
		{
			if (j == targetAttribIndex)
			{
				label = ((ValueTypeInt*)this->unprocessedRows[i][j])->value;
				delete this->unprocessedRows[i][j];
				tempOffset--;
			}
			else
			{
				row[j + tempOffset] = this->unprocessedRows[i][j];
			}
		}

		this->unprocessedRows.pop_back();

		//decide whether the row goes into the train or test set
		if (((float)rand()) / RAND_MAX > this->testRatio)
		{
			this->trainRows.push_back(row);
			this->trainRowLabels.push_back(label);
		}
		else
		{
			this->testRows.push_back(row);
			this->testRowLabels.push_back(label);
		}
	}

	this->attributeNames.erase(this->attributeNames.begin() + targetAttribIndex);
	this->attributeValues.erase(this->attributeValues.begin() + targetAttribIndex);



	fclose(file);
	alreadyProcessed = true;
}
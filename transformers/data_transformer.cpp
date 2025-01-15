#define _CRT_SECURE_NO_WARNINGS

#include "data_transformer.h"
#include <string.h>


DataTransformer::DataTransformer(const char* attributeName)
{
	int nameLength = strlen(attributeName);
	this->attributeName = new char[nameLength + 1];
	strcpy(this->attributeName, attributeName);
}

DataTransformer::~DataTransformer()
{
	delete[] this->attributeName;
}
#ifndef TRANSFORMER_WORD2INT_H
#define TRANSFORMER_WORD2INT_H

#include "./data_transformer.h"
#include <vector>

class WordToIntTransformer : DataTransformer
{
private:
	std::vector<char*> words;
public:
	WordToIntTransformer(const char* attributeName, const char** possibleWords, int wordCount);
	~WordToIntTransformer();

	//if a value is not in the words set, then it will be morbed into -1
	virtual void transformField(DataSet& dataSet);
};

#endif
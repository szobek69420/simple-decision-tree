#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <vector>

#include "data_set.h"
#include "./transformers.h"
#include "./decision_tree/decision_tree.h"

int main()
{
	std::vector<DataTransformer*> transformers = std::vector<DataTransformer*>();
	const char* arr[] = {"Male", "Female"};
	transformers.push_back((DataTransformer*)new WordToIntTransformer("Gender", arr, 2));
	transformers.push_back((DataTransformer*)new StrToIntTransformer("Age"));
	transformers.push_back((DataTransformer*)new IntervalTransformer("Age", 4));
	transformers.push_back((DataTransformer*)new IntervalTransformer("EstimatedSalary", 5));

	std::vector<const char*> excludedAttributes = std::vector<const char*>();
	excludedAttributes.push_back("User ID");

	DataSet data = DataSet(
		"./ads.csv", 
		"Purchased", 
		0.2, 
		transformers, 
		excludedAttributes
	);

	data.processData();


	DecisionTree* tree=DecisionTreeConstructor::construct(data);

	printf("Predicted, Real\n");
	for (int i = 0; i < data.testRows.size(); i++)
	{
		int result = tree->predict(data, i);
		printf("%d, %d\n", result, data.testRowLabels[i]);
	}

	delete tree;

	return 0;
}
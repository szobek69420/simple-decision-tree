#define _CRT_SECURE_NO_WARNINGS

#include "./decision_tree.h"
#include "../value_types/value_type.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DecisionTree::DecisionTree(DecisionTreeNode* root) : root(root) {}
DecisionTree::~DecisionTree() { delete root; }

double DecisionTreeConstructor::MIN_GAIN = 0.01;

struct SortHelper {
	int index;
	ValueTypeBase* value;

	SortHelper() : index(-1), value(nullptr) {}
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



class DecisionTreeNode
{
public:
	char* attributeName; //if it is a leaf node, then nullptr
	int decisionIfLeaf;
	std::vector<ValueTypeBase*> values;
	std::vector<DecisionTreeNode*> children;

public:
	DecisionTreeNode() :attributeName(nullptr), decisionIfLeaf(0), values(std::vector<ValueTypeBase*>()), children(std::vector<DecisionTreeNode*>()) {}
	~DecisionTreeNode()
	{
		if (attributeName != nullptr)
		{
			delete[] attributeName;
			for (int i = 0; i < values.size(); i++)
			{
				delete values[i];
				delete children[i];
			}
		}
	}

	int predict(const DataSet& dataSet, int testRowIndex)
	{
		if (this->attributeName == nullptr)//leaf node
			return this->decisionIfLeaf;

		int attributeIndex = -1;
		for (int i = 0; i < dataSet.attributeNames.size(); i++)
		{
			if (strcmp(this->attributeName, dataSet.attributeNames[i]) == 0)
			{
				attributeIndex = i;
				break;
			}
		}
		if (attributeIndex == -1)//the attribute is not in the data set, error
			return -1;

		for (int i = 0; i < this->values.size(); i++)
		{
			if (*dataSet.testRows[testRowIndex][attributeIndex] == *this->values[i])
				return this->children[i]->predict(dataSet, testRowIndex);
		}

		return -1;//if it reached this line, then that means that there is an error
	}
};


int DecisionTree::predict(const DataSet& dataSet, int testRowIndex)
{
	return root->predict(dataSet, testRowIndex);
}


struct DecisionTreeConstructorNode {
	char* attributeName;//if it is nullptr, then it is a leaf node
	int decisionIfLeaf;
	std::vector<ValueTypeBase*> attributeValues;//the possible values the attribute can have
	std::vector<std::vector<int>> indices;//the indices in the data set, grouped by the attribute values
	std::vector<DecisionTreeConstructorNode*> children;//obviously as many children as different attribute values

	DecisionTreeConstructorNode() : attributeName(nullptr), decisionIfLeaf(0), attributeValues(std::vector<ValueTypeBase*>()), indices(std::vector<std::vector<int>>()) {}

	~DecisionTreeConstructorNode()
	{
		if (attributeName != nullptr)//not a leaf node
		{
			delete[] attributeName;
			for (int i = 0; i < attributeValues.size(); i++)
				delete attributeValues[i];
			for (int i = 0; i < children.size(); i++)
				delete children[i];
		}
	}

	void morb(const DataSet& dataSet, const std::vector<int>& indicesToMorb)
	{
		static int depth = 0;
		//calculate base entropy
		double entropy = 0;
		int count1 = 0;

		for (int i = 0; i < indicesToMorb.size(); i++)
		{
			if (dataSet.trainRowLabels[indicesToMorb[i]] == 1)
				count1++;
		}

		if (count1 == 0 || count1 == indicesToMorb.size())//definitely leaf node
		{
			attributeName = nullptr;
			decisionIfLeaf =count1<indicesToMorb.size()-count1 ? 0 : 1;
			return;
		}

		entropy = -((double)count1) / indicesToMorb.size() * log2(((double)count1) / indicesToMorb.size()) - ((double)(indicesToMorb.size() - count1)) / indicesToMorb.size() * log2(((double)(indicesToMorb.size() - count1)) / indicesToMorb.size());

		//search for the attribute with the max gain
		double maxGain = -1;
		int maxGainAttributeIndex = -1;
		std::vector<ValueTypeBase*> maxGainAttributeValues;
		std::vector<std::vector<int>> maxGainIndices;

		for (int i = 0; i < dataSet.attributeNames.size(); i++)
		{
			std::vector<ValueTypeBase*> attributeValues;
			std::vector<std::vector<int>> indices;

			//sort the values so that they are in a managable state
			SortHelper* sorted = new SortHelper[indicesToMorb.size()];
			for (int j = 0; j < indicesToMorb.size(); j++)
				sorted[j] = SortHelper(indicesToMorb[j], dataSet.trainRows[indicesToMorb[j]][i]);
			qsort(sorted, indicesToMorb.size(), sizeof(SortHelper), SortHelper::compare);

			//fill up attributeValues and indices
			for (int j = 0; j < indicesToMorb.size(); j++)
			{
				if (attributeValues.size() == 0 || !(*attributeValues[attributeValues.size() - 1] == *(sorted[j].value)))
				{
					attributeValues.push_back(sorted[j].value->clone());
					indices.push_back(std::vector<int>());
				}

				indices[indices.size() - 1].push_back(sorted[j].index);
			}

			delete[] sorted;

			//calculate entropy
			double newEntropy = 0.0;
			for (int j = 0; j < attributeValues.size(); j++)
			{
				int tempCount = 0;
				for (int k = 0; k < indices[j].size(); k++)
				{
					if (dataSet.trainRowLabels[indices[j][k]] == 1)
						tempCount++;
				}

				if (tempCount == 0 || tempCount == indices[j].size()) //here the contributed entropy is zero
					continue;

				newEntropy += (((double)indices[j].size())/indicesToMorb.size()) *
					(-((double)tempCount) / indices[j].size() * log2(((double)tempCount) / indices[j].size())
						- ((double)(indices[j].size() - tempCount)) / indices[j].size() * log2(((double)(indices[j].size() - tempCount)) / indices[j].size()));
			}

			//check if the attribute is better than the ones before
			if (maxGainAttributeIndex == -1)
			{
				maxGain = entropy - newEntropy;
				maxGainAttributeIndex = i;
				maxGainAttributeValues = attributeValues;
				maxGainIndices = indices;
			}
			else if (maxGain < entropy - newEntropy)
			{
				for (int j = 0; j < maxGainAttributeValues.size(); j++)
					delete maxGainAttributeValues[j];

	
				maxGain = entropy - newEntropy;
				maxGainAttributeIndex = i;
				maxGainAttributeValues = attributeValues;
				maxGainIndices = indices;
			}
			else
			{
				for (int j = 0; j < attributeValues.size(); j++)
					delete attributeValues[j];
			}
		}

		//is any of the attributes any good?
		if (maxGainAttributeIndex == -1)
		{
			attributeName = nullptr;
			decisionIfLeaf = count1 < indicesToMorb.size() - count1 ? 0 : 1;
			return;
		}
		if (maxGain<DecisionTreeConstructor::MIN_GAIN)
		{
			for (int j = 0; j < maxGainAttributeValues.size(); j++)
				delete maxGainAttributeValues[j];

			attributeName = nullptr;
			decisionIfLeaf = count1 < indicesToMorb.size() - count1 ? 0 : 1;
			return;
		}

		//we have a good attribute
		this->attributeName = new char[strlen(dataSet.attributeNames[maxGainAttributeIndex]) + 1];
		strcpy(this->attributeName, dataSet.attributeNames[maxGainAttributeIndex]);

		this->attributeValues = maxGainAttributeValues;
		this->indices = maxGainIndices;


		//morb children as well
		for (int i = 0; i < this->attributeValues.size(); i++)
		{
			this->children.push_back(new DecisionTreeConstructorNode());
			this->children[i]->morb(dataSet, this->indices[i]);
		}
	}
};


void constructHelper(DecisionTreeNode* realNode, DecisionTreeConstructorNode* tempNode)
{
	if (tempNode->attributeName == nullptr)//leaf node
	{
		realNode->attributeName = nullptr;
		realNode->decisionIfLeaf = tempNode->decisionIfLeaf;
		return;
	}

	realNode->attributeName = new char[strlen(tempNode->attributeName) + 1];
	strcpy(realNode->attributeName, tempNode->attributeName);

	for (int i = 0; i < tempNode->attributeValues.size(); i++)
	{
		realNode->values.push_back(tempNode->attributeValues[i]->clone());
		realNode->children.push_back(new DecisionTreeNode());
		constructHelper(realNode->children[i], tempNode->children[i]);
	}
}

DecisionTree* DecisionTreeConstructor::construct(const DataSet& dataSet)
{
	DecisionTreeConstructorNode* root = new DecisionTreeConstructorNode();
	
	//contruct the temporary structure
	std::vector<int> indices;
	for (int i = 0; i < dataSet.trainRows.size(); i++)
		indices.push_back(i);

	root->morb(dataSet, indices);

	//construct the final decision tree structure
	DecisionTreeNode* realRoot = new DecisionTreeNode();
	constructHelper(realRoot, root);

	delete root;
	return new DecisionTree(realRoot);
}
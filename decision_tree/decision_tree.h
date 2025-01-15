#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "../data_set.h"

class DecisionTreeNode;

class DecisionTree {
private:
	DecisionTreeNode* root;

public:
	DecisionTree(DecisionTreeNode* root);
	~DecisionTree();
	int predict(const DataSet& dataSet, int rowIndex);
};

static class DecisionTreeConstructor {
public:
	static double MIN_GAIN; //if there is no attribute that would result in more gain than this then the building of the tree will stop on the specific branch
	static DecisionTree* construct(const DataSet& dataSet);
};

#endif
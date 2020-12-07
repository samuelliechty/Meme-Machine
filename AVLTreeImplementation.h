// Author : Shravya Iruvanti
// Date : 12/05/2020

#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

class MemeLabel
{
public:
	string image_url;
	string caption_text;
	MemeLabel() {};
	MemeLabel(string c_text, string i_url) {
		caption_text = c_text;
		image_url = i_url;
	};
};

class TreeNode
{
public:
	MemeLabel data;
	TreeNode* right;
	TreeNode* left;
	int tree_height;

	TreeNode(MemeLabel x)
	{
		data = x;
		right = NULL;
		left = NULL;
		tree_height = 0;
	}
};

class MemesDataTree
{
public:
	TreeNode* root;
private:
	//Left Rotation function
	//move every node one position to the left from current position
	void left_rotate(TreeNode*& node) {
		bool is_root = false;
		if (node == root)
			is_root = true;
		TreeNode* a_node;
		TreeNode* b_node;
		TreeNode* b_left_node;

		a_node = node;
		b_node = node->right;
		if (b_node != NULL) {
			b_left_node = b_node->left;

			node = b_node;
			b_node->left = a_node;
			a_node->right = b_left_node;

			//update heights after rotating
			//this is used later to get the height of the respective node 
			a_node->tree_height = 1 + max(height(a_node->left), height(a_node->right));
			b_node->tree_height = 1 + max(height(b_node->left), height(b_node->right));
		}
		if (is_root)
			root = b_node;
	};

	// Right rotation
	// move every node one position to the right from current position
	void right_rotate(TreeNode*& node) {
		bool is_root = false;
		if (node == root)
			is_root = true;
		TreeNode* a_node;
		TreeNode* b_node;
		TreeNode* b_right_node;

		a_node = node;
		b_node = node->left;
		if (b_node != NULL) {
			b_right_node = b_node->right;

			node = b_node;
			b_node->right = a_node;
			a_node->left = b_right_node;

			//update heights after rotating
			a_node->tree_height = 1 + max(height(a_node->left), height(a_node->right));
			b_node->tree_height = 1 + max(height(b_node->left), height(b_node->right));
		}
		if (is_root)
			root = b_node;
	};

	// Right Left Rotation
	// Right Left roation is a combination of one right rotation followed by one left rotation.
	void rightLeftRotation(TreeNode*& node)
	{
		right_rotate(node->right);
		left_rotate(node);
	}

	// Left Right Rotation
	// Left right roation is a combination of one left rotation followed by one right rotation.
	void leftRightRotation(TreeNode*& node)
	{
		left_rotate(node->left);
		right_rotate(node);
	}

	// return the height of the given tree node
	int height(TreeNode*& node) {
		if (node == NULL) {
			return -1;
		}
		else {
			return node->tree_height;
		}
	}

	// search function to find the node by image_url key
	// returns TreeNode node 
	void search_by_key_contains(TreeNode* root, vector<pair<string, string>>& matches, string key) {
		if (root != NULL) {
			if (root->data.image_url.find(key) != std::string::npos)
				matches.push_back(std::make_pair(root->data.image_url, root->data.caption_text));
			if (root->left)
				search_by_key_contains(root->left, matches, key);
			if (root->right)
				search_by_key_contains(root->right, matches, key);
		}
	};

	TreeNode* search_by_key(TreeNode* root, string image_url) {
		if (root == NULL)
		{
			return root;
		}
		else if (image_url < root->data.image_url)
		{
			return search_by_key(root->left, image_url);
		}
		else if (image_url > root->data.image_url)
		{
			return search_by_key(root->right, image_url);
		}
		else {
			return root;
		}
	};

	// search function to find the node by value
	// pushes all matching names to vector 
	void search_by_value(TreeNode* tree, vector<pair<string, string>>& matches, string value) {
		if (tree != NULL)
		{
			if (tree->data.caption_text.find(value) != std::string::npos) {
				matches.push_back(std::make_pair(tree->data.image_url, tree->data.caption_text));
			}
			if (tree->left) {
				search_by_value(tree->left, matches, value);
			}
			if (tree->right) {
				search_by_value(tree->right, matches, value);
			}
		}
	};

	// function to insert new Node to root
	// returns updated root node
	TreeNode* insert_node(string image_url, string caption_text, TreeNode* root) {
		if (root == NULL) {
			MemeLabel ml = MemeLabel(caption_text, image_url);
			root = new TreeNode(ml);
		}
		else {
			// decide which side to go in the tree
			if (image_url < root->data.image_url) {
				root->left = insert_node(image_url, caption_text, root->left);
				// check if the tree is balanced or not and rotate if necessary
				if (height(root->left) > height(root->right) + 1) {
					if (height(root->left->left) > height(root->left->right)) {
						right_rotate(root);
					}
					else {
						left_rotate(root);
					}
				}
			}
			else {
				root->right = insert_node(image_url, caption_text, root->right);
				// check if the tree is balanced or not and rotate if necessary
				if (height(root->right) > height(root->left) + 1) {
					if (height(root->right->right) > height(root->right->left)) {
						left_rotate(root);
					}
					else {
						right_rotate(root);
					}
				}
			}
		}
		//update TreeNode height
		int right_height = height(root->right);
		int leftheight = height(root->left);
		if (right_height > leftheight) {
			root->tree_height = 1 + right_height;
		}
		else {
			root->tree_height = 1 + leftheight;
		}
		return root;
	}

	// return balance factor of given node
	int get_balance_factor(TreeNode*& node) {
		if (root == NULL) {
			return 0;
		}
		else {
			return height(node->left) - height(node->right);
		}
	}

	// function to balance the tree based on balance factor of each node
	void balanceTree(TreeNode*& root) {

		int bf = get_balance_factor(root);

		if (bf > 1 && get_balance_factor(root->left) >= 0)
		{
			right_rotate(root);
		}
		if (bf > 1 && get_balance_factor(root->left) < 0)
		{
			left_rotate(root->left);
			right_rotate(root);
		}
		if (bf < -1 && get_balance_factor(root->right) <= 0)
		{
			left_rotate(root);
		}
		if (bf < -1 && get_balance_factor(root->right) > 0) {
			right_rotate(root->right);
			left_rotate(root);
		}
	};

	void printBST(TreeNode* root) {
		if (root != NULL) {
			if (root->left)
				printBST(root->left);
			cout << root->data.image_url << " : " << root->data.caption_text << endl;
			if (root->right)
				printBST(root->right);
		}
	}

public:

	MemesDataTree() {
		root = NULL;
	};

	// helper function to insert the new node into tree and return true if the insert is successful and false for failure
	void insert(string image_url, string caption_text) {
		if (root == NULL) {
			root = insert_node(image_url, caption_text, root);
		}
		else {
			TreeNode* nd = search_by_key(root, image_url);
			if (nd == NULL) {
				root = insert_node(image_url, caption_text, root);
			}
		}
	};

	//helper function to search the Tree by given uf_id
	vector<pair<string, string>> searchKeys(string keyString) {
		vector<pair<string, string>> matches;
		search_by_key_contains(root, matches, keyString);
		return matches;
	};

	//helper function to search the Tree by given caption_text string
	vector<pair<string, string>> searchValues(string value) {
		vector<pair<string, string>> matches;
		search_by_value(root, matches, value);
		return matches;
	};

	TreeNode* KeySearch(string imageURL) {
		return search_by_key(this->root, imageURL);
	}

	void printTree() {
		printBST(root);
	}
};

void ReadData(MemesDataTree& memesData, string fileName) {
	ifstream file(fileName);
	if (!file.is_open()) throw runtime_error("Could not open file");
	string s;
	// Using TSV lines as command line input 
	while (getline(file, s, '\n'))
	{
		istringstream ss(s);
		vector <string> record;
		int index = 0;
		string type, imageUrl, captionText;

		while (ss)
		{
			string s;
			if (!getline(ss, s, '\t')) break;
			if (index == 0)
				captionText = s;
			else if (index == 1)
				imageUrl = s;
			else if (index == 2)
				type = s;
			index++;
		}
		memesData.insert(imageUrl, captionText);
	}
}
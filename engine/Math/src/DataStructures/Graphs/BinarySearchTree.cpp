
#include "BinarySearchTree.h"

namespace mathem
{
	template <typename T_key, typename T_data>
	BinarySearchTree<T_key, T_data>::BinarySearchTree(T_key key, T_data data)
	{
		BinarySearchTree<T_key, T_data>* tree = new BinarySearchTree<T_key, T_data>;
		tree->parent = nullptr;
		tree->branchLeft = nullptr;
		tree->branchRight = nullptr;

		tree->amOfOffsprings = 0;

		tree->key = key;
		tree->data = data;

		return tree;
	}

	template <typename T_key, typename T_data>
	BinarySearchTree<T_key, T_data>::~BinarySearchTree()
	{
		deleteCounter++;
	}

	template <typename T_key, typename T_data>
	void BinarySearchTree<T_key, T_data>::insert(BinarySearchTree<T_key, T_data>* branchNode)
	{
		if (branchNode == nullptr) 
		{ 
			return;
		}

		branchNode->parent = this;

		if (branchNode->key < this->key)
		{
			if (this->branchLeft == nullptr)
				this->branchLeft = branchNode;
			else
				insert<T_key, T_data>(this->branchLeft, branchNode);
		}
		else
		{
			if (this->branchRight == nullptr)
				this->branchRight = branchNode;
			else
				insert<T_key, T_data>(this->branchRight, branchNode);
		}

		this->amOfOffsprings++;
	}

	template <typename T_key, typename T_data>
	void BinarySearchTree<T_key, T_data>::insert(T_key key, T_data data)
	{
		BinarySearchTree<T_key, T_data>* branchNode = new BinarySearchTree<T_key, T_data>;
		branchNode->branchLeft = nullptr;
		branchNode->branchRight = nullptr;
		branchNode->parent = this;
		branchNode->amOfOffsprings = 0;
		branchNode->key = key;
		branchNode->data = data;

		if (branchNode->key < this->key)
		{
			if (this->branchLeft == nullptr)
				this->branchLeft = branchNode;
			else
			{
				delete branchNode;
				insert<T_key, T_data>(this->branchLeft, key, data);
			}
		}
		else
		{
			if (this->branchRight == nullptr)
				this->branchRight = branchNode;
			else
			{
				delete branchNode;
				insert<T_key, T_data>(this->branchRight, key, data);
			}
		}

		this->amOfOffsprings++;
	}

	template <typename T_key, typename T_data>
	BinarySearchTree<T_key, T_data>* search(BinarySearchTree<T_key, T_data>* root, T_key key)
	{
		if (root == nullptr)
			return nullptr;

		if (key < root->key)
		{
			if (root->branchLeft == nullptr)
				return nullptr;
			else
				search<T_key, T_data>(root->branchLeft, key);
		}
		else if (key > root->key)
		{
			if (root->branchRight == nullptr)
				return nullptr;
			else
				search<T_key, T_data>(root->branchRight, key);
		}
		else if (key == root->key)
			return root;
	}


	template <typename T_key, typename T_data>
	void ascend(BinarySearchTree<T_key, T_data>* root, T_key* keys, T_data* data, unsigned long max)
	{
		if (root == nullptr)
			return;

		if (root->branchLeft != nullptr)
			ascend<T_key, T_data>(root->branchLeft, keys, data, max);

		data[indexA] = root->data;
		indexA++;

		if (root->branchRight != nullptr)
			ascend<T_key, T_data>(root->branchRight, keys, data, max);
	}


	template <typename T_key, typename T_data>
	void descend(BinarySearchTree<T_key, T_data>* root, T_key* keys, T_data* data, unsigned long max)
	{
		if (root == nullptr)
			return;

		if (root->branchRight != nullptr)
			descend<T_key, T_data>(root->branchRight, keys, data, max);

		data[indexD] = root->data;
		indexD++;

		if (root->branchLeft != nullptr)
			descend<T_key, T_data>(root->branchLeft, keys, data, max);
	}


	template <typename T_key, typename T_data>
	void deleteTree(BinarySearchTree<T_key, T_data>*& root)
	{
		if (root == nullptr) return;

		deleteTree<T_key, T_data>(root->branchLeft);
		deleteTree<T_key, T_data>(root->branchRight);

		delete root;
		root = nullptr;
	}


	template <typename T_keys, typename T_data>
	void treeSort(T_keys* keys, T_data* data, unsigned int length, bool order)
	{
		if (keys == nullptr || data == nullptr || length == 0)
			return;

		BinarySearchTree<T_keys, T_data>* tree = new BinarySearchTree<T_keys, T_data>(keys[0], data[0]);

		for (int i = 1; i < length; i++)
		{
			tree->insert<T_keys, T_data>(keys[i], data[i]);
		}

		if (order)
		{
			ascend<T_keys, T_data>(tree, keys, data, length);
		}
		else
		{
			descend<T_keys, T_data>(tree, keys, data, length);
		}

		indexA = 0;
		indexD = 0;

		deleteTree<T_keys, T_data>(tree);
	}
}

#ifndef MATH_BINARYSEARCHTREE_H
#define MATH_BINARYSEARCHTREE_H

namespace mathem
{

	long deleteCounter = 0;

	long indexA = 0;
	long indexD = 0;

	template <typename T_key, typename T_data>
	struct BinarySearchTree
	{
		T_key key;
		T_data data;

		long amOfOffsprings = 0;

		BinarySearchTree* branchLeft = nullptr;
		BinarySearchTree* branchRight = nullptr;
		BinarySearchTree* parent = nullptr;

		~BinarySearchTree();
	};


	template <typename T_key, typename T_data>
	BinarySearchTree<T_key, T_data>* plantSearchTree(T_key key, T_data data);


	template <typename T_key, typename T_data>
	void insert(BinarySearchTree<T_key, T_data>* node, BinarySearchTree<T_key, T_data>* branchNode);

	template <typename T_key, typename T_data>
	void insert(BinarySearchTree<T_key, T_data>* node, T_key key, T_data data);


	template <typename T_key, typename T_data>
	BinarySearchTree<T_key, T_data>* search(BinarySearchTree<T_key, T_data>* root, T_key key);


	template <typename T_key, typename T_data>
	void ascend(BinarySearchTree<T_key, T_data>* root, T_key* keys, T_data* data, unsigned long max);

	template <typename T_key, typename T_data>
	void descend(BinarySearchTree<T_key, T_data>* root, T_key* keys, T_data* data, unsigned long max);


	template <typename T_key, typename T_data>
	void deleteTree(BinarySearchTree<T_key, T_data>*& root);


	template <typename T_keys, typename T_data>
	void treeSort(T_keys* keys, T_data* data, unsigned int length, bool order = 1);
}

#endif
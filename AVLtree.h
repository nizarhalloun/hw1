#ifndef AVLTREE_H_
#define AVLTREE_H_
/* Author: Nizar Halloun, Date: 12/2/2016 */
#include <iostream>
#include <assert.h>
//////////////////////////////AVLnode class/////////////////////////////////////
template <class Key, class T>
class AVLnode {
	Key k;
	T data;
	AVLnode* left;
	AVLnode* right;
	int height;
public:
	AVLnode(Key k, T data) : k(k), data(data), left(NULL), right(NULL), height(0) {}
	T Data() {return data;}
	Key key() {return k;}
	AVLnode* Left() {return left;}
	AVLnode* Right() {return right;}
	int Height() {return height;}
	void dataSet(T newData) { data = newData;}
	void keySet(Key newKey) { k = newKey;}
	void leftSet(AVLnode* leftNew) {left = leftNew;}
	void rightSet(AVLnode* rightNew) {right = rightNew;}
	void heightSet(int heightNew) {height = heightNew;}
};
/////////////////////////////AVLtree class//////////////////////////////////////
template <class Key, class T, class Compare = std::less<Key> >
class AVLtree {
	AVLnode<Key, T>* root;
	////////////////////////////////////////////////////////////////////////////
	static T find(Key k, AVLnode<Key, T>* current) {
		if(!current) return NULL;
		if(Compare()(k, current->key())) return find(k, current->Left());
		else if(Compare()(current->key(), k)) return find(k, current->Right());
		return current->Data();
	}
	////////////////////////////////////////////////////////////////////////////
	AVLnode<Key, T>* insert(Key k, T data, AVLnode<Key, T>* current) {
		if(!current) return new AVLnode<Key, T>(k, data);
		if(Compare()(k, current->key()))
			current->leftSet(insert(k, data, current->Left()));
		 else
			current->rightSet(insert(k, data, current->Right()));
		return balanceAndRootFix(current);
	}
	////////////////////////////////////////////////////////////////////////////
	AVLnode<Key, T>* remove(Key k, AVLnode<Key, T>* current) {
		if(!current) return NULL;
		if(Compare()(k, current->key()))
			current->leftSet(remove(k, current->Left()));
		else if(Compare()(current->key(), k))
			current->rightSet(remove(k, current->Right()));
		else {// k == current->key()//
			AVLnode<Key, T>* left = current->Left();
			AVLnode<Key, T>* right = current->Right();
			if(current == root && !left && !right) rootSet(NULL);///////
			if(!right && current == root){
				rootSet(left);
				return left;
			}
			if(!right) return left;
			AVLnode<Key, T>* min = findMin(right);
			min->rightSet(removeMin(right));
			min->leftSet(left);
			if(current == root) rootSet(min);
			delete current;
 			return balanceAndRootFix(min);
		}
		return balanceAndRootFix(current);
	}
	////////////////////////////////////////////////////////////////////////////
	 static int size(AVLnode<Key, T>* current) {
		 if(!current) return 0;
		 return size(current->Left()) + size(current->Right()) + 1;
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 AVLnode<Key, T>* rootSet(AVLnode<Key, T>* newRoot) {return root = newRoot;}
	 ///////////////////////////////////////////////////////////////////////////
	 AVLnode<Key, T>* balanceAndRootFix(AVLnode<Key, T>* node) {
			AVLnode<Key, T>* newNode = 	Balance(node);
			if(node == Root() || !Root()) rootSet(newNode);
			return newNode;
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 static void clear(AVLnode<Key, T>* node) {
		 if(!node) return;
			clear(node->Left());
			clear(node->Right());
			delete node;
		}
	////////////////////////////////////////////////////////////////////////////
	 static AVLnode<Key, T>* getMax(AVLnode<Key, T>* node) {
		 if(!node) return NULL;
		 return node->Right() ? getMax(node->Right()) : node;
	 }
	////////////////////////////////////////////////////////////////////////////
	 void putDataInOrderedArray(AVLnode<Key, T>* node, T* array, int& index) {
		 if(!node) return;
		 putDataInOrderedArray(node->Right(), array, index);
		 array[index] = node->Data();
		 index++;
		 putDataInOrderedArray(node->Left(), array, index);
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 void deleteData(AVLnode<Key, T>* node) {
		 if(!node) return;
		 deleteData(node->Left());
		 delete node->Data();
		 deleteData(node->Right());

	 }
public:
	AVLtree() : root(NULL) {}
	////////////////////////////////////////////////////////////////////////////
	~AVLtree() {clear();}
	////////////////////////////////////////////////////////////////////////////
	void clear() {
		clear(root);
		rootSet(NULL);
	}
	////////////////////////////////////////////////////////////////////////////
	void putDataInOrderedArray(T* array) {
		int index = 0;
		putDataInOrderedArray(Root(), array, index);}
	////////////////////////////////////////////////////////////////////////////
	void deleteData() {deleteData(Root());}
	////////////////////////////////////////////////////////////////////////////
	void fillTreeWithGivenOrderedArray(){}
	////////////////////////////////////////////////////////////////////////////
	AVLnode<Key, T>* remove(Key k) {return remove(k, root);}
	////////////////////////////////////////////////////////////////////////////
	AVLnode<Key, T>* insert(Key k, T data) {
		if(!root) return rootSet(new AVLnode<Key, T>(k, data));
		return insert(k, data, root);
	}
	////////////////////////////////////////////////////////////////////////////
	AVLnode<Key, T>* Root() {return root;}
	////////////////////////////////////////////////////////////////////////////
	T find(Key k) {return find(k, root);}
	////////////////////////////////////////////////////////////////////////////
	 int size(){return size(root);}
	////////////////////////////////////////////////////////////////////////////
	 AVLnode<Key, T>* getMax() {return getMax(root);}
};
///////////////////////////////Other functions//////////////////////////////////
static int max(int a, int b) {return a > b ? a : b;}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static AVLnode<Key, T>* removeMin(AVLnode<Key, T>* node) {
	if(!node->Left()) return node->Right();
	node->leftSet(removeMin(node->Left()));
	return node;
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static AVLnode<Key, T>* findMin(AVLnode<Key, T>* node) {
	return node->Left() ? findMin(node->Left()) : node;
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static void heightFix(AVLnode<Key, T>* node) {node->heightSet(height(node));}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static int height(AVLnode<Key, T>* node) {
    if (!node) return -1;
    return max(height(node->Left()), height(node->Right())) + 1;
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static int balanceGet(AVLnode<Key, T>* node) {
	if(!node) return 0;
	return height(node->Left()) - height(node->Right());
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static AVLnode<Key, T>* Balance(AVLnode<Key, T>* current) {
	assert(current);
	heightFix(current);
	int balance = balanceGet(current);
	int balanceLeft = balanceGet(current->Left());
	int balanceRigh = balanceGet(current->Right());
	if(balance == 2) {//left left or left right//
		if(balanceLeft == -1)
		current->leftSet(rotateLeft(current->Left()));
		return rotateRight(current);
	}
	if(balance == -2) {//right right or right left//
		if(balanceRigh == 1)
		current->rightSet(rotateRight(current->Right()));
		return rotateLeft(current);
	}
	return current;
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static AVLnode<Key, T>* rotateLeft(AVLnode<Key, T>* x) {
	assert(x);
	AVLnode<Key, T>* y = x->Right();
	AVLnode<Key, T>* z = y->Left();
	y->leftSet(x);
	x->rightSet(z);
	x->heightSet(max(height<Key, T>(x->Left()), height<Key, T>(x->Right())) + 1);
	y->heightSet(max(height<Key, T>(y->Left()), height<Key, T>(y->Right())) + 1);
	return y;
}
////////////////////////////////////////////////////////////////////////////////
template <class Key, class T>
static AVLnode<Key, T>* rotateRight(AVLnode<Key, T>* x) {
	assert(x);
	AVLnode<Key, T>* y = x->Left();
	AVLnode<Key, T>* z = y->Right();
	y->rightSet(x);
	x->leftSet(z);
	x->heightSet(max(height<Key, T>(x->Left()), height<Key, T>(x->Right())) + 1);
	y->heightSet(max(height<Key, T>(y->Left()), height<Key, T>(y->Right())) + 1);
	return y;
}
////////////////////////////////////////////////////////////////////////////////
#endif /* AVLTREE_H_ */

#pragma once
#include<exception>
using std::logic_error;

template<typename Key>
class AVL
{
	struct node;
public:
	class iterator;
	iterator find(Key key);
	size_t count(Key key);
	iterator lower_bound(Key key);
	iterator upper_bound(Key key);
	iterator begin();
	iterator end();
	iterator insert(Key key)
	{
		return iterator(node::insert(root, key));
	}
	void erase(Key key);
	void erase(iterator it);
	AVL<Key> split(Key key);
	void merge(AVL<Key>&& other);
	AVL(std::initializer_list<Key>);
	AVL() = default;
private:
	node* root{ nullptr };
	void erase(node* vertex);
	void balance(node* vertex);
};

template<typename Key>
class AVL<Key>::iterator
{
public:
	node* data{ nullptr };
	iterator(node* data = nullptr) : data(data)
	{
	};
};

template<typename Key>
struct AVL<Key>::node
{
private:
	node *left{ nullptr };
	node *right{ nullptr };
	size_t hight{ 0 };
	node* pr{ nullptr };
public:
	Key value;
	node(Key value = Key{}, node *left = nullptr, node *right = nullptr, node* pr = nullptr)
		: value(value), left(left), right(right), pr(pr)
	{
		recalculate(this);
	}
	void change_right(node *vertex);
	void change_left(node *vertex);
	static void change_parent(node *vertex);
	static node* rotateRight(node *vertex);
	static node* rotateLeft(node *vertex);
	static void recalculate(node *vertex)
	{

	}
	static size_t balanceFactor(node *vertex);
	static node* merge(node *first, node *second, node* middle);
	static node* merge(node *first, node *second);
	static node* find(node *vertex, Key key);
	static node* insert(node *vertex, Key key)
	{
		if (vertex == nullptr)
		{
			return new node(key);
		}
		return nullptr;
	}
	static node* erase(Key key);
	static node* balance(node *vertex);
	static size_t getHight(node *vertex);
	static node* getLeft(node *vertex);
	static node* getRight(node *vertex);
	static node* getParent(node *vertex);
	static node* findMin(node *vertex);
	static node* findMax(node *vertex);
	static node* lowerBound(node *vertex, Key key);
	static node* nextVertex(node *vertex);
	static node* prevVertex(node *vertex);
};
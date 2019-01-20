#pragma once
#include <stdexcept>
#include <algorithm>
#include <cmath>
using std::logic_error;
using std::max;
using std::swap;

template<typename Key>
class AVL
{
	struct node;
public:
	class iterator;
	iterator find(const Key &key)
	{
		return iterator(node::find(root, key));
	}
	size_t count(const Key &key)
	{
		return (size_t)(find(key) != end());
	}
	iterator lower_bound(const Key &key)
	{
		return iterator{ node::lowerBound(root, key) };
	}
	iterator upper_bound(const Key &key)
	{
		if (count(key))
			return ++lower_bound(key);
		else
			return lower_bound(key);
	}
	iterator begin()
	{
		return iterator{ node::findMin(root) };
	}
	iterator end()
	{
		return iterator{ nullptr };
	}
	void insert(Key key)
	{
		root = node::insert(root, key);
	}
	void erase(Key key)
	{
		erase(find(key));
	}
	void erase(const AVL<Key>::iterator &it)
	{
		root = node::erase(root, it.data);
	}
	AVL<Key> split(Key key);
	void merge(AVL<Key>&& other)
	{
		root = node::merge(root, other.root);
		new (&other) AVL<Key>();
	}
	AVL(std::initializer_list<Key>);
	AVL() = default;
	~AVL()
	{
		if (root)
			delete root;
	}
private:
	node* root{ nullptr };

};

template<typename Key>
class AVL<Key>::iterator
{
public:
	node* data{ nullptr };
	explicit iterator(node* data = nullptr) : data(data)
	{
	};
	iterator operator++()
	{
		data = node::nextVertex(data);
		return *this;
	}
	iterator operator--()
	{
		data = node::prevVertex(data);
		return *this;
	}
	Key operator*()
	{
		return data->value;
	}
};

template<typename Key>
struct AVL<Key>::node
{
private:
	node *left{ nullptr };
	node *right{ nullptr };
	int hight{ 0 };
	node* pr{ nullptr };
public:
	Key value;
	explicit node(Key value = Key{}, node *left = nullptr, node *right = nullptr, node* pr = nullptr)
		: value(value), left(left), right(right), pr(pr) //--?
	{
		recalculate(this);
	}
	void change_right(node *vertex)
	{
		right = vertex;
		change_parent(vertex, this);
		recalculate(this);
	}
	void change_left(node *vertex)
	{
		left = vertex;
		change_parent(vertex, this);
		recalculate(this);
	}
	static void change_parent(node *vertex, node *parent)
	{
		if (vertex)
			vertex->pr = parent;
	}
	static node* rotateRight(node *vertex)
	{
		if (vertex == nullptr)
			throw logic_error("rotateRight nullvertex");
		if (getLeft(vertex) == nullptr)
			throw logic_error("rotateRight nullvertexLeftSon");

		node* tmp = getLeft(vertex);
		vertex->change_left(getRight(tmp));
		change_parent(tmp, nullptr);
		tmp->change_right(vertex);
		return tmp;
	}
	static node* rotateLeft(node *vertex)
	{
		if (vertex == nullptr)
			throw logic_error("rotateLeft nullvertex");
		if (getRight(vertex) == nullptr)
			throw logic_error("rotateLeft nullvertexRightSon");

		node* tmp = getRight(vertex);
		vertex->change_right(getLeft(tmp));
		change_parent(tmp, nullptr);
		tmp->change_left(vertex);
		return tmp;
	}
	static void recalculate(node *vertex)
	{
		if (vertex)
		{
			vertex->hight = max(getHight(getLeft(vertex)),
				getHight(getRight(vertex))) + 1;
		}
	}
	static int balanceFactor(node *vertex)
	{
		return getHight(getRight(vertex)) - getHight(getLeft(vertex));
	}
	static node* merge(node *first, node *second, const Key &middle);
	static node* merge(node *first, node *second)
	{
		if (first)
		{
			Key tmp = findMax(first)->value;
			return merge(erase(first, findMax(first)), second, tmp);
		}
		if (second)
		{
			Key tmp = findMin(second)->value;
			return merge(first, erase(second, findMin(second)), tmp);
		}
		return nullptr;
	}
	static node* find(node *vertex, const Key &key)
	{
		if (!vertex)
			return nullptr;
		if (vertex->value > key)
			return find(getLeft(vertex), key);
		if (vertex->value < key)
			return find(getRight(vertex), key);
		return vertex;
	}
	static node* erase(node* root, node *vertex)
	{
		if (vertex == nullptr)
			throw logic_error("you delete null");
		if (getLeft(vertex))
		{
			vertex->value = findMax(getLeft(vertex))->value;
			vertex->change_left(erase(getLeft(vertex), findMax(getLeft(vertex))));
			return balance(root, vertex);
		}
		if (getRight(vertex))
		{
			vertex->value = findMin(getLeft(vertex))->value;
			vertex->change_right(erase(getRight(vertex), findMin(getRight(vertex))));
			return balance(root, vertex);
		}
		if (vertex == root)
		{
			delete vertex;
			return nullptr;
		}
		node* tmp = getParent(vertex);
		eraseSon(tmp, vertex);
		delete vertex;
		return balance(root, tmp);
	}
	static void eraseSon(node* vertex, node* son)
	{
		if (getLeft(vertex) == son)
			vertex->change_left(nullptr);
		else
			vertex->change_right(nullptr);
		recalculate(vertex);
	}
	static node* balance(node* root, node* vertex)
	{
		if (abs(balanceFactor(vertex)) < 2)
			return root;
		if (vertex != root)
			return balance(root, balanceSons(getParent(vertex)));
		else
			return balance(vertex);
	}
	static node* insert(node *vertex, const Key &key)
	{
		if (vertex == nullptr)
		{
			return new node(key);
		}
		if (vertex->value > key)
		{
			vertex->change_left(insert(getLeft(vertex), key));
			return balance(vertex);
		}
		if (vertex->value < key)
		{
			vertex->change_right(insert(getRight(vertex), key));
			return balance(vertex);
		}
		return vertex;
	}
	static node* balance(node *vertex)
	{
		if (balanceFactor(vertex) == -2)
		{
			if (balanceFactor(getLeft(vertex)) > 0)
				vertex->change_left(rotateLeft(getLeft(vertex)));
			return rotateRight(vertex);
		}
		if (balanceFactor(vertex) == 2)
		{
			if (balanceFactor(getRight(vertex)) < 0)
				vertex->change_right(rotateRight(getRight(vertex)));
			return rotateLeft(vertex);
		}
		return vertex;
	}
	static node* balanceSons(node *vertex)
	{
		vertex->change_left(balance(getLeft(vertex)));
		vertex->change_right(balance(getRight(vertex)));
		return vertex;
	}
	static int getHight(node *vertex)
	{
		return vertex ? vertex->hight : 0;
	}
	static node* getLeft(node *vertex)
	{
		return vertex ? vertex->left : nullptr;
	}
	static node* getRight(node *vertex)
	{
		return vertex ? vertex->right : nullptr;
	}
	static node* getParent(node *vertex)
	{
		return vertex ? vertex->pr : nullptr;
	}
	static node* findMin(node *vertex)
	{
		if (getLeft(vertex) != nullptr)
			return findMin(getLeft(vertex));
		else
			return vertex;
	}
	static node* findMax(node *vertex)
	{
		if (getRight(vertex) != nullptr)
			return findMax(getRight(vertex));
		else
			return vertex;
	}
	static node* lowerBound(node *vertex, const Key &key)
	{
		if (!vertex)
			return nullptr;
		else if (vertex->value >= key)
		{
			node* tmp = lowerBound(getLeft(vertex), key);
			return tmp ? tmp : vertex;
		}
		else
			return lowerBound(getRight(vertex), key);
	}
	static node* upLeft(node *vertex)
	{
		if (getRight(getParent(vertex)) == vertex)
			return upLeft(getParent(vertex));
		return getParent(vertex);
	}
	static node* upRight(node *vertex)
	{
		if (getLeft(getParent(vertex)) == vertex)
			return upRight(getParent(vertex));
		return getParent(vertex);
	}
	static node* nextVertex(node *vertex)
	{
		if (vertex == nullptr)
			throw logic_error("next_ver nullptr");
		vertex = getRight(vertex) ? vertex : upLeft(vertex);
		return getRight(vertex) ? find_min(getRight(vertex)) : vertex;
	}
	static node* prevVertex(node *vertex)
	{
		if (vertex == nullptr)
			throw logic_error("prev_ver nullptr");
		vertex = getLeft(vertex) ? vertex : upRight(vertex);
		return getLeft(vertex) ? find_max(getLeft(vertex)) : vertex;
	}
	~node()
	{
		if (left)
			delete left;
		if (right)
			delete right;
	}
};
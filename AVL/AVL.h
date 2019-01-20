#pragma once
#include <stdexcept>
#include <algorithm>
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
		erase(node::find(root, key));
	}
	void erase(const AVL<Key>::iterator &it)
	{
		erase(it.data);
	}
	AVL<Key> split(Key key);
	void merge(AVL<Key>&& other)
	{
		root = node::merge(root, other.root);
		new (&other) AVL<Key>();
	}
	AVL(std::initializer_list<Key>);
	AVL() = default;
private:
	node* root{ nullptr };
	void erase(node* vertex)
	{
		if (vertex == nullptr)
			throw logic_error("you delete null");
		node* tmp = node::getParent(vertex);
		node::erase(vertex);
		balance(tmp);
	}
	void balance(node* vertex)
	{
		if (node::getParent(vertex) == nullptr)
			root = node::balance(vertex);
		else
		{
			balance(node::balanceSons(node::getParent(vertex)));
		}
	}
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
			throw logic_error("rotateRight nullvertex");
		if (getRight(vertex) == nullptr)
			throw logic_error("rotateRight nullvertexRightSon");

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
	static size_t balanceFactor(node *vertex)
	{
		return getHight(getRight(vertex)) - getHight(getLeft(vertex));
	}
	static node* merge(node *first, node *second, node* middle);
	static node* merge(node *first, node *second);
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
	static node* extract(node *vertex)
	{
		//        if(node::getLeft(vertex) != nullptr || node::getRight(vertex))
		//        {
		//            vertex->value = node::findMax(node::getLeft(vertex));
		//            vertex->change_left(node::removeMax(node::getLeft(vertex)));
		//        }
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
		recalculate(vertex);//--?
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
		vertex->changeLeft(balance(getLeft(vertex)));
		vertex->changeRight(balance(getRight(vertex)));
		return vertex;
	}
	static size_t getHight(node *vertex)
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
	node* lowerBound(node *vertex, const Key &key)
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
};
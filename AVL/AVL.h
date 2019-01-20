#pragma once
#include<exception>
using std::logic_error;

template<typename Key>
class AVL
{
	struct node;
public:
	class iterator;
	AVL<Key>::iterator find(Key key);
	size_t count(Key key);
	AVL<Key>::iterator lower_bound(Key key);
	AVL<Key>::iterator upper_bound(Key key);
	AVL<Key>::iterator begin();
	AVL<Key>::iterator end();
	AVL<Key>::iterator insert(Key key);
	void erase(Key key);
	void erase(AVL<Key>::iterator it);
	AVL<Key> split(Key key);
	void merge(AVL<Key>&& other);
private:
	node* root = nullptr;
	void merge(AVL<Key>::node *vertex, AVL<Key>::node *vertex);
	AVL<Key>::node* balance(AVL<Key>::node *vertex);
	AVL<Key>::node*	get_hight(AVL<Key>::node *vertex);
	AVL<Key>::node*	get_left(AVL<Key>::node *vertex);
	AVL<Key>::node*	get_right(AVL<Key>::node *vertex);
	AVL<Key>::node*	right_turn(AVL<Key>::node *vertex);
	AVL<Key>::node* left_turn(AVL<Key>::node *vertex);
	AVL<Key>::node* find_min(AVL<Key>::node *vertex);
	AVL<Key>::node* find_max(AVL<Key>::node *vertex);
	AVL<Key>::node* next_ver(AVL<Key>::node *vertex);
	AVL<Key>::node* prev_ver(AVL<Key>::node *vertex);
};

template<typename Key>
struct AVL<Key>::node
{
	node *left{nullptr};
	node *right{ nullptr };
	size_t hight{ 0 }, sz{ 0 };
	node* pr{nullptr};
	Key value;
};

template<typename Key>
AVL<Key>::iterator AVL<Key>::find(Key key)
{
	node *tc = root;
	while (tc != nullptr)
	{
		if (tc->value > key)
			tc = tc->left;
		else if (tc->value < key)
			tc = tc->right;
		else
			break;
	}
	return AVL<Key>::iterator(tc);
}

template<typename Key>
size_t AVL<Key>::count(Key key)
{
	return (find(key) != end()) ? 1 : 0;
}

template<typename Key>
AVL<Key>::iterator AVL<Key>::lower_bound(Key key)
{
	node *tc = root;
	node *ans = nullptr;
	while (tc != nullptr)
	{
		if (tc->value >= key)
		{
			ans = tc;
			tc = tc->left;
		}
		else
			tc = tc->right;
	}
	return AVL<Key>::iterator(ans);
}

template<typename Key>
AVL<Key>::iterator AVL<Key>::upper_bound(Key key)
{
	if (find(key))
		return ++lower_bound(key);
	else
		return lower_bound(key);
}

template<typename Key>
AVL<Key>::iterator AVL<Key>::begin()
{
	return AVL<Key>::iterator(find_min(root));
}

template<typename Key>
AVL<Key>::node * AVL<Key>::find_min(AVL<Key>::node *vertex)
{
	AVL<Key>::node *tc = vertex;
	while (get_left(tc) != nullptr)
		tc = tc->left;
	return tc;
}

template<typename Key>
AVL<Key>::node * AVL<Key>::find_max(AVL<Key>::node *vertex)
{
	AVL<Key>::node *tc = vertex;
	while (get_right(tc) != nullptr)
		tc = tc->right;
	return tc;
}

template<typename Key>
AVL<Key>::iterator AVL<Key>::end()
{
	return AVL<Key>::iterator(nullptr);
}

template<typename Key>
AVL<Key>::iterator AVL<Key>::insert(Key key)
{
	return AVL<Key>::iterator();
}

template<typename Key>
void AVL<Key>::erase(Key key)
{
}

template<typename Key>
void AVL<Key>::erase(AVL<Key>::iterator it)
{
}

template<typename Key>
AVL<Key> AVL<Key>::split(Key key)
{
	return AVL<Key>();
}

template<typename Key>
void AVL<Key>::merge(AVL<Key>&& other)
{
}

template<typename Key>
AVL<Key>::node * AVL<Key>::next_ver(AVL<Key>::node *vertex)
{
	if (!vertex)
		throw logic_error("next_ver nullll");
	if (vertex->right != nullptr)
	{
		return find_min(vertex->right);
	}
	else
	{
		for (AVL<Key>::node* tc = vertex; tc->pr != nullptr; tc = tc->pr)
		{
			if (tc->pr->left == tc)
				return tc->pr->right ? find_min(tc->pr->right) : tc->pr;
		}
		return nullptr;
	}
}

template<typename Key>
AVL<Key>::node * AVL<Key>::prev_ver(AVL<Key>::node *vertex)
{
	if (!vertex)
		throw logic_error("prev_ver nullll");
	if (vertex->left != nullptr)
	{
		return find_max(vertex->left);
	}
	else
	{
		for (AVL<Key>::node* tc = vertex; tc->pr != nullptr; tc = tc->pr)
		{
			if (tc->pr->right == tc)
				return tc->pr->left ? find_max(tc->pr->left) : tc->pr;
		}
		return nullptr;
	}
}

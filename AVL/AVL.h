#pragma once

template<
	typename Key
> class AVL
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
	balance(node *vertex);
	get_hight(node *vertex);
	get_left(node *vertex);
	get_right(node *vertex);
	right_turn(node *vertex);
	left_turn(node *vertex);
	const AVL<Key>::iterator end_{nullptr};
};

template<typename Key>
struct AVL<Key>::node
{
	node *left{nullptr};
	node *right{ nullptr };
	size_t hight{ -1 };
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
	node *tc = root;
	if (tc == nullptr)
		return end();
	while (get_left(tc) != nullptr)
		tc = tc.left;
	return AVL<Key>::iterator(tc);
}

template<typename Key>
inline AVL<Key>::iterator AVL<Key>::end()
{
	return end_;
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

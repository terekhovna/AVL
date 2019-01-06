#pragma once

template<
	typename Key,
	typename Compare = std::less<Key>
> class AVL
{
	struct node;
public:
	class iterator;
	AVL<Key, Compare>::iterator find();
	site_t count();
	AVL<Key, Compare>::iterator lower_bound();
	AVL<Key, Compare>::iterator upper_bound();
	AVL<Key, Compare>::iterator begin();
	AVL<Key, Compare>::iterator end();
	AVL<Key, Compare>::iterator insert(Key key);
	void erase(Key key);
	void erase(AVL<Key, Compare>::iterator it);
	AVL<Key, Compare> split(Key key);
	void merge(AVL<Key, Compare>&& other);
private:
	node* root = nullptr;
	const AVL<Key, Compare>::iterator end_{nullptr};
};

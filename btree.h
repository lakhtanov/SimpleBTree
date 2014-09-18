#ifndef _BTREE_H_
#define _BTREE_H_

#include <cstdlib>
#include <cstdio>

#include "pair.h"

template<typename KeyType, typename ValueType, int Range> class btree; 

template<typename KeyType, typename ValueType, int Range>
class node
{
	int number_of_keys;
	KeyType *key;
	ValueType *value;
	node **next;
	bool leaf;

	node();
	node(const node &);
	~node();

	void split(int);
	void print(char * (*)(KeyType), char * (*)(ValueType), FILE *) const;
public:
	friend class btree<KeyType, ValueType, Range>;
};

template<typename KeyType, typename ValueType, int Range>
class btree
{
	node<KeyType, ValueType, Range> *root;
	int _size;
	bool (*cmp)(KeyType, KeyType);

	node<KeyType, ValueType, Range> *find_key(KeyType) const;
	void insert_nonfull(node<KeyType, ValueType, Range> *, KeyType, ValueType);
	pair<KeyType, ValueType> erase(node<KeyType, ValueType, Range> *, KeyType, int);
	void free(node<KeyType, ValueType, Range> *);
public:
	btree(bool (*)(KeyType, KeyType));
	btree(const btree &);	
	~btree();

	bool has_key(KeyType) const;
	int size() const;
	bool empty() const;

	void insert(KeyType, ValueType);
	void erase(KeyType);
	void print_info(char * (*)(KeyType), char * (*)(ValueType), const char *) const;
	void print_info(char * (*)(KeyType), char * (*)(ValueType), FILE *) const;
	ValueType get(KeyType) const;
};

/////////////////////////////////////////////////////////////////////
// node implementation

template<typename KeyType, typename ValueType, int Range>
node<KeyType, ValueType, Range>::node()
{
	number_of_keys = 0;
	leaf = false;

	key = new KeyType[2 * Range - 1];
	value = new ValueType[2 * Range - 1];
	next = new node*[2 * Range];
}

template<typename KeyType, typename ValueType, int Range>
node<KeyType, ValueType, Range>::node(const node &nd)
{
	int i;

	key = new KeyType[2 * Range - 1];
	value = new ValueType[2 * Range - 1];
	next = new node*[2 * Range];

	number_of_keys = nd.number_of_keys;
	leaf = nd.leaf;
	for (i = 0; i < number_of_keys; i++)
	{
		key[i] = nd.key[i];
		value[i] = nd.value[i];
		next[i] = new node(*nd.next[i]);
	}
	next[i] = new node(*nd.next[i]);
}

template<typename KeyType, typename ValueType, int Range>
node<KeyType, ValueType, Range>::~node()
{	
	delete[] key;
	delete[] value;
	delete[] next;		
}

template<typename KeyType, typename ValueType, int Range>
void node<KeyType, ValueType, Range>::split(int num)
{
	int i;
	node *nxt = next[num], *new_node;
	for (i = number_of_keys + 1; i > num + 1; i--)
		next[i] = next[i - 1];
	for (i = number_of_keys; i > num; i--)
	{
		key[i] = key[i - 1];
		value[i] = value[i - 1];
	}
	key[num] = nxt->key[Range - 1];
	value[num] = nxt->value[Range - 1];

	new_node = new node;
	new_node->leaf = nxt->leaf;
	for (i = Range; i < 2 * Range - 1; i++)
	{
		new_node->key[i - Range] = nxt->key[i];
		new_node->value[i - Range] = nxt->value[i];
		new_node->next[i - Range] = nxt->next[i];
	}	
	new_node->next[Range - 1] = nxt->next[2 * Range - 1];
	nxt->number_of_keys = new_node->number_of_keys = Range - 1;
	next[num + 1] = new_node;
	number_of_keys++;
}

template<typename KeyType, typename ValueType, int Range>
void node<KeyType, ValueType, Range>::print(char * (*key_to_str)(KeyType), char * (*value_to_str)(ValueType), FILE *fout) const
{
	int i;
	char *pkey, *pvalue;

	for (i = 0; i < number_of_keys; i++)
	{
		if (!leaf)
			next[i]->print(key_to_str, value_to_str, fout);
		pkey = key_to_str(key[i]);
		pvalue = value_to_str(value[i]);
		fprintf(fout, "%s %s\n", pkey, pvalue);
		delete[] pkey;
		delete[] pvalue;
	}
	if (!leaf)
		next[number_of_keys]->print(key_to_str, value_to_str, fout);
}

/////////////////////////////////////////////////////////////////////
// btree implementation

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::free(node<KeyType, ValueType, Range> *current_node)
{
	if (!current_node)
		return;

	int i;
	if (!current_node->leaf)
	{
		for (i = 0; i <= current_node->number_of_keys; i++)
		{
			free(current_node->next[i]);
			delete current_node->next[i];
		}
	}	
}

template<typename KeyType, typename ValueType, int Range>
node<KeyType, ValueType, Range> * btree<KeyType, ValueType, Range>::find_key(KeyType key) const
{
	int i;
	node<KeyType, ValueType, Range> *current_node = root;

	while (current_node)
	{
		for (i = 0; i < current_node->number_of_keys && cmp(current_node->key[i], key); i++)
			;
		if (i < current_node->number_of_keys && key == current_node->key[i])
			return current_node;
		if (current_node->leaf)
			return NULL;
		current_node = current_node->next[i];
	}
	return NULL;
}

template<typename KeyType, typename ValueType, int Range>
btree<KeyType, ValueType, Range>::btree(bool (*f)(KeyType, KeyType))
{
	root = new node<KeyType, ValueType, Range>();
	root->leaf = true;
	cmp = f;
	_size = 0;
}

template<typename KeyType, typename ValueType, int Range>
btree<KeyType, ValueType, Range>::btree(const btree &tree)
{
	root = new node<KeyType, ValueType, Range>(*tree.root);
	cmp = tree.cmp;
	_size = tree._size;
}

template<typename KeyType, typename ValueType, int Range>
btree<KeyType, ValueType, Range>::~btree()
{
	free(root);	
	delete root;
}

template<typename KeyType, typename ValueType, int Range>
bool btree<KeyType, ValueType, Range>::has_key(KeyType key) const
{
	return find_key(key) != NULL;
}

template<typename KeyType, typename ValueType, int Range>
int btree<KeyType, ValueType, Range>::size() const
{
	return _size;
}

template<typename KeyType, typename ValueType, int Range>
bool btree<KeyType, ValueType, Range>::empty() const
{
	return _size == 0;
}

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::insert(KeyType key, ValueType value)
{
	node<KeyType, ValueType, Range> *fnd, *current_node;
	int i;

	fnd = find_key(key);
	if (fnd)	
	{
		for (i = 0; i < fnd->number_of_keys; i++)
			if (fnd->key[i] == key)
			{				
				fnd->value[i] = value;
				return;
			}
	}

	current_node = root;
	if (current_node->number_of_keys == 2 * Range - 1)
	{
		root = new node<KeyType, ValueType, Range>;
		root->next[0] = current_node;
		root->split(0);		
	}
	insert_nonfull(root, key, value);
	_size++;
}

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::insert_nonfull(node<KeyType, ValueType, Range> *current_node, KeyType key, ValueType value)
{
	if (!current_node)
		throw "Invalid pointer to node";
	int i, j, n = current_node->number_of_keys;
	for (i = 0; i < n && cmp(current_node->key[i], key); i++)
		;
	if (current_node->leaf)
	{
		for (j = n; j > i; j--)
		{
			current_node->key[j] = current_node->key[j - 1];
			current_node->value[j] = current_node->value[j - 1];			
		}
		current_node->key[i] = key;
		current_node->value[i] = value;
		current_node->number_of_keys++;
	}
	else
	{
		if (current_node->next[i]->number_of_keys == 2 * Range - 1)
		{
			current_node->split(i);
			if (cmp(current_node->key[i], key))
				i++;
		}
		insert_nonfull(current_node->next[i], key, value);
	}
}

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::erase(KeyType key)
{
	node<KeyType, ValueType, Range> *tmp;
	if (!find_key(key))
		return;
	erase(root, key, 0);
	if (root->number_of_keys == 0 && !root->leaf)
	{
		tmp = root->next[0];
		delete root;
		root = tmp;
	}
	_size--;
}

template<typename KeyType, typename ValueType, int Range>
pair<KeyType, ValueType> btree<KeyType, ValueType, Range>::erase(node<KeyType, ValueType, Range> *current_node, KeyType key, int flag)
{
	int i, j, n = current_node->number_of_keys;
	node<KeyType, ValueType, Range> *left, *right;
	pair<KeyType, ValueType> value, tmp;

	for (i = 0; i < n && cmp(current_node->key[i], key); i++)
		;
	if (flag)
	{
		if (flag == 1)
			i = n - 1;
		else if (flag == -1)
			i = 0;
	}

	if (current_node->leaf)
	{
		value = pair<KeyType, ValueType>(current_node->key[i], current_node->value[i]);
		current_node->number_of_keys--, n--;		
		for (; i < n; i++)
		{
			current_node->key[i] = current_node->key[i + 1];
			current_node->value[i] = current_node->value[i + 1];
		}
		return value;
	}
	else
	{		
		if (i < n && current_node->key[i] == key && !flag)
		{
			left = current_node->next[i];
			right = current_node->next[i + 1];
			tmp = pair<KeyType, ValueType>(current_node->key[i], current_node->value[i]);

			if (left->number_of_keys >= Range || right->number_of_keys >= Range)
			{
				if (left->number_of_keys >= Range)
					value = erase(left, key, 1);
				else
					value = erase(right, key, -1);
				current_node->key[i] = value.first;
				current_node->value[i] = value.second;
				return tmp;
			}			
			else
			{
				if (left->number_of_keys == Range - 1 && right->number_of_keys == Range - 1)
				{
					left->key[left->number_of_keys] = current_node->key[i];
					left->value[left->number_of_keys] = current_node->value[i];
					left->number_of_keys++;

					for (j = 0; j < right->number_of_keys; j++)
					{
						left->key[left->number_of_keys + j] = right->key[j];
						left->value[left->number_of_keys + j] = right->value[j];
						left->next[left->number_of_keys + j] = right->next[j];
					}
					left->number_of_keys += right->number_of_keys;
					left->next[left->number_of_keys] = right->next[right->number_of_keys];

					for (; i < current_node->number_of_keys - 1; i++)
					{
						current_node->key[i] = current_node->key[i + 1];
						current_node->value[i] = current_node->value[i + 1];
						current_node->next[i + 1] = current_node->next[i + 2];
					}					
					current_node->number_of_keys--;

					delete right;

					return erase(left, key, flag);
				}
				else
					throw "Left subtree or right subtree contain less than RANGE - 1 key";			
			}
		}
		else
		{
			if (flag == 1)
				i = n;

			if (current_node->next[i]->number_of_keys < Range)
			{
				if (i > 0 && current_node->next[i - 1]->number_of_keys >= Range)
				{
					left = current_node->next[i - 1];
					right = current_node->next[i];

					right->next[right->number_of_keys + 1] = right->next[right->number_of_keys];
					for (j = right->number_of_keys; j > 0; j--)
					{
						right->key[j] = right->key[j - 1];
						right->value[j] = right->value[j - 1];
						right->next[j] = right->next[j - 1];
					}
					right->key[0] = current_node->key[i - 1];
					right->value[0] = current_node->value[i - 1];
					right->next[0] = left->next[left->number_of_keys];

					right->number_of_keys++;

					left->number_of_keys--;
					current_node->key[i - 1] = left->key[left->number_of_keys];
					current_node->value[i - 1] = left->value[left->number_of_keys];
				}
				else if (i < current_node->number_of_keys && 
					current_node->next[i + 1]->number_of_keys >= Range)
				{
					left = current_node->next[i];
					right = current_node->next[i + 1];

					left->key[left->number_of_keys] = current_node->key[i];
					left->value[left->number_of_keys] = current_node->value[i];
					left->next[++left->number_of_keys] = right->next[0];

					current_node->key[i] = right->key[0];
					current_node->value[i] = right->value[0];

					right->number_of_keys--;
					for (j = 0; j < right->number_of_keys; j++)
					{
						right->key[j] = right->key[j + 1];
						right->value[j] = right->value[j + 1];
						right->next[j] = right->next[j + 1];
					}
					right->next[right->number_of_keys] = right->next[right->number_of_keys + 1];
				}
				else
				{
					if (i > 0)
					{
						left = current_node->next[i - 1];
						right = current_node->next[i];

						left->key[left->number_of_keys] = current_node->key[i - 1];
						left->value[left->number_of_keys] = current_node->value[i - 1];
						left->number_of_keys++;							
						for (j = 0; j < right->number_of_keys; j++)
						{
							left->key[left->number_of_keys + j] = right->key[j];
							left->value[left->number_of_keys + j] = right->value[j];
							left->next[left->number_of_keys + j] = right->next[j];
						}
						left->number_of_keys += right->number_of_keys;
						left->next[left->number_of_keys] = right->next[right->number_of_keys];

						delete right;

						for (j = i - 1; j < current_node->number_of_keys - 1; j++)
						{
							current_node->key[j] = current_node->key[j + 1];
							current_node->value[j] = current_node->value[j + 1];
							current_node->next[j + 1] = current_node->next[j + 2];
						}							
						current_node->number_of_keys--;	
						i--;
					}
					else if (i < current_node->number_of_keys)
					{
						left = current_node->next[i];
						right = current_node->next[i + 1];

						left->key[left->number_of_keys] = current_node->key[i];
						left->value[left->number_of_keys] = current_node->value[i];
						left->number_of_keys++;							
						for (j = 0; j < right->number_of_keys; j++)
						{
							left->key[left->number_of_keys + j] = right->key[j];
							left->value[left->number_of_keys + j] = right->value[j];
							left->next[left->number_of_keys + j] = right->next[j];
						}
						left->number_of_keys += right->number_of_keys;
						left->next[left->number_of_keys] = right->next[right->number_of_keys];

						delete right;

						current_node->next[i] = left;
						for (j = i; j < current_node->number_of_keys - 1; j++)
						{
							current_node->key[j] = current_node->key[j + 1];
							current_node->value[j] = current_node->value[j + 1];
							current_node->next[j + 1] = current_node->next[j + 2];
						}
						current_node->number_of_keys--;
					}
					else
						throw "Something wrong with index of subtree";
				}
			}
			return erase(current_node->next[i], key, flag);
		}
	}
}

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::print_info(char * (*key_to_str)(KeyType), char * (*value_to_str)(ValueType), FILE *fout) const
{
	if (!fout)
		throw "Invalid output stream";
	root->print(key_to_str, value_to_str, fout);
}

template<typename KeyType, typename ValueType, int Range>
void btree<KeyType, ValueType, Range>::print_info(char * (*key_to_str)(KeyType), char * (*value_to_str)(ValueType), const char *output_name) const
{
	FILE *fout;
	fout = fopen(output_name, "w");
	if (!fout)
		throw "Cannot open output file";
	print_info(key_to_str, value_to_str, fout);
}

template<typename KeyType, typename ValueType, int Range>
ValueType btree<KeyType, ValueType, Range>::get(KeyType key) const
{
	int i;
	node<KeyType, ValueType, Range> *fnd = find_key(key);
	if (!fnd)
		throw "There is no such key in the tree";
	for (i = 0; i < fnd->number_of_keys; i++)
		if (fnd->key[i] == key)
			break;
	return fnd->value[i];
}

#endif
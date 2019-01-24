#ifndef _H_LIST
#define _H_LIST

template<class type>
class list
{
public:
	class node
	{
	public:
		node (node* prev, node* next, type data)
			: _prev(prev), _next(next), _data(data)
		{	}

		node* _next;
		node* _prev;
		type  _data;
	};

	list()
	{
		Create();
	}

	~list()
	{
		Destroy();
	}

	void Create()
	{
		_head = NULL;
		_tail = NULL;
		_len = 0;
	}

	void Destroy()
	{
		while(_head != NULL)
		{
			node* temp = _head->_next;
			delete _head;
			_head = temp;
		}
	}

	void Append(type data)
	{
		node* newnode = new node(_tail, NULL, data);

		if (newnode->_prev != NULL)
			newnode->_prev->_next = newnode;

		_tail = newnode;

		if (_head == NULL)
			_head = newnode;

		_len++;
	}

	node* GetNode(int index)
	{
		node* temp = _head;
		for (; index > 0; index--)
		{
			if (temp == NULL)
				return NULL;

			temp = temp->_next;
		}

		return temp;
	}

	node* GetHead()
	{
		return _head;
	}

	node* GetTail()
	{
		return _tail;
	}

	type Get(int index)
	{
		node* target = GetNode(index);
		if (target)
		{
			return GetNode(index)->_data;
		}
		else
		{
			return NULL;
		}
	}

	bool Delete(int index)
	{
		node* target = GetNode(index);

		if (target != NULL)
		{
			if (target->_prev)
				target->_prev->_next = target->_next;

			if (target->_next)
				target->_next->_prev = target->_prev;

			if (target->_next == NULL)
				_tail = target->_prev;

			if (target->_prev == NULL)
				_head = target->_next;

			_len--;

			delete target;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Empty()
	{
		Destroy();
		Create();
	}

	bool Push(type data)
	{
		_head = new node(NULL, _head, data);
		
		if (_head)
		{
			if (_head->_next)
				_head->_next->_prev = _head;

			if (_tail == NULL)
				_tail = _head;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool Pop(type& data)
	{
		if (_head != NULL)
		{
			data = _head->_data;
			Delete(0);
			return true;
		}

		return false;
	}

	bool IsEmpty()
	{
		return (_head == NULL);
	}

	int Length()
	{
		return _len;
	}

protected:
	node* _head;
	node* _tail;
	int _len;
};

#endif
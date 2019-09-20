#pragma once
//==========================================================================================
// File Name: List.h
// Author: Brian Blackmon
// Date Created: 9/19/2019
// Purpose: 
// A container class for the list data structure.
//==========================================================================================
#include "Node.h"

template <class T>
class List
{
public:
	List()
		m_start(NULL), m_end(NULL)
	{

	}

	~List()
	{
		cleanUp();
	}

	void insert(int index, T item)
	{
		if (m_itemCount < index)
		{
			Node<T> *item = new Node<T>;

		}
	}

	void push(T item);
	void pop();
	void sort(bool ascending = true);

	T getItem(int index);
	int getSize();

private:
	Node<T> *m_start;
	Node<T> *m_end;

	int m_itemCount;

	void cleanUp()
	{
		if (m_start)
		{
			while (m_start != NULL)
			{
				Node<T> *current = m_start->m_next;

				delete m_start;

				m_start = current;
			}

			m_end = NULL;
		}
	}
};

#pragma once
//==========================================================================================
// File Name: Node.h
// Author: Brian Blackmon
// Date Created: 3/28/2019
// Purpose: 
// A node template class.
//==========================================================================================
#ifndef NULL
#define NULL nullptr;
#endif

template <class T>
class Node
{
public:
	Node(T data)
		:m_next(NULL), m_prev(NULL), m_data(data)
	{

	}

	~Node()
	{
		m_next = NULL;
		m_prev = NULL;
		m_data = NULL;
	}

	Node* m_next;
	Node* m_prev;

	T data() { return m_data; }
	void setData(T data) { m_data = data; }

private:
	T m_data;
};
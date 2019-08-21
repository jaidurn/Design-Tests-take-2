#pragma once
//==========================================================================================
// File Name: TreeNode.h
// Author: Brian Blackmon
// Date Created: 7/23/2019
// Purpose: 
// A node template class for trees.
//==========================================================================================
#include <type_traits>

template <class T>
class TreeNode
{
public:
	TreeNode(T data)
		:parent(NULL), left(NULL), right(NULL), m_data(data)
	{

	}

	~TreeNode()
	{
		parent = NULL;
		left = NULL;
		right = NULL;
	}

	T data() { return m_data; }
	void setData(T data) { m_data = data; }

	TreeNode<T> *parent;
	TreeNode<T> *left;
	TreeNode<T> *right;

private:
	T m_data;
};
#pragma once
//==========================================================================================
// File Name: GridNode.h
// Author: Brian Blackmon
// Date Created: 7/24/2019
// Purpose: 
// Holds the information for a grid style data structure.
//==========================================================================================
#include <type_traits>

template <class T>
class GridNode
{
public:
	GridNode(T data)
		:m_data(data), north(NULL), south(NULL), east(NULL), west(NULL)
	{

	}

	~GridNode()
	{
		north = NULL;
		south = NULL;
		west = NULL;
		east = NULL;

		if(std::is_pointer<m_data>::value)
		{
			m_data = NULL;
		}
	}

	GridNode<T> *north;
	GridNode<T> *south;
	GridNode<T> *west;
	GridNode<T> *east;

	void setData(T data) { m_data = data; }
	T data() { return m_data; }

private:
	T m_data;
};
#include "Grid.h"
#include <math.h>
#include <cassert>
#include <iostream>

Grid::Grid(int originX, int originY, int width, int height, int cellSize)
	:m_originX(originX), m_originY(originY), m_cellSize(cellSize), m_rowCount(0), m_columnCount(0)
{
	float columnCount = (float)(width) / (float)(cellSize);
	float rowCount = (float)(height) / (float)(cellSize);
	m_rowCount = (int)ceil(rowCount + 1);
	m_columnCount = (int)ceil(columnCount + 1);
	
	assert(0 < m_rowCount && 0 < m_columnCount);

	initializeGrid();
}


Grid::~Grid()
{
	// Clear the grid.
	for(unsigned int i = 0; i < m_grid.size(); i++)
	{
		for(unsigned int j = 0; j < m_grid[i].size(); j++)
		{
			delete m_grid[i][j];
		}

		m_grid[i].empty();
	}

	m_grid.empty();
}

Node<int>* Grid::getCoordinateCell(int x, int y)
{
	int workingX = x;
	int workingY = y;

	if (workingX < 0) { workingX = 0; }
	if (workingY < 0) { workingY = 0; }

	convertToGridCoordinates(workingX, workingY);

	return m_grid[workingX][workingY];
}

Node<int>* Grid::getCell(int cellX, int cellY)
{
	// Make sure we're not trying to use global coordinates
	Node<int> *cell = NULL;

	if (0 <= cellX && cellX < m_columnCount &&
		0 <= cellY && cellY < m_rowCount)
	{
		cell = m_grid[cellX][cellY];
	}

	return cell;
}

void Grid::add(int ID, int x, int y)
{
	convertToGridCoordinates(x, y);

//	std::cout << "Adding to cell: " << x << ", " << y << std::endl;
	
	Node<int> *node = new Node<int>(ID);

	// Make sure to attach the list
	if(m_grid[x][y])
	{
		node->m_next = m_grid[x][y];
		m_grid[x][y]->m_prev = node;
	}

	m_grid[x][y] = node;
}

void Grid::remove(int ID, int x, int y)
{
	convertToGridCoordinates(x, y);

	// Find and remove the ID
	if(m_grid[x][y])
	{
		Node<int> *node = m_grid[x][y];

		// If the ID is the first node
		if(node->data() == ID)
		{
			// Make the next node the head of the list
			if(node->m_next)
			{
				m_grid[x][y] = node->m_next;
				m_grid[x][y]->m_prev = NULL;
			}
			else
			{
				m_grid[x][y] = NULL;
			}

			delete node;
			node = NULL;
		}
		else
		{
			// Find the node and delete it
			while (node != NULL)
			{
				Node<int> *temp = node->m_next;

				if (temp != NULL)
				{
					// Delete the node and connect the list
					if (temp->data() == ID)
					{
						if (temp->m_next != NULL)
						{
							// Connect the list
							node->m_next = temp->m_next;
							temp->m_next->m_prev = node;
						}
						else
						{
							node->m_next = NULL;
						}

						delete temp;
						temp = NULL;
					}
				}

				node = node->m_next;
			}
		}
	}
}

void Grid::move(int ID, int oldX, int oldY, int movedX, int movedY)
{
	int workingOldX = oldX;
	int workingNewX = movedX;
	int workingOldY = oldY;
	int workingNewY = movedY;

	convertToGridCoordinates(workingOldX, workingOldY);
	convertToGridCoordinates(workingNewX, workingNewY);

	// Remove the ID from it's old location

	if (workingOldX != workingNewX ||
		workingOldY != workingNewY)
	{
		this->remove(ID, oldX, oldY);
		this->add(ID, movedX, movedY);
	}
}

Vector2D Grid::convertToCellCoordinates(Vector2D point)
{
	int x = (int)(round(point.getX()));
	int y = (int)(round(point.getY()));

	convertToGridCoordinates(x, y);

	return Vector2D((float)(x),(float)(y));
}

void Grid::initializeGrid()
{
	m_grid.resize(m_columnCount);

	for(unsigned int i = 0; i < m_grid.size(); i++)
	{
		m_grid[i].resize(m_rowCount);
	}
}

void Grid::convertToGridCoordinates(int& x, int& y)
{
	int workingX = x;
	int workingY = y;

	if (workingX < 0) { 
		workingX = 0; }
	if (workingY < 0) { workingY = 0; }

	workingX = (int)(workingX / m_cellSize);
	workingY = (int)(workingY / m_cellSize);

	// Make sure the coordinates are inside the boundaries
	if(m_columnCount <= workingX)
	{
		m_columnCount = workingX + 1;

		m_grid.resize(m_columnCount);

		for (int i = 0; i < m_columnCount; i++)
		{
			m_grid[i].resize(m_rowCount);
		}
	}

	if(m_rowCount <= workingY)
	{
		m_rowCount = workingY + 1;

		for (int i = 0; i < m_columnCount; i++)
		{
			m_grid[i].resize(m_rowCount);
		}
	}

	x = workingX;
	y = workingY;
}

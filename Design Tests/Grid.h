#pragma once
//==========================================================================================
// File Name: Grid.h
// Author: Brian Blackmon
// Date Created: 3/28/2019
// Purpose: 
// Stores a collection of objects in a grid.
//==========================================================================================
#include <vector>
#include "Vector2D.h"
#include "Node.h"

class Grid
{
public:
	Grid(int originX, int originY, int width, int height, int cellSize);
	~Grid();

	// Gets the cell the coordinates are in
	Node<int>* getCoordinateCell(int x, int y);
	// Gets the specific cell. DO NOT USE GLOBAL COORDINATES
	Node<int>* getCell(int cellX, int cellY);

	void add(int ID, int x, int y);
	void remove(int ID, int x, int y);
	void move(int ID, int oldX, int oldY, int movedX, int movedY);

	Vector2D convertToCellCoordinates(Vector2D point);

	int rowCount() { return m_rowCount; }
	int columnCount() { return m_columnCount; }
	int cellSize() { return m_cellSize; }

private:
	int m_originX;
	int m_originY;
	int m_cellSize;
	int m_rowCount;
	int m_columnCount;

	std::vector< std::vector< Node<int>* > > m_grid;

	void initializeGrid();
	void convertToGridCoordinates(int& x, int& y);
};


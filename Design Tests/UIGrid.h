#pragma once
//==========================================================================================
// File Name: UIGrid.h
// Author: Brian Blackmon
// Date Created: 9/30/2019
// Purpose: 
// Extends the UI Group class and adds grid functionality.
//==========================================================================================
#include "UIGroup.h"

class EntityDestroyMessage;
class InputMessage;

class UIGrid : public UIGroup
{
public:
	UIGrid(int entityID, 
		Vector2D position,
		int width,
		int height,
		int itemsPerRow,
		int rowCount);
	virtual ~UIGrid();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setVisible(bool visible);

	virtual GROUP_TYPE getGroupType() { return GROUP_GRID; }

	virtual void addItem(UIComponent *item);
	virtual void removeItem(int entityID);
	virtual void removeItem(string itemName);

	UIComponent* getCurrentItem();

	virtual void update();
	virtual void processMessage(IMessage *message);

protected:
	int m_rowCount;
	int m_itemsPerRow;
	int m_rowOffset;

	virtual void cleanUp();
	virtual void updatePositions();
	virtual void adjustCurrentItem(int amount);

	virtual void processInputMsg(InputMessage *inputMsg);
	virtual void processEntityDestroyMsg(EntityDestroyMessage *destroyMsg);
};


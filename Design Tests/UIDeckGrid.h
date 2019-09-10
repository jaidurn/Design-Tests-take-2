#pragma once
#include "UIMenu.h"
#include "Deck.h"

class InputMessage;

class UIDeckGrid : public UIMenu
{
public:
	UIDeckGrid(Vector2D position,
		int width,
		int height,
		string texturePath,
		int cardPerRow,
		int rowCount,
		Deck *deck);

	virtual ~UIDeckGrid();

	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setActive(bool active);
	virtual void setVisible(bool visible);

	virtual void update(float delta);
	virtual void processMessage(IMessage *message);

	virtual void addItem(int itemID, UIComponent *item);

private:
	int m_cardsPerRow;
	int m_rowCount;
	int m_rowOffset;

	int m_currentCard;

	void processInput(InputMessage *input);
	void decreaseCurrent(int amount);
	void increaseCurrent(int amount);
	void adjustCards();
	
	Deck *m_deck;
};


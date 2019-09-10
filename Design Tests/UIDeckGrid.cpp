#include "UIDeckGrid.h"
#include "MessageSystem.h"
#include "MoveMessage.h"
#include "RenderSystem.h"
#include "CameraMoveMessage.h"
#include "InputMessage.h"
#include "ResourceManager.h"

UIDeckGrid::UIDeckGrid(Vector2D position,
	int width,
	int height,
	string texturePath,
	int cardPerRow,
	int rowCount,
	Deck *deck)
	:UIMenu(position, width, height, texturePath),
	m_cardsPerRow(cardPerRow),
	m_rowCount(rowCount),
	m_deck(deck),
	m_rowOffset(0),
	m_currentCard(0)
{
}


UIDeckGrid::~UIDeckGrid()
{
	cleanUp();
}

//=============================================================================
// Function: void setWidth(int)
// Description:
// Sets the width of the grid and updates the width of all of the
// cards.
// Parameters:
// int width - The new width of the grid.
//=============================================================================
void UIDeckGrid::setWidth(int width)
{
	if (0 < width)
	{
		m_rect->setWidth(width);

		int cardWidth = width / m_cardsPerRow;

		auto mit = m_elements.begin();


		while (mit != m_elements.end())
		{
			mit->second->setWidth(cardWidth);
			mit++;
		}
	}
}

//=============================================================================
// Function: void setHeight(int)
// Description:
// Sets the height of the grid and resizes the height of the cards.
// Parameters:
// int height - The new height of the grid.
//=============================================================================
void UIDeckGrid::setHeight(int height)
{
	if (0 < height)
	{
		m_rect->setHeight(height);

		int cardHeight = m_rect->height() / m_rowCount;

		auto mit = m_elements.begin();
		
		while (mit != m_elements.end())
		{
			mit->second->setHeight(cardHeight);

			mit++;
		}
	}
}

//=============================================================================
// Function: void setActive(bool)
// Description:
// Sets the active state of the Deck Grid.
// Parameters:
// bool active - The new active state.
//=============================================================================
void UIDeckGrid::setActive(bool active)
{
	if (m_active != active)
	{
		if (m_active == false)
		{
			m_active = active;

			if (0 < m_rowOffset)
			{
				int cardsToUpdate = m_cardsPerRow * m_rowCount;
				int startingCard = m_cardsPerRow * m_rowOffset;
				for (int i = startingCard; i < cardsToUpdate + startingCard; i++)
				{
					if (i < m_deck->getCardCount())
					{
						auto mit = m_elements.begin();

						while (mit != m_elements.end())
						{
							if (mit->second->getEntityID() ==
								m_deck->getCard(i)->getEntityID())
							{
								mit->second->setActive(active);

								break;
							}

							mit++;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				int cardsToUpdate = m_cardsPerRow * m_rowCount;

				for (int i = 0; i < cardsToUpdate; i++)
				{
					if (i < m_deck->getCardCount())
					{
						auto mit = m_elements.begin();

						while (mit != m_elements.end())
						{
							if (mit->second->getEntityID() == m_deck->getCard(i)->getEntityID())
							{
								mit->second->setActive(active);

								break;
							}

							mit++;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			m_active = active;

			auto mit = m_elements.begin();

			while (mit != m_elements.end())
			{
				mit->second->setActive(active);
				mit++;
			}
		}
	}
}

//=============================================================================
// Function: void setVisible(bool)
// Description:
// Sets the visible state of the Deck Grid.
// Parameters:
// bool visible - The new visible state.
//=============================================================================
void UIDeckGrid::setVisible(bool visible)
{
	if (m_visible != visible)
	{
		if (m_visible == false)
		{
			m_visible = visible;

			int startingCard = m_rowOffset * m_cardsPerRow;
			int endingCard = (m_rowCount * m_cardsPerRow) + startingCard;

			UIComponent *background = getItem(m_backgroundID);

			background->setVisible(m_visible);

			if (m_deck->getCardCount() <= endingCard)
			{
				endingCard = m_deck->getCardCount() - 1;
			}

			for (int i = startingCard; i <= endingCard; i++)
			{
				UIComponent *card = getItem(m_deck->getCard(i)->getEntityID());

				if (card)
				{
					card->setVisible(m_visible);
				}
			}
		}
		else
		{
			m_visible = visible;

			auto mit = m_elements.begin();

			while (mit != m_elements.end())
			{
				mit->second->setVisible(visible);
				mit++;
			}
		}
	}
}

//=============================================================================
// Function: void update(float)
// Description:
// Updates the Deck Grid.
// Parameters:
// float delta - The time passed since last update.
//=============================================================================
void UIDeckGrid::update(float delta)
{
	if (m_active)
	{
		int startingCard = m_cardsPerRow * m_rowOffset;
		int endingCard = (m_rowCount * m_cardsPerRow) + startingCard;

		if (m_deck->getCardCount() <= endingCard)
		{
			endingCard = m_deck->getCardCount() - 1;
		}

		for (int i = startingCard; i < endingCard; i++)
		{
			UIComponent *card = getItem(m_deck->getCard(i)->getEntityID());

			if (card)
			{
				card->update(delta);
			}
		}
	}
}

//=============================================================================
// Function: void processMessage(IMessage*)
// Description:
// Handles messages passed to it and passes the messages to
// it's elements.
// Parameters:
// IMessage *message - The message to process.
//=============================================================================
void UIDeckGrid::processMessage(IMessage *message)
{
	if (message)
	{
		switch (message->type())
		{
		case IMessage::INPUT:
		{
			if (m_active)
			{
				InputMessage *input = static_cast<InputMessage*>(message);

				processInput(input);
			}

			break;
		}
		case IMessage::CAMERA_MOVE:
		{
			CameraMoveMessage *camera = static_cast<CameraMoveMessage*>(message);

			Vector2D menuDifference = m_rect->center() - camera->m_oldPosition;

			this->setPosition(camera->m_newPosition + menuDifference);

			break;
		}
		}
	}
}

//=============================================================================
// Function: void addItem(int, UIComponent*)
// Description:
// Adds a UI item to the Deck Grid and ties it to the ID.
// Parameters:
// int itemID - The ID of the item to add.
// UIComponent *item - The item to add.
//=============================================================================
void UIDeckGrid::addItem(int itemID, UIComponent *item)
{
	if (item)
	{
		auto mit = m_elements.find(itemID);

		if (itemID != m_backgroundID)
		{
			item->setWidth(m_rect->width() / m_cardsPerRow);
			item->setHeight(m_rect->height() / m_rowCount);
		}
		else
		{
			item->setWidth(m_rect->width());
			item->setHeight(m_rect->height());
			item->setPosition(m_rect->center());
		}

		item->setActive(m_active);
		item->setVisible(m_visible);

		if (mit != m_elements.end())
		{
			delete mit->second;

			mit->second = item;

			item->setActive(m_active);
			item->setVisible(m_visible);
		}
		else
		{
			m_elements.insert(std::make_pair(itemID, item));
		}

		adjustCards();
	}
	else
	{
		adjustCards();
	}
}

void UIDeckGrid::processInput(InputMessage *input)
{
	switch (input->m_inputType)
	{
	case InputMessage::INPUT_AXIS:
	{
		InputAxisMessage *axis = static_cast<InputAxisMessage*>(input);

		if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
			if (axis->m_axisMovement < 0)
			{
				decreaseCurrent(1);
			}
			else if (0 < axis->m_axisMovement)
			{
				increaseCurrent(1);
			}
		}
		else if (axis->m_axis == SDL_CONTROLLER_AXIS_LEFTY)
		{
			if (axis->m_axisMovement < 0)
			{
				decreaseCurrent(m_cardsPerRow);
			}
			else if (0 < axis->m_axisMovement)
			{
				increaseCurrent(m_cardsPerRow);
			}
		}

		break;
	}
	case InputMessage::INPUT_BUTTON:
	{
		InputButtonMessage *button = static_cast<InputButtonMessage*>(input);

		if (button->m_pressed)
		{
			switch (button->m_button)
			{
			case SDLK_LEFT:
			{
				decreaseCurrent(1);
				break;
			}
			case SDLK_RIGHT:
			{
				increaseCurrent(1);
				break;
			}
			case SDLK_UP:
			{
				decreaseCurrent(m_cardsPerRow);
				break;
			}
			case SDLK_DOWN:
			{
				increaseCurrent(m_cardsPerRow);
				break;
			}
			}
		}

		break;
	}
	case InputMessage::INPUT_MOVE:
	{
		break;
	}
	}
}

void UIDeckGrid::decreaseCurrent(int amount)
{
	UIComponent *oldCard = getItem(m_deck->getCard(m_currentCard)->getEntityID());

	Font *font = ResourceManager::instance()->getFont("Resources/Fonts/RobotoMono-", 16, Font::FONT_REGULAR, false);

	SDL_Color red{ 255, 0, 0, 255 };

	if (oldCard && font)
	{
		if (oldCard->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(oldCard);

			button->setText(" ", font, red);
		}
	}

	if (0 < m_currentCard - amount)
	{
		m_currentCard -= amount;
	}
	else
	{
		m_currentCard = 0;
	}

	UIComponent *newCard = getItem(m_deck->getCard(m_currentCard)->getEntityID());

	if (newCard && font)
	{
		if (newCard->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(newCard);

			button->setText("X", font, red);
		}
	}

	int newRow = m_currentCard / m_cardsPerRow;

	if (newRow < m_rowOffset)
	{
		m_rowOffset = newRow;
		adjustCards();
	}
}

void UIDeckGrid::increaseCurrent(int amount)
{
	UIComponent *oldCard = getItem(m_deck->getCard(m_currentCard)->getEntityID());

	Font *font = ResourceManager::instance()->getFont("Resources/Fonts/RobotoMono-", 16, Font::FONT_REGULAR, false);

	SDL_Color red{ 255, 0, 0, 255 };

	if (oldCard && font)
	{
		if (oldCard->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(oldCard);

			button->setText(" ", font, red);
		}
	}

	if (m_currentCard + amount < m_deck->getCardCount() - 1)
	{
		m_currentCard += amount;
	}
	else
	{
		m_currentCard = m_deck->getCardCount() - 1;
	}

	UIComponent *newCard = getItem(m_deck->getCard(m_currentCard)->getEntityID());

	if (newCard && font)
	{
		if (newCard->getType() == UIComponent::UI_BUTTON)
		{
			UIButton *button = static_cast<UIButton*>(newCard);

			button->setText("X", font, red);
		}
	}

	int newRow = m_currentCard / m_cardsPerRow;

	if (m_rowOffset + m_rowCount - 1 < newRow)
	{
		m_rowOffset = newRow - (m_rowCount - 1);
		adjustCards();
	}
}

void UIDeckGrid::adjustCards()
{
	float cardWidth = (float)(m_rect->width() / m_cardsPerRow);
	float cardHeight = (float)(m_rect->height() / m_rowCount);
	float startingX = m_rect->getTopLeft().getX() + (cardWidth / 2.0f);
	float startingY = m_rect->getTopLeft().getY() + (cardHeight / 2.0f) - (cardHeight * m_rowOffset);
	float currentX = startingX;
	float currentY = startingY;

	int currentRow = 0;

	for (int i = 0; i < m_deck->getCardCount(); i++)
	{
		currentRow = i / m_cardsPerRow;

		UIComponent *card = getItem(m_deck->getCard(i)->getEntityID());

		if (card)
		{
			card->setPosition(Vector2D(currentX, currentY));

			if (currentRow < m_rowOffset ||
				m_rowOffset + m_rowCount - 1 < currentRow)
			{
				card->setActive(false);
				card->setVisible(false);
			}
			else
			{
				card->setActive(m_active);
				card->setVisible(m_visible);
			}
		}

		currentX += cardWidth;

		if (m_rect->getTopLeft().getX() + (float)m_rect->width() <= currentX)
		{
			currentX = startingX;
			currentY += cardHeight;
		}
	}
}
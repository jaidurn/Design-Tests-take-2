#pragma once
//==========================================================================================
// File Name: 
// Author: 
// Date Created: 
// Purpose: 
// Extends the UIComponent class and adds button functionality.
//==========================================================================================
#include "UIComponent.h"

class UIText;
class UIGraphic;
class IMessage;
class InputMessage;
class EntityDestroyMessage;

class UIButton : public UIComponent
{
public:
	UIButton(int entityID, Vector2D position, string name, bool toggle);
	virtual ~UIButton();

	virtual void setPosition(Vector2D position);
	virtual void setWidth(int width);
	virtual void setHeight(int height);
	virtual void setVisible(bool visible);
	virtual void setActive(bool active);
	virtual void setSelected(bool selected);

	void setTextOffset(Vector2D offset);
	void setTextUI(UIText *textUI);
	void setText(string text);

	void setBackground(UIGraphic *background);
	
	void setPressed(bool pressed);
	
	bool getPressed();
	bool getReleased();
	bool getHeld();

	void processMessage(IMessage *message);
	void update();

private:
	UIText *m_textUI;
	UIGraphic *m_background;
	
	bool m_selected;
	bool m_toggle;
	bool m_pressed;
	bool m_wasPressed;

	void processInput(InputMessage *inputMsg);
	void processEntityDestroy(EntityDestroyMessage *destroyMsg);
	void updateSelected();
};


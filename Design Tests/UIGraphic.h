#pragma once
//==========================================================================================
// File Name: UIGraphic.h
// Author: Brian Blackmon
// Date Created: 9/11/2019
// Purpose: 
// Extends the UIComponent class and adds functionality for graphical UI.
//==========================================================================================
#include "UIComponent.h"
class UIGraphic : public UIComponent
{
public:
	UIGraphic(int entityID, Vector2D position, string name, string texturePath);
	virtual ~UIGraphic();

private:

};


#include "AnimationComponent.h"
#include <cmath>
#include <iostream>

AnimationComponent::AnimationComponent()
	:Component(ANIMATION), m_currentName(""), m_currentFrameIndex(0), m_currentDirection(DIR_DOWN)
{

}

AnimationComponent::~AnimationComponent()
{

}

//=============================================================================
// Function: void update()
// Description:
// Increases the current index by the speed.
//=============================================================================
void AnimationComponent::update()
{
	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			m_currentFrameIndex += mit->second.speed();

			if(m_currentFrameIndex < 0.0f)
			{
				if(mit->second.loop())
				{
					m_currentFrameIndex = (float)mit->second.frameCount(m_currentDirection) - 1.0f;
				}
				else
				{
					m_currentFrameIndex = 0.0f;
				}
			}
			else if(mit->second.frameCount(m_currentDirection) <= m_currentFrameIndex)
			{
				if(mit->second.loop())
				{
					m_currentFrameIndex = 0.0f;
				}
				else
				{
					m_currentFrameIndex = (float)mit->second.frameCount(m_currentDirection) - 1.0f;
				}
			}
		}
	}
}

//=============================================================================
// Function: string currentAnimation()
// Description:
// Gets the current animation name.
// Output:
// The name of the current animation.
// Returns "" if there isn't one.
//=============================================================================
std::string AnimationComponent::currentAnimation()
{
	return m_currentName;
}

//=============================================================================
// Function: SDL_Rect* currentFrame()
// Description:
// Gets the current Clip of the animation.
// Output:
// SDL_Rect* - A pointer to the current clip.
//=============================================================================
SDL_Rect* AnimationComponent::currentFrame()
{
	SDL_Rect *clip = NULL;

	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			int frameIndex = currentFrameIndex();
			
			if(frameIndex < mit->second.frameCount(m_currentDirection))
			{
				clip = mit->second.getFrame(m_currentDirection, frameIndex);
			}
		}
	}

	return clip;
}

//=============================================================================
// Function: float currentSpeed()
// Description:
// Gets the speed of the current animation.
// Output:
// Returns the current speed of the animation on success.
// Returns 0.0 on failure.
//=============================================================================
float AnimationComponent::currentSpeed()
{
	float speed = 0.0f;

	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			speed = mit->second.speed();
		}
	}

	return speed;
}

//=============================================================================
// Function: bool currentLoop()
// Description:
// Gets the loop status of the current animation.
// Output:
// Returns the current animation's loop status.
// Returns false if no animation.
//=============================================================================
bool AnimationComponent::currentLoop()
{
	bool loop = false;

	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			loop = mit->second.loop();
		}
	}

	return loop;
}

//=============================================================================
// Function: int currentFrameIndex()
// Description:
// Gets the current frame index by rounding it down.
// Output:
// Returns the frame index as an int
//=============================================================================
int AnimationComponent::currentFrameIndex()
{
	return (int)floor(m_currentFrameIndex);
}

//=============================================================================
// Function: int currentAnimationLength()
// Description:
// Gets the length of the current animation.
// Output:
// Returns the length of the current animation. 
// Returns -1 if there isn't one.
//=============================================================================
int AnimationComponent::currentAnimationLength()
{
	int length = -1;

	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			length = mit->second.frameCount(m_currentDirection);
		}
	}

	return length;
}

//=============================================================================
// Function: int animationLength(string)
// Description:
// Gets the length of the specified animation.
// Parameters:
// string name - The name of the animation to find.
// Output:
// Returns the length of the animation.
// Returns -1 on failure.
//=============================================================================
int AnimationComponent::animationLength(std::string name)
{
	int length = -1;

	if(name != "")
	{
		auto mit = m_animations.find(name);

		if(mit != m_animations.end())
		{
			length = mit->second.frameCount(m_currentDirection);
		}
	}

	return length;
}

//=============================================================================
// Function: void addAnimation(string)
// Description:
// Adds an animation if it doesn't exist.
// Parameters:
// string name - The name of the animation to create.
//=============================================================================
void AnimationComponent::addAnimation(std::string name, bool loop, float speed, int directionCount)
{
	if(name != "")
	{
		// Make sure there isn't an animation by this name already.
		auto mit = m_animations.find(name);

		if(mit == m_animations.end())
		{
			m_animations.insert(std::make_pair(name, Animation(loop, speed, directionCount)));
		}
	}
}

//=============================================================================
// Function: void addFrame(string, SDL_Rect)
// Description:
// Adds the frame to the specified animation if it exists.
// Currently, it won't create an animation for the frame.
// Parameters:
// string name - The name of the animation to add the frame to.
// SDL_Rect frame - The frame to add.
//=============================================================================
void AnimationComponent::addFrame(std::string name, Direction direction, SDL_Rect frame)
{
	if(name != "")
	{
		// Make sure there actually is frame information.
		if(frame.w != 0 && frame.h != 0)
		{
			auto mit = m_animations.find(name);

			if(mit != m_animations.end())
			{
				mit->second.addFrame(direction, frame);
			}
		}
	}
}

//=============================================================================
// Function: void setAnimation(string)
// Description:
// Sets the current animation to the animation name if it exists.
// Parameters:
// string name - The name of the animation to change to.
//=============================================================================
void AnimationComponent::setAnimation(std::string name)
{
	if(name != "")
	{
		auto mit = m_animations.find(name);

		if(mit != m_animations.end())
		{
			m_currentName = mit->first;

			if(mit->second.frameCount(m_currentDirection) <= m_currentFrameIndex)
			{
				resetIndex();
			}
		}
	}
}

void AnimationComponent::setCurrentDirection(Direction direction)
{
	m_currentDirection = direction;
}

//=============================================================================
// Function: void setCurrentFrame(float)
// Description:
// Sets the current frame to the specified frame.
// Parameters:
// float frame - The frame to set the animation to.
//=============================================================================
void AnimationComponent::setCurrentFrame(float frame)
{
	m_currentFrameIndex = frame;
}

//=============================================================================
// Function: void setSpeed(float)
// Description:
// Sets the animation speed.
// Parameters:
// float speed - The speed to animate the frames at.
// 1.0f = 1 frame per tick.
//=============================================================================
void AnimationComponent::setCurrentSpeed(float speed)
{
	if(m_currentName != "")
	{
		auto mit = m_animations.find(m_currentName);

		if(mit != m_animations.end())
		{
			mit->second.setSpeed(speed);
		}
	}
}

//=============================================================================
// Function: void setAnimationSpeed(string, float)
// Description:
// Updates the speed of the specified animation.
// Parameters:
// string name - The name of the animation to update.
// float speed - The speed to update to.
//=============================================================================
void AnimationComponent::setAnimationSpeed(std::string name, float speed)
{
	if(name != "")
	{
		auto mit = m_animations.find(name);

		if(mit != m_animations.end())
		{
			mit->second.setSpeed(speed);
		}
	}
}

//=============================================================================
// Function: void resetIndex()
// Description:
// Resets the index to 0.
//=============================================================================
void AnimationComponent::resetIndex()
{
	m_currentFrameIndex = 0.0f;
}
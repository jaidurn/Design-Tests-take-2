#include "Animation.h"



Animation::Animation(bool loop, float speed, int directionCount)
	:m_loop(loop), m_speed(speed), m_directionCount(directionCount)
{
	m_frames.resize(m_directionCount);
}


Animation::~Animation()
{
}

//=============================================================================
// Function: void addFrame(int direction, SDL_Rect frame)
// Description:
// Adds the frame to the frames of the current direction.
// Parameters:
// int direction - The direction to add the frame to.
// SDL_Rect frame - The frame to add.
//=============================================================================
void Animation::addFrame(int direction, SDL_Rect frame)
{
	if (0 <= direction && direction < m_directionCount)
	{
		m_frames[direction].push_back(frame);
	}
}

int Animation::frameCount(int direction)
{
	int frameCount = -1;

	if(0 <= direction && direction < m_directionCount)
	{
		frameCount = m_frames[direction].size();
	}

	return frameCount;
}

//=============================================================================
// Function: SDL_Rect* getFrame(int)
// Description:
// If the index is within the frame vector, it returns a pointer to the frame.
// Parameters:
// int index - The index of the SDL_Rect.
// Output:
// SDL_Rect* - The rect found.
// Returns NULL if one isn't found.
//=============================================================================
SDL_Rect* Animation::getFrame(int direction, int frameIndex)
{
	SDL_Rect *frame = NULL;

	if (0 <= direction && direction < m_directionCount)
	{
		if (0 <= frameIndex && frameIndex < m_frames[direction].size())
		{
			frame = &m_frames[direction][frameIndex];
		}
	}

	return frame;
}


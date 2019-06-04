#include "Animation.h"



Animation::Animation(bool loop, float speed)
	:m_loop(loop), m_speed(speed), m_frameCount(0)
{

}


Animation::~Animation()
{
}

//=============================================================================
// Function: void addFrame(SDL_Rect frame)
// Description:
// Adds the frame to the current frames.
// Parameters:
// SDL_Rect frame - The frame to add.
//=============================================================================
void Animation::addFrame(SDL_Rect frame)
{
	m_frames.push_back(frame);
	m_frameCount++;
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
SDL_Rect* Animation::getFrame(int frameIndex)
{
	SDL_Rect *frame = NULL;

	if(0 <= frameIndex && frameIndex < m_frameCount)
	{
		frame = &m_frames[frameIndex];
	}

	return frame;
}
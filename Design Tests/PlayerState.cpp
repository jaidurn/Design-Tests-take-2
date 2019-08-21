#include "PlayerState.h"



PlayerState::PlayerState(int entityID, std::string stateName)
	:IState(entityID), m_name(stateName)
{
}


PlayerState::~PlayerState()
{
}

void PlayerState::processMessage(IMessage *message)
{

}

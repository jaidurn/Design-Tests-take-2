#include "EnemyLogicComponent.h"
#include "EnemyTargetState.h"

EnemyLogicComponent::EnemyLogicComponent()
	:LogicComponent(LOGIC_ENEMY), m_currentState(NULL), m_currentStateName(""), m_currentTarget(-1), m_behavior(EnemyState::BEHAVIOR_AGGRESSIVE)
{

}


EnemyLogicComponent::~EnemyLogicComponent()
{

}

//=============================================================================
// Function: void addState(string, IState*)
// Description:
// Adds the state to the list of states under the name specified. Defaults
// the weight to 0.
// Parameters:
// string stateName - The name to add the state under.
// IState *state - The state to add.
//=============================================================================
void EnemyLogicComponent::addState(std::string stateName, IState *state)
{
	addState(stateName, state, 0.0f);
}

//=============================================================================
// Function: void addState(string, IState*, float)
// Description:
// Adds a state under the specified state name. It also sets the weight
// of the state.
// Parameters:
// string stateName - The name of the state to add.
// IState *state - The state to add.
// float weight - The weight of the state.
//=============================================================================
void EnemyLogicComponent::addState(std::string stateName, IState *state, float weight)
{
	if(stateName != "")
	{
		auto mit = m_states.find(stateName);

		if(mit != m_states.end())
		{
			delete state;
		}
		else
		{
			m_states.insert(std::make_pair(stateName, state));
			m_statesByValue.push_back(stateName);
			sortStatesByValue();
		}
	}
	else
	{
		delete state;
	}
}

void EnemyLogicComponent::setBehavior(EnemyState::Behavior behavior)
{
	m_behavior = behavior;
}

//=============================================================================
// Function: void removeState(string)
// Description:
// Removes the state from the list of states.
// Parameters:
// string stateName - The name of the state to remove.
//=============================================================================
void EnemyLogicComponent::removeState(std::string stateName)
{
	if(stateName != "")
	{
		auto mit = m_states.find(stateName);

		if(mit != m_states.end())
		{
			delete mit->second;
			mit = m_states.erase(mit);
		}
	}
}

//=============================================================================
// Function: IState* getState(string)
// Description:
// Gets the state related to the state name.
// Parameters:
// string stateName - The name of the state to get.
// Output:
// IState* - Returns a pointer to the state on success.
// Returns NULL on failure.
//=============================================================================
IState* EnemyLogicComponent::getState(std::string stateName)
{
	IState *state = NULL;

	if(stateName != "")
	{
		auto mit = m_states.find(stateName);

		if(mit != m_states.end())
		{
			state = mit->second;
		}
	}

	return state;
}

//=============================================================================
// Function: IState* currentState()
// Description:
// Gets the data for the current state.
// Output:
// IState* - Returns the data of the current state.
// Returns NULL on failure.
//=============================================================================
IState* EnemyLogicComponent::currentState()
{
	return m_currentState;
}

//=============================================================================
// Function: void changeState(string)
// Description:
// Changes the state to the specified state name if it exists.
// Parameters:
// string stateName - The name of the state to change to.
//=============================================================================
void EnemyLogicComponent::changeState(std::string stateName)
{
	if (stateName != "")
	{
		auto mit = m_states.find(stateName);

		if(mit != m_states.end())
		{
			m_currentState = mit->second;
			m_currentStateName = mit->first;
		}
	}
}

//=============================================================================
// Function: void update()
// Description:
// Looks through the states and finds the next one able to function.
//=============================================================================
void EnemyLogicComponent::update()
{
	int it = 0;
	bool stateEntered = false;

	if (m_states.size() != 0)
	{
		std::string name = m_statesByValue[it];
		IState *state = getState(name);
		EnemyState *enemy = static_cast<EnemyState*>(state);

		if (m_currentState)
		{
			EnemyState *current = static_cast<EnemyState*>(m_currentState);

			if (current->canExit())
			{
				while (!stateEntered && (unsigned int)it < m_statesByValue.size())
				{
					name = m_statesByValue[it];
					state = getState(name);
					enemy = static_cast<EnemyState*>(state);

					if (enemy->canEnter(m_currentTarget, m_behavior))
					{
						stateEntered = true;
					}

					it++;
				}

				if (stateEntered)
				{
					if (m_currentStateName != name)
					{
						if (m_currentState)
						{
							m_currentState->exit();
						}

						enemy->enter();

						m_currentStateName = name;
						m_currentState = state;
					}

					m_currentState->update();

					if (m_currentStateName == "Target")
					{
						m_currentTarget = static_cast<EnemyTargetState*>(m_currentState)->currentTarget();
					}
				}
			}
			else
			{
				m_currentState->update();
			}
		}
		else
		{
			while (!stateEntered && (unsigned int)it < m_statesByValue.size())
			{
				name = m_statesByValue[it];
				state = getState(name);
				enemy = static_cast<EnemyState*>(state);

				if (enemy->canEnter(m_currentTarget, m_behavior))
				{
					stateEntered = true;
				}

				it++;
			}

			if (stateEntered)
			{
				if (m_currentStateName != name)
				{
					if (m_currentState)
					{
						m_currentState->exit();
					}

					enemy->enter();

					m_currentStateName = name;
					m_currentState = state;
				}

				m_currentState->update();

				if (m_currentStateName == "Target")
				{
					m_currentTarget = static_cast<EnemyTargetState*>(m_currentState)->currentTarget();
				}
			}
		}
	}
}

void EnemyLogicComponent::cleanUp()
{
	auto mit = m_states.begin();

	while(mit != m_states.end())
	{
		delete mit->second;
		mit = m_states.erase(mit);

		if(mit != m_states.end())
		{
			mit++;
		}
	}
}

void EnemyLogicComponent::sortStatesByValue()
{
	// Find a way to sort.
}
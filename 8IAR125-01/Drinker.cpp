#include "Drinker.h"

bool Drinker::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drinker::Update()
{
	m_iThirst += 1;

	m_pStateMachine->Update();
}

bool Drinker::Thirsty()const
{
	if (m_iThirst >= ThirstLevel_drinker) { return true; }

	return false;
}

bool Drinker::Fighty()const
{
	if (m_Life > 0) { return true;  }

	return false;
}

bool Drinker::Rested()const
{
	if (m_Life >= MaxHealth_drinker) { return true; }

	return false;
}
#include "Drinker.h"

bool Drinker::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drinker::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_iThirst += 1;

	m_pStateMachine->Update();
}

bool Drinker::Thirsty()const
{
	if (m_iThirst >= ThirstLevel_drinker) { return true; }

	return false;
}


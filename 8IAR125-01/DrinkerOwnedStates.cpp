#include "DrinkerOwnedStates.h"
#include "fsm/State.h"
#include "Drinker.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"
#include "CoutMutex.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------
DrinkInTheBar* DrinkInTheBar::Instance()
{
	static DrinkInTheBar instance;

	return &instance;
}


void DrinkInTheBar::Enter(Drinker* drinker)
{
	if (drinker->Location() != saloon)
	{
		drinker->Sleep();
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Startin' to drink";
		CoutLock->Unlock();
		drinker->ChangeLocation(saloon);
	}
}


void DrinkInTheBar::Execute(Drinker* drinker)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Drinkin' a whiskey";
	CoutLock->Unlock();
}


void DrinkInTheBar::Exit(Drinker* drinker)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm stopping from drinking !";
	CoutLock->Unlock();
}


bool DrinkInTheBar::OnMessage(Drinker* drinker, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_BobIsInTheBar:
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\nMessage handled by " << GetNameOfEntity(drinker->ID())
			<< " at time: " << Clock->GetCurrentTime();
		CoutLock->Unlock();

		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(drinker->ID())
			<< ": Oh no Bob is here, i want to hurt him so bad with no reason '!";
		CoutLock->Unlock();
		drinker->GetFSM()->ChangeState(Fight::Instance());

		return true;

	}
	
	return false;
}

//------------------------------------------------------------------------
Fight* Fight::Instance()
{
	static Fight instance;

	return &instance;
}


void Fight::Enter(Drinker* drinker)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Startin' to fight with Bob";
	CoutLock->Unlock();
	drinker->ChangeLocation(saloon);

	//let Bob know I'm angry
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		drinker->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_LouisIsAngry,   //the message
		NO_ADDITIONAL_INFO);

}


void Fight::Execute(Drinker* drinker)
{
	if (drinker->Fighty()) {
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Fightin' with Bob";
		CoutLock->Unlock();
		drinker->DecreaseLife();
	}
	else {
		drinker->GetFSM()->ChangeState(GoHomeAndRest::Instance());
	}
}


void Fight::Exit(Drinker* drinker)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm bored, I think I'll go home now !";
	CoutLock->Unlock();
	//let Bob know I'm ok now
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		drinker->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_EndOfFight,   //the message
		NO_ADDITIONAL_INFO);
}


bool Fight::OnMessage(Drinker* drinker, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------
GoHomeAndRest* GoHomeAndRest::Instance()
{
	static GoHomeAndRest instance;

	return &instance;
}


void GoHomeAndRest::Enter(Drinker* drinker)
{
	if (drinker->Location() != saloon)
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Home, sweet home";
		CoutLock->Unlock();
		drinker->ChangeLocation(saloon);
	}
}


void GoHomeAndRest::Execute(Drinker* drinker)
{
	drinker->IncreaseThirst();
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Snorin'";
	CoutLock->Unlock();
	drinker->IncreaseLife();

	if (drinker->Rested())
	{
		drinker->GetFSM()->ChangeState(DrinkInTheBar::Instance());
	}
}


void GoHomeAndRest::Exit(Drinker* drinker)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm Thirsty, I think I'll go to the saloon now !";
	CoutLock->Unlock();
}


bool GoHomeAndRest::OnMessage(Drinker* drinker, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

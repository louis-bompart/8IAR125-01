#include "DrinkerOwnedStates.h"
#include "fsm/State.h"
#include "Drinker.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

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
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Walkin' to the saloon";

		drinker->ChangeLocation(saloon);
	}
}


void DrinkInTheBar::Execute(Drinker* drinker)
{
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Drinkin' a whiskey";
}


void DrinkInTheBar::Exit(Drinker* drinker)
{
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm leaving the saloon !";
}


bool DrinkInTheBar::OnMessage(Drinker* drinker, const Telegram& msg)
{
	/*SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Okay Hun, ahm a comin'!";

		pMiner->GetFSM()->ChangeState(Fight::Instance());

		return true;

	}//end switch
	*/
	//send msg to global message handler
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
	if (drinker->Location() != saloon)
	{
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Startin' to fight with Bob";

		drinker->ChangeLocation(saloon);
	}
}


void Fight::Execute(Drinker* drinker)
{
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Fightin' with Bob";
}


void Fight::Exit(Drinker* drinker)
{
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm bored, I think I'll go home now !";
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
		cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Home, sweet home";

		drinker->ChangeLocation(saloon);
	}
}


void GoHomeAndRest::Execute(Drinker* drinker)
{
	drinker->IncreaseThirst();
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": " << "Snorin'";
}


void GoHomeAndRest::Exit(Drinker* drinker)
{
	cout << "\n" << GetNameOfEntity(drinker->ID()) << ": "
		<< "I'm Thirsty, I think I'll go to the saloon now !";
	
	if (drinker->Thirsty())
	{
		drinker->GetFSM()->ChangeState(DrinkInTheBar::Instance());
	}
}


bool GoHomeAndRest::OnMessage(Drinker* drinker, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

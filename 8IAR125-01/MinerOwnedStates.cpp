#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
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


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";
		CoutLock->Unlock();

		pMiner->ChangeLocation(goldmine);
	}
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";
	CoutLock->Unlock();
	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
	CoutLock->Unlock();
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";
		CoutLock->Unlock();
		pMiner->ChangeLocation(bank);
	}
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "Depositing gold. Total savings now: " << pMiner->Wealth();
	CoutLock->Unlock();
	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "WooHoo! Rich enough for now. Back home to mah li'lle lady";
		CoutLock->Unlock();
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}
	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
	CoutLock->Unlock();
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";
		CoutLock->Unlock();
		pMiner->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "All mah fatigue has drained away. Time to find more gold!";
		CoutLock->Unlock();
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
		CoutLock->Unlock();
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	

	switch (msg.Msg)
	{
	case Msg_StewReady:
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Okay Hun, ahm a comin'!";
		CoutLock->Unlock();
		pMiner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
		CoutLock->Unlock();
		//let Louis know I'm in the saloon
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Louis,            //ID of recipient
			Msg_BobIsInTheBar,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	pMiner->BuyAndDrinkAWhiskey();
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";
	CoutLock->Unlock();
	pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	//  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bar";
	
}


void QuenchThirst::Exit(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Stoppin' from drinking";
	CoutLock->Unlock();
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	
	switch (msg.Msg)
	{
	case Msg_LouisIsAngry:
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": I'll Fight !";
		CoutLock->Unlock();
		pMiner->GetFSM()->ChangeState(FightBob::Instance());

		return true;

	}//end switch
	CoutLock->Unlock();

	return false;
}

//------------------------------------------------------------------------Fight

FightBob* FightBob::Instance()
{
	static FightBob instance;

	return &instance;
}

void FightBob::Enter(Miner* pMiner)
{

}

void FightBob::Execute(Miner* pMiner)
{
	//pMiner->Fight();
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Fightin' like a baws";
	CoutLock->Unlock();
}


void FightBob::Exit(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Stoppin' from fighting";
	CoutLock->Unlock();
}


bool FightBob::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
	case Msg_EndOfFight:
		while (CoutLock->Lock())
		{
			Sleep(100);
		}
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Stop Fight !";
		CoutLock->Unlock();
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());

		return true;

	}//end switch
	return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
	CoutLock->Unlock();
}

void EatStew::Execute(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";
	CoutLock->Unlock();
	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	while (CoutLock->Lock())
	{
		Sleep(100);
	}
	SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
	CoutLock->Unlock();
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}



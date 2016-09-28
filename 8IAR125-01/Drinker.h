#ifndef DRINKER_H
#define DRINKER_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrinkerOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State;

struct Telegram;

//above this value a miner is thirsty
const int ThirstLevel_drinker = 3;
const int MaxHealth_drinker = 5;

class Drinker : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Drinker>*  m_pStateMachine;

	location_type         m_Location;

	//the higher the value, the thirstier the miner
	int                   m_iThirst;

	//How bad the drinker wants to fight
	int					  m_Life;


public:

	Drinker(int id) :m_Location(shack),
		m_iThirst(0),
		m_Life(5),
		BaseGameEntity(id)

	{
		//set up state machine
		m_pStateMachine = new StateMachine<Drinker>(this);

		m_pStateMachine->SetCurrentState(DrinkInTheBar::Instance());
	}

	~Drinker() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Drinker>* GetFSM()const { return m_pStateMachine; }

	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          Thirsty()const;
	void          DecreaseThirst() { m_iThirst -= 1; }
	void          IncreaseThirst() { m_iThirst += 1; }

	void          DrinkAWhiskey() { m_iThirst = 0; }

	bool		  Fighty()const;
	void		  DecreaseLife() { m_Life--; }
	void		  IncreaseLife() { m_Life++; }

	bool	      Rested()const;
	void		  Sleep() { m_Life = 5; }

};



#endif

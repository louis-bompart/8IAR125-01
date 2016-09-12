#ifndef DRINKER_OWNED_STATES_H
#define DRINKER_OWNED_STATES_H

#include "fsm/State.h"

class Drinker;

//------------------------------------------------------------------------
class DrinkInTheBar : public State<Drinker>
{
private:

	DrinkInTheBar() {}

	//copy ctor and assignment should be private
	DrinkInTheBar(const DrinkInTheBar&);
	DrinkInTheBar& operator=(const DrinkInTheBar&);

public:

	//this is a singleton
	static DrinkInTheBar* Instance();

	virtual void Enter(Drinker* drinker);

	virtual void Execute(Drinker* drinker);

	virtual void Exit(Drinker* drinker);

	virtual bool OnMessage(Drinker* drinker, const Telegram& msg);
};

//------------------------------------------------------------------------
class Fight : public State<Drinker>
{
private:

	Fight() {}

	//copy ctor and assignment should be private
	Fight(const DrinkInTheBar&);
	Fight& operator=(const DrinkInTheBar&);

public:

	//this is a singleton
	static Fight* Instance();

	virtual void Enter(Drinker* drinker);

	virtual void Execute(Drinker* drinker);

	virtual void Exit(Drinker* drinker);

	virtual bool OnMessage(Drinker* drinker, const Telegram& msg);
};

//------------------------------------------------------------------------
class GoHomeAndRest : public State<Drinker>
{
private:

	GoHomeAndRest() {}

	//copy ctor and assignment should be private
	GoHomeAndRest(const DrinkInTheBar&);
	GoHomeAndRest& operator=(const DrinkInTheBar&);

public:

	//this is a singleton
	static GoHomeAndRest* Instance();

	virtual void Enter(Drinker* drinker);

	virtual void Execute(Drinker* drinker);

	virtual void Exit(Drinker* drinker);

	virtual bool OnMessage(Drinker* drinker, const Telegram& msg);
};


#endif
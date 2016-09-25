#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>

#include <fstream>
#include <time.h>
#include <thread>
#include <vector>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drinker.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;
using namespace Awesomium;
int WinMain()
{
	//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif

	WebCore* web_core = WebCore::Initialize(WebConfig());

	//seed random number generator
	srand((unsigned)time(NULL));

	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob);

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa);

	//create Louis
	Drinker* Louis = new Drinker(ent_Louis);

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Louis);

	std::vector<std::thread> threads;
	//run Bob and Elsa through a few Update calls
	for (int i = 0; i < 1000; ++i)
	{
		threads.push_back(Bob->UpdateT());
		threads.push_back(Elsa->UpdateT());
		threads.push_back(Louis->UpdateT());
		while (!threads.empty())
		{
			threads.back().join();
			threads.pop_back();
		}
		//dispatch any delayed messages
		Dispatch->DispatchDelayedMessages();
		web_core->Update();
		Sleep(800);
	}

	//tidy up
	delete Bob;
	delete Elsa;
	delete Louis;

	//wait for a keypress before exiting
	PressAnyKeyToContinue();
	WebCore::Shutdown();
	return 0;
}
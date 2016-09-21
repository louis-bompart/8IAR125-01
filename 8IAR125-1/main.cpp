#include <fstream>
#include <time.h>
#include <thread>
#include <vector>
#include <QtWidgets/QApplication>


#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Drinker.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#include "myclass.h"



std::ofstream os;

int main(int argc, char *argv[])
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif
  /*QT*/
  QApplication a(argc, argv);
  MyClass w;
  w.show();
  return a.exec();


  //seed random number generator
  srand((unsigned) time(NULL));

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
  for (int i=0; i<30; ++i)
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

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Elsa;
  delete Louis;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();

  return 0;
}
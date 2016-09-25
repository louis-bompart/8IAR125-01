#include "application.h"
#include "view.h"
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

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Awesomium;

class TutorialApp : public Application::Listener {
	Application* app_;
	View* view_Bob_;
	View* view_Elsa_;
	View* view_Louis_;
	Miner* Bob;
	MinersWife* Elsa;
	Drinker* Louis;
	int loopTime = 1000;
	int currentTime = 0;
public:
	TutorialApp()
		: app_(Application::Create()),
		view_Bob_(0), view_Elsa_(0), view_Louis_(0) {
		app_->set_listener(this);
		srand((unsigned)time(NULL));

		//create a miner
		Bob = new Miner(ent_Miner_Bob);

		//create his wife
		Elsa = new MinersWife(ent_Elsa);

		//create Louis
		Louis = new Drinker(ent_Louis);

		//register them with the entity manager
		EntityMgr->RegisterEntity(Bob);
		EntityMgr->RegisterEntity(Elsa);
		EntityMgr->RegisterEntity(Louis);
	}

	virtual ~TutorialApp() {
		//tidy up
		delete Bob;
		delete Elsa;
		delete Louis;

		if (view_Elsa_)
			app_->DestroyView(view_Elsa_);
		if (view_Bob_)
			app_->DestroyView(view_Bob_);
		if (view_Louis_)
			app_->DestroyView(view_Louis_);
		if (app_)
			delete app_;
	}

	void Run() {
		app_->Run();
	}

	// Inherited from Application::Listener
	virtual void OnLoaded() {
		view_Bob_ = View::Create(500, 300);
		view_Louis_ = View::Create(500, 300);
		view_Elsa_ = View::Create(500, 300);
		//seed random number generator
	}

	// Inherited from Application::Listener
	virtual void OnUpdate() {
		std::vector<std::thread> threads;
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
		std::string str = "data:text/html,<h1>Hello Bob " + std::to_string(currentTime) + "</h1>";
		currentTime++;
		WebURL url(WSLit(str.c_str()));
		view_Bob_->web_view()->LoadURL(url);

		view_Elsa_->web_view()->LoadURL(url);

		view_Louis_->web_view()->LoadURL(url);
		Sleep(10000);
		app_->web_core->Update();
	}

	// Inherited from Application::Listener
	virtual void OnShutdown() {
	}
};

#ifdef _WIN32
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t*,
	int nCmdShow) {
#else
int main() {
#endif

	TutorialApp app;
	app.Run();
	return 0;
}

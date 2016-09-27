#pragma once
#pragma warning (disable:4786)
#include <mutex>

#define CoutLock CoutMutex::Instance()

class CoutMutex
{
private:
	CoutMutex() {};
	~CoutMutex() {};
	CoutMutex (const CoutMutex&){}
	CoutMutex& operator=(const CoutMutex&);

	std::mutex m_mtx;
public:
	static CoutMutex* Instance();
	bool Lock();
	void Unlock();
};


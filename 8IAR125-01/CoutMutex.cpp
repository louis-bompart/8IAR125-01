#include "CoutMutex.h"

CoutMutex* CoutMutex::Instance()
{
	static CoutMutex instance;
	return &instance;
}
bool CoutMutex::Lock() {
	return !m_mtx.try_lock();
}
void CoutMutex::Unlock() {
	m_mtx.unlock();
}
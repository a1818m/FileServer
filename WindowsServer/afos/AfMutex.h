
#ifndef _OSAPI_MUTEX_H
#define _OSAPI_MUTEX_H

struct AfMutex_Priv;
class AfMutex
{
public:
	AfMutex();
	~AfMutex();


	int Lock();
	int TryLock();
	void Unlock();

private:
	int Init(); // ´´½¨

private:
	AfMutex_Priv* m_Priv;

};



#endif

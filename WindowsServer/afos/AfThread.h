
#ifndef _AF_THREAD_H
#define _AF_THREAD_H

struct AfThread_Priv;
class AfThread
{
public:
	AfThread();
	virtual ~AfThread();

	// ����������
	virtual int Run();

	// �ȴ����ջ���Դ
	static void Join(AfThread* thrd);

	// Sleep����
	static void Msleep(int ms);
	static void Sleep(int s);

public:
	virtual int ThreadMain() = 0;

private:
	AfThread_Priv* m_Priv;
};


#endif


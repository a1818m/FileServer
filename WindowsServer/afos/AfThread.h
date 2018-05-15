
#ifndef _AF_THREAD_H
#define _AF_THREAD_H

struct AfThread_Priv;
class AfThread
{
public:
	AfThread();
	virtual ~AfThread();

	// 创建并启动
	virtual int Run();

	// 等待和收回资源
	static void Join(AfThread* thrd);

	// Sleep函数
	static void Msleep(int ms);
	static void Sleep(int s);

public:
	virtual int ThreadMain() = 0;

private:
	AfThread_Priv* m_Priv;
};


#endif


#include "Task.h"
#include <qthread.h>

void Task::run()
{
	if (isPaused()) {
		while (isPaused()) {
			QThread::msleep(100);
		}
	}
}

bool Task::isPaused() const
{
	return m_paused;
}

void Task::pause()
{
	m_paused = true;
}

void Task::resume()
{
	m_paused = false;
}

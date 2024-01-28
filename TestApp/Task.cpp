#include "Task.h"
#include <qthread.h>

Task::Task(int numIters)
	: m_numIterations(numIters)
{
	if (m_numIterations < 1) {
		m_numIterations = 1;
	}
	setAutoDelete(false);
}

void Task::run()
{
	for (int i = 0; i < m_numIterations; ++i) {
		emit progress(100 * i / m_numIterations);

		if (isCanceled()) {
			break;
		}

		processPause();
		QThread::msleep(100); // job: sleep mseconds
	}

	m_finished = true;

	emit finished();
}

bool Task::isPaused() const
{
	return m_paused;
}

bool Task::isCanceled() const
{
	return m_canceled;
}

bool Task::isFinished() const
{
	return m_finished;
}

void Task::pause()
{
	m_paused = true;
}

void Task::resume()
{
	m_paused = false;
}

void Task::cancel()
{
	m_canceled = true;
}

void Task::processPause()
{
	if (isPaused()) {
		while (isPaused()) {
			if (isCanceled()) {
				return;
			}
			QThread::msleep(100);
		}
	}
}
